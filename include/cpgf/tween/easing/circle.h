#ifndef CPGF_TWEEN_EASING_CIRCLE_H
#define CPGF_TWEEN_EASING_CIRCLE_H

#include "cpgf/tween/gtweencommon.h"

#include <math.h>

namespace cpgf {

struct CircleEase
{
public:
	static GTweenEaseType easeIn() {
		return &_easeIn;
	}
	
	static GTweenEaseType easeOut() {
		return &_easeOut;
	}
	
	static GTweenEaseType easeInOut() {
		return &_easeInOut;
	}
	
private:
	static GTweenNumber _easeIn(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / param->total;
		return -sqrt(1.0f - t * t) + 1.0f;
	}
	
	static GTweenNumber _easeOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / param->total - 1.0f;
		return sqrt(1.0f - t * t);
	}
	
	static GTweenNumber _easeInOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / (param->total * 0.5f);
		if(t < 1.0f) {
			return -0.5f * (sqrt(1.0f - t * t) - 1.0f);
		}
		else {
			t -= 2.0f;
			return 0.5f * (sqrt(1.0f - t * t) + 1);
		}
	}
};


} // namespace cpgf


#endif
