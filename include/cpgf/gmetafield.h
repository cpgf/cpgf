#ifndef __GMETAFIELD_H
#define __GMETAFIELD_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"


#define GMETA_FIELD(field) \
	reflectField(cpgf::normalizeReflectName(GPP_STRINGIZE(field)).c_str(), &MetaCurrentClassType::field)

#define GMETA_QUALIFIED_FIELD(field) \
	using namespace cpgf; \
	reflectField(cpgf::normalizeReflectName(GPP_STRINGIZE(field)).c_str(), &field)


namespace cpgf {

template <typename FT>
void reflectField(const char * name, FT field);


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
	enum {
		Readable = NotHasMetaPolicyItem<Policy, GMetaPolicyItemForbidRead>::Result,
		Writable = NotHasMetaPolicyItem<Policy, GMetaPolicyItemForbidWrite>::Result
	};

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
		return GVariant(deduceVariantType<FT>(true), deduceVariantPointers<FT>(), *(this->field));
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
	enum {
		Readable = NotHasMetaPolicyItem<Policy, GMetaPolicyItemForbidRead>::Result,
		Writable = NotHasMetaPolicyItem<Policy, GMetaPolicyItemForbidWrite>::Result
	};

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
		return GVariant(deduceVariantType<FT>(true), deduceVariantPointers<FT>(), static_cast<OT *>(instance)->*(this->field));
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


class GMetaField : public GMetaAccessible
{
private:
	typedef GMetaAccessible super;

public:
	template <typename OT, typename FT, typename Policy>
	GMetaField(const char * name, FT OT::* field, const Policy &)
		: super(name, createMetaType<FT>(), mcatField), baseData(new meta_internal::GMetaFieldDataMember<OT, FT, Policy>(field)) {
	}

	template <typename FT, typename Policy>
	GMetaField(const char * name, FT * field, const Policy &)
		: super(name, createMetaType<FT>(), mcatField), baseData(new meta_internal::GMetaFieldDataGlobal<FT, Policy>(field)) {
		this->addModifier(metaModifierStatic);
	}

	virtual bool canGet() const;
	virtual bool canSet() const;

	virtual GVariant get(void * instance) const;
	virtual void set(void * instance, const GVariant & v) const;

	virtual size_t getSize() const;

	void * getAddress(void * instance) const;
	
	virtual GMetaConverter * createConverter() const;

private:
	GScopedPointer<meta_internal::GMetaFieldDataBase> baseData;
};


void globalAddField(GMetaField * field);

template <typename FT>
void reflectField(const char * name, FT field)
{
	globalAddField(new cpgf::GMetaField(name, field, GMetaPolicyDefault()));
}

template <typename FT, typename Policy>
void reflectField(const char * name, FT field, const Policy & policy)
{
	globalAddField(new cpgf::GMetaField(name, field, policy));
}



} // namespace cpgf



#endif
