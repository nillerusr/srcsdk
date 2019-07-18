#ifndef OVERLAYPANEL_H
#define OVERLAYPANEL_H

#include <vgui/VGUI.h>
#include <vgui_controls/Panel.h>

using namespace vgui;

class COverlayPanel : public Panel
{
	DECLARE_CLASS_SIMPLE( COverlayPanel, Panel );

public:
	COverlayPanel(Panel *parent, const char *panelName);
	
	virtual void Paint();
};

#endif // OVERLAYPANEL_H

