#ifndef CPGF_GBINDCOMMON_H
#define CPGF_GBINDCOMMON_H

#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptvalue.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/scriptbind/gscriptuserconverter.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gglobal.h"
#include "cpgf/gmetaoperatorop.h"
#include "cpgf/gsharedptr.h"
#include "cpgf/gflags.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gsharedinterface.h"
#include "cpgf/gstringutil.h"

#include "gscriptmetamap.h"
#include "gscriptgluedata.h"
#include "gscriptcommon.h"
#include "gbindcontext.h"

#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>


namespace cpgf {

extern int Error_ScriptBinding_FailVariantToScript;

class GMetaClassTraveller;
class GScriptCoreService;

namespace bind_internal {

class GBindingContext;
class GScriptDataHolder;

typedef GSharedPointer<GBindingContext> GContextPointer;
typedef GWeakPointer<GBindingContext> GWeakContextPointer;

class GClassGlueData;

class ConvertRank
{
public:
	void resetRank() {
		this->weight = 0; //ValueMatchRank_Unknown;
/* it's safe to not reset the pointers because they are used according to weight.
		this->sourceClass = nullptr;
		this->targetClass.reset();
		this->userConverter = nullptr;
		this->userConverterTag = 0;
*/
	}

public:
	int weight;
	IMetaClass * sourceClass;
	// We have to use GSharedInterface to hold targetClass
	// because the targetClass may be created dynamically from the global repository.
	// sourceClass doesn't have this problem, it's always hold by the caller.
	GSharedInterface<IMetaClass> targetClass;
	IScriptUserConverter * userConverter;
	uint32_t userConverterTag;
};

class CallableParamData
{
public:
	GScriptValue value;
	GGlueDataPointer paramGlueData;
};

class InvokeCallableParam
{
public:
	InvokeCallableParam(size_t paramCount, IScriptContext * scriptContext);
	~InvokeCallableParam();

public:
	CallableParamData * params;
	char paramsBuffer[sizeof(CallableParamData) * REF_MAX_ARITY];
	size_t paramCount;
	ConvertRank * paramRanks;
	char paramRanksBuffer[sizeof(ConvertRank) * REF_MAX_ARITY];
	ConvertRank * backParamRanks;
	char paramRanksBackBuffer[sizeof(ConvertRank) * REF_MAX_ARITY];
	GSharedInterface<IScriptContext> scriptContext;
};

class InvokeCallableResult
{
public:
	int resultCount;
	GVariant resultData;
	GSharedInterface<IMetaCallable> callable;
};

class GScriptObjectBase : public GScriptObject
{
private:
	typedef GScriptObject super;

public:
	GScriptObjectBase(const GContextPointer & context, const GScriptConfig & config);
	GScriptObjectBase(const GScriptObjectBase & other);
	virtual ~GScriptObjectBase();

	IMetaClass * cloneMetaClass(IMetaClass * metaClass);

	IMetaService * getMetaService();

	virtual IScriptContext * getContext() const;

protected:
	const GContextPointer & getBindingContext() const {
		return this->context;
	}

	virtual void doBindCoreService(const char * name, IScriptLibraryLoader * libraryLoader);

private:
	GContextPointer context;
};


class GScriptFunctionBase : public GScriptFunction
{
private:
	typedef GScriptFunction super;

public:
	explicit GScriptFunctionBase(const GContextPointer & context)
		: context(context), weakContext(context)
	{
	}

	virtual void weaken() {
		this->context.reset();
	}

protected:
	GContextPointer getBindingContext() {
		return this->weakContext.get();
	}

private:
	// Here we must use strong shared pointer,
	// otherwise the context may be freed by the script object
	// while the script function is still live.
	// But there is cyclic shared pointer chain
	// IScriptFunction->GBindingContext->GClassPool->GClassGlueData->GScriptDataHolder->IScriptFunction
	// Now the cyclic chain is broken by calling weaken when setting IScriptFunction to GScriptDataHolder
	// The solution is super ugly, but I can't find better solution for now.

