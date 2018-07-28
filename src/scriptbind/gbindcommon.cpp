#include "gbindcommon.h"

#include "cpgf/gstringmap.h"
#include "cpgf/gcallback.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/scriptbind/gscriptservice.h"
#include "cpgf/glifecycle.h"
#include "cpgf/gerrorcode.h"

#include "cpgf/metatraits/gmetaobjectlifemanager_iobject.h"
#include "cpgf/metatraits/gmetatraitsparam.h"

#include <vector>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996) // warning C4996: 'mbstowcs': This function or variable may be unsafe. Consider using mbstowcs_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#endif

using namespace std;


namespace cpgf {

G_GUARD_LIBRARY_LIFE

GScriptCoreService * doBindScriptCoreService(GScriptObject * scriptObject, const char * bindName, IScriptLibraryLoader * libraryLoader);

namespace bind_internal {


//*********************************************
// Declarations
//*********************************************

const int ValueMatchRank_Unknown = 0; // such as 2 or more dimensions pointer
const int ValueMatchRank_Convert = 50000;
const int ValueMatchRank_ConvertBwtweenFamily = 51000;
const int ValueMatchRank_ConvertWithinFamily = 52000;
const int ValueMatchRank_ConvertWithinFamilySameSigned = 53000;
const int ValueMatchRank_Implicit_Begin = 70000;
const int ValueMatchRank_Implicit_WideStringToString = ValueMatchRank_Implicit_Begin + 0;
const int ValueMatchRank_Implicit_StringToWideString = ValueMatchRank_Implicit_Begin + 1;
const int ValueMatchRank_Implicit_SharedPointerToRaw = ValueMatchRank_Implicit_Begin + 2;
const int ValueMatchRank_Implicit_CastToBase = ValueMatchRank_Implicit_Begin + 3;
const int ValueMatchRank_Implicit_CastToDerived = ValueMatchRank_Implicit_Begin + 4;
const int ValueMatchRank_Implicit_UserConvert = ValueMatchRank_Implicit_Begin + 5;
const int ValueMatchRank_Implicit_End = 80000;
const int ValueMatchRank_Equal = 100000;


//*********************************************
// Classes implementations
//*********************************************


InvokeCallableParam::InvokeCallableParam(size_t paramCount, IScriptContext * scriptContext)
	:
		params((CallableParamData *)paramsBuffer),
		paramCount(paramCount),
		paramRanks((ConvertRank *)paramRanksBuffer),
		backParamRanks((ConvertRank *)paramRanksBackBuffer),
		scriptContext(scriptContext)
{
	// Use "raw" buffer to hold the object array CallableParamData and ConvertRank.
	// If we use CallableParamData[REF_MAX_ARITY] and ConvertRank[REF_MAX_ARITY], the performance is bad due to the constructors.
	memset(this->paramsBuffer, 0, sizeof(CallableParamData) * paramCount);
	memset(this->paramRanksBuffer, 0, sizeof(ConvertRank) * paramCount);
	memset(this->paramRanksBackBuffer, 0, sizeof(ConvertRank) * paramCount);

	if(this->paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}
}

InvokeCallableParam::~InvokeCallableParam()
{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->params[i].~CallableParamData();
		this->paramRanks[i].~ConvertRank();
		this->backParamRanks[i].~ConvertRank();
	}
}



//*********************************************
// Global function implementations
//*********************************************

GScriptInstanceCv getCallableConstness(IMetaCallable * callable)
{
	if(! callable->isExplicitThis()) {
		// normal function
		GMetaType methodType = metaGetItemType(callable);
		if(methodType.isConstFunction()) {
			return GScriptInstanceCv::sicvConst;
		}

		if(methodType.isVolatileFunction()) {
			return GScriptInstanceCv::sicvVolatile;
		}

		if(methodType.isConstVolatileFunction()) {
			return GScriptInstanceCv::sicvConstVolatile;
		}
	}
	else {
		// "explicit this" function
		GMetaType selfType = metaGetParamType(callable, abstractParameterIndexBase);
		if(selfType.isPointerToConst() || selfType.isReferenceToConst()) {
			return GScriptInstanceCv::sicvConst;
		}

		if(selfType.isPointerToVolatile() || selfType.isReferenceToVolatile()) {
			return GScriptInstanceCv::sicvVolatile;
		}

		if(selfType.isPointerToConstVolatile() || selfType.isReferenceToConstVolatile()) {
			return GScriptInstanceCv::sicvConstVolatile;
		}
	}

	return GScriptInstanceCv::sicvNone;
}


bool isParamImplicitConvert(const ConvertRank & rank)
{
	return rank.weight >= ValueMatchRank_Implicit_Begin && rank.weight < ValueMatchRank_Implicit_End;
}

void rankImplicitConvertForString(ConvertRank * outputRank, const GVariant & sourceData, const GMetaType & targetType)
{
	if(variantIsString(sourceData) && targetType.isWideString()) {
		outputRank->weight = ValueMatchRank_Implicit_StringToWideString;
	}
	else if(variantIsWideString(sourceData) && targetType.isString()) {
		outputRank->weight = ValueMatchRank_Implicit_WideStringToString;
	}
}

void rankImplicitConvertForSharedPointer(ConvertRank * outputRank, const GGlueDataPointer & valueGlueData, const GMetaExtendType & targetExtendType)
{
	IMetaSharedPointerTraits * paramSharedPointerTraits = getGlueDataSharedPointerTraits(valueGlueData);
	if(paramSharedPointerTraits != nullptr) {
		GScopedInterface<IMetaSharedPointerTraits> sharedPointerTraits(targetExtendType.getSharedPointerTraits());
		if(! sharedPointerTraits) {
			outputRank->weight = ValueMatchRank_Implicit_SharedPointerToRaw;
		}
	}
}

void rankImplicitConvertForMetaClass(ConvertRank * outputRank, IMetaItem * sourceType, IMetaItem * targetType)
{

	if(sourceType == nullptr || targetType == nullptr) {
		return;
	}

	if(! metaIsClass(sourceType->getCategory()) || ! metaIsClass(targetType->getCategory())) {
		return;
	}

	IMetaClass * sourceClass = static_cast<IMetaClass *>(sourceType);
	IMetaClass * targetClass = static_cast<IMetaClass *>(targetType);

	if(sourceClass->equals(targetClass)) {
		outputRank->weight = ValueMatchRank_Equal;
	}
	else {
		if(sourceClass->isInheritedFrom(targetClass)) {
			outputRank->weight = ValueMatchRank_Implicit_CastToBase;
		}
		else if(targetClass->isInheritedFrom(sourceClass)) {
			outputRank->weight = ValueMatchRank_Implicit_CastToDerived;
		}

		if(outputRank->weight != ValueMatchRank_Unknown) {
			outputRank->sourceClass = sourceClass;
			outputRank->targetClass.reset(targetClass);
		}
	}
}

void rankImplicitConvertForUserConvert(ConvertRank * outputRank, IMetaCallable * callable,
	const InvokeCallableParam * callableParam, size_t paramIndex)
{
	uint32_t converterCount = callableParam->scriptContext->getScriptUserConverterCount();
	if(converterCount == 0) {
		return;
	}

	GScriptUserConverterParamData converterData;
	converterData.callable = callable;
	converterData.paramIndex = (uint32_t)paramIndex;
	GScriptValueData scriptValueData = callableParam->params[paramIndex].value.getData();
	GScriptValueDataScopedGuard scriptValueDataGuard(scriptValueData);
	converterData.sourceValue = &scriptValueData;

	for(uint32_t i = 0; i < converterCount; ++i) {
		GSharedInterface<IScriptUserConverter> converter(callableParam->scriptContext->getScriptUserConverterAt(i));
		uint32_t tag = converter->canConvert(&converterData);
		if(tag != 0) {
			outputRank->weight = ValueMatchRank_Implicit_UserConvert;
			outputRank->userConverter = converter.get();
			outputRank->userConverterTag = tag;

			break;
		}
	}
}

void rankCallableImplicitConvert(ConvertRank * outputRank, IMetaService * service,
	IMetaCallable * callable, const InvokeCallableParam * callableParam, size_t paramIndex, const GMetaType & targetType)
{
	rankImplicitConvertForString(outputRank, getVariantRealValue(callableParam->params[paramIndex].value.getValue()), targetType);

	if(outputRank->weight == ValueMatchRank_Unknown) {
		rankImplicitConvertForSharedPointer(outputRank, callableParam->params[paramIndex].paramGlueData,
			metaGetParamExtendType(callable, GExtendTypeCreateFlag_SharedPointerTraits, static_cast<uint32_t>(paramIndex)));
	}

	if(outputRank->weight == ValueMatchRank_Unknown) {
		GScopedInterface<IMetaTypedItem> typedItem(getTypedItemFromScriptValue(callableParam->params[paramIndex].value));
		if(typedItem) {
			GScopedInterface<IMetaTypedItem> protoType(service->findTypedItemByName(targetType.getBaseName()));
			rankImplicitConvertForMetaClass(outputRank, typedItem.get(), protoType.get());
		}
	}

	if(outputRank->weight == ValueMatchRank_Unknown) {
		rankImplicitConvertForUserConvert(outputRank, callable, callableParam, paramIndex);
	}
}

int rankFundamental(GVariantType protoType, GVariantType paramType)
{
	if(protoType == paramType) {
		return ValueMatchRank_Equal;
	}

	if(vtIsBoolean(protoType) || vtIsBoolean(paramType)) {
		return ValueMatchRank_Convert;
	}

	if(vtIsInteger(protoType) && vtIsInteger(paramType)) {
		if(vtIsSignedInteger(protoType) && vtIsSignedInteger(paramType)) {
			return ValueMatchRank_ConvertWithinFamilySameSigned;
		}
		if(vtIsUnsignedInteger(protoType) && vtIsUnsignedInteger(paramType)) {
			return ValueMatchRank_ConvertWithinFamilySameSigned;
		}
		return ValueMatchRank_ConvertWithinFamily;
	}

	if(vtIsReal(protoType) && vtIsReal(paramType)) {
		return ValueMatchRank_ConvertWithinFamilySameSigned;
	}

	return ValueMatchRank_ConvertBwtweenFamily;
}

void rankCallableParam(
	ConvertRank * outputRank,
	IMetaService * service,
	IMetaCallable * callable,
	const InvokeCallableParam * callableParam,
	size_t paramIndex
)
{
	GMetaType proto = metaGetParamType(callable, paramIndex);
	GScriptValue::Type type = callableParam->params[paramIndex].value.getType();

	if(type == GScriptValue::typeNull) {
		outputRank->weight = ValueMatchRank_Equal;
		return;
	}

	if(proto.isFundamental() && type == GScriptValue::typePrimary) {
		outputRank->weight = rankFundamental(proto.getVariantType(),
			callableParam->params[paramIndex].value.toPrimary().getType());
		return;
	}

	if(type == GScriptValue::typeScriptFunction && vtIsInterface(proto.getVariantType())) {
		outputRank->weight = ValueMatchRank_Convert;
		return;
	}

	if(proto.getPointerDimension() > 1) {
		outputRank->weight = ValueMatchRank_Unknown;
		return;
	}

	rankCallableImplicitConvert(outputRank, service, callable, callableParam, paramIndex, proto);
}

int rankCallable(
	IMetaService * service,
	const GObjectGlueDataPointer & objectData,
	IMetaCallable * callable,
	const InvokeCallableParam * callableParam,
	ConvertRank * paramRanks
)
{
	if(!! callable->isVariadic()) {
		return 0;
	}

	auto callableParamCount = callable->getParamCount();
	if(callableParamCount < callableParam->paramCount) {
		return -1;
	}

	if(callableParamCount > callableParam->paramCount + callable->getDefaultParamCount()) {
		return -1;
	}

	int rank = 1;

	const GScriptInstanceCv cv = getGlueDataCV(objectData);
	const GScriptInstanceCv methodCV = getCallableConstness(callable);
	if(cv == methodCV) {
		rank += ValueMatchRank_Equal;
	}
	else {
		if(cv != GScriptInstanceCv::sicvNone) {
			return -1;
		}
		rank += ValueMatchRank_Convert;
	}

	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		rankCallableParam(&paramRanks[i], service, callable, callableParam, i);
		rank += paramRanks[i].weight;

		if(! isParamImplicitConvert(paramRanks[i])) {
			bool ok = !! callable->checkParam(&callableParam->params[i].value.getValue().refData(), static_cast<uint32_t>(i));
			metaCheckError(callable);
			if(! ok) {
				return -1;
			}
		}
	}

