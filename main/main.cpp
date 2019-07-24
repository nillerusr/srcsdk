// ======================================
// Reverse-engineerined libmain.so
// So ,nvidia, fuck you
// ======================================

#include <stdio.h>
#include <android/log.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>
#include <EGL/egl.h>
#include "jni.h"
#include "IAppSystem.h"
#include "wrapper.h"

char dataDir[512];

const char *LauncherArgv[512];
char javaArgv[2048];
char startArgs[256][128];
int iLastArgs = 0;

bool bClient_loaded = false;
void *libclient;

static struct jnimethods_s
{
        jclass actcls;
        JavaVM *vm;
        JNIEnv *env;
        jmethodID enableTextInput;
} jni;

DLLEXPORT int Java_com_valvesoftware_ValveActivity2_setMainPackFilePath(JNIEnv *env, jclass *clazz, jstring str)
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_setMainPackFilePath" );
	return setenv( "VALVE_PAK0_PATH", env->GetStringUTFChars(str, NULL), 1 );
}
DLLEXPORT int Java_com_valvesoftware_ValveActivity2_setPatchPackFilePath(JNIEnv *env, jclass *clazz, jstring str)
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_setPatchPackFilePath" );
	return setenv( "VALVE_PAK1_PATH", env->GetStringUTFChars(str, NULL), 1 );
}
DLLEXPORT void Java_com_valvesoftware_ValveActivity2_setCacheDirectoryPath(JNIEnv *env, jclass *clazz, jstring str)
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_setCacheDirectoryPath" );
	//return setenv( "VALVE_CACHE_PATH", env->GetStringUTFChars(str, NULL), 1 );
}
DLLEXPORT void Java_com_valvesoftware_ValveActivity2_gpgsStart()
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_gpgsStart" );
}
DLLEXPORT void Java_com_valvesoftware_ValveActivity2_nativeOnActivityResult()
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_nativeOnActivityResult" );
}

DLLEXPORT void Java_com_valvesoftware_ValveActivity2_setNativeLibPath(JNIEnv *env, jclass *clazz, jstring str)
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_setNativeLibPath" );
//	snprintf(dataDir, sizeof dataDir, env->GetStringUTFChars(str, NULL));
}
DLLEXPORT void Java_com_valvesoftware_ValveActivity2_setLanguage()
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_setLanguage" );
}
DLLEXPORT int Java_com_valvesoftware_ValveActivity2_setDocumentDirectoryPath(JNIEnv *env, jclass *clazz, jstring str)
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_setDocumentDirectoryPath" );
	return setenv( "VALVE_CACHE_PATH", env->GetStringUTFChars(str, NULL), 1 );
}
DLLEXPORT int Java_com_valvesoftware_ValveActivity2_setDropMip(int a1, int a2, signed int a3)
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_setDropMip" );
}

DLLEXPORT int Java_com_valvesoftware_ValveActivity2_saveGame()
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_saveGame" );
}

DLLEXPORT int Java_com_valvesoftware_ValveActivity2_setDataDirectoryPath(JNIEnv *env, jclass *clazz, jstring str)
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity_setDataDirectoryPath" );
	snprintf(dataDir, sizeof dataDir, env->GetStringUTFChars(str, NULL));
}

DLLEXPORT int Java_com_valvesoftware_ValveActivity2_setExtrasPackFilePath(JNIEnv *env, jclass *clazz, jstring str)
{
	__android_log_print( ANDROID_LOG_DEBUG, "SourceSDK2013", "Java_com_valvesoftware_ValveActivity2_setExtrasPackFilePath" );
	return setenv( "VALVE_PAK2_PATH", env->GetStringUTFChars(str, NULL), 1 );
}

typedef void (*t_TouchEvent)(int finger, int x, int y, int act);
t_TouchEvent TouchEvent;

DLLEXPORT void clientLoaded( void )
{
	bClient_loaded = true;
	libclient = dlopen("libclient.so",0);
	TouchEvent = (t_TouchEvent)dlsym(libclient, "TouchEvent");
	__android_log_print( ANDROID_LOG_INFO, "HL2TOUCH", "CLIENT LOADED!" );
}

