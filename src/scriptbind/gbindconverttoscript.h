#ifndef GBINDCONVERTTOSCRIPT_H
#define GBINDCONVERTTOSCRIPT_H

#include "gbindmethods.h"
#include "gbindcontext.h"

#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gglobal.h"

namespace cpgf {

extern int Error_ScriptBinding_FailVariantToScript;

namespace bind_internal {

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived);

GScriptValue doCreateScriptValueFromVariant(
	const GContextPointer & context,
	const GVariant & value,
	const GMetaType & type,
	const bool transferOwnership
);
GScriptValue doCreateScriptValueFromVariant(
	const GContextPointer & context,
	const GVariant & value,
	const bool transferOwnership
);

struct ScriptValueToScriptData
{
	ScriptValueToScriptData()
		:
			outputGlueData(nullptr),
			objectData()
	{}

	explicit ScriptValueToScriptData(GGlueDataPointer * outputGlueData)
		:
			outputGlueData(outputGlueData),
			objectData()
	{}

	explicit ScriptValueToScriptData(const GObjectGlueDataPointer & objectData)
		:
			outputGlueData(nullptr),
			objectData(objectData)
	{}

	mutable GGlueDataPointer * outputGlueData;

	// used to bind method list
	GObjectGlueDataPointer objectData;
};

template <typename Methods>
typename Methods::ResultType converterToScript(
	const GContextPointer & context,
	const GVariant & value,
	IMetaConverter * converter
)
{
	if(isMetaConverterCanRead(converter->capabilityForCString())) {
		gapi_bool needFree;

		GScopedInterface<IMemoryAllocator> allocator(context->getService()->getAllocator());
		const char * s = converter->readCString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(s != nullptr) {
			typename Methods::ResultType result = Methods::doScriptValueToScript(context, GScriptValue::fromPrimary(s), ScriptValueToScriptData());

			if(needFree) {
				allocator->free((void *)s);
			}

			return result;
		}
	}

	if(isMetaConverterCanRead(converter->capabilityForCWideString())) {
		gapi_bool needFree;

		GScopedInterface<IMemoryAllocator> allocator(context->getService()->getAllocator());
		const wchar_t * ws = converter->readCWideString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(ws != nullptr) {
			typename Methods::ResultType result = Methods::doScriptValueToScript(context, GScriptValue::fromPrimary(ws), ScriptValueToScriptData());

			if(needFree) {
				allocator->free((void *)ws);
			}

			return result;
		}
	}

	return Methods::defaultValue();
}


template <typename Methods>
typename Methods::ResultType sharedPointerTraitsToScript(
	const GContextPointer & context,
	const GVariant & value,
	IMetaSharedPointerTraits * sharedPointerTraits
)
{
	GMetaTypeData typeData;
	sharedPointerTraits->getMetaType(&typeData);
	GMetaType realType(typeData);
	realType.addPointer();

	typename Methods::ResultType result;
	GGlueDataPointer glueData;

	result = Methods::doScriptValueToScript(
		context,
		doCreateScriptValueFromVariant(context, value, realType, false),
		ScriptValueToScriptData(&glueData)
	);

	if(! Methods::isSuccessResult(result)) {
		glueData.reset();
		result = Methods::doScriptValueToScript(context, GScriptValue::fromRaw(value), ScriptValueToScriptData(&glueData));
	}

	if(Methods::isSuccessResult(result) && glueData) {
		switch(glueData->getType()) {
			case gdtObject:
				std::static_pointer_cast<GObjectGlueData>(glueData)->setSharedPointerTraits(sharedPointerTraits);
				break;

			case gdtRaw:
				std::static_pointer_cast<GRawGlueData>(glueData)->setSharedPointerTraits(sharedPointerTraits);
				break;

			default:
				break;
		}
	}

	return result;
}


template <typename Methods>
typename Methods::ResultType extendVariantToScript(
	const GContextPointer & context,
	const GMetaExtendType & extendType,
	const GVariant & value
)
{
	typename Methods::ResultType result = Methods::defaultValue();

	GScopedInterface<IMetaConverter> converter(extendType.getConverter());
	if(converter) {
		result = converterToScript<Methods>(context, value, converter.get());
	}

	if(! Methods::isSuccessResult(result) && vtGetPointers(value.refData().typeData) == 0) {
		GScopedInterface<IMetaSharedPointerTraits> sharedPointerTraits(extendType.getSharedPointerTraits());
		if(sharedPointerTraits) {
			result = sharedPointerTraitsToScript<Methods>(context, value, sharedPointerTraits.get());
		}
	}

	if(! Methods::isSuccessResult(result)) {
		result = Methods::doScriptValueToScript(context, GScriptValue::fromRaw(value), ScriptValueToScriptData());
	}

	if(! Methods::isSuccessResult(result)) {
		raiseCoreException(Error_ScriptBinding_FailVariantToScript);
	}

	return result;
}

class GReturnedFromMethodObjectGuard
{
public:
	explicit GReturnedFromMethodObjectGuard(void * instance)
		: instance(instance)
	{
	}

