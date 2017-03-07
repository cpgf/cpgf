#ifndef CPGF_GMETAAPI_H
#define CPGF_GMETAAPI_H

#include "cpgf/gapi.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gmetaextendtype.h"

#include <cstdint>

namespace cpgf {


struct IMetaAnnotation;

struct IMetaItem : public IExtendObject
{
	virtual IMetaItem * G_API_CC clone() = 0;
	virtual const char * G_API_CC getName() = 0;
	virtual const char * G_API_CC getQualifiedName() = 0;
	virtual IMetaItem * G_API_CC getOwnerItem() = 0;
	virtual void G_API_CC getItemType(GMetaTypeData * outType) = 0;
	virtual void G_API_CC getItemExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags) = 0;
	virtual uint32_t G_API_CC getCategory() = 0;
	virtual IMetaAnnotation * G_API_CC getAnnotation(const char * name) = 0;
	virtual uint32_t G_API_CC getAnnotationCount() = 0;
	virtual IMetaAnnotation * G_API_CC getAnnotationAt(uint32_t index) = 0;
	virtual gapi_bool G_API_CC equals(IMetaItem * other) = 0;
	
	virtual gapi_bool G_API_CC isStatic() = 0;
};

struct IMetaTypedItem : public IMetaItem
{
	virtual void G_API_CC getMetaType(GMetaTypeData * outType) = 0;
	virtual uint32_t G_API_CC getTypeSize() = 0;

	virtual const char * G_API_CC getQualifiedName() = 0;

	virtual void * G_API_CC createInstance() = 0;
	virtual void * G_API_CC createInplace(void * placement) = 0;
	virtual void * G_API_CC cloneInstance(const void * instance) = 0;
	virtual void * G_API_CC cloneInplace(const void * instance, void * placement) = 0;

	virtual void G_API_CC destroyInstance(void * instance) = 0;
	virtual void G_API_CC destroyInplace(void * instance) = 0;
};

struct IMetaList : public IExtendObject
{
	virtual void G_API_CC add(IMetaItem * item, void * instance) = 0;
	virtual uint32_t G_API_CC getCount() = 0;
	virtual IMetaItem * G_API_CC getAt(uint32_t index) = 0;
	virtual void * G_API_CC getInstanceAt(uint32_t index) = 0;
	virtual void G_API_CC clear() = 0;
};

struct IMetaAccessible : public IMetaItem
{
	virtual gapi_bool G_API_CC canGet() = 0;
	virtual gapi_bool G_API_CC canSet() = 0;
	virtual void G_API_CC get(GVariantData * outResult, const void * instance) = 0;
	virtual void G_API_CC set(void * instance, const GVariantData * value) = 0;
	virtual void * G_API_CC getAddress(const void * instance) = 0;
	virtual uint32_t G_API_CC getSize() = 0;
};

struct IMetaField : public IMetaAccessible
{
};

struct IMetaProperty : public IMetaAccessible
{
};

struct IMetaCallable : public IMetaItem
{
	virtual void G_API_CC getParamType(GMetaTypeData * outType, uint32_t index) = 0;
	virtual uint32_t G_API_CC getParamCount() = 0;
	virtual uint32_t G_API_CC getDefaultParamCount() = 0;
	virtual gapi_bool G_API_CC hasResult() = 0;
	virtual void G_API_CC getResultType(GMetaTypeData * outType) = 0;
	virtual void G_API_CC getResultExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags) = 0;
	virtual void G_API_CC getParamExtendType(GMetaExtendTypeData * outExtendType, uint32_t flags, uint32_t index) = 0;
	virtual gapi_bool G_API_CC isVariadic() = 0;
	virtual gapi_bool G_API_CC isExplicitThis() = 0;
	virtual gapi_bool G_API_CC checkParam(const GVariantData * param, uint32_t paramIndex) = 0;
	virtual gapi_bool G_API_CC isParamTransferOwnership(uint32_t paramIndex) = 0;
	virtual gapi_bool G_API_CC isResultTransferOwnership() = 0;
	virtual void G_API_CC execute(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC executeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount) = 0;
};

