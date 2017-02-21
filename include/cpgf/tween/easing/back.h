#ifndef CPGF_TWEEN_EASING_BACK_H
#define CPGF_TWEEN_EASING_BACK_H

#include "cpgf/tween/gtweencommon.h"

namespace cpgf {

struct BackEase
{
public:
	static GTweenEaseType easeIn(GTweenNumber s = 1.70158f) {
		return GTweenEaseType(_easeIn(s));
	}
	
	static GTweenEaseType easeOut(GTweenNumber s = 1.70158f) {
		return GTweenEaseType(_easeOut(s));
	}
	
	static GTweenEaseType easeInOut(GTweenNumber s = 1.70158f) {
		return GTweenEaseType(_easeInOut(s));
	}
	
private:
	struct _easeIn {
		explicit _easeIn(GTweenNumber s) : s(s) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber t = param->current / param->total;
			return t * t * ((s + 1) * t - s);
		}
		
		GTweenNumber s;
	};
	
	struct _easeOut {
		explicit _easeOut(GTweenNumber s) : s(s) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber t = param->current / param->total;
			t = t - 1;
			return (t * t * ((s + 1) * t + s) + 1);
		}
		
		GTweenNumber s;
	};
	
	struct _easeInOut {
		explicit _easeInOut(GTweenNumber s) : s(s) {}
		
		GTweenNumber operator() (const GTweenEaseParam * param) const {
			GTweenNumber t = param->current / (param->total * 0.5f);
			GTweenNumber ns = s * 1.525f;
			if(t < 1) {
				return 0.5f * ( t * t * ((ns +1 ) * t - ns));
			}
			else {
				t -= 2;
				return 0.5f * (t * t * ((ns + 1) * t + ns) + 2);
			}
		}
		
		GTweenNumber s;
	};
	
};


} // namespace cpgf


#endif