DLLEXPORT void showKeyboard( int show )
{
	jni.env->CallStaticVoidMethod( jni.actcls, jni.enableTextInput, show );
}

DLLEXPORT void JNICALL Java_com_valvesoftware_ValveActivity2_TouchEvent(JNIEnv *env, jobject obj, jint fingerid, jint x, jint y, jint action)
{
	if( !bClient_loaded )
		return;

	TouchEvent( fingerid, x, y, action );
}

typedef void (*t_SDL_Android_Init)(JNIEnv* env, jclass cls);
t_SDL_Android_Init SDL_Android_Init;

typedef void (*t_HookInit)();
t_HookInit HookInit;

typedef void *(*t_SDL_StartTextInput)();
t_SDL_StartTextInput SDL_StartTextInput;

bool bUseGL;

void SetRenderer()
{
	if ( bUseGL )
	{
		setenv("USE_BIG_GL", "1", 1);
	}
	else
	{
		setenv("REGAL_LOG", "0", 1);
		setenv("REGAL_LOG_ERROR", "0", 1);
		setenv("REGAL_LOG_WARNING", "0", 1);
		setenv("REGAL_LOG_INFO", "0", 1);
		setenv("REGAL_LOG_HTTP", "0", 1);
		setenv("REGAL_LOG_JSON", "0", 1);
		setenv("REGAL_LOG_CALLBACK", "0", 1);
		setenv("REGAL_LOG_ONCE", "0", 1);
		setenv("REGAL_LOG_POINTERS", "0", 1);
		setenv("REGAL_LOG_THREAD", "0", 1);
		setenv("REGAL_LOG_PROCESS", "0", 1);
		setenv("REGAL_LOG_ALL", "0", 1);
		setenv("REGAL_DEBUG", "0", 1);
		setenv("REGAL_ERROR", "0", 1);
		setenv("REGAL_LOG_FILE", "/dev/null", 1);
		setenv("REGAL_EMU_SO", "0", 1);
		setenv("REGAL_THREAD_LOCKING", "0", 1);
		setenv("REGAL_FORCE_ES2_PROFILE", "1", 1);
		setenv("REGAL_SYS_GLX", "0", 1);
		setenv("REGAL_SYS_ES2", "1", 1);
		setenv("REGAL_SYS_EGL", "1", 1);
		setenv("REGAL_SYS_GL", "0", 1);
		setenv("REGAL_GL_VERSION", "2.1", 1);
		setenv("REGAL_GL_EXTENSIONS", "GL_EXT_framebuffer_object GL_EXT_framebuffer_blit GL_NV_fence GL_ARB_vertex_buffer_object GL_EXT_texture_sRGB_decode GL_EXT_texture_compression_s3tc GL_EXT_texture_compression_dxt1", 1);
	}
}

void SetArg( const char *arg )
{
	char *pch;
	char str[1024];
	strncpy( str, arg, sizeof str );
	pch = strtok (str," ");
	while (pch != NULL)
	{
		strncpy( startArgs[iLastArgs], pch, sizeof startArgs[0] );
		LauncherArgv[iLastArgs] = startArgs[iLastArgs];
		iLastArgs++;
		pch = strtok (NULL, " ");
	}
}

DLLEXPORT int Java_com_valvesoftware_ValveActivity2_setArgs(JNIEnv *env, jclass *clazz, jstring str)
{
	snprintf( javaArgv, sizeof javaArgv, env->GetStringUTFChars(str, NULL) );
}

