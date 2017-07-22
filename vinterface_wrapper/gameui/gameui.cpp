/*
gameui.cpp -- GameUI wrap
Copyright (C) 2017 a1batross

Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/ 

#include "wrapper.h"
#include "icvar.h"
#include "IGameUI.h"
#include "convar.h"
#include <stdarg.h>

IGameUI *realGameUI;
ICvar *realCvar;
GET_INTERFACE_PTR( GameUI, "libGameUI_original.so", GAMEUI_INTERFACE_VERSION, &realGameUI );

CreateInterfaceFn appFactory_backup = NULL;

ConVar gameui_xbox_wrap( "gameui_xbox_", "0" );

class CWrapCvar : public ICvar
{
public:
	// Allocate a unique DLL identifier
	virtual CVarDLLIdentifier_t AllocateDLLIdentifier()
	{
		return realCvar->AllocateDLLIdentifier();
	}

	// Register, unregister commands
	virtual void		    RegisterConCommand( ConCommandBase *pCommandBase )
	{
		realCvar->RegisterConCommand( pCommandBase );
	}
	virtual void		    UnregisterConCommand( ConCommandBase *pCommandBase )
	{
		realCvar->UnregisterConCommand( pCommandBase );
	}
	virtual void		    UnregisterConCommands( CVarDLLIdentifier_t id )
	{
		realCvar->UnregisterConCommands( id );
	}

	// If there is a +<varname> <value> on the command line, this returns the value.
	// Otherwise, it returns NULL.
	virtual const char*	     GetCommandLineValue( const char *pVariableName )
	{
		return realCvar->GetCommandLineValue( pVariableName );
	}

	// Try to find the cvar pointer by name
	virtual ConCommandBase *FindCommandBase( const char *name )
	{
		return realCvar->FindCommandBase( name );
	}
	virtual const ConCommandBase *FindCommandBase( const char *name ) const
	{
		return realCvar->FindCommandBase( name );
	}
	virtual ConVar		  *FindVar ( const char *var_name )
	{
		// HACK: 
		if( !strcmp( var_name, "gameui_xbox" ) )
		{
			LogPrintf( "gameui_xbox was removed" );
			return &gameui_xbox_wrap;
		}
		
		return realCvar->FindVar( var_name );
	}
	virtual const ConVar    *FindVar ( const char *var_name ) const
	{
		// HACK: 
		if( !strcmp( var_name, "gameui_xbox" ) )
		{
			LogPrintf( "gameui_xbox was removed" );
			return &gameui_xbox_wrap;
		}
		
		return realCvar->FindVar( var_name );
	}
	virtual ConCommand	      *FindCommand( const char *name )
	{
		return realCvar->FindCommand( name );
	}
	virtual const ConCommand *FindCommand( const char *name ) const
	{
		return realCvar->FindCommand( name );
	}

	// Get first ConCommandBase to allow iteration
	virtual ConCommandBase  *GetCommands( void )
	{
		return realCvar->GetCommands();
	}
	virtual const ConCommandBase *GetCommands( void ) const
	{
		return realCvar->GetCommands();
	}

	// Install a global change callback (to be called when any convar changes) 
	virtual void		    InstallGlobalChangeCallback( FnChangeCallback_t callback )
	{
		realCvar->InstallGlobalChangeCallback( callback );
	}
	virtual void		    RemoveGlobalChangeCallback( FnChangeCallback_t callback )
	{
		realCvar->RemoveGlobalChangeCallback( callback );
	}
	virtual void		    CallGlobalChangeCallbacks( ConVar *var, const char *pOldString, float flOldValue )
	{
		realCvar->CallGlobalChangeCallbacks( var, pOldString, flOldValue );
	}

	// Install a console printer
	virtual void		    InstallConsoleDisplayFunc( IConsoleDisplayFunc* pDisplayFunc )
	{
		realCvar->InstallConsoleDisplayFunc( pDisplayFunc );
	}
	virtual void		    RemoveConsoleDisplayFunc( IConsoleDisplayFunc* pDisplayFunc )
	{
		realCvar->RemoveConsoleDisplayFunc( pDisplayFunc );
	}
	virtual void		    ConsoleColorPrintf( const Color& clr, PRINTF_FORMAT_STRING const char *pFormat, ... ) const
	{
		static char msg[4096];
		
		va_list args;
		
		va_start( args, pFormat );
		
		vsnprintf( msg, 4096, pFormat, args );
		
		va_end( args );
		realCvar->ConsoleColorPrintf( clr, pFormat );
	}
	virtual void		    ConsolePrintf( PRINTF_FORMAT_STRING const char *pFormat, ... ) const
	{
		static char msg[4096];
		
		va_list args;
		
		va_start( args, pFormat );
		
		vsnprintf( msg, 4096, pFormat, args );
		
		va_end( args );
		realCvar->ConsolePrintf( pFormat );
	}
	virtual void		    ConsoleDPrintf( PRINTF_FORMAT_STRING const char *pFormat, ... ) const
	{
		static char msg[4096];
		
		va_list args;
		
		va_start( args, pFormat );
		
		vsnprintf( msg, 4096, pFormat, args );
		
		va_end( args );
		realCvar->ConsolePrintf( pFormat );
	}

	// Reverts cvars which contain a specific flag
	virtual void		    RevertFlaggedConVars( int nFlag )
	{
		realCvar->RevertFlaggedConVars( nFlag );
	}

	// Method allowing the engine ICvarQuery interface to take over
	// A little hacky, owing to the fact the engine is loaded
	// well after ICVar, so we can't use the standard connect pattern
	virtual void		    InstallCVarQuery( ICvarQuery *pQuery )
	{
		realCvar->InstallCVarQuery( pQuery );
	}

	virtual bool		    IsMaterialThreadSetAllowed( ) const
	{
		return realCvar->IsMaterialThreadSetAllowed();
	}
	virtual void		    QueueMaterialThreadSetValue( ConVar *pConVar, const char *pValue )
	{
		realCvar->QueueMaterialThreadSetValue( pConVar, pValue );
	}
	virtual void		    QueueMaterialThreadSetValue( ConVar *pConVar, int nValue )
	{
		realCvar->QueueMaterialThreadSetValue( pConVar, nValue );
	}
	virtual void		    QueueMaterialThreadSetValue( ConVar *pConVar, float flValue )
	{
		realCvar->QueueMaterialThreadSetValue( pConVar, flValue );
	}
	virtual bool		    HasQueuedMaterialThreadConVarSets() const
	{
		return realCvar->HasQueuedMaterialThreadConVarSets();
	}
	virtual int			     ProcessQueuedMaterialThreadConVarSets()
	{
		return realCvar->ProcessQueuedMaterialThreadConVarSets();
	}
	
	virtual bool Connect( CreateInterfaceFn factory )
	{
		return realCvar->Connect( factory );
	}
	virtual void Disconnect()
	{
		realCvar->Disconnect();
	}
	virtual void *QueryInterface( const char *pInterfaceName )
	{
		return realCvar->QueryInterface( pInterfaceName );
	}
	virtual InitReturnVal_t Init()
	{
		return realCvar->Init();
	}
	virtual void Shutdown()
	{
		realCvar->Shutdown();
	}

protected:
	virtual ICVarIteratorInternal   *FactoryInternalIterator( void )
	{
		return (ICVarIteratorInternal*)call_FactoryInternalIterator( realCvar );
	}
} g_CvarWrap;

void* CreateInterface_wrap( const char *pname, int *pReturnCode ) // move to shared wrapper code
{
	if( !strcmp( pname, CVAR_INTERFACE_VERSION ) )
		return &g_CvarWrap;
	
	if( appFactory_backup )
		return appFactory_backup( pname, pReturnCode );
	
	return NULL;
}

class CWrapGameUI : public IGameUI 
{
public:
	// initialization/shutdown
	virtual void Initialize( CreateInterfaceFn appFactory )
		{
			realCvar = (ICvar*)appFactory( CVAR_INTERFACE_VERSION, NULL );			
			appFactory_backup = appFactory;
			
			realGameUI->Initialize( CreateInterface_wrap );
		}
	virtual void PostInit()
		{
			realGameUI->PostInit();
		}

	// connect to other interfaces at the same level (gameui.dll/server.dll/client.dll)
	virtual void Connect( CreateInterfaceFn gameFactory )
		{ 
			realGameUI->Connect( gameFactory );
		}

	virtual void Start()
		{
			realGameUI->Start();
		}
	virtual void Shutdown()
		{
			realGameUI->Shutdown();
		}
	virtual void RunFrame()
		{
			realGameUI->RunFrame();
		}

	// notifications
	virtual void OnGameUIActivated()
		{
			realGameUI->OnGameUIActivated();
		}
	virtual void OnGameUIHidden()
		{
			realGameUI->OnGameUIHidden();
		}

	// OLD: Use OnConnectToServer2
	virtual void OLD_OnConnectToServer(const char *game, int IP, int port)
		{
			realGameUI->OLD_OnConnectToServer( game, IP, port );
		} 

	virtual void OnDisconnectFromServer_OLD( uint8 eSteamLoginFailure, const char *username )
		{
			realGameUI->OnDisconnectFromServer_OLD( eSteamLoginFailure, username );
		}
	virtual void OnLevelLoadingStarted(bool bShowProgressDialog)
		{
			realGameUI->OnLevelLoadingStarted( bShowProgressDialog );
		}
	virtual void OnLevelLoadingFinished(bool bError, const char *failureReason, const char *extendedReason)
		{
			realGameUI->OnLevelLoadingFinished( bError, failureReason, extendedReason );
		}

	// level loading progress, returns true if the screen needs updating
	virtual bool UpdateProgressBar(float progress, const char *statusText)
		{
			return realGameUI->UpdateProgressBar( progress, statusText );
		}
	// Shows progress desc, returns previous setting... (used with custom progress bars )
	virtual bool SetShowProgressText( bool show )
		{
			return realGameUI->SetShowProgressText( show );
		}

	// !!!!!!!!!members added after "GameUI011" initial release!!!!!!!!!!!!!!!!!!!
	virtual void ShowNewGameDialog( int chapter )
		{ 
			realGameUI->ShowNewGameDialog( chapter );
		}

	// Xbox 360
	virtual void SessionNotification( const int notification, const int param )
		{
			realGameUI->SessionNotification( notification, param );
		}
	virtual void SystemNotification( const int notification )
		{
			realGameUI->SystemNotification( notification );
		}
	virtual void ShowMessageDialog( const uint nType, vgui::Panel *pOwner )
		{
			realGameUI->ShowMessageDialog( nType, pOwner );
		}
	virtual void UpdatePlayerInfo( uint64 nPlayerId, const char *pName, int nTeam, byte cVoiceState, int nPlayersNeeded, bool bHost )
		{
			realGameUI->UpdatePlayerInfo( nPlayerId, pName, nTeam, cVoiceState, nPlayersNeeded, bHost );
		}
	virtual void SessionSearchResult( int searchIdx, void *pHostData, XSESSION_SEARCHRESULT *pResult, int ping )
		{
			realGameUI->SessionSearchResult( searchIdx, pHostData, pResult, ping );
		}
	virtual void OnCreditsFinished( void )
		{
			realGameUI->OnCreditsFinished();
		}

	// inserts specified panel as background for level load dialog
	virtual void SetLoadingBackgroundDialog( vgui::VPANEL panel )
		{
			realGameUI->SetLoadingBackgroundDialog( panel );
		}

	// Bonus maps interfaces
	virtual void BonusMapUnlock( const char *pchFileName, const char *pchMapName )
		{
			realGameUI->BonusMapUnlock( pchFileName, pchMapName );
		}
	virtual void BonusMapComplete( const char *pchFileName, const char *pchMapName )
		{
			realGameUI->BonusMapComplete( pchFileName, pchMapName );
		}
	virtual void BonusMapChallengeUpdate( const char *pchFileName, const char *pchMapName, const char *pchChallengeName, int iBest )
		{
			realGameUI->BonusMapChallengeUpdate( pchFileName, pchMapName, pchChallengeName, iBest );
		}
	virtual void BonusMapChallengeNames( char *pchFileName, char *pchMapName, char *pchChallengeName )
		{
			realGameUI->BonusMapChallengeNames( pchFileName, pchMapName, pchChallengeName );
		}
	virtual void BonusMapChallengeObjectives( int &iBronze, int &iSilver, int &iGold )
		{
			realGameUI->BonusMapChallengeObjectives( iBronze, iSilver, iGold );
		}
	virtual void BonusMapDatabaseSave( void )
		{
			realGameUI->BonusMapDatabaseSave();
		}
	virtual int BonusMapNumAdvancedCompleted( void )
		{
			return realGameUI->BonusMapNumAdvancedCompleted();
		}
	virtual void BonusMapNumMedals( int piNumMedals[ 3 ] )
		{
			realGameUI->BonusMapNumMedals( piNumMedals );
		}

	virtual void OnConnectToServer2(const char *game, int IP, int connectionPort, int queryPort)
		{
			realGameUI->OnConnectToServer2( game, IP, connectionPort, queryPort );
		}

	// X360 Storage device validation:
	//	      returns true right away if storage device has been previously selected.
	//	      otherwise returns false and will set the variable pointed by pStorageDeviceValidated to 1
	//				once the storage device is selected by user.
	virtual bool ValidateStorageDevice( int *pStorageDeviceValidated )
		{
			return realGameUI->ValidateStorageDevice( pStorageDeviceValidated );
		}

	virtual void SetProgressOnStart()
		{
			realGameUI->SetProgressOnStart();
		}
	virtual void OnDisconnectFromServer( uint8 eSteamLoginFailure )
		{
			realGameUI->OnDisconnectFromServer( eSteamLoginFailure );
		}

	virtual void OnConfirmQuit( void )
		{
			realGameUI->OnConfirmQuit();
		}

	virtual bool IsMainMenuVisible( void )
		{
			return realGameUI->IsMainMenuVisible();
		}

	// Client DLL is providing us with a panel that it wants to replace the main menu with
	virtual void SetMainMenuOverride( vgui::VPANEL panel )
		{
			realGameUI->SetMainMenuOverride( panel );
		}
	// Client DLL is telling us that a main menu command was issued, probably from its custom main menu panel
	virtual void SendMainMenuCommand( const char *pszCommand )
		{
			realGameUI->SendMainMenuCommand( pszCommand );
		}
} g_GameUI;
IGameUI *g_pGameUI = &g_GameUI;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CWrapGameUI, IGameUI, GAMEUI_INTERFACE_VERSION, g_GameUI );
