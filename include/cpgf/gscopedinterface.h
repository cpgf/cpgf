#ifndef CPGF_GSCOPEDINTERFACE_H
#define CPGF_GSCOPEDINTERFACE_H

#include "cpgf/gscopedptr.h"

#include <cstddef>
#include <algorithm>


namespace cpgf {


template <typename T>
struct GScopedInterfaceDeleter
{
	static inline void Delete(T * p) {
		if(p) {
			p->releaseReference();
		}
	}
};

template <typename T>
struct GScopedInterfaceResetPredict
{
	// dont restrict that p != this->rawPointer
	// even if the pointer is the same, it's necessary to reset to release one reference count.
	static inline bool CanReset(T * /*pointerOfMine*/, T * /*pointerToReset*/) {
		return true;
	}
};

template <typename T>
class GScopedInterface : public GScopedPointer<T, GScopedInterfaceDeleter<T>, GScopedInterfaceResetPredict<T> >
{
private:
	typedef GScopedPointer<T, GScopedInterfaceDeleter<T>, GScopedInterfaceResetPredict<T> > super;

public:
	GScopedInterface(): super() {
	}

	explicit GScopedInterface(T * p) : super(p) {
	}

};


} // namespace cpgf


#endif