	~GReturnedFromMethodObjectGuard() {
		if(this->objectLifeManager && this->instance != nullptr) {
			this->objectLifeManager->returnedFromMethod(this->instance);
		}
	}

	void reset(IMetaObjectLifeManager * objectLifeManager) {
		this->objectLifeManager.reset(objectLifeManager);
	}

	IMetaObjectLifeManager * getObjectLifeManager() const {
		return this->objectLifeManager.get();
	}

private:
	GScopedInterface<IMetaObjectLifeManager> objectLifeManager;
	void * instance;
};

template <typename Methods>
typename Methods::ResultType methodResultToScript(
	const GContextPointer & context,
	IMetaCallable * callable,
	InvokeCallableResult * resultValue
)
{
	if(resultValue->resultCount > 0) {
		typename Methods::ResultType result;

		GVariant value = resultValue->resultData;
		GMetaType type;

		if(vtIsTypedVar(resultValue->resultData.getType())) {
			value = getVariantRealValue(resultValue->resultData);
			type = getVariantRealMetaType(resultValue->resultData);
		}
		else {
			value = resultValue->resultData;
			callable->getResultType(&type.refData());
			metaCheckError(callable);
		}

		void * instance = nullptr;
		if(canFromVariant<void *>(value)) {
			instance = objectAddressFromVariant(value);
		}
		GReturnedFromMethodObjectGuard objectGuard(instance);
		objectGuard.reset(createObjectLifeManagerForInterface(value));
		if(objectGuard.getObjectLifeManager() == nullptr) {
			objectGuard.reset(metaGetResultExtendType(callable, GExtendTypeCreateFlag_ObjectLifeManager).getObjectLifeManager());
		}

		result = Methods::doScriptValueToScript(
			context,
			doCreateScriptValueFromVariant(context, value, type, !! callable->isResultTransferOwnership()),
			ScriptValueToScriptData()
		);

		if(! Methods::isSuccessResult(result)) {
			result = extendVariantToScript<Methods>(
				context,
				metaGetResultExtendType(callable, GExtendTypeCreateFlag_Converter | GExtendTypeCreateFlag_SharedPointerTraits),
				value
			);
		}

		return result;
	}

	return Methods::defaultValue();
}


template <typename Methods>
typename Methods::ResultType accessibleToScript(
	const GContextPointer & context,
	IMetaAccessible * accessible,
	void * instance,
	bool instanceIsConst
)
{
	GMetaType type;
	GVariant value = getAccessibleValueAndType(instance, accessible, &type, instanceIsConst);

	typename Methods::ResultType result = Methods::doScriptValueToScript(
		context,
		doCreateScriptValueFromVariant(context, value, type, false),
		ScriptValueToScriptData()
	);

	if(! Methods::isSuccessResult(result)) {
		result = extendVariantToScript<Methods>(
			context,
			metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Converter | GExtendTypeCreateFlag_SharedPointerTraits),
			value
		);
	}

	return result;
}


template <typename Methods>
typename Methods::ResultType namedMemberToScript(const GGlueDataPointer & glueData, const char * name)
{
	GClassGlueDataPointer classData;
	GObjectGlueDataPointer objectData;
	if(glueData->getType() == gdtObject) {
		objectData = std::static_pointer_cast<GObjectGlueData>(glueData);
		classData = objectData->getClassData();
	}
	else {
		GASSERT(glueData->getType() == gdtClass);
		classData = std::static_pointer_cast<GClassGlueData>(glueData);
	}
	
	GContextPointer context = classData->getBindingContext();

	if(! classData->getMetaClass()) {
		return Methods::defaultValue();
	}

	GMetaClassTraveller traveller(classData->getMetaClass(), getGlueDataInstanceAddress(glueData));

	void * instance = nullptr;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance, nullptr));

		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = context->getClassData(metaClass.get())->getClassMap();
		if(! mapClass) {
			continue;
		}

		GMetaMapItem * mapItem = mapClass->findItem(name);
		if(mapItem == nullptr) {
			continue;
		}

		GObjectGlueDataPointer castedObjectData;
		if(instance != nullptr && objectData) {
			castedObjectData = context->newObjectGlueData(context->getClassData(metaClass.get()), instance, false, objectData->getCV());
		}

		return Methods::doScriptValueToScript(
			context,
			mapItem->getScriptValue(),
			ScriptValueToScriptData(castedObjectData)
		);
	}

	GScriptDataHolder * dataHolder = nullptr;
	if(objectData) {
		dataHolder = objectData->getDataHolder().get();
	}
	if(dataHolder == nullptr && classData) {
		dataHolder = classData->getDataHolder().get();
	}
	if(dataHolder != nullptr) {
		const GScriptValue * scriptValue = dataHolder->findValue(name);
		if(scriptValue != nullptr) {
			return Methods::doScriptValueToScript(
				context,
				*scriptValue,
				ScriptValueToScriptData(objectData)
			);
		}
	}

	return Methods::defaultValue();
}


} //namespace bind_internal

} //namespace cpgf


#endif