	GContextPointer context;
	GWeakContextPointer weakContext;
};


class GScriptArrayBase : public GScriptArray
{
private:
	typedef GScriptArray super;

public:
	explicit GScriptArrayBase(const GContextPointer & context)
		: context(context)
	{
	}

protected:
	GContextPointer getBindingContext() {
		return this->context;
	}

private:
	// Here we must use strong shared pointer,
	// otherwise the context may be freed by the script object
	// while the script array is still live.
	GContextPointer context;
};


ObjectPointerCV metaTypeToCV(const GMetaType & type);

int rankCallable(
	IMetaService * service,
	const GObjectGlueDataPointer & objectData,
	IMetaCallable * callable,
	const InvokeCallableParam * callbackParam,
	ConvertRank * paramRanks
);

bool allowAccessData(const GScriptConfig & config, bool isInstance, IMetaAccessible * accessible);

void * doInvokeConstructor(
	const GContextPointer & context,
	IMetaService * service,
	IMetaClass * metaClass,
	InvokeCallableParam * callableParam
);
InvokeCallableResult doInvokeMethodList(
	const GContextPointer & context,
	const GObjectGlueDataPointer & objectData,
	const GMethodGlueDataPointer & methodData,
	InvokeCallableParam * callableParam
);

bool shouldRemoveReference(const GMetaType & type);

wchar_t * stringToWideString(const char * s);
char * wideStringToString(const wchar_t * ws);

GScriptValue glueDataToScriptValue(const GGlueDataPointer & glueData);

GVariant getAccessibleValueAndType(
	void * instance,
	IMetaAccessible * accessible,
	GMetaType * outType,
	bool instanceIsConst
);

IMetaClass * selectBoundClass(IMetaClass * currentClass, IMetaClass * derived);

bool setValueOnNamedMember(
	const GGlueDataPointer & glueData,
	const char * name,
	const GScriptValue & value,
	const GGlueDataPointer & valueGlueData
);

ObjectPointerCV getGlueDataCV(const GGlueDataPointer & glueData);
GVariant getGlueDataInstance(const GGlueDataPointer & glueData);
void * getGlueDataInstanceAddress(const GGlueDataPointer & glueData);
IMetaClass * getGlueDataMetaClass(const GGlueDataPointer & glueData);
IMetaSharedPointerTraits * getGlueDataSharedPointerTraits(const GGlueDataPointer & glueData);

InvokeCallableResult doInvokeOperator(
	const GContextPointer & context,
	const GObjectGlueDataPointer & objectData,
	IMetaClass * metaClass,
	GMetaOpType op,
	InvokeCallableParam * callableParam
);

IMetaObjectLifeManager * createObjectLifeManagerForInterface(const GVariant & value);

IMetaList * getMethodListFromMapItem(GMetaMapItem * mapItem, void * instance);

std::string getMethodNameFromMethodList(IMetaList * methodList);

inline void * getInstanceHash(const GVariant & instance)
{
	return objectAddressFromVariant(instance);
}


struct GScriptObjectCacheKey {
	GScriptObjectCacheKey() : key(nullptr), className(nullptr), cv(opcvNone) {
	}

	GScriptObjectCacheKey(void * key, const char * className, ObjectPointerCV cv)
		: key(key), className(className), cv(cv) {
	}

	bool operator < (const GScriptObjectCacheKey & other) const {
		if(key < other.key) {
			return true;
		}
		if(key == other.key) {
			if(cv < other.cv) {
				return true;
			}
			if(cv > other.cv) {
				return false;
			}

			if(className == other.className) {
				return false;
			}
			else {
				return strcmp(className, other.className) < 0;
			}
		}
		return false;
	}

	void * key;
	const char * className;
	ObjectPointerCV cv;
};

class GScriptObjectCacheData {
public:
	virtual ~GScriptObjectCacheData() {}
};

class GScriptObjectCache
{
private:
	typedef std::map<GScriptObjectCacheKey, GSharedPointer<GScriptObjectCacheData> > ObjectMapType;

public:
	template <class T>
	T * findScriptObject(const GVariant & instance, const GClassGlueDataPointer & classData,
		ObjectPointerCV cv)
	{
		GScriptObjectCacheKey key(
			getInstanceHash(instance),
			classData->getMetaClass()->getQualifiedName(),
			cv
		);
		typename ObjectMapType::iterator it = this->objectMap.find(key);
		if(it != this->objectMap.end()) {
			return dynamic_cast<T *>(it->second.get());
		}
		return nullptr;
	}

	void addScriptObject(
		const GVariant & instance,
		const GClassGlueDataPointer & classData,
		ObjectPointerCV cv,
		GScriptObjectCacheData * scriptObject
	)
	{
		GScriptObjectCacheKey key(
			getInstanceHash(instance),
			classData->getMetaClass()->getQualifiedName(),
			cv
		);
		this->objectMap.insert(std::make_pair(key, GSharedPointer<GScriptObjectCacheData>(scriptObject)));
	}

	void freeScriptObject(GGlueDataWrapper * dataWrapper)
	{
		GVariant instance = getGlueDataInstance(dataWrapper->getData());
		if(instance.isEmpty()) {
			return;
		}
		GScriptObjectCacheKey entry(
			getInstanceHash(instance),
			getGlueDataMetaClass(dataWrapper->getData())->getQualifiedName(),
			getGlueDataCV(dataWrapper->getData())
		);

		this->objectMap.erase(entry);
	}

	void clear() {
		this->objectMap.clear();
	}

private:
	ObjectMapType objectMap;
};

struct ConvertRankBuffer
{
	explicit ConvertRankBuffer(const size_t paramCount)
		: paramCount(paramCount), paramRanks((ConvertRank *)paramRanksBuffer)
	{
		memset(this->paramRanksBuffer, 0, sizeof(ConvertRank) * paramCount);
	};
	
	~ConvertRankBuffer()
	{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->paramRanks[i].~ConvertRank();
	}
	}

