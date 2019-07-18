/*
client.h -- client wrapper
Copyright (C) 2017 a1batross

Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/ 
#pragma once

#include <utllinkedlist.h>

#include "wrapper.h"
#include "cdll_int.h"
#include "ienginevgui.h"
#include "inputsystem/iinputsystem.h"
#include "IGameUIFuncs.h"
#include "filesystem.h"
#include "IGameUI.h"
#include "vgui/ISurface.h"
#include "vgui/IInputInternal.h"
#include "vgui/IScheme.h"
#include "vgui/KeyCode.h"
#include "OverlayPanel.h"

extern IBaseClientDLL *realClientDLL; // real client implementation
extern IBaseClientDLL *wrapClientDLL; // wrapper class
#define CMD_SIZE 64

// NULL until client is initialized
extern IVEngineClient	*engine;
extern IFileSystem		*filesystem;
extern IEngineVGui		*enginevgui;
extern IGameUIFuncs		*gameuifuncs;
extern IInputSystem		*inputsystem;
extern IGameUI			*gameui;
extern vgui::ISurface		*g_pSurface;
extern vgui::IScheme 		*g_pScheme;
extern vgui::IInputInternal 	*g_pInputInternal;

enum ETouchButtonType
{
	touch_command = 0, // Tap button
	touch_move,    // Like a joystick stick.
	touch_joy,     // Like a joystick stick, centered.
	touch_dpad,    // Only two directions.
	touch_look,     // Like a touchpad.
};

enum ETouchState
{
	state_none = 0,
	state_edit,
	state_edit_move
};

enum ETouchRound
{
	round_none = 0,
	round_grid,
	round_aspect
};

struct rgba_t 
{ 
	rgba_t(	unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255 ) : r( r ), g( g ), b( b ), a( a ) { }
	rgba_t() : r( 0 ), g( 0 ), b( 0 ), a( 0 ) { }
	rgba_t( unsigned char *x ) : r( x[0] ), g( x[1] ), b( x[2] ), a( x[3] ) { }
	
	operator unsigned char*() { return &r; }
	
	unsigned char r, g, b, a; 
};

enum eventtype_t
{
	event_touchmotion,
	event_touchup,
	event_touchdown
};

struct event_clientcmd_t
{
	char buf[CMD_SIZE];
};

typedef struct event_s
{
	eventtype_t type;
	event_clientcmd_t clientCmd;
	int x;
	int y;
	int fingerid;
} event_t;

class CTouchButton
{
public:
	// Touch button type: tap, stick or slider
	ETouchButtonType type;
	
	// Field of button in pixels
	float x1, y1, x2, y2;
	
	// Button texture
	int texture;
	
	rgba_t color;
	char texturefile[256];
	char command[256];
	char name[32];
	int finger;
	int flags;
	float fade;
	float fadespeed;
	float fadeend;
	float aspect;
	int textureID;
};

class CTouchDefaultButton
{
public:
	char name[32];
	char texturefile[256];
	char command[256];
	float x1, y1, x2, y2;
	rgba_t color;
	ETouchRound round;
	float aspect;
	int flags;
	int textureID;
};

class CTouchControls
{
public:
	CTouchControls();
	~CTouchControls();
	
	void Init( );
	void VidInit( );
	
	void Shutdown( );
	
	void Paint( );
	void Frame( );

	void IN_TouchAddDefaultButton( const char *name, const char *texturefile, const char *command, float x1, float y1, float x2, float y2, rgba_t color, ETouchRound round, float aspect, int flags );
	void IN_TouchAddButton( const char *name, const char *texturefile, const char *command, ETouchButtonType type, float x1, float y1, float x2, float y2, int fingerid, rgba_t color );
	void IN_Move( );
	void IN_Look( );
	void ButtonPress( event_t *ev );
	void TouchMotion( event_t *ev );
private:
	bool initialized;
	ETouchState state;
	CUtlLinkedList<CTouchButton> btns;
	
	int look_finger;
	int move_finger;
	float forward, side;
	float yaw, pitch;
	CTouchButton *move;
	
	float move_start_x, move_start_y;
	float dx, dy;

	bool w,a,s,d;
	// editing
	CTouchButton *edit;
	CTouchButton *selection;
	int resize_finger;
	bool showbuttons;
	bool clientonly;
	rgba_t scolor;
	int swidth;
	bool precision;
	// textures
	int showtexture;
	int hidetexture;
	int resettexture;
	int closetexture;
	int joytexture; // touch indicator
	bool configchanged;
	vgui::HFont textfont;
	COverlayPanel *overlayPanel;
};

extern CTouchControls g_Touch;

void Android_RunEvents();

// TODO: InputSystem communication and faking mouse/gamepad