	return rank;
}

bool implicitConvertForMetaClassCast(const ConvertRank & rank, GVariant * v);

bool doConvertForMetaClassCast(
		const GContextPointer & context,
		GVariant * v,
		const GMetaType & targetType,
		const GGlueDataPointer & valueGlueData
	)
{
	IMetaClass * sourceClass = getGlueDataMetaClass(valueGlueData);
	if(sourceClass != nullptr) {
		GScopedInterface<IMetaClass> targetClass(context->getService()->findClassByName(targetType.getBaseName()));
		if(targetClass) {
			ConvertRank rank = ConvertRank();

			rankImplicitConvertForMetaClass(&rank, sourceClass, targetClass.get());
			return implicitConvertForMetaClassCast(rank, v);
		}
	}

	return false;
}

bool implicitConvertForString(const ConvertRank & rank, GVariant * v, GVariant * holder)
{
	switch(rank.weight) {
		case ValueMatchRank_Implicit_StringToWideString: {
			*holder = *v;
			*v = GVariant();
			const char * s = fromVariant<char *>(*holder);
			std::wstring ws(stringToWideString(s));
			*v = createWideStringVariant(ws.c_str());

			return true;
		}

		case ValueMatchRank_Implicit_WideStringToString: {
			*holder = *v;
			*v = GVariant();
			const wchar_t * ws = fromVariant<wchar_t *>(*holder);
			std::string s(wideStringToString(ws));
			*v = createStringVariant(s.c_str());

			return true;
		}
	}

	return false;
}

