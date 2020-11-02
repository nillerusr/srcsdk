#include "client.h"
#include <gameui/BasePanel.h>
#include <gameui/NewGameDialog.h>
#include "MenuTouch.h"
#include <gameui/MouseMessageForwardingPanel.h>
#include "MenuTouch.h"
#include "vgui_controls/pch_vgui_controls.h"

#define CHECK_BUTTON(p, b) if( strcmp(p->GetClassName(), ""#b) == 0 ) { vgui::Button *butt = (vgui::Button*)p; butt->DoClick(); }
#define CHECK_BUTTON2(p, b) if( strcmp(p->GetClassName(), ""#b) == 0 ) { vgui::b *butt = (vgui::b*)p; butt->DoClick(); }

bool IsButton(const char *classname)
{
	LogPrintf("IsButton: %s\n", classname);
    return false;
}


void MenuTouch::TouchDown(Panel *panel)
{
    if( panel )
    {
        if( strcmp(panel->GetClassName(), "CSelectionOverlayPanel") == 0 )
        {
            CSelectionOverlayPanel *SelectionPanel = (CSelectionOverlayPanel*)panel;
            SelectionPanel->OnMousePressed(MOUSE_LEFT);
        }
        else if( strcmp(panel->GetClassName(), "CMouseMessageForwardingPanel") == 0 )
        {
            CMouseMessageForwardingPanel *SelectionPanel = (CMouseMessageForwardingPanel*)panel;
            SelectionPanel->OnMousePressed(MOUSE_LEFT);
        }
    }
}

void MenuTouch::TouchUp(Panel *panel)
{
    if( panel )
    {
		if( strcmp(panel->GetClassName(), "TextEntry") == 0 )
			showKeyboard(1);
		
		CHECK_BUTTON( panel, PageTab );
		CHECK_BUTTON( panel, CGameMenuItem );
		CHECK_BUTTON( panel, CCvarToggleCheckButton );
		CHECK_BUTTON( panel, RadioButton );
		CHECK_BUTTON( panel, Button );
		CHECK_BUTTON( panel, CNewGamePlayButton );
		CHECK_BUTTON2( panel, CheckButton );
		CHECK_BUTTON2( panel, MenuButton );
		CHECK_BUTTON2( panel, ComboBox );
		CHECK_BUTTON2( panel, ToggleButton );
		CHECK_BUTTON2( panel, MenuItem );
    }
}
