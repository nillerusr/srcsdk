#include <stdio.h>
#include <android/log.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include "wrap_cmdline.h"

void *hModule = NULL;
void *(*__real_CommandLine_Tier0)( void );
ICommandLine *realCmdLine = NULL;

class WrapCommandLine : public ICommandLine 
{
public:
	virtual void		CreateCmdLine( const char *commandline ) 
	{
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "Args(string): %s", commandline);
		
		realCmdLine->CreateCmdLine( commandline );
	}
	virtual void		CreateCmdLine( int argc, char **argv ) 
	{
		char args[1024];
		
		for( int i = 0, j = 0; i < argc; i++ )
		{
			int len = strlen(argv[i]);
			strcpy(&args[j], argv[i]);		
		
			args[j + len + 1] = ' ';
			j += len + 1;
		}
	
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "Args(array): %s", args);
		
#if 1
		char *fakeargs[] = 
		{
			"/data/data/com.nvidia.valvesoftware.halflife2/hl2_linux", 
			"-console",
			"-dev", "5",
			"-condebug",
			//"-basedir", "/data/data/com.nvidia.valvesoftware.halflife2/files/",
			//"+con_logfile", "/sdcard/console.log",
			//"+gameui_xbox", "0"
		};
		realCmdLine->CreateCmdLine( sizeof(fakeargs) / sizeof(char*), fakeargs );
#else
		realCmdLine->CreateCmdLine(argc, argv);
#endif
	}
	virtual const char	*GetCmdLine( void ) const 
	{
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "Requested cmd line");
		return realCmdLine->GetCmdLine();
	}

	// Check whether a particular parameter exists
	virtual	const char	*CheckParm( const char *psz, const char **ppszValue = 0 ) const 
	{
		if( strcmp( psz, "-demo" ) && strcmp( psz, "-tools" ) && strcmp( psz, "-disableboneuniformbuffers" ) && strcmp( psz, "-dumpallshaders") ) // срёт!
			__android_log_print(ANDROID_LOG_INFO, "Wrapper", "CheckParm( %s )", psz);
		
		return realCmdLine->CheckParm( psz, ppszValue );
	}
	
	virtual void		RemoveParm( const char *parm ) 
	{
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "RemoveParm( %s )", parm);
		realCmdLine->RemoveParm( parm );
	}
	virtual void		AppendParm( const char *pszParm, const char *pszValues ) 
	{ 
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "AppendParm( %s, %s )", pszParm, pszValues );
		realCmdLine->AppendParm( pszParm, pszValues );
	}

	// Returns the argument after the one specified, or the default if not found
	virtual const char	*ParmValue( const char *psz, const char *pDefaultVal = 0 ) const 
	{
		if( !strcmp( psz, "-fs_log" ) )
			return "/sdcard/source.log";
				
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "ParmValue( %s, %s )", psz, pDefaultVal );
		return realCmdLine->ParmValue( psz, pDefaultVal );
	}
	virtual int			ParmValue( const char *psz, int nDefaultVal ) const 
	{
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "ParmValue( %s, int %i )", psz, nDefaultVal );
		return realCmdLine->ParmValue( psz, nDefaultVal );
	}
	virtual float		ParmValue( const char *psz, float flDefaultVal ) const 
	{
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "ParmValue( %s, float %f )", psz, flDefaultVal );
		return realCmdLine->ParmValue( psz, flDefaultVal );
	}

	// Gets at particular parameters
	virtual int			ParmCount() const 
	{
		//__android_log_print(ANDROID_LOG_INFO, "Wrapper", "ParmCount()");
		return realCmdLine->ParmCount();
	}
	virtual int			FindParm( const char *psz ) const 
	{
		//__android_log_print(ANDROID_LOG_INFO, "Wrapper", "FindParm( %s )", psz );
		return realCmdLine->FindParm( psz );
	}	// Returns 0 if not found.
	virtual const char* GetParm( int nIndex ) const 
	{
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "GetParm( %i )", nIndex );
		return realCmdLine->GetParm( nIndex );
	}
	
	// copies the string passwed
	virtual void SetParm( int nIndex, char const *pNewParm ) 
	{
		__android_log_print(ANDROID_LOG_INFO, "Wrapper", "SetParm( %i, %s )", nIndex, pNewParm );
		realCmdLine->SetParm( nIndex, pNewParm );
	}	
} wrapCmdLine;

ICommandLine *MyCommandLine_Tier0( void )
{
	// I am unsure is RTLD_NEXT will work here...
	if( !realCmdLine )
	{
		hModule = dlopen("libtier0.so", RTLD_NOW);
		if( hModule )
		{
			__real_CommandLine_Tier0 = (void* (*)())dlsym(hModule, "CommandLine_Tier0");
			if( __real_CommandLine_Tier0 )
			{
				realCmdLine = (ICommandLine *)__real_CommandLine_Tier0();
				if( !realCmdLine )
				{
					__android_log_print(ANDROID_LOG_INFO, "Wrapper", "CommandLine_Tier0 returned null");
					return 0;
				}
			}
			else
			{
				__android_log_print(ANDROID_LOG_INFO, "Wrapper", "dlsym(CommandLine_Tier0) returned null");
				return 0;
			}
		}
		else
		{
			__android_log_print(ANDROID_LOG_INFO, "Wrapper", "dlopen(libtier0) returned null");
			return 0;
		}
	}

	/*char dir[512];
	__android_log_print(ANDROID_LOG_INFO, "DIRECTORY", "%s", getcwd(dir, 512));*/
	return &wrapCmdLine;
}
