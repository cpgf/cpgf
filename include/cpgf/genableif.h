#ifndef __GENABLEIF_H
#define __GENABLEIF_H

namespace cpgf {


template <bool, typename T = void>
struct GEnableIf;

template <typename T>
struct GEnableIf <true, T> {
	typedef T Result;
};

template <bool, typename T = void>
struct GDisableIf;

template <typename T>
struct GDisableIf <false, T> {
	typedef T Result;
};


} // namespace cpgf


#endif
