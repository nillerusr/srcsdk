/*
gameinterface.cpp -- Server wrapper
Copyright (C) 2017 a1batross
Modified for server wrapper, there's no additional copyrights.
Modded by [-=KILL MAN=-] aka RusJJ.

Licensed under WTFPL license.
Thanks to Valve for SDK, to me for an idea. 
Please, don't punish, Mr. Newell. :)
*/ 

#include "wrapper.h"
#include "eiface.h"
#include "filesystem.h"

IServerGameDLL *realServerDLL;

IVEngineServer	*engine = NULL;
IFileSystem *filesystem = NULL; // Looks like this is useless. Can be removed?

GET_INTERFACE_PTR( Server, getenv("LIBRARY_SERVER"), INTERFACEVERSION_SERVERGAMEDLL, &realServerDLL );

class CWrapServer : public IServerGameDLL
{
public:
	virtual bool DLLInit( CreateInterfaceFn engineFactory, CreateInterfaceFn physicsFactory, CreateInterfaceFn fileSystemFactory, CGlobalVars *pGlobals)
	{
		if ( (engine = (IVEngineServer*)engineFactory( INTERFACEVERSION_VENGINESERVER, NULL )) == NULL )
			return false;
		if ( (filesystem = (IFileSystem *)engineFactory( FILESYSTEM_INTERFACE_VERSION, NULL)) == NULL )
			return false;

		return realServerDLL->DLLInit( engineFactory, physicsFactory, fileSystemFactory, pGlobals );
	}

	virtual bool ReplayInit( CreateInterfaceFn fnReplayFactory )
	{
		return realServerDLL->ReplayInit( fnReplayFactory );
	}
	
	virtual bool GameInit( void )
	{
		return realServerDLL->GameInit();
	}
	
	virtual bool LevelInit( char const *pMapName, char const *pMapEntities, char const *pOldLevel, 
							char const *pLandmarkName, bool loadGame, bool background )
	{
		return realServerDLL->LevelInit( pMapName, pMapEntities, pOldLevel, pLandmarkName, loadGame, background );
	}
	
	virtual void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
	{
		realServerDLL->ServerActivate( pEdictList, edictCount, clientMax );
	}
	
	virtual void GameFrame( bool simulating )
	{
		realServerDLL->GameFrame( simulating );
	}
	
	virtual void PreClientUpdate( bool simulating )
	{
		realServerDLL->PreClientUpdate( simulating );
	}
	
	virtual void LevelShutdown( void )
	{
		realServerDLL->LevelShutdown();
	}
	
	virtual void GameShutdown( void )
	{
		realServerDLL->GameShutdown();
	}
	
	virtual void DLLShutdown( void )
	{
		realServerDLL->DLLShutdown();
	}
	
	virtual float GetTickInterval( void ) const
	{
		return realServerDLL->GetTickInterval();
	}
	
	virtual ServerClass* GetAllServerClasses( void )
	{
		return realServerDLL->GetAllServerClasses();
	}
	
	virtual const char *GetGameDescription( void )
	{
		return realServerDLL->GetGameDescription();
	}
	
	virtual void CreateNetworkStringTables( void )
	{
		realServerDLL->CreateNetworkStringTables();
	}
	
	virtual CSaveRestoreData *SaveInit( int size )
	{
		return realServerDLL->SaveInit( size );
	}
	
	virtual void SaveWriteFields( CSaveRestoreData * d1, const char *d2, void *d3, datamap_t *d4, typedescription_t *d5, int d6 )
	{
		realServerDLL->SaveWriteFields( d1, d2, d3, d4, d5, d6 );
	}
	
	virtual void SaveReadFields( CSaveRestoreData * d1, const char *d2, void *d3, datamap_t *d4, typedescription_t *d5, int d6 )
	{
		realServerDLL->SaveReadFields( d1, d2, d3, d4, d5, d6 );
	}
	
	virtual void SaveGlobalState( CSaveRestoreData *d1 )
	{
		realServerDLL->SaveGlobalState( d1 );
	}
	
