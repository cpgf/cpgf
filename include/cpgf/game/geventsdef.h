#ifndef __GEVENTSDEF_H
#define __GEVENTSDEF_H

#include <queue>
#include <map>

#include "cpgf/game/gevent.h"
#include "cpgf/game/ginput.h"


namespace cpgf {



class GTouchInputEvent : public GEvent
{
private:
	typedef GEvent super;

public:
	GTouchInputEvent(void * source, const GInputData & data)
		: super(data.message, source), key(data.key), screenX(data.screenX), screenY(data.screenY) {
	}

	GTouchInputEvent(void * source, int message, int key, gcoord /*x*/, gcoord /*y*/)
		: super(message, source), key(key), screenX(screenX), screenY(screenY) {
	}


public:
	int key;
	gcoord screenX;
	gcoord screenY;
};



} // namespace cpgf


#endif

