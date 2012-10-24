#include "cpgf/gmetaapiutil.h"


namespace cpgf {


void metaCheckError(IExtendObject * object)
{
	if(object != NULL && object->getErrorCode() != Error_None) {
		raiseException(object->getErrorCode(), object->getErrorMessage());
	}
}

GMetaType metaGetItemType(IMetaItem * item)
{
	GMetaType type;

	item->getItemType(&type.refData());
	metaCheckError(item);

	return type;
}

GMetaExtendType metaGetItemExtendType(IMetaItem * item, uint32_t flags)
{
	GMetaExtendType type;

	item->getItemExtendType(&type.refData(), flags);
	metaCheckError(item);

	return type;
}

GMetaType metaGetResultType(IMetaCallable * callable)
{
	GMetaType type;

	callable->getResultType(&type.refData());
	metaCheckError(callable);

	return type;
}

GMetaExtendType metaGetResultExtendType(IMetaCallable * callable, uint32_t flags)
{
	GMetaExtendType type;

	callable->getResultExtendType(&type.refData(), flags);
	metaCheckError(callable);

	return type;
}

GMetaExtendType metaGetParamExtendType(IMetaCallable * callable, uint32_t flags, uint32_t index)
{
	GMetaExtendType type;

	callable->getParamExtendType(&type.refData(), flags, index);
	metaCheckError(callable);

	return type;
}

GVariant metaGetValue(IMetaAccessible * accessible, const void * obj)
{
	GVariant v;

	accessible->get(&v.refData(), obj);
	metaCheckError(accessible);

	return v;
}

void metaSetValue(IMetaAccessible * accessible, void * obj, const GVariant & value)
{
	GVariantData data(value.refData());

	accessible->set(obj, &data);
	metaCheckError(accessible);
}

bool metaCheckParam(IMetaCallable * callable, const GVariant & param, size_t paramIndex)
{
	GVariantData data(param.refData());
	
	bool ok = !! callable->checkParam(&data, static_cast<uint32_t>(paramIndex));
	metaCheckError(callable);

	return ok;
}

GMetaType metaGetParamType(IMetaCallable * callable, size_t paramIndex)
{
	GMetaType type;

	callable->getParamType(&type.refData(), static_cast<uint32_t>(paramIndex));
	metaCheckError(callable);

	return type;
}

GVariant metaInvokeOperatorUnary(IMetaOperator * op, const GVariant & p0)
{
	GVariant v;

	op->invokeUnary(&v.refData(), &p0.refData());
	metaCheckError(op);

	return v;
}

GVariant metaInvokeOperatorBinary(IMetaOperator * op, const GVariant & p0, const GVariant & p1)
{
	GVariant v;

	op->invokeBinary(&v.refData(), &p0.refData(), &p1.refData());
	metaCheckError(op);

	return v;
}

GVariant metaGetEnumValue(IMetaEnum * metaEnum, size_t index)
{
	GVariant v;

	metaEnum->getValue(&v.refData(), static_cast<unsigned int>(index));
	metaCheckError(metaEnum);

	return v;
}

GVariant metaGetAnnotationVariant(IMetaAnnotationValue * annotationValue)
{
	GVariant v;

	annotationValue->getVariant(&v.refData());
	metaCheckError(annotationValue);

	return v;
}

IMetaClass * metaGetGlobalMetaClass(IMetaService * service, size_t index)
{
	GScopedInterface<IMetaModule> module(service->getModuleAt(static_cast<uint32_t>(index)));
	return module->getGlobalMetaClass();
}


#define DEF_LOAD_PARAM_HELPER(N, unused) paramData[N] = &GPP_CONCAT(p, N).refData();

#define DEF_LOAD_PARAM(N) \
	const GVariantData * paramData[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER, GPP_EMPTY())

#define DEF_CALL_HELPER(N, unused) \
	GVariant metaInvokeMethod(IMetaMethod * method, void * obj GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		GVariant v; \
		method->invokeIndirectly(&v.refData(), obj, paramData, N); \
		metaCheckError(method); \
		return v; \
	} \
	void * metaInvokeConstructor(IMetaConstructor * constructor GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		void * obj = constructor->invokeIndirectly(paramData, N); \
		metaCheckError(constructor); \
		return obj; \
	} \
	GVariant metaInvokeOperatorFunctor(IMetaOperator * op, void * obj GPP_REPEAT_TAIL_PARAMS(N, const GVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		GVariant v; \
		op->invokeFunctorIndirectly(&v.refData(), obj, paramData, N); \
		metaCheckError(op); \
		return v; \
	}

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())

#undef DEF_CALL_HELPER
#undef DEF_LOAD_PARAM
#undef DEF_LOAD_PARAM_HELPER


} // namespace cpgf
