/*
tier0_hacks.cpp - Hacking into tier0 funcs
Copyright (C) a1batross

Licensed under WTFPL License
*/

#include <dlfcn.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdarg.h>
#include "funutils.h"
#include "wrapper.h"
// #include "wrap_cmdline.h"

#define SPEW_MESSAGE_OFFSET 0x50
// #define SPEW_MESSAGE 0x00008E60

static void *spewFunc;
static void *DefaultSpewFunc;
static unsigned char spewFunc_old[HIJACK_SIZE];

void *(*GetSpewOutputFunc)();
void (*SDL_StartTextInput)();

typedef int (*SpewFunc)( int, char* );

static int meow, meow2;

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
	static int meow, meow2; // trampolines don't have NULL checks
	
	spewFunc = dl_rel_offset_sym( handle, "DefaultSpewFunc", SPEW_MESSAGE_OFFSET );
	DefaultSpewFunc = tier0->Resolve("DefaultSpewFunc");
	GetSpewOutputFunc = (void*(*)())tier0->Resolve( "GetSpewOutputFunc" );	
	
	simple_hook( spewFunc, (void*)Tier0_SpewMessage, spewFunc_old );
	
	// fun_rewrite( spewFunc, (const void*)spewFunc_new, TRAMP_LENGTH, spewFunc_old );
	// fun_rewrite( commandLineFunc, (const void*)commandLineFunc_new, TRAMP_LENGTH, commandLineFunc_old );
	
	TestPrint( "You should see this in console. If not, contact any magician." );
}

void Tier0_Hack_Free( Module *tier0 )
{
	fun_rewrite( spewFunc, spewFunc_old, HIJACK_SIZE, NULL );
}

void SDLWrap_Init( Module *SDL )
{
	SDL_StartTextInput = (void(*)())SDL->Resolve( "SDL_StartTextInput" );
	
	SDL_StartTextInput();
}

WRAP_LIBRARY( Tier0, "libtier0.so", Tier0_Hack_Init, Tier0_Hack_Free );
WRAP_LIBRARY( SDL, "libSDL.so", SDLWrap_Init );
