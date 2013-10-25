#ifndef CPGF_MAINFRAME_H
#define CPGF_MAINFRAME_H

#include "../uiresource/mainui.h"

#include "paneltweentest.h"


class MainFrame : public MainFrameRes
{
private:
	typedef MainFrameRes super;

public:
	MainFrame();

private:
	PanelTweenTest * tweenPanel;
};


#endif
