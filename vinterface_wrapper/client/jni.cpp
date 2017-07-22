#include "client.h"
#include <jni.h>
#include <pthread.h>
#include "miniSDL.h"
#include "vgui/IInputInternal.h"

#define ANDROID_MAX_EVENTS 128
#define CMD_SIZE 64

enum eventtype_t
{
	event_clientcmd,
	event_touchmotion,
	event_touchup,
	event_touchdown
};

struct event_clientcmd_t
{
	char buf[CMD_SIZE];
};

typedef struct event_s
{
	eventtype_t type;
	union
	{
		event_clientcmd_t clientCmd;
		SDL_TouchFingerEvent tfinger; 
	} e;
} event_t;

static struct {
	pthread_mutex_t mutex; // this mutex is locked while not running frame, used for events synchronization
	pthread_mutex_t framemutex; // this mutex is locked while engine is running and unlocked while it reading events, used for pause in background.
	event_t queue[ANDROID_MAX_EVENTS];
	volatile int count;
	float mousex, mousey;
} events = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER };

/*static struct jnimethods_s
{
	JavaVM *vm;
	JNIEnv *env;
} jni;*/

#define Android_Lock() pthread_mutex_lock(&events.mutex);
#define Android_Unlock() pthread_mutex_unlock(&events.mutex);
#define Android_PushEvent() Android_Unlock()

vgui::IInputInternal *g_pInputInternal = NULL;
vgui::IInput *g_pInput = NULL;

GET_INTERFACE_PTR( VGUI_InputInternal, "libvgui2.so", VGUI_INPUTINTERNAL_INTERFACE_VERSION, &g_pInputInternal );
GET_INTERFACE_PTR( VGUI_Input, "libvgui2.so", VGUI_INPUT_INTERFACE_VERSION, &g_pInput );

typedef int (*SDL_EventFilter)(void *userdata, SDL_Event *event );
extern "C" int SDLWrap_EventFilter( void *userdata, SDL_Event *event );

typedef int (*SDL_SetEventFilter_t)(SDL_EventFilter filter, void *userdata );

void SDLWrap_Init( Module *SDL2 )
{
	SDL_SetEventFilter_t SDL_AddEventWatch = (SDL_SetEventFilter_t)SDL2->Resolve("SDL_AddEventWatch");
	
	SDL_AddEventWatch( SDLWrap_EventFilter, NULL );
}

WRAP_LIBRARY( SDL2, "libSDL2.so", SDLWrap_Init );

/*
========================
Android_AllocEvent

Lock event queue and return pointer to next event.
Caller must do Android_PushEvent() to unlock queue after setting parameters.
========================
*/
event_t *Android_AllocEvent()
{
	Android_Lock();
	if( events.count == ANDROID_MAX_EVENTS )
	{
		events.count--; //override last event
		__android_log_print( ANDROID_LOG_ERROR, "SourceSDK2013", "Too many events!!!" );
	}
	return &events.queue[ events.count++ ];
}

void Android_RunEvents()
{
	int i, w, h;
	Android_Lock();
	pthread_mutex_unlock( &events.framemutex );
	
	engine->GetScreenSize( w, h );
	
	for( int i = 0; i < events.count; i++ )
	{
		event_t *event = events.queue + i;
		
		switch( event->type )
		{
		case event_clientcmd:
			engine->ClientCmd( event->e.clientCmd.buf );
			break;
		case event_touchmotion:
			// handled
			break;
		case event_touchdown:
			
			g_pInputInternal->InternalMousePressed( MOUSE_LEFT );
			LogPrintf( "Mouse pressed" );
			if( g_pInputInternal->IsMouseDown( MOUSE_LEFT ) )
			{
				LogPrintf( "VGUI accepted this" );
			}
			break;
		case event_touchup:
			LogPrintf( "Mouse released" );
			g_pInputInternal->InternalMouseReleased( MOUSE_LEFT );
			if( !g_pInputInternal->IsMouseDown( MOUSE_LEFT ) )
			{
				LogPrintf( "VGUI accepted this" );
			}
			break;
		}
	}
	events.count = 0;
	
	Android_Unlock();
	pthread_mutex_lock( &events.framemutex );
}

extern "C" 
{
int SDLWrap_EventFilter(void* userdata, SDL_Event* event)
{	
	event_t *ev = Android_AllocEvent();
	switch( event->type )
	{
		case SDL_FINGERMOTION:
			ev->type = event_touchmotion;
			ev->e.tfinger = event->tfinger;
			break;
		case SDL_FINGERUP:
			ev->type = event_touchup;
			ev->e.tfinger = event->tfinger;
			break;
		case SDL_FINGERDOWN:
			ev->type = event_touchdown;
			ev->e.tfinger = event->tfinger;
			break;
		default: break;
	}
	
	Android_PushEvent();
	
	return 1;
}

JNIEXPORT void JNICALL Java_com_valvesoftware_ValveActivity_clientCommand( JNIEnv *env, jclass clazz, jstring jstr )
{
	const char *str = env->GetStringUTFChars( jstr, 0 );
	event_t *event = Android_AllocEvent();
	
	event->type = event_clientcmd;
	strncpy( event->e.clientCmd.buf, str, CMD_SIZE-1 );
	event->e.clientCmd.buf[CMD_SIZE-1] = 0;
	
}

JNIEXPORT jboolean JNICALL Java_com_valvesoftware_ValveActivity_isGameUIActive( JNIEnv *env, jclass clazz )
{
	if( enginevgui )
		return enginevgui->IsGameUIVisible();
	
	return false;
}

JNIEXPORT jboolean JNICALL Java_com_valvesoftware_ValveActivity_shouldDrawControls( JNIEnv *env, jclass clazz )
{
	return ( engine && engine->IsInGame() ) || ( enginevgui && enginevgui->IsGameUIVisible() );
}
}
