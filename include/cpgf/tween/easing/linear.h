#ifndef CPGF_TWEEN_EASING_LINEAR_H
#define CPGF_TWEEN_EASING_LINEAR_H

#include "cpgf/tween/gtweencommon.h"

namespace cpgf {

struct LinearEase
{
public:
	static GTweenEaseType ease() {
		return &_ease;
	}
	
	static GTweenEaseType easeIn() {
		return ease();
	}
	
	static GTweenEaseType easeOut() {
		return ease();
	}
	
	static GTweenEaseType easeInOut() {
		return ease();
	}
	
private:
	static GTweenNumber _ease(const GTweenEaseParam * param) {
		return param->current / param->total;
	}
};


} // namespace cpgf


#endif
