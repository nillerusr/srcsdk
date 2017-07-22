/*
tier0_hacks.cpp - Hacking into tier0 funcs
Copyright (C) a1batross

Licensed under WTFPL License
*/

#include <dlfcn.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdarg.h>
#include "jni.h"
#include "funutils.h"
#include "wrapper.h"
// #include "wrap_cmdline.h"

#define SPEW_MESSAGE_OFFSET 0x50
// #define SPEW_MESSAGE 0x00008E60

static void *spewFunc;
static unsigned char spewFunc_old[HIJACK_SIZE];

typedef void *(*GetSpewOutputFunc_t)();
GetSpewOutputFunc_t GetSpewOutputFunc;

typedef void (*SDL_StartTextInput_t)();
SDL_StartTextInput_t SDL_StartTextInput;

typedef int (*SpewFunc)( int, char* );
SpewFunc DefaultSpewFunc;

int Tier0_SpewMessage( int type, const char *msg, va_list arg )
{	
	char log[4096];
	SpewFunc func;

	vsnprintf(log, sizeof( log ), msg, arg);
	log[sizeof(log)-1] = 0;

	__android_log_print( ANDROID_LOG_INFO, "SRCENGINE", "%s", log );

	func = (SpewFunc)GetSpewOutputFunc();

	if( func && func != DefaultSpewFunc )
		func( 0, log );

	return 1;
}

void TestPrint( const char *msg, ... )
{
	va_list arg;
	
	va_start(arg, msg);
	
	((int(*)(int, const char *, va_list))spewFunc)( 0, msg, arg );
	
	va_end(arg);
}

void Tier0_Hack_Init( Module *tier0 )
{
	const void *handle = tier0->GetHandle();
	
	spewFunc = dl_rel_offset_sym( handle, "DefaultSpewFunc", SPEW_MESSAGE_OFFSET );
	DefaultSpewFunc = (SpewFunc)tier0->Resolve("DefaultSpewFunc");
	GetSpewOutputFunc = (GetSpewOutputFunc_t)tier0->Resolve( "GetSpewOutputFunc" );	
	
	// any calls leads to hack
	simple_hook( spewFunc, (void*)Tier0_SpewMessage, spewFunc_old );
		
	TestPrint( "You should see this in console. If not, contact any magician." );
}

void Tier0_Hack_Free( Module *tier0 )
{
	fun_rewrite( spewFunc, spewFunc_old, HIJACK_SIZE, NULL );
}

void SDLWrap_Init( Module *SDL )
{
	SDL_StartTextInput = (SDL_StartTextInput_t)SDL->Resolve( "SDL_StartTextInput" );
	
	SDL_StartTextInput();
}

WRAP_LIBRARY( Tier0, "libtier0.so", Tier0_Hack_Init, Tier0_Hack_Free );
WRAP_LIBRARY( SDL, "libSDL2.so", SDLWrap_Init );

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* x)
{
	// initialize everything. tierhook is not a module, so CreateInterface will never be called on it here
	g_Loader.LoadAllLibraries(); 
	return JNI_VERSION_1_4;
}

