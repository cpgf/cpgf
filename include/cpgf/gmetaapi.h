#ifndef __GMETAAPI_H
#define __GMETAAPI_H

#include "cpgf/gapi.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gstdint.h"


namespace cpgf {


enum GMetaFilters {
	metaFilterIgnoreInstance = 1 << 0,
	metaFilterIgnoreStatic = 1 << 1,

	metaFilterConstMethod = 1 << 2,
	metaFilterVolatileMethod = 1 << 3,
	metaFilterConstVolatileMethod = 1 << 4,
};

enum GMetaOpType {
	mopBinaryFirst = 1,

	mopAdd = mopBinaryFirst + 0, // a + b
	mopSub = mopBinaryFirst + 1, // a - b
	mopMul = mopBinaryFirst + 2, // a * b
	mopDiv = mopBinaryFirst + 3, // a / b
	mopMod = mopBinaryFirst + 4, // a % b

	mopAddAssign = mopBinaryFirst + 5, // a += b
	mopSubAssign = mopBinaryFirst + 6, // a -= b
	mopMulAssign = mopBinaryFirst + 7, // a *= b
	mopDivAssign = mopBinaryFirst + 8, // a /= b
	mopModAssign = mopBinaryFirst + 9, // a %= b

	mopAssign = mopBinaryFirst + 10, // a = b

	mopEqual = mopBinaryFirst + 11, // a == b
	mopNotEqual = mopBinaryFirst + 12, // a != b
	mopGreater = mopBinaryFirst + 13, // a > b
	mopLessor = mopBinaryFirst + 14, // a < b
	mopGreaterEqual = mopBinaryFirst + 15, // a >= b
	mopLessorEqual = mopBinaryFirst + 16, // a <= b

	mopLogicAnd = mopBinaryFirst + 17, // a && b
	mopLogicOr = mopBinaryFirst + 18, // a || b

	mopBitAnd = mopBinaryFirst + 19, // a & b
	mopBitOr = mopBinaryFirst + 20, // a | b
	mopBitXor = mopBinaryFirst + 21, // a ^ b
	mopBitLeftShift = mopBinaryFirst + 22, // a << b
	mopBitRightShift = mopBinaryFirst + 23, // a >> b

	mopBitAndAssign = mopBinaryFirst + 24, // a &= b
	mopBitOrAssign = mopBinaryFirst + 25, // a |= b
	mopBitXorAssign = mopBinaryFirst + 26, // a ^= b
	mopBitLeftShiftAssign = mopBinaryFirst + 27, // a <<= b
	mopBitRightShiftAssign = mopBinaryFirst + 28, // a >>= b

	mopComma = mopBinaryFirst + 29, // a , b

	mopSubscript = mopBinaryFirst + 30, // a[b]

	mopPointerMember = mopBinaryFirst + 31, // a->*b

	mopBinaryLast = mopBinaryFirst + 31,


	mopUnaryFirst = mopBinaryLast + 1,

	mopPlus = mopUnaryFirst + 0, // +a
	mopNeg = mopUnaryFirst + 1, // -a

	mopPreInc = mopUnaryFirst + 2, // ++a
	mopPostInc = mopUnaryFirst + 3, // a++
	mopPreDec = mopUnaryFirst + 4, // --a
	mopPostDec = mopUnaryFirst + 5, // a--

	mopLogicNot = mopUnaryFirst + 6, // !a

	mopBitNot = mopUnaryFirst + 7, // ~a

	mopAddress = mopUnaryFirst + 8, // &a
	mopDerefer = mopUnaryFirst + 9, // *a

	mopCast = mopUnaryFirst + 10, // T(a)

	mopMember = mopUnaryFirst + 11, // a->b

	mopUnaryLast = mopUnaryFirst + 11,