bool implicitConvertForSharedPointer(
		const GContextPointer & context,
		const ConvertRank & rank,
		GVariant * v,
		const GMetaType & targetType,
		const GGlueDataPointer & valueGlueData
	)
{
	switch(rank.weight) {
		case ValueMatchRank_Implicit_SharedPointerToRaw: {
			IMetaSharedPointerTraits * paramSharedPointerTraits = getGlueDataSharedPointerTraits(valueGlueData);
			*v = paramSharedPointerTraits->getPointer(objectAddressFromVariant(*v));

			doConvertForMetaClassCast(context, v, targetType, valueGlueData);

			return true;
		}
	}

	return false;
}

bool implicitConvertForMetaClassCast(const ConvertRank & rank, GVariant * v)
{
	switch(rank.weight) {
		case ValueMatchRank_Implicit_CastToBase: {
			*v = metaCastToBase(objectAddressFromVariant(*v), rank.sourceClass, rank.targetClass.get());

			return true;
		}

		case ValueMatchRank_Implicit_CastToDerived: {
			*v = metaCastToDerived(objectAddressFromVariant(*v), rank.sourceClass, rank.targetClass.get());

			return true;
		}
	}

	return false;
}

bool implicitConvertForUserConvert(
		const ConvertRank & rank,
		GVariant * v,
		IMetaCallable * callable,
		InvokeCallableParam * callableParam,
		size_t paramIndex
	)
{
	if(rank.weight != ValueMatchRank_Implicit_UserConvert) {
		return false;
	}

	// below block is copied from function rankImplicitConvertForUserConvert
	// we can't extract it to a functio because we need GScriptValueDataScopedGuard
	GScriptUserConverterParamData converterData;
	converterData.callable = callable;
	converterData.paramIndex = (uint32_t)paramIndex;
	GScriptValueData scriptValueData = callableParam->params[paramIndex].value.getData();
	GScriptValueDataScopedGuard scriptValueDataGuard(scriptValueData);
	converterData.sourceValue = &scriptValueData;

	rank.userConverter->convert(&v->refData(), &converterData, rank.userConverterTag);

	return true;
}

