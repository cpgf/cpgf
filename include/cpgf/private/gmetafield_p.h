#ifndef __GMETAFIELD_P_H
#define __GMETAFIELD_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"


namespace cpgf {

namespace meta_internal {

struct GMetaFieldDataVirtual
{
	void (*deleteObject)(void * self);
	bool (*canGet)(const void * self);
	bool (*canSet)(const void * self);
	GVariant (*get)(const void * self, void * instance);
	void (*set)(const void * self, void * instance, const GVariant & v);
	size_t (*getFieldSize)(const void * self);
	void * (*getFieldAddress)(const void * self, void * instance);
	GMetaConverter * (*createConverter)(const void * self);
};

class GMetaFieldDataBase
{
public:
	void deleteObject() {
		this->virtualFunctions->deleteObject(this);
	}

	bool canGet() const {
		return this->virtualFunctions->canGet(this);
	}

	bool canSet() const {
		return this->virtualFunctions->canSet(this);
	}

	GVariant get(void * instance) const {
		return this->virtualFunctions->get(this, instance);
	}

	void set(void * instance, const GVariant & v) const {
		this->virtualFunctions->set(this, instance, v);
	}

	size_t getFieldSize() const {
		return this->virtualFunctions->getFieldSize(this);
	}

	void * getFieldAddress(void * instance) const {
		return this->virtualFunctions->getFieldAddress(this, instance);
	}

	GMetaConverter * createConverter() const {
		return this->virtualFunctions->createConverter(this);
	}

protected:
	GMetaFieldDataVirtual * virtualFunctions;
};


template <typename FT, typename Policy>
class GMetaFieldDataGlobal : public GMetaFieldDataBase
{
private:
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result && !IsConst<FT>::Result));

private:
	static bool virtualCanGet(const void * self) {
		(void)self;

		return Readable;
	}

	static bool virtualCanSet(const void * self) {
		(void)self;

		return Writable;
	}

	static GVariant virtualGet(const void * self, void * instance) {
		return static_cast<const GMetaFieldDataGlobal *>(self)->doGet<void>(instance);
	}

	static void virtualSet(const void * self, void * instance, const GVariant & value) {
		static_cast<const GMetaFieldDataGlobal *>(self)->doSet<void>(instance, value);
	}

	static size_t virtualGetFieldSize(const void * self) {
		(void)self;

		return sizeof(FT);
	}

	static void * virtualGetFieldAddress(const void * self, void * instance) {
		(void)instance;
		return (void *)(static_cast<const GMetaFieldDataGlobal *>(self)->field);
	}
	
	static GMetaConverter * virtualCreateConverter(const void * self) {
		(void)self;

		return GMetaConverterTraits<FT>::createConverter();
	}

public:
	GMetaFieldDataGlobal(FT * field) : field(field) {
		static GMetaFieldDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaFieldDataGlobal>,
			&virtualCanGet,
			&virtualCanSet,
			&virtualGet,
			&virtualSet,
			&virtualGetFieldSize,
			&virtualGetFieldAddress,
			&virtualCreateConverter
		};
		this->virtualFunctions = &thisFunctions;
	}

private:	
	template <typename T>
	GVariant  doGet(typename GEnableIf<Readable, T>::Result * instance) const {
		(void)instance;

		GVarTypeData data = GVarTypeData();
		deduceVariantType<FT>(data, true);
		return GVariant(data, *(this->field));
	}

	template <typename T>
	GVariant  doGet(typename GDisableIf<Readable, T>::Result * instance) const {
		(void)instance;

		meta_internal::handleForbidAccessError(true);
		
		return GVariant();
	}

	template <typename T>
	void doSet(typename GEnableIf<Writable, T>::Result * instance, const GVariant & value) const {
		(void)instance;
		*(this->field) = fromVariant<FT>(value);
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable, T>::Result * instance, const GVariant & value) const {
		(void)instance;
		(void)value;

		meta_internal::handleForbidAccessError(false);
	}

private:
	mutable FT * field;
};

template <typename OT, typename FT, typename Policy>
class GMetaFieldDataMember : public GMetaFieldDataBase
{
private:
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result && !IsArray<FT>::Result));

private:
	static bool virtualCanGet(const void * self) {
		(void)self;

		return Readable;
	}

	static bool virtualCanSet(const void * self) {
		(void)self;

		return Writable;
	}

	static GVariant virtualGet(const void * self, void * instance) {
		return static_cast<const GMetaFieldDataMember *>(self)->doGet<void>(instance);
	}

	static void virtualSet(const void * self, void * instance, const GVariant & value) {
		static_cast<const GMetaFieldDataMember *>(self)->doSet<void>(instance, value);
	}

	static size_t virtualGetFieldSize(const void * self) {
		(void)self;

		return sizeof(FT);
	}

	static void * virtualGetFieldAddress(const void * self, void * instance) {
		(void)instance;
		return &(static_cast<OT *>(instance)->*(static_cast<const GMetaFieldDataMember *>(self)->field));
	}
	
	static GMetaConverter * virtualCreateConverter(const void * self) {
		(void)self;

		return GMetaConverterTraits<FT>::createConverter();
	}

public:
	GMetaFieldDataMember(FT OT::* field) : field(field) {
		static GMetaFieldDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaFieldDataMember>,
			&virtualCanGet, &virtualCanSet,
			&virtualGet, &virtualSet,
			&virtualGetFieldSize, &virtualGetFieldAddress,
			&virtualCreateConverter
		};
		this->virtualFunctions = &thisFunctions;
	}

private:
	template <typename T>
	GVariant  doGet(typename GEnableIf<Readable, T>::Result * instance) const {
		GVarTypeData data = GVarTypeData();
		deduceVariantType<FT>(data, true);
		return GVariant(data, static_cast<OT *>(instance)->*(this->field));
	}

	template <typename T>
	GVariant  doGet(typename GDisableIf<Readable, T>::Result * instance) const {
		(void)instance;

		meta_internal::handleForbidAccessError(true);
		
		return GVariant();
	}

	template <typename T>
	void doSet(typename GEnableIf<Writable, T>::Result * instance, const GVariant & value) const {
		static_cast<OT *>(instance)->*(this->field) = fromVariant<FT>(value);
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable, T>::Result * instance, const GVariant & value) const {
		(void)instance;
		(void)value;

		meta_internal::handleForbidAccessError(false);
	}

private:
	mutable FT OT::* field;
};


} // namespace meta_internal



} // namespace cpgf



#endif
