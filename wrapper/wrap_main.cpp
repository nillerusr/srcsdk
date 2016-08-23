#include <stdio.h>
#include <android/log.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>
#include "wrap_cmdline.h"

//#define USE_LD_WRAP
//#define WRAP_VSPRINTF

#ifdef USE_LD_WRAP
	#define REAL(x) __real_##x
	#define WRAP(x) __wrap_##x
#else
	#define REAL(x) x // just to check signature
	#define WRAP(x) x
#endif

extern "C"
{

#ifdef WRAP_VSNPRINTF
int REAL( vsnprintf )( char *str, size_t size, const char *format, va_list ap );
int REAL( vsprintf )( char *str, const char *format, va_list arg );
#endif
#ifdef WRAP_VSNPRINTF
int WRAP(vsnprintf)( char *str, size_t size, const char *format, va_list ap )
{
	static int skip;
#ifndef USE_LD_WRAP
	static int (*__real_vsnprintf)(char *str, size_t size, const char *format, va_list ap);

	__real_vsnprintf = dlsym(RTLD_NEXT, "vsnprintf");
#endif
	if( __real_vsnprintf )
	{
		int ret = __real_vsnprintf(str, size, format, ap);
	
		if( Q_strstr(format, ) && !skip && ret > 0 && str && size > 0 ) 
		{
			skip = 1;
			str[ret > size ? size : ret] = '\0';
			__android_log_write(ANDROID_LOG_INFO, "Wrapper", str);
		}
		else skip = 0;
		return ret;
	}
	return -1;
}

int WRAP(vsprintf)( char * str, const char * format, va_list ap )
{
	static int skip;
#ifndef USE_LD_WRAP
	int (*__real_vsprintf)(char *str, const char *format, va_list ap);

	__real_vsprintf = dlsym(RTLD_NEXT, "vsprintf");
#endif
	if( __real_vsprintf )
	{
		int ret = __real_vsprintf(str, format, ap);
	
		if( !skip && ret > 0 && str ) 
		{
			skip = 1;
			str[ret] = '\0';
			__android_log_write(ANDROID_LOG_INFO, "Wrapper", str);
		}
		else skip = 0;
		return ret;
	}
	return -1;
}
#endif // WRAP_VSNPRINTF

#if 0
int androidwrapper_vswprintf( wchar_t * ws, size_t len, const wchar_t * format, va_list arg )
{
	// TODO: switch to ld wrap
	static void *hModule;
	static int (*__real_android_vswprintf)( wchar_t * ws, size_t len, const wchar_t * format, va_list arg );

	// I am unsure is RTLD_NEXT will work here...
	if( !hModule ) hModule = dlopen("libandroidwrapper.so", RTLD_NOW);
	if( !__real_android_vswprintf ) __real_android_vswprintf = dlsym(hModule, "androidwrapper_vswprintf");
	
	int ret = __real_android_vswprintf( ws, len, format, arg );
	
	char *s = malloc( len );
	for( ; ws; ws++, s++ )
	{
		*s = *ws; // HACK!!!
	}
	*s = '\0';
	
	__android_log_write(ANDROID_LOG_INFO, "Wrapper", s);
	
	free(s);
	// TODO: log wide vsprintf
	
	return ret;
}
#endif

void *REAL( CommandLine_Tier0 )( void );
void *WRAP( CommandLine_Tier0 )( void )
{
	return MyCommandLine_Tier0();
}

/*FILE *fopen( const char * filename, const char * mode )
{
	static FILE *(*__real_fopen)( const char * filename, const char * mode ) = dlsym(RTLD_NEXT, "fopen");
	
	if( !strstr(filename, "console.log") )
	{
		filename = "/sdcard/console.log";
	}
	
	return __real_fopen( filename, mode );
}*/

} // extern "C"





