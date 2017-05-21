/*
cdll_int.cpp -- VClient017 wrap
Copyright (C) 2017 a1batross

Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/ 

#include "wrapper.h"
#include "cdll_int.h"
#include "ienginevgui.h"
#include "inputsystem/iinputsystem.h"
#include "IGameUIFuncs.h"
#include "filesystem.h"

IBaseClientDLL *realClientDLL;
// we don't provide own full implementation of VClient017, so we need wrap it
// we also provide ourselves as "libclient.so", so real library was moved to "libclient_original.so"
GET_INTERFACE_PTR( "libclient_original.so", CLIENT_DLL_INTERFACE_VERSION, &realClientDLL );

// event catching
WRAP_LIBRARY( SDL2, "libSDL2.so" );

// this is required when wrapping client for some reason
WRAP_MODULE( GameUI, "libGameUI.so" ); 

IVEngineClient	*engine = NULL;
IFileSystem *filesystem = NULL;
IEngineVGui *enginevgui = NULL;
IGameUIFuncs *gameuifuncs = NULL;
IInputSystem *inputsystem = NULL;

class CWrapClient : public IBaseClientDLL
{
public:
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
		
		
		return realClientDLL->Init( appSystemFactory, physicsFactory, pGlobals );
	}

	virtual void					PostInit()
	{
		realClientDLL->PostInit();
	}
	
	virtual void					Shutdown( void )
	{
		realClientDLL->Shutdown();
	}

	virtual bool					ReplayInit( CreateInterfaceFn fnReplayFactory )
	{
		return realClientDLL->ReplayInit(fnReplayFactory);
	}
	virtual bool					ReplayPostInit()
	{
		return realClientDLL->ReplayPostInit();
	}

	virtual void					LevelInitPreEntity( const char *pMapName )
	{
		realClientDLL->LevelInitPreEntity( pMapName );
	}
	
	virtual void					LevelInitPostEntity()
	{
		realClientDLL->LevelInitPostEntity();
	}
	
	virtual void					LevelShutdown( void )
	{
		realClientDLL->LevelShutdown();
	}

	virtual ClientClass				*GetAllClasses( void )
	{
		return realClientDLL->GetAllClasses();
	}

	virtual int						HudVidInit( void )
	{
		return realClientDLL->HudVidInit();
	}
	
	virtual void					HudProcessInput( bool bActive )
	{
		return realClientDLL->HudProcessInput( bActive );
	}
	
	virtual void					HudUpdate( bool bActive )
	{
		return realClientDLL->HudUpdate( bActive );
	}
	
	virtual void					HudReset( void ) 
	{
		realClientDLL->HudReset();
	}
	
	virtual void					HudText( const char * message )
	{
		realClientDLL->HudText( message );
	}

	// Mouse Input Interfaces
	virtual void					IN_ActivateMouse( void )
	{
		realClientDLL->IN_ActivateMouse();
	}
	virtual void					IN_DeactivateMouse( void )
	{
		realClientDLL->IN_DeactivateMouse();
	}
	virtual void					IN_Accumulate( void )
	{
		realClientDLL->IN_Accumulate();
	}
	virtual void					IN_ClearStates( void )
	{
		realClientDLL->IN_ClearStates();
	}
	
	virtual bool					IN_IsKeyDown( const char *name, bool& isdown )
	{
		return realClientDLL->IN_IsKeyDown( name, isdown );
	}
	
	virtual void					IN_OnMouseWheeled( int nDelta )
	{
		realClientDLL->IN_OnMouseWheeled( nDelta );
	}
	
	// Raw signal
	virtual int						IN_KeyEvent( int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding )
	{
		return realClientDLL->IN_KeyEvent( eventcode, keynum, pszCurrentBinding );
	}
	virtual void					IN_SetSampleTime( float frametime )
	{
		realClientDLL->IN_SetSampleTime( frametime );
	}
	
	// Create movement command
	virtual void					CreateMove ( int sequence_number, float input_sample_frametime, bool active )
	{
		realClientDLL->CreateMove( sequence_number, input_sample_frametime, active );
	}
	
	virtual void					ExtraMouseSample( float frametime, bool active )
	{
		realClientDLL->ExtraMouseSample(frametime, active);
	}
	virtual bool					WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand )
	{
		return realClientDLL->WriteUsercmdDeltaToBuffer( buf,from, to, isnewcommand );
	}
	virtual void					EncodeUserCmdToBuffer( bf_write& buf, int slot )
	{
		realClientDLL->EncodeUserCmdToBuffer( buf, slot );
	}
	virtual void					DecodeUserCmdFromBuffer( bf_read& buf, int slot )
	{
		realClientDLL->DecodeUserCmdFromBuffer( buf, slot );
	}

	virtual void					View_Render( vrect_t *rect )
	{
		realClientDLL->View_Render( rect );
	}
	virtual void					RenderView( const CViewSetup &view, int nClearFlags, int whatToDraw )
	{
		realClientDLL->RenderView( view, nClearFlags, whatToDraw );
	}
	
	virtual void					View_Fade( ScreenFade_t *pSF )
	{
		realClientDLL->View_Fade( pSF );
	}
	
	virtual void					SetCrosshairAngle( const QAngle& angle )
	{
		realClientDLL->SetCrosshairAngle( angle );
	}

	virtual void					InitSprite( CEngineSprite *pSprite, const char *loadname )
	{
		realClientDLL->InitSprite( pSprite, loadname );
	}
	
	virtual void					ShutdownSprite( CEngineSprite *pSprite )
	{
		realClientDLL->ShutdownSprite( pSprite );
	}

	virtual int						GetSpriteSize( void ) const
	{
		return realClientDLL->GetSpriteSize();
	}

	virtual void					VoiceStatus( int entindex, qboolean bTalking )
	{
		realClientDLL->VoiceStatus( entindex, bTalking );
	}

	virtual void					InstallStringTableCallback( const char *tableName )
	{
		realClientDLL->InstallStringTableCallback( tableName );
	}

	virtual void					FrameStageNotify( ClientFrameStage_t curStage )
	{
		realClientDLL->FrameStageNotify( curStage );
	}

	virtual bool					DispatchUserMessage( int msg_type, bf_read &msg_data )
	{
		return realClientDLL->DispatchUserMessage( msg_type, msg_data );
	}

	// Save/restore system hooks
	virtual CSaveRestoreData  *SaveInit( int size )
	{
		return realClientDLL->SaveInit( size );
	}
	virtual void			SaveWriteFields( CSaveRestoreData *a, const char *b, void *c, datamap_t *d, typedescription_t *e, int f )
	{
		realClientDLL->SaveWriteFields( a, b, c, d, e, f );
	}
	virtual void			SaveReadFields( CSaveRestoreData *a, const char *b, void *c, datamap_t *d, typedescription_t *e, int f )
	{
		realClientDLL->SaveReadFields( a, b, c, d, e, f );
	}
	virtual void			PreSave( CSaveRestoreData *a )
	{
		realClientDLL->PreSave( a );
	}
	virtual void			Save( CSaveRestoreData *a )
	{
		realClientDLL->Save( a );
	}
	virtual void			WriteSaveHeaders( CSaveRestoreData *a )
	{
		realClientDLL->WriteSaveHeaders( a );
	}
	virtual void			ReadRestoreHeaders( CSaveRestoreData *a )
	{
		realClientDLL->ReadRestoreHeaders( a );
	}
	virtual void			Restore( CSaveRestoreData *a, bool b )
	{
		realClientDLL->Restore( a, b );
	}
	virtual void			DispatchOnRestore()
	{
		realClientDLL->DispatchOnRestore();
	}
	virtual void			WriteSaveGameScreenshot( const char *pFilename )
	{
		realClientDLL->WriteSaveGameScreenshot( pFilename );
	}

	// Given a list of "S(wavname) S(wavname2)" tokens, look up the localized text and emit
	//  the appropriate close caption if running with closecaption = 1
	virtual void			EmitSentenceCloseCaption( char const *tokenstream )
	{
		realClientDLL->EmitSentenceCloseCaption( tokenstream );
	}
	virtual void			EmitCloseCaption( char const *captionname, float duration )
	{
		realClientDLL->EmitCloseCaption( captionname, duration );
	}

	virtual CStandardRecvProxies* GetStandardRecvProxies()
	{
		return realClientDLL->GetStandardRecvProxies();
	}

	virtual bool			CanRecordDemo( char *errorMsg, int length ) const
	{
		return realClientDLL->CanRecordDemo( errorMsg, length );
	}

	virtual void			OnDemoRecordStart( char const* pDemoBaseName )
	{
		realClientDLL->OnDemoRecordStart( pDemoBaseName );
	}
	virtual void			OnDemoRecordStop()
	{
		realClientDLL->OnDemoRecordStop();
	}
	virtual void			OnDemoPlaybackStart( char const* pDemoBaseName )
	{
		realClientDLL->OnDemoPlaybackStart( pDemoBaseName );
	}
	virtual void			OnDemoPlaybackStop()
	{
		realClientDLL->OnDemoPlaybackStop();
	}

	virtual bool			ShouldDrawDropdownConsole()
	{
		return realClientDLL->ShouldDrawDropdownConsole();
	}

	// Get client screen dimensions
	virtual int				GetScreenWidth()
	{
		return realClientDLL->GetScreenWidth();
	}
	virtual int				GetScreenHeight()
	{
		return realClientDLL->GetScreenHeight();
	}

	// save game screenshot writing
	virtual void			WriteSaveGameScreenshotOfSize( const char *pFilename, int width, int height, bool bCreatePowerOf2Padded/*=false*/, bool bWriteVTF/*=false*/ )
	{
		realClientDLL->WriteSaveGameScreenshotOfSize( pFilename, width, height, bCreatePowerOf2Padded, bWriteVTF );
	}

	// Gets the location of the player viewpoint
	virtual bool			GetPlayerView( CViewSetup &playerView )
	{
		return realClientDLL->GetPlayerView( playerView );
	}

	// Matchmaking
	virtual void			SetupGameProperties( CUtlVector< XUSER_CONTEXT > &contexts, CUtlVector< XUSER_PROPERTY > &properties )
	{
		realClientDLL->SetupGameProperties( contexts, properties );
	}
	virtual uint			GetPresenceID( const char *pIDName )
	{
		return realClientDLL->GetPresenceID( pIDName );
	}
	virtual const char		*GetPropertyIdString( const uint id )
	{
		return realClientDLL->GetPropertyIdString( id );
	}
	virtual void			GetPropertyDisplayString( uint id, uint value, char *pOutput, int nBytes )
	{
		return realClientDLL->GetPropertyDisplayString( id, value, pOutput, nBytes );
	}
	virtual void			StartStatsReporting( HANDLE handle, bool bArbitrated )
	{
		realClientDLL->StartStatsReporting( handle, bArbitrated );
	}

	virtual void			InvalidateMdlCache()
	{
		realClientDLL->InvalidateMdlCache();
	}

	virtual void			ReloadFilesInList( IFileList *pFilesToReload )
	{
		realClientDLL->ReloadFilesInList( pFilesToReload );
	}

	// Let the client handle UI toggle - if this function returns false, the UI will toggle, otherwise it will not.
	virtual bool			HandleUiToggle()
	{
		return realClientDLL->HandleUiToggle();
	}

	// Allow the console to be shown?
	virtual bool			ShouldAllowConsole()
	{
		return realClientDLL->ShouldAllowConsole();
	}

	// Get renamed recv tables
	virtual CRenamedRecvTableInfo	*GetRenamedRecvTableInfos()
	{
		return realClientDLL->GetRenamedRecvTableInfos();
	}

	// Get the mouthinfo for the sound being played inside UI panels
	virtual CMouthInfo		*GetClientUIMouthInfo()
	{
		return realClientDLL->GetClientUIMouthInfo();
	}

	// Notify the client that a file has been received from the game server
	virtual void			FileReceived( const char * fileName, unsigned int transferID )
	{
		realClientDLL->FileReceived( fileName, transferID );
	}

	virtual const char* TranslateEffectForVisionFilter( const char *pchEffectType, const char *pchEffectName )
	{
		return realClientDLL->TranslateEffectForVisionFilter( pchEffectType, pchEffectName );
	}
	
	virtual void			ClientAdjustStartSoundParams( struct StartSoundParams_t& params )
	{
		realClientDLL->ClientAdjustStartSoundParams( params );
	}
	
	// Returns true if the disconnect command has been handled by the client
	virtual bool DisconnectAttempt( void )
	{
		return realClientDLL->DisconnectAttempt();
	}
	
	virtual bool IsConnectedUserInfoChangeAllowed( IConVar *pCvar )
	{
		return realClientDLL->IsConnectedUserInfoChangeAllowed( pCvar );
	}
};

CWrapClient gWrapClient;
IBaseClientDLL *clientdll = &gWrapClient;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CWrapClient, IBaseClientDLL, CLIENT_DLL_INTERFACE_VERSION, gWrapClient );