void SetStartArgs()
{
	SetArg(dataDir);
	SetArg("-game hl2 "
			"-nosteam "
			"+sv_unlockedchapters 99 "
			"+mat_queue_mode 2 "
			"+mat_picmip 0 "
			"-ignoredxsupportcfg "
			"-regal "
			"+gl_rt_forcergba 1 "
			"+mat_antialias 0 "
			"-mat_antialias 0 "
			"+r_flashlightdepthtexture 1 "
			"+gl_dropmips 1 "
			"+cc_lang english "
			"-insecure");

	if( bUseGL )
		SetArg("-userclip "
				"-gl_enablesamplerobjects "
				"-egl "
				"+gl_enabletexsubimage 1 "
				"+mat_motion_blur_enabled 1 "
				"+mat_disable_bloom 0 "
				"+mat_colorcorrection 1 "
				"+mat_parallaxmap 1 "
				"+mat_reducefillrate 0 "
				"+r_shadowrendertotexture 1 "
				"+mat_bumpmap 1 "
				"+mat_specular 1 "
				"+gl_blitmode 1 "
				"+gl_supportMapBuffer 0 "
				"-gl_separatedepthstencil 0 "
				"-gl_nodepthtexture 0 "
				"+r_flashlight_version2 0 "
				"+gl_emurgba16 0 "
				"+gl_emunooverwrite 0");
	else
		SetArg("-nouserclip "
				"-gl_disablesamplerobjects "
				"+gl_enabletexsubimage 0 "
				"+mat_motion_blur_enabled 0 "
				"+mat_disable_bloom 1 "
				"+mat_colorcorrection 0 "
				"+mat_parallaxmap 0 "
				"+mat_reducefillrate 1 "
				"+r_shadowrendertotexture 0 "
				"+mat_bumpmap 0 "
				"+mat_specular 0 "
				"+gl_blitmode 0 "
				"+gl_supportMapBuffer 1 "
				"-gl_separatedepthstencil 0 "// default is 1
				"-gl_nodepthtexture 1 "
				"+r_flashlight_version2 1 "
				"+gl_emurgba16 1 "
				"+gl_emunooverwrite 1");
}

#define ANDROID_SERVICES_INTERFACE_VERSION "AndroidServicesVersion001"

class IAndroidServices : public IAppSystem
{
public:
	virtual bool Connect( CreateInterfaceFn factory ) = 0;
	virtual void *QueryInterface( const char *pInterfaceName ) = 0;

	virtual void LogIn() = 0;
	virtual void LogOut() = 0;
	virtual int GetLoginState() = 0;
	virtual int GetLoginError() = 0;
	virtual int UploadSave() = 0;
	virtual int DownloadSave() = 0;
	virtual void FreeSave() = 0;
	virtual int DeleteSave() = 0;
	virtual int EnumerateSaves() = 0;
	virtual void FreeEnumeration() = 0;
	virtual int GetImage() = 0;
	virtual int AchievementGetSteps() = 0;
	virtual void AchievementSetSteps() = 0;
	virtual int AchievementIsUnlocked() = 0;
	virtual void AchievementUnlock() = 0;
	virtual void ViewAchievements() = 0;
};

class CGooglePlayGamesServices : public CBaseAppSystem<IAndroidServices>
{
	typedef CBaseAppSystem<IAndroidServices> BaseClass;

	public:
	virtual bool Connect( CreateInterfaceFn factory )
	{
		LogPrintf("CGooglePlayGamesServices::Connect");

		return BaseClass::Connect(factory);
	}

	virtual void *QueryInterface( const char *pInterfaceName )
	{
		LogPrintf("CGooglePlayGamesServices::QueryInterface");

		if (!strncmp(pInterfaceName, ANDROID_SERVICES_INTERFACE_VERSION, strlen(ANDROID_SERVICES_INTERFACE_VERSION) + 1))
			return (IAndroidServices*)this;

		return BaseClass::QueryInterface( pInterfaceName );
	}

