/*
wrapper.cpp -- an ultimate VInterface wrapper for Source Tegra port
Copyright (C) 2017 a1batross

Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/

#include "wrapper.h"

struct Loader  g_Loader;
InterfaceReg *InterfaceReg::s_pInterfaceRegs = NULL;
WrapInterfaceReg *WrapInterfaceReg::s_pInterfaceRegs = NULL;

InterfaceReg::InterfaceReg( InstantiateInterfaceFn fn, const char *pName ) :
	m_pName(pName)
{
	m_CreateFn = fn;
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}

WrapInterfaceReg::WrapInterfaceReg( const char *pModuleName, const char *pName, IBaseInterface **iface ) :
	m_szModuleName( pModuleName ), m_szName( pName ), m_pIface( iface ), m_bIsModule( true )
{
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}

WrapInterfaceReg::WrapInterfaceReg( const char *pModuleName, bool bIsModule ) :
	m_szModuleName( pModuleName ), m_szName( NULL ), m_pIface( NULL ), m_bIsModule( bIsModule )
{
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}

/*void (*realSDL_AddEventWatch)(void *filter, void *userdata);
void (*realSDL_DelEventWatch)(void *filter, void *userdata);
int Wrapper_EventFilter( void *userdata, void *event )
{
	LogPrintf( "Got event\n" );
	
	return 0;
}*/

Loader::Loader(void)
{
	/*if( !SDL2.Load("libSDL2.so", false) ) 
		abort();
	
	realSDL_AddEventWatch = (void(*)(void*, void*))SDL2.Resolve("SDL_AddEventWatch");
	realSDL_DelEventWatch = (void(*)(void*, void*))SDL2.Resolve("SDL_DelEventWatch");
	
	realSDL_AddEventWatch( (void*)Wrapper_EventFilter, NULL );*/

	for( WrapInterfaceReg *pCur = WrapInterfaceReg::s_pInterfaceRegs; pCur; pCur = pCur->m_pNext )
	{
		Module *module;
		
		// check for twice added modules
		int i = 0;
		for( ; i < modules.Count(); i++ )
			if( !strcmp( modules[i]->Name(), pCur->m_szModuleName ) )
				break;
			
		if( i == modules.Count() )
		{
			module = new Module();
			module->Load( pCur->m_szModuleName, pCur->m_bIsModule );
			modules.AddToTail(module);
		}
		else
		{
			module = modules[i];
		}
		
		if( pCur->m_pIface )
		{
			if( ( *(pCur->m_pIface) = module->GetInterface( pCur->m_szName, NULL ) ) )
			{
				LogPrintf( "Got interface pointer to %s from %s", pCur->m_szName, module->Name() );
			}
		}
	}
}

Loader::~Loader( void )
{
	// realSDL_DelEventWatch( (void*)Wrapper_EventFilter, NULL );
	for( int i = 0; i < g_Loader.modules.Count(); i++ )
		delete g_Loader.modules[i];
}

extern "C" __attribute__((visiblity("default"))) void* CreateInterface( const char *pName, int *pReturnCode )
{
	void *ret;
	LogPrintf("Requested Interface: %s", pName);
	
	for(InterfaceReg *pCur = InterfaceReg::s_pInterfaceRegs; pCur; pCur=pCur->m_pNext)
	{
		if( !strcmp(pCur->m_pName, pName) )
		{
			LogPrintf( "Overriden." );
			if (pReturnCode) *pReturnCode = IFACE_OK;
			return pCur->m_CreateFn();
		}
	}
	
	for(int i = 0; i < g_Loader.modules.Count(); i++ )
	{
		ret = g_Loader.modules[i]->GetInterface( pName, pReturnCode );
		if( ret )
		{
			LogPrintf( "Found in %s", g_Loader.modules[i]->Name() );
			return ret;
		}
	}
	
	LogPrintf( "Interface %s not found", pName );
	if( pReturnCode ) *pReturnCode = IFACE_FAILED;
	
	return NULL;
}