void implicitConvertCallableParam(
		const GContextPointer & context,
		GVariant * holder,
		IMetaCallable * callable,
		InvokeCallableParam * callableParam,
		size_t paramIndex
	)
{
	const ConvertRank & rank = callableParam->paramRanks[paramIndex];
	GMetaType targetType(metaGetParamType(callable, paramIndex));
	const GGlueDataPointer & valueGlueData = callableParam->params[paramIndex].paramGlueData;
	GVariant * v = &callableParam->params[paramIndex].value.getValue();

	if(! implicitConvertForString(rank, v, holder)) {
		if(! implicitConvertForSharedPointer(context, rank, v, targetType, valueGlueData)) {
			if(! implicitConvertForMetaClassCast(rank, v)) {
				implicitConvertForUserConvert(rank, v, callable, callableParam, paramIndex);
			}
		}
	}
}

void doInvokeCallable(
		const GContextPointer & context,
		void * instance,
		IMetaCallable * callable,
		InvokeCallableParam * callableParam,
		InvokeCallableResult * result
	)
{
	result->resultCount = callable->hasResult() ? 1 : 0;

	GVariant holder;

	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		if(isParamImplicitConvert(callableParam->paramRanks[i])) {
			implicitConvertCallableParam(
				context,
				&holder,
				callable,
				callableParam,
				i
			);
		}
	}

	const GVariantData * data[REF_MAX_ARITY];
	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		data[i] = &callableParam->params[i].value.getValue().refData();
	}
	callable->executeIndirectly(&result->resultData.refData(), instance, data, static_cast<uint32_t>(callableParam->paramCount));
	metaCheckError(callable);

	for(uint32_t i = 0; i < callableParam->paramCount; ++i) {
		if(callable->isParamTransferOwnership(i)
			&& callableParam->params[i].paramGlueData
			&& callableParam->params[i].paramGlueData->getType() == gdtObject) {
			static_cast<GObjectGlueData *>(callableParam->params[i].paramGlueData.get())->setAllowGC(false);
		}
	}
}