	virtual void RestoreGlobalState( CSaveRestoreData *d1 )
	{
		realServerDLL->RestoreGlobalState( d1 );
	}
	
	virtual void PreSave( CSaveRestoreData *d1 )
	{
		realServerDLL->PreSave( d1 );
	}
	
	virtual void Save( CSaveRestoreData *d1 )
	{
		realServerDLL->Save( d1 );
	}
	
	virtual void GetSaveComment( char *comment, int maxlength, float flMinutes, float flSeconds, bool bNoTime = false )
	{
		realServerDLL->GetSaveComment( comment, maxlength, flMinutes, flSeconds, bNoTime );
	}
	
	virtual void WriteSaveHeaders( CSaveRestoreData *d1 )
	{
		realServerDLL->WriteSaveHeaders( d1 );
	}
	
	virtual void ReadRestoreHeaders( CSaveRestoreData *d1 )
	{
		realServerDLL->ReadRestoreHeaders( d1 );
	}
	
	virtual void Restore( CSaveRestoreData *d1, bool d2 )
	{
		realServerDLL->Restore( d1, d2 );
	}
	
	virtual bool IsRestoring()
	{
		return realServerDLL->IsRestoring();
	}
	
	virtual int CreateEntityTransitionList( CSaveRestoreData *d1, int d2 )
	{
		return realServerDLL->CreateEntityTransitionList( d1, d2 );
	}
	
	virtual void BuildAdjacentMapList( void )
	{
		realServerDLL->BuildAdjacentMapList();
	}
	
	virtual bool GetUserMessageInfo( int msg_type, char *name, int maxnamelength, int& size )
	{
		return realServerDLL->GetUserMessageInfo( msg_type, name, maxnamelength, size );
	}
	
	virtual CStandardSendProxies* GetStandardSendProxies()
	{
		return realServerDLL->GetStandardSendProxies();
	}
	
	virtual void PostInit()
	{
		realServerDLL->PostInit();
	}
	
	virtual void Think( bool finalTick )
	{
		realServerDLL->Think( finalTick );
	}
	
#ifdef _XBOX // Why not?
	virtual void GetTitleName( const char *pMapName, char* pTitleBuff, int titleBuffSize )
	{
		realServerDLL->GetTitleName( pMapName, pTitleBuff, titleBuffSize );
	}
#endif

	virtual void PreSaveGameLoaded( char const *pSaveName, bool bCurrentlyInGame )
	{
		realServerDLL->PreSaveGameLoaded( pSaveName, bCurrentlyInGame );
	}
	
	virtual bool ShouldHideServer( void )
	{
		return realServerDLL->ShouldHideServer();
	}
	
	virtual void InvalidateMdlCache()
	{
		realServerDLL->InvalidateMdlCache();
	}
	
	virtual void OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
	{
		realServerDLL->OnQueryCvarValueFinished( iCookie, pPlayerEntity, eStatus, pCvarName, pCvarValue );
	}
	
	virtual void GameServerSteamAPIActivated( void )
	{
		realServerDLL->GameServerSteamAPIActivated();
	}
	
	virtual void GameServerSteamAPIShutdown( void )
	{
		realServerDLL->GameServerSteamAPIShutdown();
	}
	
	virtual void SetServerHibernation( bool bHibernating )
	{
		realServerDLL->SetServerHibernation( bHibernating );
	}
	
	virtual IServerGCLobby *GetServerGCLobby()
	{
		return realServerDLL->GetServerGCLobby();
	}
	
	virtual const char *GetServerBrowserMapOverride()
	{
		return realServerDLL->GetServerBrowserMapOverride();
	}
	
	virtual const char *GetServerBrowserGameData()
	{
		return realServerDLL->GetServerBrowserGameData();
	}
};

CWrapServer gWrapServer;
IServerGameDLL *serverdll = &gWrapServer;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CWrapServer, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL, gWrapServer );

