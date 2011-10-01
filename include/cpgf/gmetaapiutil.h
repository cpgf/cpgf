#ifndef __GMETAAPIUTIL_H
#define __GMETAAPIUTIL_H

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gscopedptr.h"



namespace cpgf {


template <typename Meta>
void metaCheckError(Meta & meta)
{
	if(meta && meta->getErrorCode() != metaError_None) {
		throw GMetaException(static_cast<MetaErrorCode>(meta->getErrorCode()), meta->getErrorMessage());
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
	GVarData data;

	const_cast<Meta &>(meta)->get(obj, &data);
	
	metaCheckError(meta);

	return GVariant(data);
}

template <typename Meta>
void metaSetValue(const Meta & meta, void * obj, const GVariant & value)
{
	GVarData data(value.getData());
	const_cast<Meta &>(meta)->set(obj, &data);

	metaCheckError(meta);
}

template <typename Meta>
bool metaCheckParam(const Meta & meta, const GVariant & param, size_t paramIndex)
{
	GVarData data(param.getData());
	
	bool ok = !! meta->checkParam(&data, paramIndex);

	metaCheckError(meta);

	return ok;
}

template <typename Meta>
GMetaType metaGetParamType(const Meta & meta, size_t paramIndex)
{
	GMetaTypeData typeData;

	const_cast<Meta &>(meta)->getParamType(paramIndex, &typeData);
	
	metaCheckError(meta);

	return GMetaType(typeData);
}


#define DEF_LOAD_PARAM_HELPER(N, unused) paramData[N] = &GPP_CONCAT(p, N).data;

#define DEF_LOAD_PARAM(N) \
	const GVarData * paramData[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER, GPP_EMPTY())

#define DEF_CALL_HELPER(N, unused) \
	template <typename Meta> \
	GVariant metaCallMethod(Meta & method, void * obj GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		GVarData data; \
		method->callIndirectly(&data, obj, paramData, N); \
		metaCheckError(method); \
		return GVariant(data); \
	} \
	template <typename Meta> \
	void * metaCallConstructor(Meta & constructor GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		void * obj = constructor->callIndirectly(paramData, N); \
		metaCheckError(constructor); \
		return obj; \
	} \
	template <typename Meta> \
	GVariant metaCallOperatorFunctor(Meta & op, void * obj GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		GVarData data; \
		op->invokeFunctorIndirectly(&data, obj, paramData, N); \
		metaCheckError(op); \
		return GVariant(data); \
	}

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())

#undef DEF_CALL_HELPER
#undef DEF_LOAD_PARAM
#undef DEF_LOAD_PARAM_HELPER

template <typename Meta>
GVariant metaCallOperatorUnary(Meta & op, const GVariant & p0)
{
	GVarData data;

	op->invokeUnary(&data, &p0.data);
	metaCheckError(op);

	return GVariant(data);
}

template <typename Meta>
GVariant metaCallOperatorBinary(Meta & op, const GVariant & p0, const GVariant & p1)
{
	GVarData data;

	op->invokeBinary(&data, &p0.data, &p1.data);
	metaCheckError(op);

	return GVariant(data);
}

template <typename Meta>
GVariant metaGetEnumValue(Meta & metaEnum, size_t index)
{
	GVarData data;

	metaEnum->getValue(static_cast<unsigned int>(index), &data);
	metaCheckError(metaEnum);

	return GVariant(data);
}

template <typename Meta>
GVariant metaGetAnnotationVariant(Meta & annotationValue)
{
	GVarData data;

	annotationValue->getVariant(&data);
	metaCheckError(annotationValue);

	return GVariant(data);
}



} // namespace cpgf



#endif
