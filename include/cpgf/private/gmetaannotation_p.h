#ifndef __GMETAANNOTATION_P_H
#define __GMETAANNOTATION_P_H

#include "cpgf/gmetacommon.h"


namespace cpgf {

namespace meta_internal {

const GVariantType vtAnnoString = vtUserBegin + 0;
const GVariantType vtAnnoWideString = vtUserBegin + 1;

inline std::string * duplicateAnnoString(const volatile char * s)
{
	return new std::string(const_cast<const char *>(s));
}

inline std::wstring * duplicateAnnoWideString(const volatile wchar_t * s)
{
	return new std::wstring(const_cast<const wchar_t *>(s));
}

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
		GVarTypeData data = GVarTypeData();
		deduceVariantType<T>(data, true);
		variant_internal::InitVariant<true>(var, data, static_cast<typename variant_internal::DeducePassType<T>::PassType>(value));
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIfResult<
	GOrResult5<
		IsSameType<T, char *>,
		IsSameType<T, const char *>,
		IsSameType<T, char * &>,
		IsSameType<T, const char * &>,
		IsCharArray<typename RemoveReference<T>::Result>
	>
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		vtSetType(var.data.typeData, vtAnnoString);
		var.data.valueObject = duplicateAnnoString(value);
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIfResult<
	GOrResult3<
		IsSameType<T, std::string>,
		IsSameType<T, std::string &>,
		IsSameType<T, const std::string &>
	>
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		vtSetType(var.data.typeData, vtAnnoString);
		var.data.valueObject = duplicateAnnoString(value.c_str());
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIfResult<
	GOrResult5<
		IsSameType<T, wchar_t *>,
		IsSameType<T, const wchar_t *>,
		IsSameType<T, wchar_t * &>,
		IsSameType<T, const wchar_t * &>,
		IsWideCharArray<typename RemoveReference<T>::Result>
	>
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		vtSetType(var.data.typeData, vtAnnoWideString);
		var.data.valueObject = duplicateAnnoWideString(value);
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIfResult<
	GOrResult3<
		IsSameType<T, std::wstring>,
		IsSameType<T, std::wstring &>,
		IsSameType<T, const std::wstring &>
	>
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		vtSetType(var.data.typeData, vtAnnoWideString);
		var.data.valueObject = duplicateAnnoWideString(value.c_str());
	}
};


class GAnnotationItemImplement;
class GMetaAnnotationImplement;

} // namespace meta_internal




} // namespace cpgf

#endif
