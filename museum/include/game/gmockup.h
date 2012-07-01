#ifndef __GMOCKUP_H
#define __GMOCKUP_H

#include "cpgf/game/gcore.h"
#include "cpgf/game/gconfig.h"
#include "cpgf/game/gapplication.h"

#if defined(G_OS_WIN)
	#include "windows.h"
#endif

namespace cpgf {

struct GAppInitConfig
{
	GAppInitConfig() : engineHandler(NULL) {
	}

	IEngineEvent *engineHandler;

	bool fullScreen;
	int width;
	int height;

#if defined(G_OS_WIN)
	HWND parentHandle;
	int bitsPerPixel;
#endif
};

bool gInitializeApplication(GAppInitConfig *config);
bool gStartupMainLoop();

gmillisecond getMillisecond();

} // namespace cpgf


#endif

