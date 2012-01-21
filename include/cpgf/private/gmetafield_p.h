#ifndef __GMETAFIELD_P_H
#define __GMETAFIELD_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"


namespace cpgf {

namespace meta_internal {

class GMetaFieldDataBase
{
public:
	virtual bool canGet() const = 0;
	virtual bool canSet() const = 0;
	virtual GVariant get(void * instance) const = 0;
	virtual void set(void * instance, const GVariant & v) const = 0;
	virtual size_t getFieldSize() const = 0;
	virtual void * getFieldAddress(void * instance) const = 0;
	virtual GMetaConverter * createConverter() const = 0;
};


template <typename FT, typename Policy>
class GMetaFieldDataGlobal : public GMetaFieldDataBase
{
private:
	G_STATIC_CONSTANT(bool, Readable = (PolicyNotHasRule<Policy, GMetaRuleForbidRead>::Result));
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result));

public:
	GMetaFieldDataGlobal(FT * field) : field(field) {
	}

	~GMetaFieldDataGlobal() {
	}

	virtual bool canGet() const {
		return Readable;
	}

	virtual bool canSet() const {
		return Writable;
	}

	virtual GVariant get(void * instance) const {
		return this->doGet<void>(instance);
	}

	virtual void set(void * instance, const GVariant & value) const {
		this->doSet<void>(instance, value);
	}

	virtual size_t getFieldSize() const {
		return sizeof(FT);
	}

	virtual void * getFieldAddress(void * instance) const {
		(void)instance;
		return this->field;
	}
	
	virtual GMetaConverter * createConverter() const {
		return GMetaConverterTraits<FT>::createConverter();
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
	G_STATIC_CONSTANT(bool, Writable = (PolicyNotHasRule<Policy, GMetaRuleForbidWrite>::Result));

public:
	GMetaFieldDataMember(FT OT::* field) : field(field) {
	}

	~GMetaFieldDataMember() {
	}

	virtual bool canGet() const {
		return Readable;
	}

	virtual bool canSet() const {
		return Writable;
	}

	virtual GVariant get(void * instance) const {
		return this->doGet<void>(instance);
	}

	virtual void set(void * instance, const GVariant & value) const {
		this->doSet<void>(instance, value);
	}

	virtual size_t getFieldSize() const {
		return sizeof(FT);
	}

	virtual void * getFieldAddress(void * instance) const {
		return &(static_cast<OT *>(instance)->*(this->field));
	}
	
	virtual GMetaConverter * createConverter() const {
		return GMetaConverterTraits<FT>::createConverter();
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
