#ifndef GBINDCONVERTTOSCRIPT_H
#define GBINDCONVERTTOSCRIPT_H

#include "gbindmethods.h"

namespace cpgf {

extern int Error_ScriptBinding_FailVariantToScript;

namespace bind_internal {

struct ScriptValueToScriptData
{
	ScriptValueToScriptData()
		: outputGlueData(nullptr)
	{}

	explicit ScriptValueToScriptData(GGlueDataPointer * outputGlueData)
		: outputGlueData(outputGlueData)
	{}

	mutable GGlueDataPointer * outputGlueData;
};

template <typename Methods>
typename Methods::ResultType complexVariantToScript(
	const GContextPointer & context,
	const GVariant & value,
	const GMetaType & type,
	GGlueDataPointer * outputGlueData
)
{
	GVariantType vt = static_cast<GVariantType>((GVtType)value.getType() & ~(GVtType)GVariantType::maskByReference);

	if(! type.isEmpty() && type.getPointerDimension() <= 1) {
		GScopedInterface<IMetaTypedItem> typedItem(context->getService()->findTypedItemByName(type.getBaseName()));
		if(typedItem) {
			GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

			return Methods::doScriptValueToScript(
				context,
				GScriptValue::fromObject(value, gdynamic_cast<IMetaClass *>(typedItem.get()), false, metaTypeToCV(type)),
				ScriptValueToScriptData(outputGlueData)
			);
		}
		else {
			if(vtIsInterface(vt)) {
				IObject * obj = fromVariant<IObject *>(value);
				if(dynamic_cast<IMetaClass *>(obj)) { // !!! GUID
					IMetaClass * metaClass = dynamic_cast<IMetaClass *>(obj);
					return Methods::doScriptValueToScript(context, GScriptValue::fromClass(metaClass), ScriptValueToScriptData(outputGlueData));
				}
			}
		}

		// TODO: is this block really reached?
		if(shouldRemoveReference(type)) {
			GMetaType newType(type);
			newType.removeReference();

			return Methods::doScriptValueToScript(context, GScriptValue::fromPrimary(createTypedVariant(value, newType)), ScriptValueToScriptData(outputGlueData));
		}
	}

	return Methods::defaultValue();
}


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
	IMetaSharedPointerTraits * sharedPointerTraits,
	const GBindValueFlags & flags
)
{
	GMetaTypeData typeData;
	sharedPointerTraits->getMetaType(&typeData);
	GMetaType realType(typeData);
	realType.addPointer();

	typename Methods::ResultType result;
	GGlueDataPointer glueData;

	result = Methods::doScriptValueToScript(context,  GScriptValue::fromPrimary(createTypedVariant(value, realType)), ScriptValueToScriptData(&glueData));
	if(! Methods::isSuccessResult(result)) {
		glueData.reset();
		result = Methods::doScriptValueToScript(context, GScriptValue::fromRaw(value), ScriptValueToScriptData(&glueData));
	}
	if(Methods::isSuccessResult(result) && glueData) {
		switch(glueData->getType()) {
			case gdtObject:
				sharedStaticCast<GObjectGlueData>(glueData)->setSharedPointerTraits(sharedPointerTraits);
				break;

			case gdtRaw:
				sharedStaticCast<GRawGlueData>(glueData)->setSharedPointerTraits(sharedPointerTraits);
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
	const GVariant & value,
	const GBindValueFlags & flags
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
			result = sharedPointerTraitsToScript<Methods>(context, value, sharedPointerTraits.get(), flags);
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

		GBindValueFlags flags;
		flags.setByBool(bvfAllowGC, !! callable->isResultTransferOwnership());
		result = Methods::doVariantToScript(context, createTypedVariant(value, type), flags, nullptr);

		if(! Methods::isSuccessResult(result)) {
			result = extendVariantToScript<Methods>(context,
				metaGetResultExtendType(callable, GExtendTypeCreateFlag_Converter | GExtendTypeCreateFlag_SharedPointerTraits),
				value, flags);
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

	typename Methods::ResultType result = Methods::doScriptValueToScript(context, GScriptValue::fromPrimary(createTypedVariant(value, type)), ScriptValueToScriptData());

	if(! Methods::isSuccessResult(result)) {
		result = extendVariantToScript<Methods>(context,
			metaGetItemExtendType(accessible, GExtendTypeCreateFlag_Converter | GExtendTypeCreateFlag_SharedPointerTraits),
			value, GBindValueFlags());
	}

	return result;
}


template <typename Methods>
typename Methods::ResultType namedMemberToScript(const GGlueDataPointer & glueData, const char * name)
{
	bool isInstance = (glueData->getType() == gdtObject);
	GClassGlueDataPointer classData;
	GObjectGlueDataPointer objectData;
	if(glueData->getType() == gdtObject) {
		objectData = sharedStaticCast<GObjectGlueData>(glueData);
		classData = objectData->getClassData();
	}
	else {
		GASSERT(glueData->getType() == gdtClass);
		classData = sharedStaticCast<GClassGlueData>(glueData);
	}

	if(! classData->getMetaClass()) {
		return Methods::defaultValue();
	}

	const GScriptConfig & config = classData->getBindingContext()->getConfig();
	GContextPointer context = classData->getBindingContext();

	GMetaClassTraveller traveller(classData->getMetaClass(), getGlueDataInstanceAddress(glueData));

	void * instance = nullptr;
	IMetaClass * outDerived;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance, &outDerived));
		GScopedInterface<IMetaClass> derived(outDerived);

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

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {
				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(config, isInstance, data.get())) {
					return accessibleToScript<Methods>(context, data.get(), instance, getGlueDataCV(glueData) == GScriptInstanceCv::sicvConst);
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList: {
				GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getUserData());
				GContextPointer context = classData->getBindingContext();
				if(data == nullptr) {
					GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass.get(), derived.get()));

					GScopedInterface<IMetaList> metaList(getMethodListFromMapItem(mapItem, getGlueDataInstanceAddress(objectData)));
					GMethodGlueDataPointer glueData = context->newMethodGlueData(context->getClassData(boundClass.get()), metaList.get());
					data = new GMapItemMethodData(glueData);
					mapItem->setUserData(data);
				}
				GScopedInterface<IMetaList> metaList(getMethodListFromMapItem(mapItem, getGlueDataInstanceAddress(objectData)));
				GGlueDataPointer objectGlueData = sharedStaticCast<GGlueData>(objectData);
				//return Methods::doScriptValueToScript(context, GScriptValue::fromOverloadedMethods(metaList.get()), &objectGlueData);
				
				return Methods::doMethodsToScript(classData, mapItem, metaClass.get(), derived.get(), objectData);
			}

			case mmitEnum:
				if(! isInstance || config.allowAccessEnumTypeViaInstance()) {
					GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
					return Methods::doScriptValueToScript(context, GScriptValue::fromEnum(metaEnum.get()), ScriptValueToScriptData());
				}
				break;

			case mmitEnumValue:
				if(! isInstance || config.allowAccessEnumValueViaInstance()) {
					GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
					return Methods::doScriptValueToScript(
						context,
						GScriptValue::fromPrimary(metaGetEnumValue(metaEnum.get(), static_cast<uint32_t>(mapItem->getEnumIndex()))),
						ScriptValueToScriptData()
					);
				}
				break;

			case mmitClass:
				if(! isInstance || config.allowAccessClassViaInstance()) {
					GScopedInterface<IMetaClass> innerMetaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
					return Methods::doScriptValueToScript(context, GScriptValue::fromClass(innerMetaClass.get()), ScriptValueToScriptData());
				}
				break;

			default:
				break;
		}
	}

	return Methods::defaultValue();
}


} //namespace bind_internal

} //namespace cpgf


#endif
