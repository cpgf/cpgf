#ifndef CPGF_GMETAFIELD_P_H
#define CPGF_GMETAFIELD_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gexception.h"


namespace cpgf {


extern int Error_Meta_ReadDenied;
extern int Error_Meta_WriteDenied;

namespace meta_internal {

struct GMetaFieldDataVirtual
{
	void (*deleteSelf)(void * self);
	bool (*canGet)();
	bool (*canSet)();
	GVariant (*get)(const void * self, const void * instance);
	void (*set)(const void * self, void * instance, const GVariant & v);
	size_t (*getFieldSize)();
	void * (*getFieldAddress)(const void * self, const void * instance);
	GMetaExtendType (*getItemExtendType)(uint32_t flags, const GMetaItem * metaItem);
};

class GMetaFieldDataBase
{
public:
	void deleteSelf();

	bool canGet() const;
	bool canSet() const;

	GVariant get(const void * instance) const;
	void set(void * instance, const GVariant & v) const;

	size_t getFieldSize() const;
	void * getFieldAddress(const void * instance) const;

	// must be defined in header to make template function overloading happy.
	GMetaExtendType getItemExtendType(uint32_t flags, const GMetaItem * metaItem) const {
		return this->virtualFunctions->getItemExtendType(flags, metaItem);
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
	static bool virtualCanGet() {
		return Readable;
	}

	static bool virtualCanSet() {
		return Writable;
	}

	static GVariant virtualGet(const void * self, const void * instance) {
		return static_cast<const GMetaFieldDataGlobal *>(self)->doGet<void>(instance);
	}

	static void virtualSet(const void * self, void * instance, const GVariant & value) {
		static_cast<const GMetaFieldDataGlobal *>(self)->doSet<void>(instance, value);
	}

	static size_t virtualGetFieldSize() {
		return sizeof(FT);
	}

	static void * virtualGetFieldAddress(const void * self, const void * /*instance*/) {
		return (void *)(static_cast<const GMetaFieldDataGlobal *>(self)->field);
	}
	
	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem)
	{
		return createMetaExtendType<FT>(flags, metaItem);
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
	GVariant  doGet(typename GEnableIf<Readable, T>::Result const * /*instance*/) const {
		return createVariant<FT>(*(this->field), true);
	}

	template <typename T>
	GVariant  doGet(typename GDisableIf<Readable, T>::Result const * /*instance*/) const {
		raiseCoreException(Error_Meta_ReadDenied);
		
		return GVariant();
	}

	template <typename T>
	void doSet(typename GEnableIf<Writable && !IsArray<FT>::Result, T>::Result * /*instance*/, const GVariant & value) const {
		*(this->field) = fromVariant<FT>(value);
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable && !IsArray<FT>::Result, T>::Result * /*instance*/, const GVariant & /*value*/) const {
		raiseCoreException(Error_Meta_WriteDenied);
	}

private:
	mutable FT * field;
};

template <typename OT, typename FT, typename Policy>
class GMetaFieldDataMember : public GMetaFieldDataBase
{
private:
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result && !IsConst<FT>::Result));

private:
	static bool virtualCanGet() {
		return Readable;
	}

	static bool virtualCanSet() {
		return Writable;
	}

	static GVariant virtualGet(const void * self, const void * instance) {
		return static_cast<const GMetaFieldDataMember *>(self)->doGet<void>(instance);
	}

	static void virtualSet(const void * self, void * instance, const GVariant & value) {
		static_cast<const GMetaFieldDataMember *>(self)->doSet<void>(instance, value);
	}

	static size_t virtualGetFieldSize() {
		return sizeof(FT);
	}

	static void * virtualGetFieldAddress(const void * self, const void * instance) {
		return (void *)(&(static_cast<OT *>(const_cast<void *>(instance))->*(static_cast<const GMetaFieldDataMember *>(self)->field)));
	}
	
	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem)
	{
		return createMetaExtendType<FT>(flags, metaItem);
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
	GVariant  doGet(typename GEnableIf<Readable, T>::Result const * instance) const {
		return createVariant<FT>(static_cast<const OT *>(instance)->*(this->field), true);
	}

	template <typename T>
	GVariant  doGet(typename GDisableIf<Readable, T>::Result const * /*instance*/) const {
		raiseCoreException(Error_Meta_ReadDenied);
		
		return GVariant();
	}

	template <typename T>
	void doSet(typename GEnableIf<Writable && !IsArray<FT>::Result, T>::Result * instance, const GVariant & value) const {
		static_cast<OT *>(instance)->*(this->field) = fromVariant<FT>(value);
	}

	template <typename T>
	void doSet(typename GDisableIf<Writable && !IsArray<FT>::Result, T>::Result * /*instance*/, const GVariant & /*value*/) const {
		raiseCoreException(Error_Meta_WriteDenied);
	}

private:
	mutable FT OT::* field;
};


} // namespace meta_internal



} // namespace cpgf



#endif
