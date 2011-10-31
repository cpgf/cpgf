#ifndef __GVARIANT_H
#define __GVARIANT_H

#include "cpgf/gvartypedata.h"
#include "cpgf/gerrorcode.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#pragma warning(disable:4800) // warning C4800: 'int' : forcing value to bool 'true' or 'false' (performance warning)
#endif

namespace cpgf {

class GVariant;

template <typename T>
bool canFromVariant(const GVariant & v);

namespace variant_internal {

template <bool CanShadow, typename T>
void InitVariant(GVariant & v, const GVarTypeData & typeData, const T & value);

template <typename T>
struct ArrayToPointer;

template <typename T, typename Enabled = void>
struct DeducePassType
{
	typedef T Result;
	typedef const typename RemoveReference<T>::Result & PassType;
};

template <typename T>
struct DeducePassType <T, typename GEnableIf<ArrayToPointer<T>::IsArray>::Result>
{
	typedef typename ArrayToPointer<T>::Result Result;
	typedef typename ArrayToPointer<T>::Result PassType;
};

class IVariantShadowObject
{
public:
	virtual void G_API_CC retain() = 0;
	virtual void G_API_CC release() = 0;
	virtual void * G_API_CC getObject() = 0;
};

template <typename T>
class GVariantShadowObject : public IVariantShadowObject
{
public:
	GVariantShadowObject(const T & obj) : refCount(1), obj(obj) {
	}

	virtual void G_API_CC retain() {
		++this->refCount;
	}

	virtual void G_API_CC release() {
		--this->refCount;
		if(!this->refCount) {
			delete this;
		}
	}

	virtual void * G_API_CC getObject() {
		return (void *)(&this->obj);
	}

private:
	int refCount;
	T obj;
};

struct GVariantDataHolder
{
	int a;
	int b;
	int c;
};


} // namespace variant_internal


inline bool vtIsEmpty(int vt) {
	return vt == vtEmpty;
}

inline bool vtIsFundamental(int vt) {
	return vt >= vtFundamentalBegin && vt <= vtFundamentalEnd;
}

inline bool vtIsBoolean(int vt) {
	return vt == vtBool;
}

inline bool vtIsInteger(int vt) {
	return vt >= vtIntegerBegin && vt <= vtIntegerEnd;
}

inline bool vtIsReal(int vt) {
	return vt >= vtFloat && vt <= vtLongDouble;
}

inline bool vtIsByPointer(int vt) {
	return (vt & byPointer) != 0;
}

inline bool vtIsByReference(int vt) {
	return (vt & byReference) != 0;
}

inline bool vtIsVoidPointer(int vt) {
	return vt == (byPointer | vtVoid);
}

#pragma pack(push, 1)
#pragma pack(1)
struct GVariantData
{
	GVarTypeData typeData;

	union {
		variant_internal::GVariantDataHolder holder;

		bool valueBool;
		char valueChar;
		wchar_t valueWchar;
		signed char valueSignedChar;
		unsigned char valueUnsignedChar;
		signed short valueSignedShort;
		unsigned short valueUnsignedShort;
		signed int valueSignedInt;
		unsigned int valueUnsignedInt;
		signed long valueSignedLong;
		unsigned long valueUnsignedLong;
		signed long long valueSignedLongLong;
		unsigned long long valueUnsignedLongLong;
		float valueFloat;
		double valueDouble;
		long double valueLongDouble;
		const volatile void * valuePointer;
		const volatile void * valueObject;

		const volatile bool * ptrBool;
		const volatile char * ptrChar;
		const volatile wchar_t * ptrWchar;
		const volatile signed char * ptrSignedChar;
		const volatile unsigned char * ptrUnsignedChar;
		const volatile signed short * ptrSignedShort;
		const volatile unsigned short * ptrUnsignedShort;;
		const volatile signed int * ptrSignedInt;
		const volatile unsigned int * ptrUnsignedInt;
		const volatile signed long * ptrSignedLong;
		const volatile unsigned long * ptrUnsignedLong;
		const volatile signed long long * ptrSignedLongLong;
		const volatile unsigned long long * ptrUnsignedLongLong;
		const volatile float * ptrFloat;
		const volatile double * ptrDouble;
		const volatile long double * ptrLongDouble;
		const volatile void * ptrPointer;
		const volatile void * ptrObject;

		variant_internal::IVariantShadowObject * shadowObject;

	};
};
#pragma pack(pop)

namespace variant_internal {

inline unsigned int getVariantTypeSize(GVariantType type);
inline void adjustVariantType(GVariant * data);

} // namespace variant_internal

class GMetaVariant;

class GVariant
{
//	GASSERT_STATIC(sizeof(GVariantData) == 16);

public:
	GVariant() {
		vtInit(data.typeData);
		data.holder.a = 0;
		data.holder.b = 0;
		data.holder.c = 0;
	}

	GVariant(const GVariantData & data) : data(data) {
		if(vtGetSize(this->data.typeData) != variant_internal::getVariantTypeSize(static_cast<GVariantType>(vtGetType(this->data.typeData)))) {
			variant_internal::adjustVariantType(this);
		}
	}

