#ifndef __TIMER_H
#define __TIMER_H

#include "cpgf/gcallback.h"

#include "wx/timer.h"

typedef cpgf::GCallback<void ()> TimerCallback;

const int FrameTime = 50;

class TestTimer : public wxTimer
{
public:
	TestTimer(const TimerCallback & callback) : callback(callback) {
		this->Start(FrameTime);
	}
	
	virtual void Notify() {
		this->callback();
	}
	
private:
	TimerCallback callback;
};


#endif
