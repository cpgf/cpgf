#ifndef __TESTEVENT_H
#define __TESTEVENT_H


#include <functional>

#include <cxxtest/TestSuite.h>

#include "cpgf/game/gevent.h"
#include "cpgf/gfunctiontraits.h"

using namespace cpgf;

const int EventTypeFirst = 1;

class EventObject;

static EventObject * cbObject;
static GEventDispatcher dispatcher;

class GEvent2 : public GEvent
{
private:
	typedef GEvent super;
public:
	GEvent2(int message) : super(message), aaa(message * 10) {
	}

private:
	int aaa;
};

class EventObject
{
public:
	EventObject() {
	}

	EventObject(const EventObject & other) {
	    (void)other;
	}

	EventObject & operator = (const EventObject & other) {
	    (void)other;
	    return *this;
	}

	void operator () (const GEvent2 & e) {
		TS_TRACE("opperator ()");
		if(e.getMessage() == EventTypeFirst) {
		}
	}

	void eventHandler1(const GEvent & e) const {
		if(e.getMessage() == EventTypeFirst) {
		}
	}

	void eventHandler2(const GEvent2 & e) const {
		if(e.getMessage() == EventTypeFirst) {
		}
	}
};

inline void globalEventHandler2(const GEvent2 & e) {
	if(e.getMessage() == EventTypeFirst) {
	}
}

class TestEvent : public CxxTest::TestSuite
{
public:
	void setUp() {
		cbObject = new EventObject;
	}

	void tearDown() {
		delete cbObject;
	}

public:
	void test000() {}

	void test1() {
		dispatcher.addListener(EventTypeFirst, GCallback<void (const GEvent &)>(cbObject, &EventObject::eventHandler1));
		dispatcher.removeListener(EventTypeFirst, GCallback<void (const GEvent &)>(cbObject, &EventObject::eventHandler1));
		dispatcher.sendEvent(GEvent(EventTypeFirst));

		GEventQueue eq;
		eq.addListener(EventTypeFirst, makeEventListener(cbObject, &EventObject::eventHandler2));
		eq.addListener(EventTypeFirst, makeEventListener(&globalEventHandler2));
		eq.addListener(EventTypeFirst, makeEventFunctorListener<const GEvent2 &>(EventObject()));
		eq.postEvent(GEvent2(EventTypeFirst));
		eq.processEvents();
	}
};


#endif