	virtual void LogIn()
	{
		LogPrintf("CGooglePlayGamesServices::Login");
	}
	virtual void LogOut()
	{
		LogPrintf("CGooglePlayGamesServices::LogOut");
	}
	virtual int GetLoginState()
	{
		LogPrintf("CGooglePlayGamesServices::GetLoginState");
		return 0;
	}
	virtual int GetLoginError()
	{
		LogPrintf("CGooglePlayGamesServices::GetLoginError");
		return 0;
	}
	virtual int UploadSave()
	{
		LogPrintf("CGooglePlayGamesServices::UploadSave");
		return 0;
	}
	virtual int DownloadSave()
	{
		LogPrintf("CGooglePlayGamesServices::DownloadSave");
		return 0;
	}
	virtual void FreeSave()
	{
		LogPrintf("CGooglePlayGamesServices::FreeSave");
	}
	virtual int DeleteSave()
	{
		LogPrintf("CGooglePlayGamesServices::DeleteSave");
		return 0;
	}
	virtual int EnumerateSaves()
	{
		LogPrintf("CGooglePlayGamesServices::EnumerateSaves");
		return 0;
	}
	virtual void FreeEnumeration()
	{
		LogPrintf("CGooglePlayGamesServices::FreeEnumeration");
	}
	virtual int GetImage()
	{
		LogPrintf("CGooglePlayGamesServices::GetImage");
		return 0;
	}
	virtual int AchievementGetSteps()
	{
		LogPrintf("CGooglePlayGamesServices::AchievementGetSteps");
		return 0;
	}
	virtual void AchievementSetSteps()
	{
		LogPrintf("CGooglePlayGamesServices::AchievementSetSteps");
	}
	virtual int AchievementIsUnlocked()
	{
		LogPrintf("CGooglePlayGamesServices::AchievementIsUnlocked");
		return 0;
	}
	virtual void AchievementUnlock()
	{
		LogPrintf("CGooglePlayGamesServices::AchievementUnlock");
	}
	virtual void ViewAchievements()
	{
		LogPrintf("CGooglePlayGamesServices::ViewAchievements");
	}
};

static CGooglePlayGamesServices g_GPGS;

typedef int (*t_LauncherMainAndroid)(int argc, const char **argv, void *google, int huy);
t_LauncherMainAndroid LauncherMainAndroid;

DLLEXPORT int Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv *env, jclass cls, jobject obj)
{
	LogPrintf("Java_org_libsdl_app_SDLActivity_nativeInit");

	// init sdl
	void *sdlHandle = dlopen("libSDL2.so", 0);

	SDL_Android_Init = (t_SDL_Android_Init)dlsym(sdlHandle, "SDL_Android_Init");
	LogPrintf("SDL_Android_Init: 0x%X", SDL_Android_Init);
	SDL_Android_Init(env, cls);

	SDL_StartTextInput = (t_SDL_StartTextInput)dlsym(sdlHandle, "SDL_StartTextInput");
	LogPrintf("SDL_StartTextInput: 0x%X", SDL_StartTextInput);
	SDL_StartTextInput();

	chdir(dataDir);
	getcwd(dataDir, sizeof dataDir);
	setenv( "VALVE_GAME_PATH", dataDir, 1 );
	snprintf(dataDir, sizeof dataDir, "%s/hl2_linux", dataDir);

	bUseGL = false;

	SetRenderer();
	SetStartArgs();
	SetArg(javaArgv);

	void *engineHandle = dlopen("libengine.so", 0);
	LogPrintf("engineHandle: 0x%X", engineHandle);
	void *launcherHandle = dlopen("liblauncher.so", 0);
	LogPrintf("launcherHandle: 0x%X", launcherHandle);
	void *tierhook = dlopen("libtierhook.so", 0);
	HookInit = (t_HookInit)dlsym(tierhook,"HookInit");
	HookInit();

	jni.env = env;
	jni.actcls = env->FindClass("org/libsdl/app/SDLActivity");
	jni.enableTextInput = env->GetStaticMethodID(jni.actcls, "showKeyboard", "(I)V");

	LauncherMainAndroid = (t_LauncherMainAndroid)dlsym(launcherHandle, "LauncherMainAndroid");
	LogPrintf("LauncherMainAndroid: 0x%X", LauncherMainAndroid);

	if (LauncherMainAndroid)
		LauncherMainAndroid(iLastArgs, LauncherArgv, &g_GPGS, 0);

	dlclose(tierhook);
	dlclose(launcherHandle);
	dlclose(engineHandle);

	return 0;
}

