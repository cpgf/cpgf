#ifndef CPGF_GMETAFUNDAMENTAL_P_H
#define CPGF_GMETAFUNDAMENTAL_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"


namespace cpgf {


namespace meta_internal {

struct GMetaFundamentalDataVirtual
{
	size_t (*getTypeSize)();
	GVariantType (*getVariantType)();
	GVariant (*getValue)(const void * instance);
	
	void * (*createInstance)();
	void * (*createInplace)(void * placement);
	void * (*cloneInstance)(const void * instance);
	void * (*cloneInplace)(const void * instance, void * placement);
	void (*destroyInstance)(void * o);
	
	GMetaExtendType (*getItemExtendType)(uint32_t flags);
};

class GMetaFundamentalData
{
public:
	size_t getTypeSize() const;

	GVariantType getVariantType() const;

	GVariant getValue(const void * instance) const;

	void * createInstance() const;
	void * createInplace(void * placement) const;
	void * cloneInstance(const void * instance) const;
	void * cloneInplace(const void * instance, void * placement) const;

	void destroyInstance(void * o) const;

	// must be defined in header to make template function overloading happy.
	GMetaExtendType getItemExtendType(uint32_t flags) const {
		return this->virtualFunctions->getItemExtendType(flags);
	}

protected:
	GMetaFundamentalDataVirtual * virtualFunctions;
};


template <typename T>
class GMetaFundamentalDataImplement : public GMetaFundamentalData
{
private:
	static size_t virtualGetTypeSize() {
		return sizeof(T);
	}

	static GVariantType virtualGetVariantType() {
		GVarTypeData data;
		deduceVariantType<T>(data);
		return vtGetType(data);
	}
	
	static GVariant virtualGetValue(const void * instance) {
		return GVariant(*static_cast<const T *>(instance));
	}

	static void * virtualCreateInstance() {
		return new T(0);
	}

	static void * virtualCreateInplace(void * placement) {
		*static_cast<T *>(placement) = 0;

		return placement;
	}

	static void * virtualCloneInstance(const void * instance) {
		return new T(*static_cast<const T *>(instance));
	}

	static void * virtualCloneInplace(const void * instance, void * placement) {
		*static_cast<T *>(placement) = *static_cast<const T *>(instance);

		return placement;
	}

	static void virtualDestroyInstance(void * instance) {
		delete static_cast<T *>(instance);
	}

	static GMetaExtendType virtualGetItemExtendType(uint32_t flags) {
		return createMetaExtendType<T>(flags);
	}

public:
	GMetaFundamentalDataImplement() {
		static GMetaFundamentalDataVirtual thisFunctions = {
			&virtualGetTypeSize,
			&virtualGetVariantType,
			&virtualGetValue,
			&virtualCreateInstance,
			&virtualCreateInplace,
			&virtualCloneInstance,
			&virtualCloneInplace,
			&virtualDestroyInstance,
			&virtualGetItemExtendType
		};

		this->virtualFunctions = &thisFunctions;

	}
};


} // namespace meta_internal


} // namespace cpgf




#endif
