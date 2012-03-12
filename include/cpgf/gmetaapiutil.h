#ifndef __GMETAAPIUTIL_H
#define __GMETAAPIUTIL_H

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gexception.h"



namespace cpgf {


template <typename Meta>
void metaCheckError(Meta & meta)
{
	if(meta && meta->getErrorCode() != Error_None) {
		raiseException(meta->getErrorCode(), meta->getErrorMessage());
	}
}


template <typename Meta>
GMetaType metaGetItemType(const Meta & meta)
{
	GMetaTypeData typeData;

	const_cast<Meta &>(meta)->getItemType(&typeData);
	
	metaCheckError(meta);

	return GMetaType(typeData);
}

template <typename Meta>
GVariant metaGetValue(const Meta & meta, void * obj)
{
	GVariant v;

	const_cast<Meta &>(meta)->get(&v.data, obj);
	
	metaCheckError(meta);

	return v;
}

template <typename Meta>
void metaSetValue(const Meta & meta, void * obj, const GVariant & value)
{
	GVariantData data(value.getData());
	const_cast<Meta &>(meta)->set(obj, &data);

	metaCheckError(meta);
}

template <typename Meta>
bool metaCheckParam(const Meta & meta, const GVariant & param, size_t paramIndex)
{
	GVariantData data(param.getData());
	
	bool ok = !! meta->checkParam(&data, static_cast<uint32_t>(paramIndex));

	metaCheckError(meta);

	return ok;
}

template <typename Meta>
GMetaType metaGetParamType(const Meta & meta, size_t paramIndex)
{
	GMetaTypeData typeData;

	const_cast<Meta &>(meta)->getParamType(&typeData, static_cast<uint32_t>(paramIndex));
	
	metaCheckError(meta);

	return GMetaType(typeData);
}


#define DEF_LOAD_PARAM_HELPER(N, unused) paramData[N] = &GPP_CONCAT(p, N).data;

#define DEF_LOAD_PARAM(N) \
	const GVariantData * paramData[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER, GPP_EMPTY())

#define DEF_CALL_HELPER(N, unused) \
	template <typename Meta> \
	GVariant metaInvokeMethod(Meta & method, void * obj GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		GVariant v; \
		method->invokeIndirectly(&v.data, obj, paramData, N); \
		metaCheckError(method); \
		return v; \
	} \
	template <typename Meta> \
	void * metaInvokeConstructor(Meta & constructor GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		void * obj = constructor->invokeIndirectly(paramData, N); \
		metaCheckError(constructor); \
		return obj; \
	} \
	template <typename Meta> \
	GVariant metaInvokeOperatorFunctor(Meta & op, void * obj GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		GVariant v; \
		op->invokeFunctorIndirectly(&v.data, obj, paramData, N); \
		metaCheckError(op); \
		return v; \
	}

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())

#undef DEF_CALL_HELPER
#undef DEF_LOAD_PARAM
#undef DEF_LOAD_PARAM_HELPER

template <typename Meta>
GVariant metaInvokeOperatorUnary(Meta & op, const GVariant & p0)
{
	GVariant v;

	op->invokeUnary(&v.data, &p0.data);
	metaCheckError(op);

	return v;
}

template <typename Meta>
GVariant metaInvokeOperatorBinary(Meta & op, const GVariant & p0, const GVariant & p1)
{
	GVariant v;

	op->invokeBinary(&v.data, &p0.data, &p1.data);
	metaCheckError(op);

	return v;
}

template <typename Meta>
GVariant metaGetEnumValue(Meta & metaEnum, size_t index)
{
	GVariant v;

	metaEnum->getValue(&v.data, static_cast<unsigned int>(index));
	metaCheckError(metaEnum);

	return v;
}

template <typename Meta>
GVariant metaGetAnnotationVariant(Meta & annotationValue)
{
	GVariant v;

	annotationValue->getVariant(&v.data);
	metaCheckError(annotationValue);

	return v;
}

template <typename Meta>
IMetaClass * metaGetGlobalMetaClass(Meta & service, size_t index)
{
	GScopedInterface<IMetaModule> module(service->getModuleAt(static_cast<uint32_t>(index)));
	return module->getGlobalMetaClass();
}

IMetaItem * metaItemToInterface(const GMetaItem * item);

const GMetaClass * findAppropriateDerivedClass(void * instance, const GMetaClass * metaClass, void ** outCastedInstance);
IMetaClass * findAppropriateDerivedClass(void * instance, IMetaClass * metaClass, void ** outCastedInstance);


} // namespace cpgf



#endif
