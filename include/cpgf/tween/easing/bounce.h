#ifndef CPGF_TWEEN_EASING_BOUNCE_H
#define CPGF_TWEEN_EASING_BOUNCE_H

#include "cpgf/tween/gtweencommon.h"

namespace cpgf {

struct BounceEase
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
		GTweenEaseParam p;
		p.current = param->total - param->current;
		p.total = param->total;
		return 1 - _easeOut(&p);
	}
	
	static GTweenNumber _easeOut(const GTweenEaseParam * param) {
		GTweenNumber t = param->current / param->total;
		if(t < 1.0f / 2.75f) {
			return 7.5625f * t * t;
		}
		else if(t < 2.0f /2.75f) {
			t -= 1.5f /2.75f;
			return 7.5625f * t * t + 0.75f;
		}
		else if(t < 2.5f / 2.75f) {
			t -= 2.25f / 2.75f;
			return 7.5625f * t * t + 0.9375f;
		} else {
			t -= 2.625f / 2.75f;
			return 7.5625f * t * t + 0.984375f;
		}
	}
	
	static GTweenNumber _easeInOut(const GTweenEaseParam * param) {
		GTweenEaseParam p;
		if(param->current < param->total * 0.5f) {
			p.current = param->current * 2;
			p.total = param->total;
			return _easeIn(&p) * 0.5f;
		}
		else {
			p.current = param->current * 2 - param->total;
			p.total = param->total;
			return _easeOut(&p) * 0.5f + 0.5f;
		}
	}
};


} // namespace cpgf


#endif
