#ifndef __GMETACOMMON_P_H
#define __GMETACOMMON_P_H

#include <string.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif


namespace cpgf {

const size_t abstractParameterIndexBase = 0x1000000;

namespace meta_internal {


void handleForbidAccessError(bool isRead);

void * newZeroBuffer(void * buffer, size_t size, void * copy);

class GMetaItemImplement;

struct CStringCompare
{
	bool operator () (const char * a, const char * b) const {
		return strcmp(a, b) < 0;
	}
};


template <typename T>
struct GScopedPointerDeleter_BaseMeta
{
	static inline void Delete(T * p) {
		p->deleteSelf();
	}
};

template <typename T>
void virtualBaseMetaDeleter(void * self)
{
	delete static_cast<T *>(self);
}

template <typename Policy>
void adjustParamIndex(size_t & index)
{
	if(index >= abstractParameterIndexBase) {
		index -= abstractParameterIndexBase;
	}
	else {
		if(PolicyHasRule<Policy, GMetaRuleExplicitThis>::Result) {
			++index;
		}
	}
}

} // namespace meta_internal



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif
