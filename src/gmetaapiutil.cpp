#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gerrorcode.h"


namespace cpgf {


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


void metaCheckError(IExtendObject * object)
{
	if(object != nullptr && object->getErrorCode() != Error_None) {
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

GMetaType metaGetTypedItemMetaType(IMetaTypedItem * item)
{
	GMetaType type;

	item->getMetaType(&type.refData());
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


const GMetaClass * findAppropriateDerivedClass(const void * instance, const GMetaClass * metaClass, void ** outCastedInstance)
{
	if(outCastedInstance != nullptr) {
		*outCastedInstance = const_cast<void *>(instance);
	}

	if(! metaClass->isPolymorphic()) {
		return metaClass;
	}

	const GMetaClass * currentClass = metaClass;

	for(;;) {
		void * derivedInstance = nullptr;
		size_t derivedCount = currentClass->getDerivedCount();

		for(size_t i = 0; i < derivedCount; ++i) {
			derivedInstance = currentClass->castToDerived(instance, i);
			if(derivedInstance != nullptr) {
				currentClass = currentClass->getDerivedClass(i);
				instance = derivedInstance;
				break;
			}
		}

		if(derivedInstance == nullptr) {
			break;
		}
	}

	if(outCastedInstance != nullptr) {
		*outCastedInstance = const_cast<void *>(instance);
	}
	return currentClass;
}

IMetaClass * findAppropriateDerivedClass(const void * instance, IMetaClass * metaClass, void ** outCastedInstance)
{
	if(outCastedInstance != nullptr) {
		*outCastedInstance = const_cast<void *>(instance);
	}

	if(! metaClass->isPolymorphic()) {
		metaClass->addReference();
		return metaClass;
	}

	metaClass->addReference();
	GScopedInterface<IMetaClass> currentClass(metaClass);

	for(;;) {
		void * derivedInstance = nullptr;
		uint32_t derivedCount = currentClass->getDerivedCount();

		for(uint32_t i = 0; i < derivedCount; ++i) {
			derivedInstance = currentClass->castToDerived(instance, i);
			if(derivedInstance != nullptr) {
				currentClass.reset(currentClass->getDerivedClass(i));
				instance = derivedInstance;
				break;
			}
		}

		if(derivedInstance == nullptr) {
			break;
		}
	}

	if(outCastedInstance != nullptr) {
		*outCastedInstance = const_cast<void *>(instance);
	}
	return currentClass.take();
}

void * doMetaCastToBase(void * instance, IMetaClass * currentClass, IMetaClass * targetBaseClass)
{
	if(currentClass->equals(targetBaseClass)) {
		return instance;
	}
	
	for(uint32_t i = 0; i < currentClass->getBaseCount(); ++i) {
		GScopedInterface<IMetaClass> base(currentClass->getBaseClass(i));
		if(! base) {
			continue;
		}
		
		void * castedInstance = doMetaCastToBase(currentClass->castToBase(instance, i), base.get(), targetBaseClass);
		if(castedInstance != nullptr) {
			return castedInstance;
		}
	}
	
	return nullptr;
}

void * metaCastToBase(void * instance, IMetaClass * currentClass, IMetaClass * targetBaseClass)
{
	if(instance == nullptr) {
		return instance;
	}
	
	return doMetaCastToBase(instance, currentClass, targetBaseClass);
}

void * doMetaCastToDerived(void * instance, IMetaClass * currentClass, IMetaClass * targetDerivedClass)
{
	if(currentClass->equals(targetDerivedClass)) {
		return instance;
	}
	
	for(uint32_t i = 0; i < currentClass->getDerivedCount(); ++i) {
		GScopedInterface<IMetaClass> derived(currentClass->getDerivedClass(i));
		if(! derived) {
			continue;
		}
		
		void * castedInstance = doMetaCastToDerived(currentClass->castToDerived(instance, i), derived.get(), targetDerivedClass);
		if(castedInstance != nullptr) {
			return castedInstance;
		}
	}
	
	return nullptr;
}

void * metaCastToDerived(void * instance, IMetaClass * currentClass, IMetaClass * targetDerivedClass)
{
	if(instance == nullptr) {
		return instance;
	}
	
	return doMetaCastToDerived(instance, currentClass, targetDerivedClass);
}

void * doMetaCastAny(void * instance, IMetaClass * fromClass, IMetaClass * toClass)
{
	if(fromClass->equals(toClass)) {
		return instance;
	}

	void * casted;

	casted = metaCastToBase(instance, fromClass, toClass);
	if(casted == nullptr) {
		casted = metaCastToDerived(instance, fromClass, toClass);
	}

	// TODO: support multi-inheritance that fromClass and toClass are not on the same inheritance line.

	return casted;
}

void * metaCastAny(void * instance, IMetaClass * fromClass, IMetaClass * toClass)
{
	if(instance == nullptr) {
		return instance;
	}

	return doMetaCastAny(instance, fromClass, toClass);
}


} // namespace cpgf
