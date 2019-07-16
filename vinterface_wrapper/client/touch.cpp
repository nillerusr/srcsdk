/*
touch.cpp -- touchcontrols
Copyright (C) 2017 a1batross
Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/

#include "client.h"
#include "convar.h"
#include <dlfcn.h>
#include <string.h>

#define TOUCH_YAW 220
#define TOUCH_PITCH 90

CTouchControls g_Touch;
static CTouchDefaultButton g_DefaultButtons[64];
static int g_LastDefaultButton = 0;
int screen_h, screen_w;

static CTouchButton g_Buttons[64];
static int g_LastButton = 0;

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

	engine->GetScreenSize( screen_w, screen_h );
	void *touchlib = dlopen("libtouch.so",RTLD_LAZY);
	void (*clientLoaded)(void) = (void (*)(void))dlsym( touchlib, "clientLoaded" );
	clientLoaded();
	dlclose( touchlib );
	w = a = s = d = false;
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

	//textfont = g_pScheme->GetFont( "Default" );
	
	move_start_x = move_start_y = 0.0f;

	// textures
	showtexture = hidetexture = resettexture = closetexture = joytexture = 0;
	configchanged = false;

	rgba_t color(255, 255, 255, 255);

	// buttons
/*
	IN_TouchAddDefaultButton( "Y", "", "XB_Y", 50, 0, 100, 50, color, round_none, 0, 0 );
	IN_TouchAddDefaultButton( "X", "", "XB_X", 0, 60, 50, 110, color, round_none, 0, 0 );	
	IN_TouchAddDefaultButton( "A", "", "XB_A", 50, 120, 100, 170, color, round_none, 0, 0 );	
	IN_TouchAddDefaultButton( "B", "", "XB_B", 100, 60, 150, 110, color, round_none, 0, 0 );	

	// dpad
	IN_TouchAddDefaultButton( "UP", "", "D_UP", 300, 0, 350, 50, color, round_none, 0, 0 );
	IN_TouchAddDefaultButton( "LEFT", "", "D_LEFT", 250, 60, 300, 110, color, round_none, 0, 0 );	
	IN_TouchAddDefaultButton( "DOWN", "", "D_DOWN", 300, 120, 350, 170, color, round_none, 0, 0 );	
	IN_TouchAddDefaultButton( "RIGHT", "", "D_RIGHT", 350, 60, 400, 110, color, round_none, 0, 0 );	
*/

	IN_TouchAddButton( "invnext", "", "invnext", touch_command, 0.000000, 0.533333, 0.120000, 0.746667, -1, color );
	IN_TouchAddButton( "invprev", "", "invprev", touch_command, 0.000000, 0.071111, 0.120000, 0.284444, -1, color );
	IN_TouchAddButton( "use", "", "+use", touch_command, 0.880000, 0.462222, 1.000000, 0.675556, -1, color );
	IN_TouchAddButton( "jump", "", "+jump", touch_command, 0.880000, 0.213333, 1.000000, 0.426667, -1, color );
	IN_TouchAddButton( "attack", "", "+attack", touch_command, 0.760000, 0.533333, 0.880000, 0.746667, -1, color );
	IN_TouchAddButton( "attack2", "", "+attack2", touch_command, 0.760000, 0.320000, 0.880000, 0.533333, -1, color );
	IN_TouchAddButton( "loadquick", "", "load quick", touch_command, 0.760000, 0.000000, 0.840000, 0.142222, -1, color );
	IN_TouchAddButton( "savequick", "", "save quick", touch_command, 0.840000, 0.000000, 0.920000, 0.142222, -1, color );
	IN_TouchAddButton( "reload", "", "+reload", touch_command, 0.000000, 0.320000, 0.120000, 0.533333, -1, color );
	IN_TouchAddButton( "flashlight", "", "impulse 100", touch_command, 0.920000, 0.000000, 1.000000, 0.142222, -1, color );
	IN_TouchAddButton( "duck", "", "+duck", touch_command, 0.880000, 0.746667, 1.000000, 0.960000, -1, color );
	IN_TouchAddButton( "tduck", "", ";+duck", touch_command, 0.560000, 0.817778, 0.620000, 0.924444, -1, color );
	IN_TouchAddButton( "look", "", "", touch_look, 0.5, 0, 1, 1, -1, color );
	IN_TouchAddButton( "move", "", "", touch_move, 0, 0, 0.5, 1, -1, color );

	//IN_TouchAddButton( "menu", "", "map test1", touch_command, 0.000000, 0.820000, 0.040000, 0.860000, -1, color );

	Msg( "CTouchControls::Init()" );
}

void CTouchControls::VidInit( )
{
	
}

void CTouchControls::Shutdown( )
{
}

void CTouchControls::Paint( )
{
	if (!initialized)
		return;

	for (int i = 0; i < g_LastButton; i++)
	{
		if( g_Buttons[i].type == touch_move || g_Buttons[i].type == touch_look )
			continue;
		g_pSurface->DrawSetColor(255, 0, 0, 155);
		g_pSurface->DrawOutlinedRect(g_Buttons[i].x1, g_Buttons[i].y1, g_Buttons[i].x2, g_Buttons[i].y2);
	
		//g_pSurface->DrawSetTexture( g_Buttons[i].textureID );
       	 	//g_pSurface->DrawSetColor(50,50,50,100);
		//g_pSurface->DrawTexturedRect( g_Buttons[i].x1, g_Buttons[i].y1, g_Buttons[i].x2, g_Buttons[i].y2 );
	}
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
	g_DefaultButtons[g_LastDefaultButton].textureID = g_pSurface->CreateNewTextureID();
	g_pSurface->DrawSetTextureFile( g_DefaultButtons[g_LastDefaultButton].textureID, "vgui/steam/games/icon_hl2", true, false);

	g_LastDefaultButton++;
}

