#ifndef __TWEEN_EASING_QUART_H
#define __TWEEN_EASING_QUART_H

#include "cpgf/tween/gtweeneaseparam.h"

namespace cpgf {

struct QuartEase
{
	static GTweenNumber easeIn(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / param->total;
		return t * t * t * t;
	}
	
	static GTweenNumber easeOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / param->total - 1;
		return - t * t * t * t + 1;
	}
	
	static GTweenNumber easeInOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / (param->total * 0.5);
		if(t < 1) {
			return 0.5 * t * t * t * t;
		}
		else {
			t -= 2;
			return -0.5 * (t * t * t * t - 2);
		}
	}
};


} // namespace cpgf


#endif
