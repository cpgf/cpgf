#ifndef CPGF_GMETACOMMON_P_H
#define CPGF_GMETACOMMON_P_H

#include <string.h>


namespace cpgf {

namespace meta_internal {


void * newZeroBuffer(void * buffer, size_t size, void * copy);

class GMetaItemImplement;


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

} // namespace meta_internal



} // namespace cpgf



#endif
