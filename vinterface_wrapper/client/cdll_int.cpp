/*
cdll_int.cpp -- VClient017 wrap
Copyright (C) 2017 a1batross

Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/ 

#include "client.h"
#include "tier1.h"
#include "inputsystem/ButtonCode.h"

IBaseClientDLL *realClientDLL;
// we don't provide own full implementation of VClient017, so we need wrap it
// we also provide ourselves as "libclient.so", so real library was moved to "libclient_original.so"
GET_INTERFACE_PTR( Client, "libclient_original.so", CLIENT_DLL_INTERFACE_VERSION, &realClientDLL );

IVEngineClient	*engine = NULL;
IFileSystem *filesystem = NULL;
IEngineVGui *enginevgui = NULL;
IGameUIFuncs *gameuifuncs = NULL;
IInputSystem *inputsystem = NULL;
IGameUI *gameui = NULL;

GET_INTERFACE_PTR( IGameUI, "libGameUI.so", GAMEUI_INTERFACE_VERSION, &gameui );

class CWrapClient : public IBaseClientDLL
{
public:
	void **backup;
	size_t backupSize;
	
	virtual int						Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals )
	{
		if ( (engine = (IVEngineClient *)appSystemFactory( VENGINE_CLIENT_INTERFACE_VERSION, NULL )) == NULL )
			return false;
		if ( (enginevgui = (IEngineVGui *)appSystemFactory(VENGINE_VGUI_VERSION, NULL )) == NULL )
			return false;
		if ( (filesystem = (IFileSystem *)appSystemFactory(FILESYSTEM_INTERFACE_VERSION, NULL)) == NULL )
			return false;
		if ( (gameuifuncs = (IGameUIFuncs * )appSystemFactory( VENGINE_GAMEUIFUNCS_VERSION, NULL )) == NULL )
			return false;
		if ( (inputsystem = (IInputSystem *)appSystemFactory(INPUTSYSTEM_INTERFACE_VERSION, NULL)) == NULL )
			return false;
		
		ConnectTier1Libraries( &appSystemFactory, 1 );
		
		LogPrintf( "Hello from CWrapClient. Running on %s, ver. %i", engine->GetProductVersionString(), engine->GetEngineBuildNumber() );
		engine->ClientCmd("con_enable 1");
		engine->ClientCmd("developer 1");
		
		g_Touch.Init();
		
		int ret = realClientDLL->Init( appSystemFactory, physicsFactory, pGlobals );
		
		ConVar_Register( FCVAR_CLIENTDLL );
		
		return ret;
	}
	
	virtual void					Shutdown( void )
	{
		g_Touch.Shutdown();
		realClientDLL->Shutdown();
		ConVar_Unregister();
		DisconnectTier1Libraries();
	}
	
	virtual void					HudProcessInput( bool bActive )
	{
		Android_RunEvents(); // called every frame, regardless is active client or not
		g_Touch.Frame();
		return realClientDLL->HudProcessInput( bActive );
	}
} gWrapClient;
IBaseClientDLL *clientdll = &gWrapClient;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CWrapClient, IBaseClientDLL, CLIENT_DLL_INTERFACE_VERSION, gWrapClient );
