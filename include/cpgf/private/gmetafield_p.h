#ifndef CPGF_GMETAFIELD_P_H
#define CPGF_GMETAFIELD_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gexception.h"

namespace cpgf {

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

template <typename Accessor>
class GMetaFieldDataAccessor : public GMetaFieldDataBase
{
private:
	static constexpr bool Readable = Accessor::Readable;
	static constexpr bool Writable = Accessor::Writable;
	
	typedef typename Accessor::ValueType ValueType;

private:
	static bool virtualCanGet() {
		return Readable;
	}

	static bool virtualCanSet() {
		return Writable;
	}

	static GVariant virtualGet(const void * self, const void * instance) {
		return createVariant<ValueType>(static_cast<const GMetaFieldDataAccessor *>(self)->accessor.get(instance), true);
	}

	static void virtualSet(const void * self, void * instance, const GVariant & value) {
		static_cast<const GMetaFieldDataAccessor *>(self)->accessor.set(
			instance,
			(const typename Accessor::SetterType::PassType &)fromVariant<ValueType>(value)
		);
	}

	static size_t virtualGetFieldSize() {
		return sizeof(ValueType);
	}

	static void * virtualGetFieldAddress(const void * self, const void * instance) {
		return static_cast<const GMetaFieldDataAccessor *>(self)->accessor.getAddress(instance);
	}
	
	static GMetaExtendType virtualGetItemExtendType(uint32_t flags, const GMetaItem * metaItem)
	{
		return createMetaExtendType<ValueType>(flags, metaItem);
	}

public:
	GMetaFieldDataAccessor(const Accessor & accessor) : accessor(accessor) {
		static GMetaFieldDataVirtual thisFunctions = {
			&virtualBaseMetaDeleter<GMetaFieldDataAccessor>,
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
	mutable Accessor accessor;
};


} // namespace meta_internal



} // namespace cpgf



#endif
