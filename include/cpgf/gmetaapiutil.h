#ifndef __GMETAAPIUTIL_H
#define __GMETAAPIUTIL_H

#include "cpgf/gmetaapi.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gexception.h"



namespace cpgf {


template <typename Meta>
void metaCheckError(const Meta & meta)
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
GMetaExtendType metaGetItemExtendType(const Meta & meta, uint32_t flags)
{
	GMetaExtendType type;

	const_cast<Meta &>(meta)->getItemExtendType(&type.refData(), flags);
	metaCheckError(meta);

	return type;
}

template <typename Meta>
GMetaType metaGetResultType(const Meta & meta)
{
	GMetaTypeData typeData;

	const_cast<Meta &>(meta)->getResultType(&typeData);
	
	metaCheckError(meta);

	return GMetaType(typeData);
}

template <typename Meta>
GMetaExtendType metaGetResultExtendType(const Meta & meta, uint32_t flags)
{
	GMetaExtendType type;

	const_cast<Meta &>(meta)->getResultExtendType(&type.refData(), flags);
	metaCheckError(meta);

	return type;
}

template <typename Meta>
GMetaExtendType metaGetParamExtendType(const Meta & meta, uint32_t flags, uint32_t index)
{
	GMetaExtendType type;

	const_cast<Meta &>(meta)->getParamExtendType(&type.refData(), flags, index);
	metaCheckError(meta);

	return type;
}

template <typename Meta>
GVariant metaGetValue(const Meta & meta, const void * obj)
{
	GVariant v;

	const_cast<Meta &>(meta)->get(&v.refData(), obj);
	
	metaCheckError(meta);

	return v;
}

template <typename Meta>
void metaSetValue(const Meta & meta, void * obj, const GVariant & value)
{
	GVariantData data(value.refData());
	const_cast<Meta &>(meta)->set(obj, &data);

	metaCheckError(meta);
}

template <typename Meta>
bool metaCheckParam(const Meta & meta, const GVariant & param, size_t paramIndex)
{
	GVariantData data(param.refData());
	
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


#define DEF_LOAD_PARAM_HELPER(N, unused) paramData[N] = &GPP_CONCAT(p, N).refData();

#define DEF_LOAD_PARAM(N) \
	const GVariantData * paramData[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER, GPP_EMPTY())

#define DEF_CALL_HELPER(N, unused) \
	template <typename Meta> \
	GVariant metaInvokeMethod(const Meta & method, void * obj GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		GVariant v; \
		const_cast<Meta &>(method)->invokeIndirectly(&v.refData(), obj, paramData, N); \
		metaCheckError(method); \
		return v; \
	} \
	template <typename Meta> \
	void * metaInvokeConstructor(const Meta & constructor GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		void * obj = const_cast<Meta &>(constructor)->invokeIndirectly(paramData, N); \
		metaCheckError(constructor); \
		return obj; \
	} \
	template <typename Meta> \
	GVariant metaInvokeOperatorFunctor(const Meta & op, void * obj GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		GVariant v; \
		const_cast<Meta &>(op)->invokeFunctorIndirectly(&v.refData(), obj, paramData, N); \
		metaCheckError(op); \
		return v; \
	}

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())

#undef DEF_CALL_HELPER
#undef DEF_LOAD_PARAM
#undef DEF_LOAD_PARAM_HELPER

template <typename Meta>
GVariant metaInvokeOperatorUnary(const Meta & op, const GVariant & p0)
{
	GVariant v;

	const_cast<Meta &>(op)->invokeUnary(&v.refData(), &p0.refData());
	metaCheckError(op);

	return v;
}

template <typename Meta>
GVariant metaInvokeOperatorBinary(const Meta & op, const GVariant & p0, const GVariant & p1)
{
	GVariant v;

	const_cast<Meta &>(op)->invokeBinary(&v.refData(), &p0.refData(), &p1.refData());
	metaCheckError(op);

	return v;
}

template <typename Meta>
GVariant metaGetEnumValue(const Meta & metaEnum, size_t index)
{
	GVariant v;

	metaEnum->getValue(&v.refData(), static_cast<unsigned int>(index));
	metaCheckError(metaEnum);

	return v;
}

template <typename Meta>
GVariant metaGetAnnotationVariant(const Meta & annotationValue)
{
	GVariant v;

	annotationValue->getVariant(&v.refData());
	metaCheckError(annotationValue);

	return v;
}

template <typename Meta>
IMetaClass * metaGetGlobalMetaClass(const Meta & service, size_t index)
{
	GScopedInterface<IMetaModule> module(const_cast<Meta &>(service)->getModuleAt(static_cast<uint32_t>(index)));
	return module->getGlobalMetaClass();
}

IMetaItem * metaItemToInterface(const GMetaItem * item, bool freeItem);
IMetaItem * metaItemToInterface(const GMetaItem * item);

const GMetaClass * findAppropriateDerivedClass(const void * instance, const GMetaClass * metaClass, void ** outCastedInstance);
IMetaClass * findAppropriateDerivedClass(const void * instance, IMetaClass * metaClass, void ** outCastedInstance);


} // namespace cpgf



#endif
