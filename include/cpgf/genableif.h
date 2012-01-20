#ifndef __GENABLEIF_H
#define __GENABLEIF_H

#include "cpgf/gcompiler.h"

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
	G_STATIC_CONSTANT(bool, Result = ! T::Result);
};


template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
struct GAndResult7 {
	G_STATIC_CONSTANT(bool, Result = T1::Result && T2::Result && T3::Result && T4::Result && T5::Result && T6::Result && T7::Result);
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct GAndResult6 {
	G_STATIC_CONSTANT(bool, Result = T1::Result && T2::Result && T3::Result && T4::Result && T5::Result && T6::Result);
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct GAndResult5 {
	G_STATIC_CONSTANT(bool, Result = T1::Result && T2::Result && T3::Result && T4::Result && T5::Result);
};

template <typename T1, typename T2, typename T3, typename T4>
struct GAndResult4 {
	G_STATIC_CONSTANT(bool, Result = T1::Result && T2::Result && T3::Result && T4::Result);
};

template <typename T1, typename T2, typename T3>
struct GAndResult3 {
	G_STATIC_CONSTANT(bool, Result = T1::Result && T2::Result && T3::Result);
};

template <typename T1, typename T2>
struct GAndResult2 {
	G_STATIC_CONSTANT(bool, Result = T1::Result && T2::Result);
};


template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
struct GOrResult7 {
	G_STATIC_CONSTANT(bool, Result = T1::Result || T2::Result || T3::Result || T4::Result || T5::Result || T6::Result || T7::Result);
};

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
struct GOrResult6 {
	G_STATIC_CONSTANT(bool, Result = T1::Result || T2::Result || T3::Result || T4::Result || T5::Result || T6::Result);
};

template <typename T1, typename T2, typename T3, typename T4, typename T5>
struct GOrResult5 {
	G_STATIC_CONSTANT(bool, Result = T1::Result || T2::Result || T3::Result || T4::Result || T5::Result);
};

template <typename T1, typename T2, typename T3, typename T4>
struct GOrResult4 {
	G_STATIC_CONSTANT(bool, Result = T1::Result || T2::Result || T3::Result || T4::Result);
};

template <typename T1, typename T2, typename T3>
struct GOrResult3 {
	G_STATIC_CONSTANT(bool, Result = T1::Result || T2::Result || T3::Result);
};

template <typename T1, typename T2>
struct GOrResult2 {
	G_STATIC_CONSTANT(bool, Result = T1::Result || T2::Result);
};

template <bool B1, bool B2, bool B3 = true, bool B4 = true, bool B5 = true, bool B6 = true, bool B7 = true>
struct GBoolAnd;

template <bool B1, bool B2, bool B3, bool B4, bool B5, bool B6, bool B7>
struct GBoolAnd
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <>
struct GBoolAnd <true, true, true, true, true, true, true>
{
	G_STATIC_CONSTANT(bool, Result = true);
};


} // namespace cpgf


#endif