void CTouchControls::IN_TouchAddButton( const char *name, const char *texturefile, const char *command, ETouchButtonType type, float x1, float y1, float x2, float y2, int fingerid, rgba_t color )
{
	if( g_LastButton >= 64 )
		return;

	strncpy( g_Buttons[g_LastButton].name, name, 32 );
	strncpy( g_Buttons[g_LastButton].texturefile, texturefile, 256 );
	strncpy( g_Buttons[g_LastButton].command, command, 256 );
	g_Buttons[g_LastButton].x1 = (int)(x1*screen_w);
	g_Buttons[g_LastButton].y1 = (int)(y1*screen_h);
	g_Buttons[g_LastButton].x2 = (int)(x2*screen_w);
	g_Buttons[g_LastButton].y2 = (int)(y2*screen_h);
	g_Buttons[g_LastButton].color = color;
	g_Buttons[g_LastButton].type = type;
	g_Buttons[g_LastButton].textureID = g_pSurface->CreateNewTextureID();
	g_pSurface->DrawSetTextureFile( g_Buttons[g_LastButton].textureID, "vgui/steam/games/icon_hl2", true, false);

	g_LastButton++;
}

void CTouchControls::TouchMotion( event_t *ev )
{
	float x = (float)ev->x / screen_w;
	float y = (float)ev->y / screen_h;

	LogPrintf( "TouchMotion, %f, %f", x, y );

	for (int i = 0; i < g_LastButton; i++)
	{
		if( g_Buttons[i].finger == ev->fingerid )
		{
			if( g_Buttons[i].type == touch_move )
			{
				//LogPrintf( "TouchMotion, touch_move x:%f, y:%f, startx:%f, starty:%f", x, y, move_start_x, move_start_y );
	
				if( ( move_start_x - x ) >= 0.01 ) {
					if( !a ) {
						a = true;
						engine->ClientCmd("+moveleft");
					}
				}
				else if( a )
				{
					a = false;
					engine->ClientCmd("-moveleft");
				}

				if( ( move_start_x - x ) <= -0.01 ) {
					if( !d ) {
						d = true;
						engine->ClientCmd("+moveright");
					}
				}

				else if( d ) {
					d = false;
					engine->ClientCmd("-moveright");
				}

				if( ( move_start_y - y ) <= -0.01 ) {
					if( !s ) {
						s = true;
						engine->ClientCmd("+back");
					}
				}
				else if( s ) {
					s = false;
					engine->ClientCmd("-back");
				}

				if( ( move_start_y - y ) >=0.01 ) {
					if( !w ) {
						w = true;
						engine->ClientCmd("+forward");
					}
				}
				else if( w ) {
					w = false;
					engine->ClientCmd("-forward");
				}
			}
			else if( g_Buttons[i].type == touch_look )
			{
				QAngle ang, newang;
				engine->GetViewAngles( ang );
				LogPrintf( "Angles %f, %f", ang.x, ang.y );
				newang.y = ang.y + TOUCH_YAW * ( dx - x );
				newang.x = ang.x - TOUCH_PITCH * ( dy - y );
				newang.z = ang.z;
				LogPrintf( "NewAngles %f, %f", newang.x, newang.y );
				engine->SetViewAngles( newang );
				dx = x;
				dy = y;
			}
		}
	}
}

void CTouchControls::ButtonPress( event_t *ev )
{
	if( ev->type == event_touchdown )
	{
		for (int i = 0; i < g_LastButton; i++)
		{
			if(  ev->x > g_Buttons[i].x1 && ev->x < g_Buttons[i].x2 && ev->y > g_Buttons[i].y1 && ev->y < g_Buttons[i].y2 )
			{
				g_Buttons[i].finger = ev->fingerid;
				if( g_Buttons[i].type == touch_move )
				{
					move_start_x = (float)ev->x / screen_w;
					move_start_y = (float)ev->y / screen_h;
				}
				else if( g_Buttons[i].type == touch_look )
				{
					dx = (float)ev->x / screen_w;
					dy = (float)ev->y / screen_h;
				}
				else
				{
					engine->ClientCmd( g_Buttons[i].command );
				}
			}
		}
	}
	else if( ev->type == event_touchup )
	{
		for (int i = 0; i < g_LastButton; i++)
		{
			if( g_Buttons[i].finger == ev->fingerid )
			{
				if( g_Buttons[i].type == touch_move )
				{
					if( w ) { engine->ClientCmd("-forward"); }
					if( a ) { engine->ClientCmd("-moveleft"); }
					if( s ) { engine->ClientCmd("-back"); }
					if( d ) { engine->ClientCmd("-moveright"); }
				}
				g_Buttons[i].finger = -1;
				if( g_Buttons[i].command[0] == '+' )
				{
					char cmd[256];
					snprintf( cmd, sizeof cmd, "%s", g_Buttons[i].command );
					cmd[0] = '-';
					engine->ClientCmd( cmd );
				}
			}
		}
	}
}
