#ifndef __TWEEN_EASING_SIN_H
#define __TWEEN_EASING_SIN_H

#include "cpgf/tween/gtweeneaseparam.h"

#include <math.h>

namespace cpgf {

struct SinEase
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
		return -cos(t * pi() * 0.5f) + 1.0f;
	}
	
	static GTweenNumber _easeOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / param->total;
		return sin(t * pi() * 0.5f);
	}
	
	static GTweenNumber _easeInOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / param->total;
		return -0.5f * (cos(pi() * t) - 1.0f);
	}
	
	static GTweenNumber pi() {
		return 3.14159265f;
	}
};


} // namespace cpgf


#endif
