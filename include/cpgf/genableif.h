#ifndef __GENABLEIF_H
#define __GENABLEIF_H

namespace cpgf {


template <bool, typename T = void>
struct GEnableIf {
	typedef T Result;
};

template <typename T>
struct GEnableIf <false, T> {
};

template <typename C, typename T = void>
struct GEnableIfResult : public GEnableIf<C::Result, T> {
};


template <bool, typename T = void>
struct GDisableIf {
	typedef T Result;
};

template <typename T>
struct GDisableIf <true, T> {
};

template <typename C, typename T = void>
struct GDisableIfResult : public GDisableIf<C::Result, T> {
};


template <typename T>
struct GNotResult {
	enum { Result = ! T::Result };
};


template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
struct GAndResult7 {
	enum { Result = T1::Result && T2::Result && T3::Result && T4::Result && T5::Result && T6::Result && T7::Result };
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct GAndResult6 {
	enum { Result = T1::Result && T2::Result && T3::Result && T4::Result && T5::Result && T6::Result };
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct GAndResult5 {
	enum { Result = T1::Result && T2::Result && T3::Result && T4::Result && T5::Result };
};

template <typename T1, typename T2, typename T3, typename T4>
struct GAndResult4 {
	enum { Result = T1::Result && T2::Result && T3::Result && T4::Result };
};

template <typename T1, typename T2, typename T3>
struct GAndResult3 {
	enum { Result = T1::Result && T2::Result && T3::Result };
};

template <typename T1, typename T2>
struct GAndResult2 {
	enum { Result = T1::Result && T2::Result };
};


template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
struct GOrResult7 {
	enum { Result = T1::Result || T2::Result || T3::Result || T4::Result || T5::Result || T6::Result || T7::Result };
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct GOrResult6 {
	enum { Result = T1::Result || T2::Result || T3::Result || T4::Result || T5::Result || T6::Result };
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct GOrResult5 {
	enum { Result = T1::Result || T2::Result || T3::Result || T4::Result || T5::Result };
};

template <typename T1, typename T2, typename T3, typename T4>
struct GOrResult4 {
	enum { Result = T1::Result || T2::Result || T3::Result || T4::Result };
};

template <typename T1, typename T2, typename T3>
struct GOrResult3 {
	enum { Result = T1::Result || T2::Result || T3::Result };
};

template <typename T1, typename T2>
struct GOrResult2 {
	enum { Result = T1::Result || T2::Result };
};


} // namespace cpgf


#endif