void * doInvokeConstructor(
		const GContextPointer & context,
		IMetaService * service,
		IMetaClass * metaClass,
		InvokeCallableParam * callableParam
	)
{
	void * instance = nullptr;

	if(callableParam->paramCount == 0 && metaClass->canCreateInstance()) {
		instance = metaClass->createInstance();
	}
	else {
		const int maxRankIndex = findAppropriateCallable(
			service,
			GObjectGlueDataPointer(),
			[=](const uint32_t index) { return metaClass->getConstructorAt(index); },
			metaClass->getConstructorCount(),
			callableParam,
			[](IMetaCallable *) { return true; }
		);

		if(maxRankIndex >= 0) {
			InvokeCallableResult result;

			GScopedInterface<IMetaConstructor> constructor(metaClass->getConstructorAt(static_cast<uint32_t>(maxRankIndex)));
			doInvokeCallable(context, nullptr, constructor.get(), callableParam, &result);
			instance = objectAddressFromVariant(GVariant(result.resultData));
		}
	}

	return instance;
}

IMetaMethod * doGetCallableFromScriptValue(
		void ** outInstance,
		const GScriptValue & scriptValue,
		IMetaService * service,
		const GObjectGlueDataPointer & objectData,
		InvokeCallableParam * callableParam
	)
{
	switch(scriptValue.getType()) {
		case GScriptValue::typeMethod: {
			GScopedInterface<IMetaMethod> method(scriptValue.toMethod(outInstance));
				const int weight = rankCallable(service, objectData, method.get(), callableParam, callableParam->backParamRanks);
				if(weight >= 0) {
					std::swap(callableParam->paramRanks, callableParam->backParamRanks);
					return method.release();
				}
		}
			break;

		case GScriptValue::typeOverloadedMethods: {
			GScopedInterface<IMetaList> methodList(scriptValue.toOverloadedMethods());
			const int maxRankIndex = findAppropriateCallable(
				service,
				objectData,
				[&](const uint32_t index) { return methodList->getAt(index); },
				methodList->getCount(),
				callableParam,
				[](IMetaCallable *) { return true; }
			);
			if(maxRankIndex >= 0) {
				*outInstance = methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex));
				return static_cast<IMetaMethod *>(methodList->getAt(maxRankIndex));
			}
		}
			break;
			
		default:
			break;
	}
	
	return nullptr;
}

