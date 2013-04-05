#ifndef __TWEEN_EASING_ELASTIC_H
#define __TWEEN_EASING_ELASTIC_H

#include "cpgf/tween/gtweeneaseparam.h"

namespace cpgf {

struct ElasticEase
{
public:
	static GTweenEaseType easeIn(GTweenNumber a = 0.0f, GTweenNumber p = 0.0f) {
		return GTweenEaseType(_easeIn(a, p));
	}
	
	static GTweenEaseType easeOut(GTweenNumber a = 0.0f, GTweenNumber p = 0.0f) {
		return GTweenEaseType(_easeOut(a, p));
	}
	
	static GTweenEaseType easeInOut(GTweenNumber a = 0.0f, GTweenNumber p = 0.0f) {
		return GTweenEaseType(_easeInOut(a, p));
	}
	
private:
	struct _easeIn {
		_easeIn(GTweenNumber a, GTweenNumber p) : a(a), p(p) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber s;
			if(param->current == 0.0f) {
				return 0.0f;
			}
			if(param->current >= param->total) {
				return 1.0f;
			}
			GTweenNumber t = param->current / param->total;
			GTweenNumber np = p;
			GTweenNumber na = a;
			if(np == 0.0) {
				np = param->total * 0.3f;
			}
			if(na == 0.0f || na < 1.0f) {
				na = 1.0f;
				s = np / 4.0f;
			}
			else {
				s = np / pi2() * asin(1.0f / na);
			}
			t -= 1.0f;
			return -na * pow(2, 10 * t) * sin((t * param->total - s) * pi2() / np);
		}
		
		GTweenNumber a;
		GTweenNumber p;
	};
	
	struct _easeOut {
		_easeOut(GTweenNumber a, GTweenNumber p) : a(a), p(p) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber s;
			if(param->current == 0.0f) {
				return 0.0f;
			}
			if(param->current >= param->total) {
				return 1.0f;
			}
			GTweenNumber t = param->current / param->total;
			GTweenNumber np = p;
			GTweenNumber na = a;
			if(np == 0.0) {
				np = param->total * 0.3f;
			}
			if(na == 0.0f || na < 1.0f) {
				na = 1.0f;
				s = np / 4.0f;
			}
			else {
				s = np / pi2() * asin(1.0f / na);
			}
			return na * pow(2, -10 * t) * sin((t * param->total - s) * pi2() / np) + 1.0f;
		}
		
		GTweenNumber a;
		GTweenNumber p;
	};
	
	struct _easeInOut {
		_easeInOut(GTweenNumber a, GTweenNumber p) : a(a), p(p) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber s;
			if(param->current == 0.0f) {
				return 0.0f;
			}
			if(param->current >= param->total) {
				return 1.0f;
			}
			GTweenNumber t = param->current / (param->total * 0.5f);
			GTweenNumber np = p;
			GTweenNumber na = a;
			if(np == 0.0) {
				np = param->total * (0.3f * 1.5f);
			}
			if(na == 0.0f || na < 1.0f) {
				na = 1.0f;
				s = np / 4.0f;
			}
			else {
				s = np / pi2() * asin(1.0f / na);
			}
			if(t < 1) {
				t -= 1.0f;
				return -0.5f * na * pow(2, 10 * t) * sin((t * param->total - s) * pi2() / np);
			}
			else {
				t -= 1.0f;
				return na * pow(2, -10 * t) * sin((t * param->total - s) * pi2() / np) * 0.5f + 1.0f;
			}
		}
		
		GTweenNumber a;
		GTweenNumber p;
	};
	
	static GTweenNumber pi() {
		return 3.14159265f;
	}
	
	static GTweenNumber pi2() {
		return 2 * pi();
	}
};


} // namespace cpgf


#endif
