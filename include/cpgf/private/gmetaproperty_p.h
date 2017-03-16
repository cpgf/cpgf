#ifndef CPGF_GMETAPROPERTY_P_H
#define CPGF_GMETAPROPERTY_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gcallback.h"
#include "cpgf/gifelse.h"
#include "cpgf/accessor/ggetter.h"
#include "cpgf/accessor/gsetter.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // conditional expression is constant
#endif

#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8008 //Condition is always true
#pragma warn -8066 //Unreachable code
#endif


namespace cpgf {

namespace meta_internal {

template <typename Getter, typename Policy>
class GMetaGetter : public GInstanceGetter <Getter, Policy>
{
private:
	typedef GInstanceGetter <Getter, Policy> super;

public:
	GMetaGetter(const Getter & getter) : super(getter) {
	}

	GVariant get(const void * instance) const {
		return this->doGet<void>(instance);
	}

	void * getPropertyAddress(const void * instance) const {
		return super::getAddress(instance);
	}

private:	
	template <typename T>
	GVariant doGet(typename GEnableIf<super::Readable, T>::Result const * instance) const {
		return createVariant<typename super::ValueType>(super::get(instance), true);
	}

	template <typename T>
	GVariant doGet(typename GDisableIf<super::Readable, T>::Result const * /*instance*/) const {
		raiseCoreException(Error_Meta_ReadDenied);
		
		return GVariant();
	}
};


template <typename Setter, typename Policy>
class GMetaSetter : public GInstanceSetter <Setter, Policy>
{
private:
	typedef GInstanceSetter <Setter, Policy> super;

public:
	GMetaSetter(const Setter & setter) : super(setter) {
	}

	void set(void * instance, const GVariant & value) const {
		super::set(instance, fromVariant<typename super::PassType>(value));
	}

	void * getPropertyAddress(void * instance) const {
		return super::getAddress(instance);
	}

};


struct GMetaPropertyDataVirtual
{
	void (*deleteSelf)(void * self);
	bool (*canGet)();
	bool (*canSet)();
	GVariant (*get)(const void * self, const void * instance);
	void (*set)(const void * self, void * instance, const GVariant & v);
	size_t (*getPropertySize)();
	void * (*getPropertyAddress)(const void * self, const void * instance);
	GMetaExtendType (*getItemExtendType)(uint32_t flags, const GMetaItem * metaItem);
};

class GMetaPropertyDataBase
{
public:
	void deleteSelf();
	
	bool canGet() const;
	bool canSet() const;

	GVariant get(const void * instance) const;
	void set(void * instance, const GVariant & v) const;

	size_t getPropertySize() const;
	void * getPropertyAddress(const void * instance) const;

	// must be defined in header to make template function overloading happy.
	GMetaExtendType getItemExtendType(uint32_t flags, const GMetaItem * metaItem) const {
		return this->virtualFunctions->getItemExtendType(flags, metaItem);
	}

protected:
	GMetaPropertyDataVirtual * virtualFunctions;
};

template <typename Getter, typename Setter, typename Policy>
class GMetaPropertyData : public GMetaPropertyDataBase
{
private:
	typedef GMetaGetter<Getter, Policy> GetterType;
	typedef GMetaSetter<Setter, Policy> SetterType;
	
private:
	static bool virtualCanGet() {
		return GetterType::Readable;
	}

	static bool virtualCanSet() {
		return SetterType::Writable;
	}

	static GVariant virtualGet(const void * self, const void * instance) {
		return static_cast<const GMetaPropertyData *>(self)->metaGetter.get(instance);
	}

	static void virtualSet(const void * self, void * instance, const GVariant & value) {
		static_cast<const GMetaPropertyData *>(self)->metaSetter.set(instance, value);
	}

	static void * virtualGetPropertyAddress(const void * self, const void * instance) {
		return static_cast<const GMetaPropertyData *>(self)->metaGetter.getPropertyAddress(instance);
	}

	static size_t virtualGetPropertySize() {
		if(GetterType::HasGetter) {
			return sizeof(typename GetterType::ValueType);
		}

		if(SetterType::HasSetter) {
			return sizeof(typename SetterType::ValueType);
		}

		raiseCoreException(Error_Meta_ReadDenied);
		
		return 0;
	}

	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem)
	{
		if(GetterType::HasGetter) {
			return createMetaExtendType<typename GetterType::ValueType>(flags, metaItem);
		}

		if(SetterType::HasSetter) {
			return createMetaExtendType<typename SetterType::ValueType>(flags, metaItem);
		}

		return GMetaExtendType();
	}

public:
	GMetaPropertyData(const Getter & getter, const Setter & setter) : metaGetter(getter), metaSetter(setter) {
		static GMetaPropertyDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaPropertyData>,
			&virtualCanGet,
			&virtualCanSet,
			&virtualGet,
			&virtualSet,
			&virtualGetPropertySize,
			&virtualGetPropertyAddress,
			virtualGetItemExtendType
		};
		this->virtualFunctions = &thisFunctions;
	}

private:
	GMetaGetter<Getter, Policy> metaGetter;
	GMetaSetter<Setter, Policy> metaSetter;
};


template <typename Getter, typename Setter, typename Policy>
GMetaType createPropertyType() {
	if(GMetaGetter<Getter, Policy>::HasGetter) {
		return createMetaType<typename GMetaGetter<Getter, Policy>::ValueType>();
	}

	if(GMetaSetter<Setter, Policy>::HasSetter) {
		return createMetaType<typename GMetaSetter<Setter, Policy>::ValueType>();
	}

	return GMetaType();
}

} // namespace meta_internal




} // namespace cpgf


#ifdef G_COMPILER_CPPBUILDER
#pragma warn .8008 //Condition is always true
#pragma warn .8066 //Unreachable code
#endif


#if defined(_MSC_VER)
#pragma warning(pop)
#endif



#endif