	size_t paramCount;
	ConvertRank * paramRanks;
	char paramRanksBuffer[sizeof(ConvertRank) * REF_MAX_ARITY];
};

template <typename Getter, typename Predict>
int findAppropriateCallable(
	IMetaService * service,
	const GObjectGlueDataPointer & objectData,
	const Getter & getter,
	const size_t callableCount,
	InvokeCallableParam * callableParam,
	const Predict & predict
)
{
	int maxRank = -1;
	int maxRankIndex = -1;

	for(size_t i = 0; i < callableCount; ++i) {
		GScopedInterface<IMetaCallable> meta(gdynamic_cast<IMetaCallable *>(getter(static_cast<uint32_t>(i))));
		if(predict(meta.get())) {
			const int weight = rankCallable(service, objectData, meta.get(), callableParam, callableParam->backParamRanks);
			if(weight > maxRank) {
				maxRank = weight;
				maxRankIndex = static_cast<int>(i);
				std::swap(callableParam->paramRanks, callableParam->backParamRanks);
			}
			if(callableCount > 1) {
				for(size_t i = 0; i < callableParam->paramCount; ++i) {
					callableParam->backParamRanks[i].resetRank();
				}
			}
		}
	}

	return maxRankIndex;
}


template <typename Methods>
typename Methods::ResultType complexVariantToScript(
	const GContextPointer & context,
	const GVariant & value,
	const GMetaType & type,
	const GBindValueFlags & flags,
	GGlueDataPointer * outputGlueData
)
{
	GVariantType vt = static_cast<GVariantType>((GVtType)value.getType() & ~(GVtType)GVariantType::maskByReference);

	if(! type.isEmpty() && type.getPointerDimension() <= 1) {
		GScopedInterface<IMetaTypedItem> typedItem(context->getService()->findTypedItemByName(type.getBaseName()));
		if(typedItem) {
			GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

			return Methods::doObjectToScript(context, context->getOrNewClassData(value, gdynamic_cast<IMetaClass *>(typedItem.get())),
				value, flags, metaTypeToCV(type), outputGlueData);
		}
		else {
			if(vtIsInterface(vt)) {
				IObject * obj = fromVariant<IObject *>(value);
				if(dynamic_cast<IMetaClass *>(obj)) { // !!! GUID
					IMetaClass * metaClass = dynamic_cast<IMetaClass *>(obj);
					return Methods::doClassToScript(context, metaClass);
				}
			}
		}

		if(shouldRemoveReference(type)) {
			GMetaType newType(type);
			newType.removeReference();

			return Methods::doVariantToScript(context, createTypedVariant(value, newType), flags, outputGlueData);
		}
	}

	if(flags.has(bvfAllowRaw)) {
		return Methods::doRawToScript(context, value, outputGlueData);
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
			typename Methods::ResultType result = Methods::doStringToScript(context, s);

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
			typename Methods::ResultType result = Methods::doWideStringToScript(context, ws);

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

	GBindValueFlags newFlags = flags;
	newFlags.set(bvfAllowRaw);

	typename Methods::ResultType result;
	GGlueDataPointer glueData;

	result = Methods::doVariantToScript(context, createTypedVariant(value, realType), newFlags, &glueData);
	if(! Methods::isSuccessResult(result)) {
		glueData.reset();
		result = Methods::doRawToScript(context, value, &glueData);
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
		result = Methods::doRawToScript(context, value, nullptr);
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

	typename Methods::ResultType result = Methods::doVariantToScript(context, createTypedVariant(value, type), GBindValueFlags(), nullptr);

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
					return accessibleToScript<Methods>(context, data.get(), instance, getGlueDataCV(glueData) == opcvConst);
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList: {
				return Methods::doMethodsToScript(classData, mapItem, metaClass.get(), derived.get(), objectData);
			}

			case mmitEnum:
				if(! isInstance || config.allowAccessEnumTypeViaInstance()) {
					return Methods::doEnumToScript(classData, mapItem, name);
				}
				break;

			case mmitEnumValue:
				if(! isInstance || config.allowAccessEnumValueViaInstance()) {
					GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
					return Methods::doVariantToScript(context, metaGetEnumValue(metaEnum.get(), static_cast<uint32_t>(mapItem->getEnumIndex())), GBindValueFlags(bvfAllowRaw), nullptr);
				}
				break;

			case mmitClass:
				if(! isInstance || config.allowAccessClassViaInstance()) {
					GScopedInterface<IMetaClass> innerMetaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
					return Methods::doClassToScript(context, innerMetaClass.get());
				}
				break;

			default:
				break;
		}
	}

	return Methods::defaultValue();
}


class GMapItemMethodData : public GUserData
{
public:
	explicit GMapItemMethodData(const GMethodGlueDataPointer & methodData)
		: methodData(methodData) {
	}

	const GMethodGlueDataPointer & getMethodData() const {
		return this->methodData;
	}

private:
	GMethodGlueDataPointer methodData;
};


} // namespace bind_internal


} // namespace cpgf



#endif
