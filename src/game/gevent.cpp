#include <string>

#include "cpgf/game/gevent.h"

namespace cpgf {

GWiseMap<std::string, int> eventMessageMap;

int registerEventMessage()
{
	static int availableEventMessage = -1;
	
	int msg = availableEventMessage;

	--availableEventMessage;

	return msg;
}

int registerEventMessage(const char * eventName)
{
	std::string name(eventName);

	GWiseMap<std::string, int>::iterator it = eventMessageMap.find(name);
	if(it == eventMessageMap.end()) {
		int msg = registerEventMessage();

		eventMessageMap.insert(name, msg);

		return msg;
	}
	else {
		return it->second;
	}
}


void GEventQueue::doProcessEvent(GEvent * e)
{
	if(e->getReceiver() != NULL) {
		e->getReceiver()->processEvent(e);
	}
	else {
		this->sendEvent(*e);
	}
}

bool GEventQueue::processNextEvent()
{
	GEvent * e = this->getNextEvent();

	if(e != NULL) {
		this->doProcessEvent(e);

		delete e;

		return true;
	}
	else {
		return false;
	}
}

void GEventQueue::processEvents()
{
	while(this->processNextEvent()) {
	};
}

void GEventQueue::clearEvents()
{
	while(!this->eventQueue.empty()) {
		GEvent * e = eventQueue.front();
		eventQueue.pop();

		delete e;
	}
}

GEvent * GEventQueue::getOrRemoveNextEvent(bool remove)
{
	if(eventQueue.empty()) {
		return NULL;
	}
	else {
		GEvent * e = eventQueue.front();
		if(remove) {
			eventQueue.pop();
		}

		return e;
	}
}


} // namespace cpgf
