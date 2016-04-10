#ifndef CPGF_TWEEN_EASING_ELASTIC_H
#define CPGF_TWEEN_EASING_ELASTIC_H

#include "cpgf/tween/gtweencommon.h"

namespace cpgf {

struct ElasticEase
{
public:
	static GTweenEaseType easeIn(GTweenNumber amplitude = 0.0f, GTweenNumber period = 0.0f) {
		return GTweenEaseType(_easeIn(amplitude, period));
	}
	
	static GTweenEaseType easeOut(GTweenNumber amplitude = 0.0f, GTweenNumber period = 0.0f) {
		return GTweenEaseType(_easeOut(amplitude, period));
	}
	
	static GTweenEaseType easeInOut(GTweenNumber amplitude = 0.0f, GTweenNumber period = 0.0f) {
		return GTweenEaseType(_easeInOut(amplitude, period));
	}
	
private:
	struct _easeIn {
		_easeIn(GTweenNumber amplitude, GTweenNumber period) : amplitude(amplitude), period(period) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber s;
			if(param->current == 0.0f) {
				return 0.0f;
			}
			if(param->current >= param->total) {
				return 1.0f;
			}
			GTweenNumber t = param->current / param->total;
			GTweenNumber np = period;
			GTweenNumber na = amplitude;
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
			return (GTweenNumber)(-na * pow(2, 10 * t) * sin((t * param->total - s) * pi2() / np));
		}
		
		GTweenNumber amplitude;
		GTweenNumber period;
	};
	
	struct _easeOut {
		_easeOut(GTweenNumber amplitude, GTweenNumber period) : amplitude(amplitude), period(period) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber s;
			if(param->current == 0.0f) {
				return 0.0f;
			}
			if(param->current >= param->total) {
				return 1.0f;
			}
			GTweenNumber t = param->current / param->total;
			GTweenNumber np = period;
			GTweenNumber na = amplitude;
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
			return (GTweenNumber)(na * pow(2, -10 * t) * sin((t * param->total - s) * pi2() / np) + 1.0f);
		}
		
		GTweenNumber amplitude;
		GTweenNumber period;
	};
	
	struct _easeInOut {
		_easeInOut(GTweenNumber amplitude, GTweenNumber period) : amplitude(amplitude), period(period) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber s;
			if(param->current == 0.0f) {
				return 0.0f;
			}
			if(param->current >= param->total) {
				return 1.0f;
			}
			GTweenNumber t = param->current / (param->total * 0.5f);
			GTweenNumber np = period;
			GTweenNumber na = amplitude;
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
				return (GTweenNumber)(-0.5f * na * pow(2, 10 * t) * sin((t * param->total - s) * pi2() / np));
			}
			else {
				t -= 1.0f;
				return (GTweenNumber)(na * pow(2, -10 * t) * sin((t * param->total - s) * pi2() / np) * 0.5f + 1.0f);
			}
		}
		
		GTweenNumber amplitude;
		GTweenNumber period;
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