InvokeCallableResult doInvokeMethodList(
		const GContextPointer & context,
		const GObjectGlueDataPointer & objectData,
		const GMethodGlueDataPointer & methodData,
		InvokeCallableParam * callableParam
	)
{
	void * methodInstance = nullptr;
	GScopedInterface<IMetaMethod> methodToInvoke(doGetCallableFromScriptValue(
		&methodInstance,
		methodData->getScriptValue(),
		context->getService(),
		objectData,
		callableParam
	));
	
	if(methodToInvoke) {
		InvokeCallableResult result;
		void * instance = nullptr;
		if(objectData) {
			instance = objectData->getInstanceAddress();
			if(instance != nullptr) {
				auto classData = objectData->getClassData();
				if(classData) {
					GScopedInterface<IMetaClass> callableClass(gdynamic_cast<IMetaClass *>(methodToInvoke->getOwnerItem()));
					if(classData->getMetaClass() != callableClass.get()) {
						instance = metaCastAny(instance, classData->getMetaClass(), callableClass.get());
					}
				}
			}
		}
		else {
			// This happens if an object method is bound to script as a global function.
			instance = methodInstance;
		}
		doInvokeCallable(context, instance, methodToInvoke.get(), callableParam, &result);
		result.callable.reset(methodToInvoke.get());
		return result;
	}

	raiseCoreException(Error_ScriptBinding_CantFindMatchedMethod);

	return InvokeCallableResult();
}

GScriptValue glueDataToScriptValue(const GGlueDataPointer & glueData)
{
	if(glueData) {
		switch(glueData->getType()) {
			case gdtClass: {
				GClassGlueDataPointer classData = std::static_pointer_cast<GClassGlueData>(glueData);;
				return GScriptValue::fromClass(classData->getMetaClass());
			}

			case gdtObject: {
				GObjectGlueDataPointer objectData = std::static_pointer_cast<GObjectGlueData>(glueData);
				return GScriptValue::fromObject(objectData->getInstance(), objectData->getClassData()->getMetaClass(), objectData->isAllowGC(), objectData->getCV());
			}

			case gdtRaw: {
				GRawGlueDataPointer rawData = std::static_pointer_cast<GRawGlueData>(glueData);
				return GScriptValue::fromRaw(rawData->getData());
			}

			case gdtMethod:
			case gdtObjectAndMethod: {
				GMethodGlueDataPointer methodGlueData;
				if(glueData->getType() == gdtMethod) {
					methodGlueData = std::static_pointer_cast<GMethodGlueData>(glueData);
				}
				else {
					methodGlueData = std::static_pointer_cast<GObjectAndMethodGlueData>(glueData)->getMethodData();
				}
				
				return methodGlueData->getScriptValue();
			}

			case gdtEnum:
				return GScriptValue::fromEnum(std::static_pointer_cast<GEnumGlueData>(glueData)->getMetaEnum());

			default:
				break;
		}
	}

	return GScriptValue();
}

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived)
{
	if(derived == nullptr) {
		currentClass->addReference();
		return currentClass;
	}
	else {
		if(derived->getBaseCount() > 0 && derived->getBaseClass(0)) {
			// always choose first base because we only support single inheritance in script binding
			return derived->getBaseClass(0);
		}
		else {
			derived->addReference();
			return derived;
		}
	}
}

void setValueToScriptDataHolder(const GGlueDataPointer & glueData, const char * name, const GScriptValue & value)
{
	GScriptDataHolder * dataHolder = nullptr;

	if(glueData->getType() == gdtObject) {
		dataHolder = std::static_pointer_cast<GObjectGlueData>(glueData)->getDataHolder().get();
	}
	else {
		if(glueData->getType() == gdtClass) {
			dataHolder = std::static_pointer_cast<GClassGlueData>(glueData)->getDataHolder().get();
		}
	}

	if(dataHolder != nullptr) {
		dataHolder->setScriptValue(glueData->getBindingContext(), name, value);
	}
}

