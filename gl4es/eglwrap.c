#include <string.h>
#include <EGL/egl.h>
#include <dlfcn.h>
#include <android/log.h>
#include <gl4esinit.h>
#include <stdbool.h>

#define EXPORT __attribute__((visibility("default")))

typedef EGLBoolean (*t_eglSwapBuffers)(EGLDisplay display, EGLSurface surface);
t_eglSwapBuffers _eglSwapBuffers;
typedef EGLBoolean (*t_eglMakeCurrent)(EGLDisplay display, EGLSurface draw, EGLSurface read, EGLContext context);
t_eglMakeCurrent _eglMakeCurrent;
void *(*_eglGetProcAddress)( const char * );

void *libgles;

void *eglwrap_GetProcAddress( const char *procname )
{
	void *result = dlsym(libgles, procname);
	if(result)
		return result;
	else
		return _eglGetProcAddress(procname);
}

EXPORT void egl_init()
{
	__android_log_print( ANDROID_LOG_VERBOSE, "EGLWRAP", "egl_init" );
	void *libegl = dlopen("libEGL.so", RTLD_LAZY);
	libgles = dlopen("libGLESv2.so", RTLD_LAZY);
	_eglSwapBuffers = dlsym(libegl, "eglSwapBuffers" );
	_eglMakeCurrent = dlsym(libegl, "eglMakeCurrent" );
	_eglGetProcAddress = dlsym(libegl, "eglGetProcAddress" );
}

EXPORT EGLBoolean eglBindAPI( EGLenum api )
{
	return EGL_TRUE;
}

EXPORT EGLBoolean eglSwapBuffers(EGLDisplay display, EGLSurface surface)
{
	__android_log_print( ANDROID_LOG_VERBOSE, "EGLWRAP", "eglSwapBuffers" );
	return _eglSwapBuffers(display, surface);
}

EXPORT EGLenum eglQueryAPI()
{
	__android_log_print( ANDROID_LOG_VERBOSE, "EGLWRAP", "eglQueryAPI" );
        return EGL_OPENGL_API;
}

EXPORT EGLBoolean eglMakeCurrent(EGLDisplay display, EGLSurface draw, EGLSurface read, EGLContext context)
{
	__android_log_print( ANDROID_LOG_VERBOSE, "EGLWRAP", "eglMakeCurrent" );
	EGLBoolean result = _eglMakeCurrent(display, draw, read, context);
	// Here initialize gl4es
	set_getprocaddress(&eglwrap_GetProcAddress);
	initialize_gl4es();
	return result;
}

EXPORT EGLAPI __eglMustCastToProperFunctionPointerType EGLAPIENTRY eglGetProcAddress(const char *procname)
{
	void * retval =  gl4es_GetProcAddress(procname);
	__android_log_print( ANDROID_LOG_VERBOSE, "EGLWRAP", "gl4es_GetProcAddress(%s) = %p", procname, retval );
	return retval;

}