struct IMetaMethod : public IMetaCallable
{
	virtual void G_API_CC invoke(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount) = 0;
};

struct IMetaConstructor : public IMetaCallable
{
	virtual void * G_API_CC invoke(const GVariantData * params, uint32_t paramCount) = 0;
	virtual void * G_API_CC invokeIndirectly(GVariantData const * const * params, uint32_t paramCount) = 0;
};

struct IMetaOperator : public IMetaCallable
{
	virtual int32_t G_API_CC getOperator() = 0;
	virtual void G_API_CC invokeUnary(GVariantData * outResult, const GVariantData * p0) = 0;
	virtual void G_API_CC invokeBinary(GVariantData * outResult, const GVariantData * p0, const GVariantData * p1) = 0;
	virtual void G_API_CC invokeFunctor(GVariantData * outResult, void * instance, const GVariantData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeFunctorIndirectly(GVariantData * outResult, void * instance, GVariantData const * const * params, uint32_t paramCount) = 0;
};

struct IMetaFundamental : public IMetaTypedItem
{
	virtual void G_API_CC getValue(GVariantData * outResult, const void * instance) = 0;
};

struct IMetaEnum : public IMetaTypedItem
{
	virtual uint32_t G_API_CC getCount() = 0;
	virtual const char * G_API_CC getKey(uint32_t index) = 0;
	virtual void G_API_CC getValue(GVariantData * outResult, uint32_t index) = 0;
	virtual int32_t G_API_CC findKey(const char * key) = 0;
};

struct IMetaAnnotationValue : public IExtendObject
{
	virtual void G_API_CC getVariant(GVariantData * outVariant) = 0;
	virtual gapi_bool G_API_CC canToString() = 0;
	virtual gapi_bool G_API_CC canToWideString() = 0;
	virtual gapi_bool G_API_CC canToInt() = 0;
	virtual const char * G_API_CC toString() = 0;
	virtual const wchar_t * G_API_CC toWideString() = 0;
	virtual int32_t G_API_CC toInt32() = 0;
	virtual gapi_bool G_API_CC toBoolean() = 0;
};

struct IMetaAnnotation : public IMetaItem
{
	virtual IMetaItem * G_API_CC getMetaItem() = 0;
	virtual IMetaAnnotationValue * G_API_CC getValue(const char * name) = 0;
	virtual uint32_t G_API_CC getCount() = 0;
	virtual const char * G_API_CC getNameAt(uint32_t index) = 0;
	virtual IMetaAnnotationValue * G_API_CC getValueAt(uint32_t index) = 0;
};

struct IMetaClass : public IMetaTypedItem
{
	virtual IMetaConstructor * G_API_CC getConstructorByParamCount(uint32_t paramCount) = 0;
	virtual uint32_t G_API_CC getConstructorCount() = 0;
	virtual IMetaConstructor * G_API_CC getConstructorAt(uint32_t index) = 0;

	virtual IMetaField * G_API_CC getFieldInHierarchy(const char * name, void ** instance) = 0;
	virtual IMetaField * G_API_CC getField(const char * name) = 0;
	virtual uint32_t G_API_CC getFieldCount() = 0;
	virtual IMetaField * G_API_CC getFieldAt(uint32_t index) = 0;

	virtual IMetaProperty * G_API_CC getPropertyInHierarchy(const char * name, void ** instance) = 0;
	virtual IMetaProperty * G_API_CC getProperty(const char * name) = 0;
	virtual uint32_t G_API_CC getPropertyCount() = 0;
	virtual IMetaProperty * G_API_CC getPropertyAt(uint32_t index) = 0;