void doSetValueOnAccessible(
		const GContextPointer & context,
		IMetaAccessible * accessible,
		const GGlueDataPointer & instanceGlueData,
		GVariant value,
		const GGlueDataPointer & valueGlueData
	)
{
	ConvertRank rank = ConvertRank();

	GMetaType targetType(metaGetItemType(accessible));
	rankImplicitConvertForSharedPointer(&rank, valueGlueData, metaGetItemExtendType(accessible, GExtendTypeCreateFlag_SharedPointerTraits));
	if(rank.weight != ValueMatchRank_Unknown) {
		implicitConvertForSharedPointer(context, rank, &value, targetType, valueGlueData);
	}
	else {
		doConvertForMetaClassCast(context, &value, targetType, valueGlueData);
	}

	metaSetValue(accessible, getGlueDataInstanceAddress(instanceGlueData), value);
}

bool setValueOnNamedMember(
		const GGlueDataPointer & instanceGlueData,
		const char * name,
		const GScriptValue & value,
		const GGlueDataPointer & valueGlueData
	)
{
	if(getGlueDataCV(instanceGlueData) == GScriptInstanceCv::sicvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);

		return false;
	}

	GClassGlueDataPointer classData;
	GObjectGlueDataPointer objectData;
	if(instanceGlueData->getType() == gdtObject) {
		objectData = std::static_pointer_cast<GObjectGlueData>(instanceGlueData);
		classData = objectData->getClassData();
	}
	else {
		GASSERT(instanceGlueData->getType() == gdtClass);
		classData = std::static_pointer_cast<GClassGlueData>(instanceGlueData);
	}

	GContextPointer context = classData->getBindingContext();

	GMetaClassTraveller traveller(classData->getMetaClass(), getGlueDataInstanceAddress(instanceGlueData));

	void * instance = nullptr;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
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
		
		const GScriptValue & scriptValue = mapItem->getScriptValue();
		switch(scriptValue.getType()) {
		case GScriptValue::typeAccessible: {
			void * tempInstance;
			GScopedInterface<IMetaAccessible> data(scriptValue.toAccessible(&tempInstance));
			doSetValueOnAccessible(context, data.get(), instanceGlueData, value.getValue(), valueGlueData);
			return true;
		}
			break;

		case GScriptValue::typeEnum:
		case GScriptValue::typePrimary: {
			raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);
			return false;
		}
			break;
		
		default:
			break;
		}
		
		break;
	}

	setValueToScriptDataHolder(instanceGlueData, name, value);
	return true;
}

InvokeCallableResult doInvokeOperator(
		const GContextPointer & context,
		const GObjectGlueDataPointer & objectData,
		IMetaClass * metaClass,
		GMetaOpType op,
		InvokeCallableParam * callableParam
	)
{
	const int maxRankIndex = findAppropriateCallable(
		context->getService(),
		objectData,
		[=](const uint32_t index) { return metaClass->getOperatorAt(index); },
		metaClass->getOperatorCount(),
		callableParam,
		[=](IMetaCallable * t) { return gdynamic_cast<IMetaOperator *>(t)->getOperator() == op; }
	);

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;

		GScopedInterface<IMetaOperator> metaOperator(metaClass->getOperatorAt(maxRankIndex));
		doInvokeCallable(context, objectData->getInstanceAddress(), metaOperator.get(), callableParam, &result);
		result.callable.reset(metaOperator.get());
		return result;
	}

	raiseCoreException(Error_ScriptBinding_CantFindMatchedOperator);

	return InvokeCallableResult();
}

std::string getMethodNameFromMethodList(IMetaList * methodList)
{
	if(methodList->getCount() > 0) {
		GScopedInterface<IMetaItem> item(methodList->getAt(0));
		return item->getName();
	}
	else {
		return "";
	}
}

} // namespace bind_internal


} // namespace cpgf



