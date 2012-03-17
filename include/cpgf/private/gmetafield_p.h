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
	GMetaExtendType (*getItemExtendType)(const void * self, uint32_t flags);
};

class GMetaFieldDataBase
{
public:
	void deleteObject();

	bool canGet() const;
	bool canSet() const;

	GVariant get(void * instance) const;
	void set(void * instance, const GVariant & v) const;

	size_t getFieldSize() const;
	void * getFieldAddress(void * instance) const;

	GMetaExtendType getItemExtendType(uint32_t flags) const;

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
	
	static GMetaExtendType virtualGetItemExtendType(const void * self, uint32_t flags)
	{
		(void)self;
		
		return createMetaExtendType<FT>(flags);
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
			&virtualGetItemExtendType
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
	
	static GMetaExtendType virtualGetItemExtendType(const void * self, uint32_t flags)
	{
		(void)self;
		
		return createMetaExtendType<FT>(flags);
	}

public:
	GMetaFieldDataMember(FT OT::* field) : field(field) {
		static GMetaFieldDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaFieldDataMember>,
			&virtualCanGet, &virtualCanSet,
			&virtualGet, &virtualSet,
			&virtualGetFieldSize, &virtualGetFieldAddress,
			&virtualGetItemExtendType
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
