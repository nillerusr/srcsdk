/*
touch.cpp -- touchcontrols
Copyright (C) 2017 a1batross

Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/

#include "client.h"
#include "convar.h"

#include <string.h>

ConVar touch_pitch( "touch_pitch", "90", 0, "touch pitch sensitivity" );
ConVar touch_yaw( "touch_yaw", "120", 0, "touch yaw sensitivity" );
ConVar touch_forwardzone( "touch_forwardzone", "0.06", 0, "forward touch zone" );
ConVar touch_sidezone( "touch_sidezone", "0.06", 0, "side touch zone" );
ConVar touch_grid_enable( "touch_grid_enable", "1", 0, "enable touch grid" );
ConVar touch_grid_count( "touch_grid_count", "50", 0, "touch grid count" );
ConVar touch_config_file( "touch_config_file", "touch.cfg", FCVAR_ARCHIVE, "current touch profile file" );
ConVar touch_enable( "touch_enable", "1", FCVAR_ARCHIVE, "enable touch controls" );
ConVar touch_in_menu( "touch_in_menu", "0", 0, "draw touch in menu (for internal use only)" );
ConVar touch_joy_radius( "touch_joy_radius", "1.0", 0, "joy radius multiplier" );
ConVar touch_dpad_radius( "touch_dpad_radius", "1.0", 0, "dpad radius multiplier" );
ConVar touch_move_indicator( "touch_move_indicator", "0.0", 0, "indicate move events (0 to disable)" );
ConVar touch_highlight_r( "touch_highlight_r", "1.0", 0, "highlight r color" );
ConVar touch_highlight_g( "touch_highlight_g", "1.0", 0, "highlight g color" );
ConVar touch_highlight_b( "touch_highlight_b", "1.0", 0, "highlight b color" );
ConVar touch_highlight_a( "touch_highlight_a", "1.0", 0, "highlight alpha" );
ConVar touch_precise_amount( "touch_precise_amount", "0.5", 0, "sensitivity multiplier for precise-look" );
ConVar touch_joy_texture( "touch_joy_texture", "touch_default/joy.tga", 0, "texture for move indicator");

CTouchControls g_Touch;
static CTouchDefaultButton g_DefaultButtons[64];
static int g_LastDefaultButton;

CTouchControls::CTouchControls()
{
	initialized = false;
}

CTouchControls::~CTouchControls()
{
}

void CTouchControls::Init( )
{
	if( initialized )
		return;
	
	initialized = true;
	btns.EnsureCapacity( 64 );
	look_finger = move_finger = resize_finger = -1;
	scolor = rgba_t( -1, -1, -1, -1 );
	state = state_none;
	swidth = 1;
	move = edit = selection = NULL;
	showbuttons = true;
	clientonly = false;
	precision = false;
	
	move_start_x = move_start_y = 0.0f;
	forward = side = 0.0f;
	yaw = pitch = 0.0f;
	
	// textures
	showtexture = hidetexture = resettexture = closetexture = joytexture = 0;
	configchanged = false;
	Msg( "CTouchControls::Init()" );
}

void CTouchControls::VidInit( )
{
	
}

void CTouchControls::Shutdown( )
{
}

void CTouchControls::Frame( )
{
	
}

void CTouchControls::IN_TouchAddDefaultButton( const char *name, const char *texturefile, const char *command, float x1, float y1, float x2, float y2, rgba_t color, ETouchRound round, float aspect, int flags )
{
	if( g_LastDefaultButton >= 255 )
		return;
	strncpy( g_DefaultButtons[g_LastDefaultButton].name, name, 32 );
	strncpy( g_DefaultButtons[g_LastDefaultButton].texturefile, texturefile, 256 );
	strncpy( g_DefaultButtons[g_LastDefaultButton].command, command, 256 );
	g_DefaultButtons[g_LastDefaultButton].x1 = x1;
	g_DefaultButtons[g_LastDefaultButton].y1 = y1;
	g_DefaultButtons[g_LastDefaultButton].x2 = x2;
	g_DefaultButtons[g_LastDefaultButton].y2 = y2;
	g_DefaultButtons[g_LastDefaultButton].color = color;
	g_DefaultButtons[g_LastDefaultButton].round = round;
	g_DefaultButtons[g_LastDefaultButton].aspect = aspect;
	g_DefaultButtons[g_LastDefaultButton].flags = flags;
	g_LastDefaultButton++;

}