	mopFunctor = mopUnaryLast + 1, // a(p0, p1...pn)
};

struct IMetaAnnotation;

struct IMetaItem : public IApiObject
{
	virtual const char * G_API_CC getName() = 0;
	virtual const char * G_API_CC getQualifiedName() = 0;
	virtual IMetaItem * G_API_CC getOwnerItem() = 0;
	virtual void G_API_CC getItemType(GMetaTypeData * outType) = 0;
	virtual uint32_t G_API_CC getCategory() = 0;
	virtual IMetaAnnotation * G_API_CC getAnnotation(const char * name) = 0;
	virtual uint32_t G_API_CC getAnnotationCount() = 0;
	virtual IMetaAnnotation * G_API_CC getAnnotationAt(uint32_t index) = 0;
	virtual gapi_bool G_API_CC equals(IMetaItem * other) = 0;
	
	virtual gapi_bool G_API_CC isStatic() = 0;
	virtual gapi_bool G_API_CC isField() = 0;
	virtual gapi_bool G_API_CC isProperty() = 0;
	virtual gapi_bool G_API_CC isMethod() = 0;
	virtual gapi_bool G_API_CC isEnum() = 0;
	virtual gapi_bool G_API_CC isOperator() = 0;
	virtual gapi_bool G_API_CC isConstructor() = 0;
	virtual gapi_bool G_API_CC isClass() = 0;
	virtual gapi_bool G_API_CC isAnnotation() = 0;
	virtual gapi_bool G_API_CC isFundamental() = 0;
};

struct IMetaTypedItem : public IMetaItem
{
	virtual void G_API_CC getMetaType(GMetaTypeData * outType) = 0;
	virtual uint32_t G_API_CC getTypeSize() = 0;

	virtual const char * G_API_CC getTypeName() = 0;

	virtual void * G_API_CC createInstance() = 0;
	virtual void * G_API_CC createInplace(void * placement) = 0;
	virtual void * G_API_CC cloneInstance(void * instance) = 0;
	virtual void * G_API_CC cloneInplace(void * instance, void * placement) = 0;

