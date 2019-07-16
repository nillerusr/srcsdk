#include "client.h"
#include <jni.h>
#include <pthread.h>
#include "miniSDL.h"
#include "funutils.h"
#include "vgui/IInputInternal.h"

#define ANDROID_MAX_EVENTS 128

enum actions
{
	ACTION_DOWN = 0,
	ACTION_UP,
	ACTION_MOVE
};

static struct {
	pthread_mutex_t mutex; // this mutex is locked while not running frame, used for events synchronization
	pthread_mutex_t framemutex; // this mutex is locked while engine is running and unlocked while it reading events, used for pause in background.
	event_t queue[ANDROID_MAX_EVENTS];
	volatile int count;
	float mousex, mousey;
} events = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER };

#define Android_Lock() pthread_mutex_lock(&events.mutex);
#define Android_Unlock() pthread_mutex_unlock(&events.mutex);
#define Android_PushEvent() Android_Unlock()

#define DECLARE_JNI_INTERFACE( ret, name, ... ) \
	JNIEXPORT ret JNICALL Java_org_libsdl_app_SDLActivity_##name( JNIEnv *env, jclass clazz, __VA_ARGS__ )

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
	Android_Lock();
	pthread_mutex_unlock( &events.framemutex );

	for( int i = 0; i < events.count; i++ )
	{
		event_t *event = events.queue + i;
		
		switch( event->type )
		{
		case event_touchmotion:
			g_Touch.TouchMotion( event );
			break;
		case event_touchdown:
			g_Touch.ButtonPress( event );
			break;
		case event_touchup:
			g_Touch.ButtonPress( event );
			break;
		}
	}
	events.count = 0;
	
	Android_Unlock();
	pthread_mutex_lock( &events.framemutex );
}

DLLEXPORT int TouchEvent( int touchDevId, int fingerid, int x, int y, int action )
{
	event_t *ev = Android_AllocEvent();
	switch( action )
	{
		case ACTION_DOWN:
			ev->type = event_touchdown;
		break;
		case ACTION_UP:
			ev->type = event_touchup;
		break;
		case ACTION_MOVE:
			ev->type = event_touchmotion;
			ev->touchDevId = touchDevId;
		break;
		default: break;
	}

	ev->x = x;
	ev-> y = y;
	ev->fingerid = fingerid;
	LogPrintf( "TouchEvent, %d, %d", x, y );

	Android_PushEvent();
}
