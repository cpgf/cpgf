#ifndef CPGF_GMETAANNOTATION_P_H
#define CPGF_GMETAANNOTATION_P_H

#include "cpgf/gmetacommon.h"


namespace cpgf {

namespace meta_internal {

template <typename T>
struct IsCharArray
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <unsigned int N>
struct IsCharArray <char[N]>
{
	G_STATIC_CONSTANT(bool, Result = true);
};

template <unsigned int N>
struct IsCharArray <const char[N]>
{
	G_STATIC_CONSTANT(bool, Result = true);
};

template <typename T>
struct IsWideCharArray
{
	G_STATIC_CONSTANT(bool, Result = false);
};

template <unsigned int N>
struct IsWideCharArray <wchar_t[N]>
{
	G_STATIC_CONSTANT(bool, Result = true);
};

template <unsigned int N>
struct IsWideCharArray <const wchar_t[N]>
{
	G_STATIC_CONSTANT(bool, Result = true);
};

template <typename T, typename Enabled = void>
struct InitAnnoVariant
{
	static void init(GVariant & var, const T & value) {
		var = createVariant<T>(value, true);
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIfResult<
	GOrResult<
		IsSameType<T, char *>,
		IsSameType<T, const char *>,
		IsSameType<T, char * &>,
		IsSameType<T, const char * &>,
		IsCharArray<typename RemoveReference<T>::Result>
	>
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		var = createStringVariant(value);
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIfResult<
	GOrResult<
		IsSameType<T, std::string>,
		IsSameType<T, std::string &>,
		IsSameType<T, const std::string &>
	>
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		var = createStringVariant(value.c_str());
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIfResult<
	GOrResult<
		IsSameType<T, wchar_t *>,
		IsSameType<T, const wchar_t *>,
		IsSameType<T, wchar_t * &>,
		IsSameType<T, const wchar_t * &>,
		IsWideCharArray<typename RemoveReference<T>::Result>
	>
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		var = createWideStringVariant(value);
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIfResult<
	GOrResult<
		IsSameType<T, std::wstring>,
		IsSameType<T, std::wstring &>,
		IsSameType<T, const std::wstring &>
	>
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		var = createWideStringVariant(value.c_str());
	}
};


class GAnnotationItemImplement;
class GMetaAnnotationImplement;

} // namespace meta_internal




} // namespace cpgf

#endif