	virtual void G_API_CC destroyInstance(void * instance) = 0;
};

struct IMetaList : public IApiObject
{
	virtual void G_API_CC add(IMetaItem * item, void * instance) = 0;
	virtual uint32_t G_API_CC getCount() = 0;
	virtual IMetaItem * G_API_CC getAt(uint32_t index) = 0;
	virtual void * G_API_CC getInstanceAt(uint32_t index) = 0;
	virtual void G_API_CC clear() = 0;
};

struct IMetaConverter : public IApiObject
{
	virtual gapi_bool G_API_CC canToCString() = 0;
	virtual const char * G_API_CC toCString(const void * instance, gapi_bool * needFree) = 0;
};

struct IMetaAccessible : public IMetaItem
{
	virtual gapi_bool G_API_CC canGet() = 0;
	virtual gapi_bool G_API_CC canSet() = 0;
	virtual void G_API_CC get(void * instance, GVarData * outValue) = 0;
	virtual void G_API_CC set(void * instance, const GVarData * value) = 0;
	virtual uint32_t G_API_CC getSize() = 0;
	virtual IMetaConverter * G_API_CC createConverter() = 0;
};

struct IMetaField : public IMetaAccessible
{
	virtual void * G_API_CC getAddress(void * instance) = 0;
};

struct IMetaProperty : public IMetaAccessible
{
};

struct IMetaCallable : public IMetaItem
{
	virtual void G_API_CC getParamType(uint32_t index, GMetaTypeData * outType) = 0;
	virtual uint32_t G_API_CC getParamCount() = 0;
	virtual gapi_bool G_API_CC hasResult() = 0;
	virtual void G_API_CC getResultType(GMetaTypeData * outType) = 0;
	virtual gapi_bool G_API_CC isVariadic() = 0;
	virtual gapi_bool G_API_CC checkParam(const GVarData * param, uint32_t paramIndex) = 0;
	virtual gapi_bool G_API_CC isParamTransferOwnership(uint32_t paramIndex) = 0;
	virtual gapi_bool G_API_CC isResultTransferOwnership() = 0;
	virtual void G_API_CC execute(GVarData * outResult, void * instance, const GVarData * params, uint32_t paramCount) = 0;
	virtual IMetaConverter * G_API_CC createResultConverter() = 0;
};

struct IMetaMethod : public IMetaCallable
{
	virtual void G_API_CC invoke(GVarData * outResult, void * instance, const GVarData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeIndirectly(GVarData * outResult, void * instance, GVarData const * const * params, uint32_t paramCount) = 0;
};

struct IMetaConstructor : public IMetaCallable
{
	virtual void * G_API_CC invoke(const GVarData * params, uint32_t paramCount) = 0;
	virtual void * G_API_CC invokeIndirectly(GVarData const * const * params, uint32_t paramCount) = 0;
};

struct IMetaOperator : public IMetaCallable
{
	virtual int32_t G_API_CC getOperator() = 0;
	virtual void G_API_CC invokeUnary(GVarData * outResult, const GVarData * p0) = 0;
	virtual void G_API_CC invokeBinary(GVarData * outResult, const GVarData * p0, const GVarData * p1) = 0;
	virtual void G_API_CC invokeFunctor(GVarData * outResult, void * instance, const GVarData * params, uint32_t paramCount) = 0;
	virtual void G_API_CC invokeFunctorIndirectly(GVarData * outResult, void * instance, GVarData const * const * params, uint32_t paramCount) = 0;
};

struct IMetaFundamental : public IMetaTypedItem
{
	virtual void G_API_CC getValue(void * instance, GVarData * outValue) = 0;
};

struct IMetaEnum : public IMetaTypedItem
{
	virtual uint32_t G_API_CC getCount() = 0;
	virtual const char * G_API_CC getKey(uint32_t index) = 0;
	virtual void G_API_CC getValue(uint32_t index, GVarData * outValue) = 0;
	virtual int32_t G_API_CC findKey(const char * key) = 0;
};

struct IMetaAnnotationValue : public IApiObject
{
	virtual void G_API_CC getVariant(GVarData * outVariant) = 0;
	virtual gapi_bool G_API_CC canToString() = 0;
	virtual gapi_bool G_API_CC canToWideString() = 0;
	virtual gapi_bool G_API_CC canToInt() = 0;
	virtual const char * G_API_CC toString() = 0;
	virtual const wchar_t * G_API_CC toWideString() = 0;
	virtual int32_t G_API_CC toInt32() = 0;
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
	virtual void G_API_CC getMethodList(IMetaList * methodList, const char * name, uint32_t filters) = 0;
	virtual void G_API_CC getMethodListInHierarchy(IMetaList * methodList, const char * name, uint32_t filters, void * instance) = 0;

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
	virtual gapi_bool G_API_CC canCreateInstance() = 0;
	virtual gapi_bool G_API_CC canCopyInstance() = 0;
	
	virtual IMetaClass * G_API_CC getBaseClass(uint32_t baseIndex) = 0;
	virtual uint32_t G_API_CC getBaseCount() = 0;

	virtual gapi_bool G_API_CC isInheritedFrom(IMetaClass * ancient) = 0;

	virtual void * G_API_CC castFromBase(void * base, uint32_t baseIndex) = 0;
	virtual void * G_API_CC castToBase(void * self, uint32_t baseIndex) = 0;
};


struct IMetaService : public IApiObject
{
	virtual IMetaClass * G_API_CC getGlobalMetaClass() = 0;

	virtual IMetaList * G_API_CC createMetaList() = 0;

	virtual void * G_API_CC allocateMemory(uint32_t size) = 0;
	virtual void G_API_CC freeMemory(const void * p) = 0;

	virtual IMetaTypedItem * G_API_CC findTypedItemByName(const char * name) = 0;
	virtual IMetaFundamental * G_API_CC findFundamental(GVariantType vt) = 0;
	virtual IMetaClass * G_API_CC findClassByName(const char * name) = 0;
	virtual IMetaClass * G_API_CC findClassByType(const GMetaTypeData * type) = 0;
};


IMetaService * createMetaService();


} // namespace cpgf



#endif
