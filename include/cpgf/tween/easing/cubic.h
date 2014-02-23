#ifndef CPGF_TWEEN_EASING_CUBIC_H
#define CPGF_TWEEN_EASING_CUBIC_H

#include "cpgf/tween/gtweencommon.h"

namespace cpgf {

struct CubicEase
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
		return t * t * t;
	}
	
	static GTweenNumber _easeOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / param->total - 1.0f;
		return t * t * t + 1.0f;
	}
	
	static GTweenNumber _easeInOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / (param->total * 0.5f);
		if(t < 1.0f) {
			return 0.5f * t * t * t;
		}
		else {
			t -= 2.0f;
			return 0.5f * (t * t * t + 2.0f);
		}
	}
};


} // namespace cpgf


#endif
