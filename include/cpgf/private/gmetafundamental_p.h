#ifndef __GMETAFUNDAMENTAL_P_H
#define __GMETAFUNDAMENTAL_P_H

#include "cpgf/gmetacommon.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"


namespace cpgf {


namespace meta_internal {

struct GMetaFundamentalDataVirtual
{
	size_t (*getTypeSize)(const void * self);
	GVariantType (*getVariantType)(const void * self);
	GVariant (*getValue)(const void * self, void * instance);
	
	void * (*createInstance)(const void * self);
	void * (*createInplace)(const void * self, void * placement);
	void * (*cloneInstance)(const void * self, void * instance);
	void * (*cloneInplace)(const void * self, void * instance, void * placement);
	void (*destroyInstance)(const void * self, void * o);
};

class GMetaFundamentalData
{
public:
	size_t getTypeSize() const {
		return this->virtualFunctions->getTypeSize(this);
	}

	GVariantType getVariantType() const {
		return this->virtualFunctions->getVariantType(this);
	}

	GVariant getValue(void * instance) const {
		return this->virtualFunctions->getValue(this, instance);
	}

	void * createInstance() const {
		return this->virtualFunctions->createInstance(this);
	}

	void * createInplace(void * placement) const {
		return this->virtualFunctions->createInplace(this, placement);
	}

	void * cloneInstance(void * instance) const {
		return this->virtualFunctions->cloneInstance(this, instance);
	}

	void * cloneInplace(void * instance, void * placement) const {
		return this->virtualFunctions->cloneInplace(this, instance, placement);
	}

	void destroyInstance(void * o) const {
		this->virtualFunctions->destroyInstance(this, o);
	}

protected:
	GMetaFundamentalDataVirtual * virtualFunctions;
};


template <typename T>
class GMetaFundamentalDataImplement : public GMetaFundamentalData
{
private:
	static size_t virtualGetTypeSize(const void * self) {
		(void)self;

		return sizeof(T);
	}

	static GVariantType virtualGetVariantType(const void * self) {
		(void)self;

		GVarTypeData data;
		deduceVariantType<T>(data);
		return vtGetType(data);
	}
	
	static GVariant virtualGetValue(const void * self, void * instance) {
		(void)self;

		return GVariant(*static_cast<T *>(instance));
	}

	static void * virtualCreateInstance(const void * self) {
		(void)self;

		return new T(0);
	}

	static void * virtualCreateInplace(const void * self, void * placement) {
		(void)self;

		*static_cast<T *>(placement) = 0;

		return placement;
	}

	static void * virtualCloneInstance(const void * self, void * instance) {
		(void)self;

		return new T(*static_cast<T *>(instance));
	}

	static void * virtualCloneInplace(const void * self, void * instance, void * placement) {
		(void)self;

		*static_cast<T *>(placement) = *static_cast<T *>(instance);

		return placement;
	}

	static void virtualDestroyInstance(const void * self, void * instance) {
		(void)self;

		delete static_cast<T *>(instance);
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
			&virtualDestroyInstance
		};

		this->virtualFunctions = &thisFunctions;

	}
};


} // namespace meta_internal


} // namespace cpgf




#endif
