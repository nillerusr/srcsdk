#include "OverlayPanel.h"
#include "client.h"

#include <vgui/ISurface.h>

COverlayPanel::COverlayPanel(Panel *parent, const char *panelName) : Panel(parent, panelName)
{
	int w, h;
	engine->GetScreenSize(w, h);
	SetBounds( 0, 0, w, h );

	SetFgColor( Color( 0, 0, 0, 255 ) );
	SetPaintBackgroundEnabled( false );

	SetKeyBoardInputEnabled( false );
	SetMouseInputEnabled( false );
}

void COverlayPanel::Paint()
{
	g_Touch.Paint();
}
