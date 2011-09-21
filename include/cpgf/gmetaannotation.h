#ifndef __GMETAANNOTATION_H
#define __GMETAANNOTATION_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gscopedptr.h"

#include <string>
#include <string.h>



#define GMETA_ANNOTATION(anno) \
	using namespace cpgf; \
	reflectAnnotation(GPP_STRINGIZE(anno))

#define GMETA_QUALIFIED_ANNOTATION(anno) GMETA_ANNOTATION(anno)



namespace cpgf {

inline cpgf::GMetaAnnotation & reflectAnnotation(const char * name);
inline void flushAnnotation();


const GVariantType vtAnnoString = vtUserBegin + 0;
const GVariantType vtAnnoWideString = vtUserBegin + 1;


namespace meta_internal {

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
	enum { Result = false };
};

template <unsigned int N>
struct IsCharArray <char[N]>
{
	enum { Result = true };
};

template <unsigned int N>
struct IsCharArray <const char[N]>
{
	enum { Result = true };
};

template <typename T>
struct IsWideCharArray
{
	enum { Result = false };
};

template <unsigned int N>
struct IsWideCharArray <wchar_t[N]>
{
	enum { Result = true };
};

template <unsigned int N>
struct IsWideCharArray <const wchar_t[N]>
{
	enum { Result = true };
};

template <typename T, typename Enabled = void>
struct InitAnnoVariant
{
	static void init(GVariant & var, const T & value) {
		GVariantType vt = deduceVariantType<T>(true);
		variant_internal::InitVariant(var, vt, static_cast<typename variant_internal::DeducePassType<T>::PassType>(value));
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIf<
	IsSameType<T, char *>::Result || IsSameType<T, const char *>::Result
	|| IsSameType<T, char * &>::Result || IsSameType<T, const char * &>::Result
	|| IsCharArray<typename RemoveReference<T>::Result>::Result
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		var.data.type = static_cast<unsigned short>(vtAnnoString);
		var.data.valueObject = duplicateAnnoString(value);
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIf<
	IsSameType<T, std::string>::Result || IsSameType<T, std::string &>::Result || IsSameType<T, const std::string &>::Result
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		var.data.type = static_cast<unsigned short>(vtAnnoString);
		var.data.valueObject = duplicateAnnoString(value.c_str());
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIf<
	IsSameType<T, wchar_t *>::Result || IsSameType<T, const wchar_t *>::Result
	|| IsSameType<T, wchar_t * &>::Result || IsSameType<T, const wchar_t * &>::Result
	|| IsWideCharArray<typename RemoveReference<T>::Result>::Result
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		var.data.type = static_cast<unsigned short>(vtAnnoWideString);
		var.data.valueObject = duplicateAnnoWideString(value);
	}
};

template <typename T>
struct InitAnnoVariant <T, typename GEnableIf<
	IsSameType<T, std::wstring>::Result || IsSameType<T, std::wstring &>::Result || IsSameType<T, const std::wstring &>::Result
	>::Result>
{
	static void init(GVariant & var, const T & value) {
		var.data.type = static_cast<unsigned short>(vtAnnoWideString);
		var.data.valueObject = duplicateAnnoWideString(value.c_str());
	}
};


class GAnnotationItemImplement;
class GMetaAnnotationImplement;

} // namespace meta_internal


class GAnnotationValue
{
public:
	GAnnotationValue() {
	}

	template <typename T>
	explicit GAnnotationValue(const T & value) {
		meta_internal::InitAnnoVariant<T>::init(this->var, value);
	}

	GAnnotationValue(const GAnnotationValue & other);
	GAnnotationValue & operator = (GAnnotationValue other);

	~GAnnotationValue();

	void swap(GAnnotationValue & other);

	const GVariant * getVariant() const {
		return &this->var;
	}

	bool canToString() const {
		return this->var.getType() == vtAnnoString;
	}

	bool canToWideString() const {
		return this->var.getType() == vtAnnoWideString;
	}

	bool canToInt() const {
		return canFromVariant<int>(this->var);
	}

	const char * toString() const;
	const wchar_t * toWideString() const;

	int toInt() const {
		return fromVariant<int>(this->var);
	}

	template <typename T>
	T toObject() const {
		return fromVariant<T>(this->var);
	}

private:
	GVariant var;
};


class GAnnotationItem
{
public:
	GAnnotationItem();
	~GAnnotationItem();

	GAnnotationItem(const char * name, const GAnnotationValue & value);

	GAnnotationItem(const GAnnotationItem & other);

	GAnnotationItem & operator = (const GAnnotationItem & other);

	const char * getName() const;
	const GAnnotationValue * getValue() const;
	
private:
	GScopedPointer<meta_internal::GAnnotationItemImplement> implement;
	GAnnotationValue value;
};


class GMetaAnnotation : public GMetaItem
{
private:
	typedef GMetaItem super;

public:
	GMetaAnnotation(const char * name);
	virtual ~GMetaAnnotation();

	template <typename T>
	void addItem(const char * name, const T & value) {
		this->doAddItem(new GAnnotationItem(name, GAnnotationValue(value)));
	}

	template <typename T>
	GMetaAnnotation & operator() (const char * name, const T & value) {
		this->addItem(name, value);

		return *this;
	}

	const GMetaItem * getMetaItem() const {
		return this->metaItem;
	}

	const GAnnotationValue * getValue(const char * name) const;
	size_t getCount() const;
	const char * getNameAt(size_t index) const;
	const GAnnotationValue * getValueAt(size_t index) const;

private:
	void setMetaItem(const GMetaItem * metaItem) {
		this->metaItem = metaItem;
	}

	void clear();
	
	void doAddItem(GAnnotationItem * item);

private:
	GScopedPointer<meta_internal::GMetaAnnotationImplement> implement;
	const GMetaItem * metaItem;

private:
	friend class GMetaClass;
};


GMetaAnnotation & globaAddAnnotation(GMetaAnnotation * annotation);
void globalFlushAnnotation();

inline cpgf::GMetaAnnotation & reflectAnnotation(const char * name)
{
	return globaAddAnnotation(new cpgf::GMetaAnnotation(name));
};

inline void flushAnnotation()
{
	globalFlushAnnotation();
}


} // namespace cpgf

#endif
