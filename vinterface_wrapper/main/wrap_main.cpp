#include <stdio.h>
#include <android/log.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>
#include "wrap_cmdline.h"
#include "wrapper.h"

#include "jni.h"

typedef int (*tpJava_com_valvesoftware_ValveActivity_setMainPackFilePath)(int a1);
typedef int (*tpJava_com_valvesoftware_ValveActivity_setPatchPackFilePath)(int a1);
typedef void (*tpJava_com_valvesoftware_ValveActivity_setCacheDirectoryPath)();
typedef int (*tpJava_com_valvesoftware_ValveActivity_setDocumentDirectoryPath)(int a1);
typedef int (*tpJava_com_valvesoftware_ValveActivity_setDropMip)(int a1, int a2, signed int a3);
typedef int (*tpJava_com_valvesoftware_ValveActivity_saveGame)();
typedef int (*tpJava_org_libsdl_app_SDLActivity_nativeInit)();
typedef int (*tpSDL_main)();

tpJava_com_valvesoftware_ValveActivity_setMainPackFilePath pJava_com_valvesoftware_ValveActivity_setMainPackFilePath;
tpJava_com_valvesoftware_ValveActivity_setPatchPackFilePath pJava_com_valvesoftware_ValveActivity_setPatchPackFilePath;
tpJava_com_valvesoftware_ValveActivity_setCacheDirectoryPath pJava_com_valvesoftware_ValveActivity_setCacheDirectoryPath;
tpJava_com_valvesoftware_ValveActivity_setDocumentDirectoryPath pJava_com_valvesoftware_ValveActivity_setDocumentDirectoryPath;
tpJava_com_valvesoftware_ValveActivity_setDropMip pJava_com_valvesoftware_ValveActivity_setDropMip;
tpJava_com_valvesoftware_ValveActivity_saveGame pJava_com_valvesoftware_ValveActivity_saveGame;
tpJava_org_libsdl_app_SDLActivity_nativeInit pJava_org_libsdl_app_SDLActivity_nativeInit;
tpSDL_main pSDL_main;

void Tier0_Hack_Init_Post( Module *tier0 );
void Tier0_Hack_Free_Post( Module *tier0 );

Module *tier0;

DLLEXPORT int Java_com_valvesoftware_ValveActivity_setMainPackFilePath(JNIEnv *env, jclass *clazz, jstring str)
{
	return setenv( "VALVE_PAK0_PATH", env->GetStringUTFChars(str, NULL), 1 );
}
DLLEXPORT int Java_com_valvesoftware_ValveActivity_setPatchPackFilePath(JNIEnv *env, jclass *clazz, jstring str)
{
	return setenv( "VALVE_PAK1_PATH", env->GetStringUTFChars(str, NULL), 1 );
}
DLLEXPORT void Java_com_valvesoftware_ValveActivity_setCacheDirectoryPath()
{
}

DLLEXPORT int Java_com_valvesoftware_ValveActivity_setDocumentDirectoryPath(JNIEnv *env, jclass *clazz, jstring str)
{
	return setenv( "VALVE_CACHE_PATH", env->GetStringUTFChars(str, NULL), 1 );
}
DLLEXPORT int Java_com_valvesoftware_ValveActivity_setDropMip(int a1, int a2, signed int a3)
{
	/*g_Loader.LoadAllLibraries();
	return pJava_com_valvesoftware_ValveActivity_setDropMip( a1, a2, a3 );*/
}
DLLEXPORT int Java_com_valvesoftware_ValveActivity_saveGame()
{
	/*g_Loader.LoadAllLibraries();
	return pJava_com_valvesoftware_ValveActivity_saveGame();*/
}
DLLEXPORT int SDL_main()
{
}

extern "C" int SDL_Android_Init( void ); // libSDL2.so

DLLEXPORT int Java_org_libsdl_app_SDLActivity_nativeInit()
{
	g_Loader.LoadAllLibraries();
	
	/*if( !tier0 )
	{
		tier0 = new Module();
		tier0->Load("libtier0.so");
		Tier0_Hack_Init_Post( tier0 );
	}*/

	SDL_Android_Init();
		
	return pSDL_main();
}

#define str(s) #s
#define RESOLVE_AND_ASSIGN( module, func ) p##func = (tp##func)(module)->Resolve( str(func) )

void Main_Init( Module *main )
{
	RESOLVE_AND_ASSIGN(main, Java_com_valvesoftware_ValveActivity_setMainPackFilePath);
	RESOLVE_AND_ASSIGN(main, Java_com_valvesoftware_ValveActivity_setPatchPackFilePath);
	RESOLVE_AND_ASSIGN(main, Java_com_valvesoftware_ValveActivity_setCacheDirectoryPath);
	RESOLVE_AND_ASSIGN(main, Java_com_valvesoftware_ValveActivity_setDocumentDirectoryPath);
	RESOLVE_AND_ASSIGN(main, Java_com_valvesoftware_ValveActivity_setDropMip);
	RESOLVE_AND_ASSIGN(main, Java_com_valvesoftware_ValveActivity_saveGame);
	RESOLVE_AND_ASSIGN(main, Java_org_libsdl_app_SDLActivity_nativeInit);
	RESOLVE_AND_ASSIGN(main, SDL_main);
}

void Main_Free( Module *main )
{
	if( tier0 )
		Tier0_Hack_Free_Post( NULL );
}

WRAP_LIBRARY( Main, "libmain_original.so", Main_Init, Main_Free );
