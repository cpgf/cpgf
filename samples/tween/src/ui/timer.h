#ifndef CPGF_TIMER_H
#define CPGF_TIMER_H

#include "cpgf/gcallback.h"

#include "wx/timer.h"

typedef cpgf::GCallback<void (int)> TimerCallback;

const int FrameTime = 10;

class TestTimer : public wxTimer
{
public:
	TestTimer(const TimerCallback & callback)
		: callback(callback), previousTime(wxGetLocalTimeMillis()),
			frameRate(0), timeInCurrentSecond(0), frameCountInCurrentSecond(0)
	{
		this->Start(FrameTime);
	}
	
	virtual void Notify() {
		wxLongLong t = wxGetLocalTimeMillis();
		int dt = (int)((t - this->previousTime).ToLong());
		this->previousTime = t;

		++frameCountInCurrentSecond;
		timeInCurrentSecond += dt;
		if(timeInCurrentSecond >= 1000) {
			frameRate = frameCountInCurrentSecond;
			timeInCurrentSecond = 0;
			frameCountInCurrentSecond = 0;
		}

		this->callback(dt);
	}

	int getFrameRate() const {
		return this->frameRate;
	}
	
private:
	TimerCallback callback;
	wxLongLong previousTime;
	int frameRate;
	int timeInCurrentSecond;
	int frameCountInCurrentSecond;
};


#endif
