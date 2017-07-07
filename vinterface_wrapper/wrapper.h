/*
wrapper.cpp -- an ultimate VInterface wrapper for Source Tegra port
Copyright (C) 2017 a1batross

Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/

#pragma once

#include <stdlib.h>

#ifdef ANDROID
#include <dlfcn.h>
#include <android/log.h>
#include <stdio.h>
#define TAG "SourceSDK2013"
#define PRIO ANDROID_LOG_DEBUG
#define LogPrintf(...) do { __android_log_print(PRIO, TAG, __VA_ARGS__); printf( __VA_ARGS__); } while( 0 );
#else
#include <stdio.h>
#define LogPrintf(...) printf(__VA_ARGS__)
#endif

// don't link tier1
#include "interface.h"

#undef DLLEXPORT
#define DLLEXPORT extern "C" __attribute__((visibility("default")))

class Module
{
public:
	Module() : m_szName(NULL), m_pLibrary(NULL), CreateInterface(NULL) {}
	~Module() { Unload(); }
	
	bool Load(const char *name, bool isModule = true)
	{
		m_szName = name;
#ifdef POSIX
		m_pLibrary = dlopen(name, RTLD_NOW);
#else
#error "Port me"
#endif
		if( !m_pLibrary )
		{
			LogPrintf( "Module::Load: Failed to load %s: %s", name, dlerror() );
			return false;
		}
		
		
		if( isModule )
		{
			CreateInterface = (void*(*)(const char*, int *))Resolve( "CreateInterface" );
			if( !CreateInterface )
			{
				return false;
			}
		}
		
		return true;
	}
	
	void Unload()
	{
		if( m_pLibrary )
		{
#ifdef POSIX
			dlclose( m_pLibrary );
#else
#error "Port me"
#endif
		}
		
		m_pLibrary = NULL;
		CreateInterface = NULL;
		m_szName = NULL;
	}
	
	IBaseInterface *GetInterface( const char *pName, int *pReturnCode ) const
	{
		if( CreateInterface )
			return (IBaseInterface*)CreateInterface( pName, pReturnCode );
		
		return NULL;
	}
	
	void *Resolve(const char *name) const
	{
#ifdef POSIX
		void *ret = dlsym( m_pLibrary, name );
#else
#error "Port me"
#endif
		if( !ret )
		{
			LogPrintf( "Module::Resolve: Failed to resolve %s from %s: %s", name, m_szName, dlerror() );
		}
		return ret;
	}
	
	const char *Name( void ) const
	{
		return m_szName;
	}
	
	const void *GetHandle( void ) const
	{
		return m_pLibrary;
	}
	
private:
	const char *m_szName;
	void *m_pLibrary;
	void *(*CreateInterface)(const char *pName, int *pReturnCode );
};

struct Loader
{
	Loader( void );
	~Loader( void );
	
	void LoadAllLibraries( void );
	
	struct LoaderPrivate *p;
};
extern struct Loader g_Loader;

typedef void (*WrapInitFn)( Module *module );

// Used to wrap classes.
struct WrapInterfaceReg
{
	WrapInterfaceReg(const char *pModuleName, const char *pName, IBaseInterface **iface, WrapInitFn fn = NULL, WrapInitFn fnDestruct = NULL );
	WrapInterfaceReg(const char *pModuleName, bool isModule = true, WrapInitFn fn = NULL, WrapInitFn fnDestruct = NULL );
	~WrapInterfaceReg();

	IBaseInterface			**m_pIface;
	const char 				*m_szModuleName;
	const char	 			*m_szName;
	bool					m_bIsModule;
	WrapInitFn				m_pfn;
	WrapInitFn				m_pfnDestruct;

	WrapInterfaceReg		*m_pNext; // For the global list.
	static WrapInterfaceReg	*s_pInterfaceRegs;
};

#define VA_ARGS(...) , ##__VA_ARGS__

#define GET_INTERFACE_PTR(szReadableName, pModule, pName, iface, ...) \
	static WrapInterfaceReg __g_Wrap##szReadableName##_reg((pModule), (pName), (IBaseInterface**)(iface) VA_ARGS(__VA_ARGS__) )
#define WRAP_LIBRARY( szReadableName, pModule, ... )	static WrapInterfaceReg __g_Wrap##szReadableName##_reg(pModule, false VA_ARGS(__VA_ARGS__) )
#define WRAP_MODULE( szReadableName, pModule, ... )	static WrapInterfaceReg __g_Wrap##szReadableName##_reg(pModule, true VA_ARGS(__VA_ARGS__) )
