#ifndef CPGF_GMETAAPIUTIL_H
#define CPGF_GMETAAPIUTIL_H

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gexception.h"



namespace cpgf {


#define DEF_CALL_HELPER(N, unused) \
	GVariant metaInvokeMethod(IMetaMethod * method, void * obj GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GVariant & p)); \
	void * metaInvokeConstructor(IMetaConstructor * constructor GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)); \
	GVariant metaInvokeOperatorFunctor(IMetaOperator * op, void * obj GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p));

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())

#undef DEF_CALL_HELPER

void metaCheckError(IExtendObject * object);

GMetaType metaGetItemType(IMetaItem * item);
GMetaType metaGetTypedItemMetaType(IMetaTypedItem * item);
GMetaExtendType metaGetItemExtendType(IMetaItem * item, uint32_t flags);

GMetaType metaGetResultType(IMetaCallable * callable);
GMetaExtendType metaGetResultExtendType(IMetaCallable * callable, uint32_t flags);
GMetaExtendType metaGetParamExtendType(IMetaCallable * callable, uint32_t flags, uint32_t index);

GVariant metaGetValue(IMetaAccessible * accessible, const void * obj);
void metaSetValue(IMetaAccessible * accessible, void * obj, const GVariant & value);

bool metaCheckParam(IMetaCallable * callable, const GVariant & param, size_t paramIndex);
GMetaType metaGetParamType(IMetaCallable * callable, size_t paramIndex);

GVariant metaInvokeOperatorUnary(IMetaOperator * op, const GVariant & p0);
GVariant metaInvokeOperatorBinary(IMetaOperator * op, const GVariant & p0, const GVariant & p1);

GVariant metaGetEnumValue(IMetaEnum * metaEnum, size_t index);

GVariant metaGetAnnotationVariant(IMetaAnnotationValue * annotationValue);

IMetaClass * metaGetGlobalMetaClass(IMetaService * service, size_t index);

IMetaItem * metaItemToInterface(const GMetaItem * item, bool freeItem);
IMetaItem * metaItemToInterface(const GMetaItem * item);

const GMetaClass * findAppropriateDerivedClass(const void * instance, const GMetaClass * metaClass, void ** outCastedInstance);
IMetaClass * findAppropriateDerivedClass(const void * instance, IMetaClass * metaClass, void ** outCastedInstance);

void * metaCastToBase(void * instance, IMetaClass * currentClass, IMetaClass * targetBaseClass);
void * metaCastToDerived(void * instance, IMetaClass * currentClass, IMetaClass * targetDerivedClass);
void * metaCastAny(void * instance, IMetaClass * fromClass, IMetaClass * toClass);


} // namespace cpgf



#endif
