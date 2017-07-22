#include "wrapper.h"
#include "inputsystem/iinputsystem.h"

IInputSystem *realIS;
GET_INTERFACE_PTR( InputSystem, "libinputsystem_original.so", INPUTSYSTEM_INTERFACE_VERSION, &realIS );

class CWrapInputSystem : public IInputSystem
{
public:
	// Here's where the app systems get to learn about each other 
	virtual bool Connect( CreateInterfaceFn factory )
	{
		return realIS->Connect( factory );
	}
	virtual void Disconnect()
	{
		realIS->Disconnect();
	}

	// Here's where systems can access other interfaces implemented by this object
	// Returns NULL if it doesn't implement the requested interface
	virtual void *QueryInterface( const char *pInterfaceName )
	{
		return realIS->QueryInterface( pInterfaceName );
	}
	
	// Init, shutdown
	virtual InitReturnVal_t Init()
	{
		return realIS->Init();
	}
	virtual void Shutdown()
	{
		realIS->Shutdown();
	}

	
	// Attach, detach input system from a particular window
	// This window should be the root window for the application
	// Only 1 window should be attached at any given time.
	virtual void AttachToWindow( void* hWnd )
	{
		realIS->AttachToWindow( hWnd );
	}
	virtual void DetachFromWindow( )
	{
		realIS->DetachFromWindow();
	}

	// Enables/disables input. PollInputState will not update current 
	// button/analog states when it is called if the system is disabled.
	virtual void EnableInput( bool bEnable )
	{
		realIS->EnableInput( bEnable );
	}

	// Enables/disables the windows message pump. PollInputState will not
	// Peek/Dispatch messages if this is disabled
	virtual void EnableMessagePump( bool bEnable )
	{
		realIS->EnableMessagePump( bEnable );
	}

	// Polls the current input state
	virtual void PollInputState()
	{
		realIS->PollInputState();
	}

	// Gets the time of the last polling in ms
	virtual int GetPollTick() const
	{
		return realIS->GetPollTick();
	}

	// Is a button down? "Buttons" are binary-state input devices (mouse buttons, keyboard keys)
	virtual bool IsButtonDown( ButtonCode_t code ) const
	{
		return realIS->IsButtonDown( code );
	}

	// Returns the tick at which the button was pressed and released
	virtual int GetButtonPressedTick( ButtonCode_t code ) const
	{
		return realIS->GetButtonPressedTick( code );
	}
	virtual int GetButtonReleasedTick( ButtonCode_t code ) const
	{
		return realIS->GetButtonReleasedTick( code );
	}

	// Gets the value of an analog input device this frame
	// Includes joysticks, mousewheel, mouse
	virtual int GetAnalogValue( AnalogCode_t code ) const
	{
		return realIS->GetAnalogValue( code );
	}

	// Gets the change in a particular analog input device this frame
	// Includes joysticks, mousewheel, mouse
	virtual int GetAnalogDelta( AnalogCode_t code ) const
	{
		return realIS->GetAnalogDelta( code );
	}

	// Returns the input events since the last poll
	virtual int GetEventCount() const
	{
		return realIS->GetEventCount();
	}
	virtual const InputEvent_t* GetEventData( ) const
	{
		return realIS->GetEventData();
	}

	// Posts a user-defined event into the event queue; this is expected
	// to be called in overridden wndprocs connected to the root panel.
	virtual void PostUserEvent( const InputEvent_t &event )
	{
		realIS->PostUserEvent( event );
	}

	// Returns the number of joysticks
	virtual int GetJoystickCount() const
	{
		return realIS->GetJoystickCount();
	}

	// Enable/disable joystick, it has perf costs
	virtual void EnableJoystickInput( int nJoystick, bool bEnable )
	{
		realIS->EnableJoystickInput( nJoystick, bEnable );
	}

	// Enable/disable diagonal joystick POV (simultaneous POV buttons down)
	virtual void EnableJoystickDiagonalPOV( int nJoystick, bool bEnable )
	{
		realIS->EnableJoystickDiagonalPOV( nJoystick, bEnable );
	}

	// Sample the joystick and append events to the input queue
	virtual void SampleDevices( void )
	{
		realIS->SampleDevices( );
	}

	// FIXME: Currently force-feedback is only supported on the Xbox 360
	virtual void SetRumble( float fLeftMotor, float fRightMotor, int userId = INVALID_USER_ID )
	{
		realIS->SetRumble( fLeftMotor, fRightMotor, userId );
	}
	virtual void StopRumble( void )
	{
		realIS->StopRumble();
	}

	// Resets the input state
	virtual void ResetInputState()
	{
		realIS->ResetInputState();
	}

	// Sets a player as the primary user - all other controllers will be ignored.
	virtual void SetPrimaryUserId( int userId )
	{
		realIS->SetPrimaryUserId( userId );
	}

	// Convert back + forth between ButtonCode/AnalogCode + strings
	virtual const char *ButtonCodeToString( ButtonCode_t code ) const
	{
		return realIS->ButtonCodeToString( code );
	}
	virtual const char *AnalogCodeToString( AnalogCode_t code ) const
	{
		return realIS->AnalogCodeToString( code );
	}
	virtual ButtonCode_t StringToButtonCode( const char *pString ) const
	{
		return realIS->StringToButtonCode( pString );
	}
	virtual AnalogCode_t StringToAnalogCode( const char *pString ) const
	{
		return realIS->StringToAnalogCode( pString );
	}

	// Sleeps until input happens. Pass a negative number to sleep infinitely
	virtual void SleepUntilInput( int nMaxSleepTimeMS = -1 )
	{
		realIS->SleepUntilInput( nMaxSleepTimeMS );
	}

	// Convert back + forth between virtual codes + button codes
	// FIXME: This is a temporary piece of code
	virtual ButtonCode_t VirtualKeyToButtonCode( int nVirtualKey ) const
	{
		return realIS->VirtualKeyToButtonCode( nVirtualKey );
	}
	virtual int ButtonCodeToVirtualKey( ButtonCode_t code ) const
	{
		return realIS->ButtonCodeToVirtualKey( code );
	}
	virtual ButtonCode_t ScanCodeToButtonCode( int lParam ) const
	{
		return realIS->ScanCodeToButtonCode( lParam );
	}

	// How many times have we called PollInputState?
	virtual int GetPollCount() const
	{
		return realIS->GetPollCount();
	}

	// Sets the cursor position
	virtual void SetCursorPosition( int x, int y )
	{
		realIS->SetCursorPosition( x, y );
	}

	// NVNT get address to haptics interface
	virtual void *GetHapticsInterfaceAddress() const
	{
		return realIS->GetHapticsInterfaceAddress();
	}

	virtual void SetNovintPure( bool bPure )
	{
		realIS->SetNovintPure( bPure );
	}

	// read and clear accumulated raw input values
	virtual bool GetRawMouseAccumulators( int& accumX, int& accumY )
	{
		return realIS->GetRawMouseAccumulators( accumX, accumY );
	}

	// tell the input system that we're not a game, we're console text mode.
	// this is used for dedicated servers to not initialize joystick system.
	// this needs to be called before CInputSystem::Init (e.g. in PreInit of
	// some system) if you want ot prevent the joystick system from ever
	// being initialized.
	virtual void SetConsoleTextMode( bool bConsoleTextMode )
	{
		return SetConsoleTextMode( bConsoleTextMode );
	}
} g_InputSystem;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CWrapInputSystem, IInputSystem, 
						INPUTSYSTEM_INTERFACE_VERSION, g_InputSystem );