	template <typename T>
	GVariant(const T & value) {
		GVarTypeData typeData;
		deduceVariantType<T>(typeData);
		variant_internal::InitVariant<true>(*this, typeData, static_cast<typename variant_internal::DeducePassType<T>::PassType>(value));
	}

	template <typename T>
	GVariant(const GVarTypeData & typeData, const T & value) {
		variant_internal::InitVariant<true>(*this, typeData, value);
	}

	GVariant(const GVariant & other) : data(other.data) {
		if(vtGetType(this->data.typeData) == vtShadow) {
			this->data.shadowObject->retain();
		}
	}

	~GVariant() {
		if(vtGetType(this->data.typeData) == vtShadow) {
			this->data.shadowObject->release();
		}
	}

	GVariant & operator = (GVariant other) {
		this->swap(other);

		return *this;
	}

	void swap(GVariant & other) {
		using std::swap;

		swap(this->data.typeData, other.data.typeData);
		swap(this->data.holder, other.data.holder);
	}

	const GVarTypeData & getTypeData() const {
		return this->data.typeData;
	}

	GVariantType getType() const {
		return vtGetType(this->data.typeData);
	}

	GVariantType getBaseType() const {
		return vtGetBaseType(this->data.typeData);
	}

	int getPointers() const {
		return vtGetPointers(this->data.typeData);
	}

	bool isEmpty() const {
		return vtIsEmpty(this->getType());
	}

	GVariantData getData() const {
		return this->data;
	}

	GVariantData takeData() {
		GVariantData result = this->data;

		vtInit(this->data.typeData);

		return result;
	}

public:
	GVariantData data;

private:
	GVariant(const GMetaVariant &); //disable
};

inline void swap(GVariant & a, GVariant & b)
{
	a.swap(b);
}

extern void raiseException(int errorCode, const std::string & message);

inline void failedCast() {
	raiseException(Error_Variant_FailCast, "GVariant: cast failure");
}

inline void checkFailCast(bool success) {
	if(!success) {
		failedCast();
	}
}


#include "cpgf/private/gvariant_p.h"


template <bool CanShadow, typename T>
GVariant createVariant(const T & value, bool allowShadow = false)
{
	GVarTypeData typeData;
	deduceVariantType<T>(typeData, allowShadow);
	GVariant v;
	variant_internal::InitVariant<CanShadow>(v, typeData, static_cast<typename variant_internal::DeducePassType<T>::PassType>(value));
	return v;
}

template <typename T>
GVariant createVariantFromCopyable(const T & value, bool allowShadow)
{
	return createVariant<true>(value, allowShadow);
}

template <typename T>
GVariant createVariantFromCopyable(const T & value)
{
	return createVariant<true>(value, false);
}

template <typename T>
bool canFromVariant(const GVariant & v)
{
	return variant_internal::CanCastFromVariant<typename RemoveConstVolatile<T>::Result, false>::canCast(vtGetType(v.data.typeData));
}

template <typename T, bool KeepConstRef>
bool canFromVariant(const GVariant & v)
{
	return variant_internal::CanCastFromVariant<typename RemoveConstVolatile<T>::Result, KeepConstRef>::canCast(vtGetType(v.data.typeData));
}

template <typename T>
typename variant_internal::CastFromVariant<T, false>::ResultType fromVariant(const GVariant & v)
{
	return variant_internal::CastFromVariant<typename RemoveConstVolatile<T>::Result, false>::cast(v);
}

template <typename T, bool KeepConstRef>
typename variant_internal::CastFromVariant<T, KeepConstRef>::ResultType fromVariant(const GVariant & v)
{
	return variant_internal::CastFromVariant<typename RemoveConstVolatile<T>::Result, KeepConstRef>::cast(v);
}

template <typename T>
GVariant pointerToRefVariant(T * p)
{
	GVariant v(p);

	if(vtIsByPointer(v.getType())) {
		vtSetType(v.data.typeData, (vtGetType(v.data.typeData) & ~byPointer) | byReference);
	}

	return v;
}

inline GVariant pointerToRefVariant(const GVariant & p)
{
	GVariant v(p);

	if(vtIsByPointer(v.getType())) {
		vtSetType(v.data.typeData, (vtGetType(v.data.typeData) & ~byPointer) | byReference);
	}

	return v;
}

inline GVariant pointerToObjectVariant(void * p)
{
	GVariant result;

	vtSetType(result.data.typeData, vtObject | byPointer);
	result.data.ptrObject = p;
	vtSetSize(result.data.typeData, sizeof(void *));

	return result;
}

inline void * objectAddressFromVariant(const GVariant & v)
{
	if(v.getType() == vtShadow) {
		return v.data.shadowObject->getObject();
	}

	return fromVariant<void *>(v);
}

inline void * referenceAddressFromVariant(const GVariant & v)
{
	if(vtIsByReference(v.getType())) {
		return const_cast<void *>(v.data.ptrObject);
	}
	else {
		return objectAddressFromVariant(v);
	}
}

inline void initializeVarData(GVariantData * data)
{
	vtInit(data->typeData);
}



} // namespace cpgf


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


#endif

