#ifndef __GMETAFUNDAMENTAL_H
#define __GMETAFUNDAMENTAL_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"


namespace cpgf {


namespace meta_internal {

class GMetaFundamentalData
{
public:
	virtual size_t getTypeSize() const = 0;
	virtual GVariantType getVariantType() const = 0;
	virtual GVariant getValue(void * instance) const = 0;

	virtual void * createInstance() const = 0;
	virtual void * createInplace(void * placement) const = 0;
	virtual void * cloneInstance(void * instance) const = 0;
	virtual void * cloneInplace(void * instance, void * placement) const = 0;

	virtual void destroyInstance(void * o) const = 0;

};


template <typename T>
class GMetaFundamentalDataImplement : public GMetaFundamentalData
{
public:
	virtual size_t getTypeSize() const {
		return sizeof(T);
	}

	virtual GVariantType getVariantType() const {
		return deduceVariantType<T>();
	}
	
	virtual GVariant getValue(void * instance) const {
		return GVariant(*static_cast<T *>(instance));
	}

	virtual void * createInstance() const {
		return new T(0);
	}

	virtual void * createInplace(void * placement) const {
		*static_cast<T *>(placement) = 0;

		return placement;
	}

	virtual void * cloneInstance(void * instance) const {
		return new T(*static_cast<T *>(instance));
	}

	virtual void * cloneInplace(void * instance, void * placement) const {
		*static_cast<T *>(placement) = *static_cast<T *>(instance);

		return placement;
	}

	virtual void destroyInstance(void * instance) const {
		delete static_cast<T *>(instance);
	}

};


} // namespace meta_internal


class GMetaFundamental : public GMetaTypedItem
{
private:
	typedef GMetaTypedItem super;

public:
	template <typename T>
	GMetaFundamental(T *, const char * name)
		: super(name, createMetaType<T>(), mcatFundamental) {
		new (this->dataBuffer) meta_internal::GMetaFundamentalDataImplement<T>;
	}

	GVariantType getVariantType() const {
		return this->getData()->getVariantType();
	}

	GVariant getValue(void * instance) const {
		return this->getData()->getValue(instance);
	}

	virtual size_t getTypeSize() const {
		return this->getData()->getTypeSize();
	}

	virtual void * createInstance() const {
		return this->getData()->createInstance();
	}

	virtual void * createInplace(void * placement) const {
		return this->getData()->createInplace(placement);
	}

	virtual void * cloneInstance(void * instance) const {
		return this->getData()->cloneInstance(instance);
	}

	virtual void * cloneInplace(void * instance, void * placement) const {
		return this->getData()->cloneInplace(instance, placement);
	}


	virtual void destroyInstance(void * instance) const {
		this->getData()->destroyInstance(instance);
	}


private:
	const meta_internal::GMetaFundamentalData * getData() const {
		return static_cast<const meta_internal::GMetaFundamentalData *>(static_cast<const void *>(this->dataBuffer));
	}

private:
	char dataBuffer[sizeof(meta_internal::GMetaFundamentalDataImplement<int>)];
};


} // namespace cpgf




#endif