	virtual IMetaMethod * G_API_CC getMethodInHierarchy(const char * name, void ** instance) = 0;
	virtual IMetaMethod * G_API_CC getMethod(const char * name) = 0;
	virtual uint32_t G_API_CC getMethodCount() = 0;
	virtual IMetaMethod * G_API_CC getMethodAt(uint32_t index) = 0;

	virtual IMetaOperator * G_API_CC getOperatorInHierarchy(uint32_t op, void ** instance) = 0;
	virtual IMetaOperator * G_API_CC getOperator(uint32_t op) = 0;
	virtual uint32_t G_API_CC getOperatorCount() = 0;
	virtual IMetaOperator * G_API_CC getOperatorAt(uint32_t index) = 0;

	virtual IMetaEnum * G_API_CC getEnumInHierarchy(const char * name, void ** instance) = 0;
	virtual IMetaEnum * G_API_CC getEnum(const char * name) = 0;
	virtual uint32_t G_API_CC getEnumCount() = 0;
	virtual IMetaEnum * G_API_CC getEnumAt(uint32_t index) = 0;

	virtual IMetaClass * G_API_CC getClassInHierarchy(const char * name, void ** instance) = 0;
	virtual IMetaClass * G_API_CC getClass(const char * name) = 0;
	virtual uint32_t G_API_CC getClassCount() = 0;
	virtual IMetaClass * G_API_CC getClassAt(uint32_t index) = 0;

	virtual uint32_t G_API_CC getMetaCount() = 0;
	virtual IMetaItem * G_API_CC getMetaAt(uint32_t index) = 0;

	virtual gapi_bool G_API_CC isGlobal() = 0;
	virtual gapi_bool G_API_CC isAbstract() = 0;
	virtual gapi_bool G_API_CC isPolymorphic() = 0;
	virtual gapi_bool G_API_CC canCreateInstance() = 0;
	virtual gapi_bool G_API_CC canCopyInstance() = 0;
	
	virtual IMetaClass * G_API_CC getBaseClass(uint32_t baseIndex) = 0;
	virtual uint32_t G_API_CC getBaseCount() = 0;

	virtual IMetaClass * G_API_CC getDerivedClass(uint32_t derivedIndex) = 0;
	virtual uint32_t G_API_CC getDerivedCount() = 0;

	virtual gapi_bool G_API_CC isInheritedFrom(IMetaClass * ancient) = 0;

	virtual void * G_API_CC castFromBase(const void * base, uint32_t baseIndex) = 0;
	virtual void * G_API_CC castToBase(const void * self, uint32_t baseIndex) = 0;
	
	virtual void * G_API_CC castFromDerived(const void * derived, uint32_t derivedIndex) = 0;
	virtual void * G_API_CC castToDerived(const void * self, uint32_t derivedIndex) = 0;
};


struct IMetaModule : public IExtendObject
{
	virtual IMetaClass * G_API_CC getGlobalMetaClass() = 0;

	virtual IMetaTypedItem * G_API_CC findTypedItemByName(const char * name) = 0;
	virtual IMetaClass * G_API_CC findClassByName(const char * name) = 0;
};


struct IMetaService : public IExtendObject
{
	virtual void G_API_CC addModule(IMetaModule * module) = 0;
	virtual uint32_t G_API_CC getModuleCount() = 0;
	virtual IMetaModule * G_API_CC getModuleAt(uint32_t index) = 0;

	virtual IMemoryAllocator * G_API_CC getAllocator() = 0;

	virtual IMetaTypedItem * G_API_CC findTypedItemByName(const char * name) = 0;
	virtual IMetaClass * G_API_CC findClassByName(const char * name) = 0;
};


class GMetaClass;
class GMetaModule;
IMetaModule * createMetaModule(GMetaModule * module, GMetaClass * metaClass);

IMetaService * createMetaService(IMetaModule * primaryModule);
IMetaService * createDefaultMetaService();

IMetaList * createMetaList();

} // namespace cpgf



#endif
