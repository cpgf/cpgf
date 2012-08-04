#include "gbindcommon_new.h"

#include "cpgf/gstringmap.h"
#include "cpgf/gcallback.h"
#include "cpgf/gmetaclasstraveller.h"

#include <vector>

using namespace std;


namespace cpgf {

namespace _bind_internal {


//*********************************************
// Declarations
//*********************************************

// such as 2 or more dimensions pointer
const int ParamMatchRank_Unknown = 0;
const int ParamMatchRank_Convert = 50000;
const int ParamMatchRank_Implicit_Begin = 70000;
const int ParamMatchRank_Implicit_WideStringToString = ParamMatchRank_Implicit_Begin + 0;
const int ParamMatchRank_Implicit_StringToWideString = ParamMatchRank_Implicit_Begin + 1;
const int ParamMatchRank_Implicit_End = 80000;
const int ParamMatchRank_Equal = 100000;


struct FindCallablePredict {
	bool operator () (IMetaCallable *) {
		return true;
	}
};

class GClassItem
{
private:
	typedef vector<GClassGlueDataPointer> ListType;

public:
	explicit GClassItem(const GClassGlueDataPointer & classGlueData)
		: classGlueData(classGlueData)
	{
	}

	const GClassGlueDataPointer & getClassGlueData() const {
		return this->classGlueData;
	}

private:
	GClassGlueDataPointer classGlueData;
};

class GClassPool
{
private:
	typedef GStringMap<GClassItem, GStringMapReuseKey> MapType;

public:
	bool hasMetaClass(IMetaClass * metaClass) const;
	void addMetaClass(const GClassGlueDataPointer & classGlueData);
	GClassGlueDataPointer getMetaClass(IMetaClass * metaClass) const;
	
private:
	GClassItem * getClassItem(IMetaClass * metaClass) const;

private:
	mutable MapType classMap;
};

bool GClassPool::hasMetaClass(IMetaClass * metaClass) const
{
	return this->getClassItem(metaClass) != NULL;
}

void GClassPool::addMetaClass(const GClassGlueDataPointer & classGlueData)
{
	this->classMap.set(classGlueData->getMetaClass()->getQualifiedName(), GClassItem(classGlueData));
}

GClassGlueDataPointer GClassPool::getMetaClass(IMetaClass * metaClass) const
{
	GClassItem * item = this->getClassItem(metaClass);
	if(item == NULL) {
		return GClassGlueDataPointer();
	}
	else {
		return item->getClassGlueData();
	}
}

GClassItem * GClassPool::getClassItem(IMetaClass * metaClass) const
{
	MapType::iterator it = this->classMap.find(metaClass->getQualifiedName());
	if(it == this->classMap.end()) {
		return NULL;
	}
	else {
		return &it->second;
	}
}


//*********************************************
// Classes implementations
//*********************************************


GMetaMapItem::GMetaMapItem()
	: item(NULL), type(mmitNone), enumIndex(0)
{
}

GMetaMapItem::GMetaMapItem(IMetaItem * item, GMetaMapItemType type)
	: item(item), type(type), enumIndex(0)
{
}

GMetaMapItem::GMetaMapItem(size_t enumIndex, IMetaEnum * item)
	: item(item), type(mmitEnumValue), enumIndex(enumIndex)
{
}

GMetaMapItem::GMetaMapItem(IMetaList * metaList)
	: item(metaList), type(mmitMethodList), enumIndex(0)
{
}

GMetaMapItem::GMetaMapItem(const GMetaMapItem & other)
	: item(other.item), type(other.type), enumIndex(other.enumIndex)
{
}

GMetaMapItem::~GMetaMapItem()
{
}

GMetaMapItem & GMetaMapItem::operator = (GMetaMapItem other)
{
	this->swap(other);

	return *this;
}

void GMetaMapItem::swap(GMetaMapItem & other)
{
	using std::swap;
	
	swap(this->item, other.item);
	swap(this->type, other.type);
	swap(this->enumIndex, other.enumIndex);
}

GMetaMapItemType GMetaMapItem::getType() const
{
	return this->type;
}

IObject * GMetaMapItem::getItem() const
{
	if(! this->item) {
		return NULL;
	}
	
	this->item->addReference();
	
	return this->item.get();
}


GMetaMapClass::GMetaMapClass(IMetaClass * metaClass)
{
	this->buildMap(metaClass);
}

GMetaMapItem * GMetaMapClass::findItem(const char * name)
{
	MapType::iterator it = this->itemMap.find(name);
	if(it == this->itemMap.end()) {
		return NULL;
	}
	else {
		return &this->itemMap[name];
	}
}

void GMetaMapClass::buildMap(IMetaClass * metaClass)
{
	using namespace std;
	
	uint32_t count;
	uint32_t i;
	
	count = metaClass->getClassCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaClass> innerClass(metaClass->getClassAt(i));
		const char * name = innerClass->getName();
		this->itemMap[name] = GMetaMapItem(innerClass.get(), mmitClass);
	}

	count = metaClass->getEnumCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaEnum> metaEnum(metaClass->getEnumAt(i));
		const char * name = metaEnum->getName();
		this->itemMap[name] = GMetaMapItem(metaEnum.get(), mmitEnum);
		
		uint32_t keyCount = metaEnum->getCount();
		for(uint32_t k = 0; k < keyCount; ++k) {
			const char * keyName = metaEnum->getKey(k);
			this->itemMap[keyName] = GMetaMapItem(k, metaEnum.get());
		}
	}
	
	count = metaClass->getFieldCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaField> field(metaClass->getFieldAt(i));
		const char * name = field->getName();
		this->itemMap[name] = GMetaMapItem(field.get(), mmitField);
	}
	
	count = metaClass->getPropertyCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaProperty> prop(metaClass->getPropertyAt(i));
		const char * name = prop->getName();
		this->itemMap[name] = GMetaMapItem(prop.get(), mmitProperty);
	}
	
	count = metaClass->getMethodCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaMethod> method(metaClass->getMethodAt(i));
		const char * name = method->getName();
		MapType::iterator it = this->itemMap.find(name);
		if(it == this->itemMap.end()) {
			this->itemMap[name] = GMetaMapItem(method.get(), mmitMethod);
		}
		else {
			GMetaMapItem & item = it->second;
			if(item.getType() == mmitMethod) {
				GScopedInterface<IMetaList> metaList(createMetaList());
				GScopedInterface<IMetaItem> metaItem(gdynamic_cast<IMetaItem *>(item.getItem()));
				metaList->add(metaItem.get(), NULL);
				metaList->add(method.get(), NULL);
				this->itemMap[name] = GMetaMapItem(metaList.get());
			}
			else {
				GASSERT(item.getType() == mmitMethodList);
				
				GScopedInterface<IMetaList> metaList(gdynamic_cast<IMetaList *>(item.getItem()));
				metaList->add(method.get(), NULL);
			}
		}
	}
}


const GMetaMapClassPointer & GClassGlueData::getClassMap() const
{
	if(! this->classMap) {
		this->classMap.reset(new GMetaMapClass(this->getMetaClass()));
	}
	return this->classMap;
}


GObjectGlueData::GObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classGlueData, void * instance,
	bool allowGC, ObjectPointerCV cv, ObjectGlueDataType dataType)
	: super(gdtObject, context), classGlueData(classGlueData), instance(instance), allowGC(allowGC), cv(cv), dataType(dataType)
{
	switch(dataType) {
		case ogdtInterface:
			this->allowGC = false;
			this->interfaceObject.reset(static_cast<IObject *>(instance));
			break;

		default:
			break;
	}

	GScopedInterface<IMetaScriptWrapper> scriptWrapper(metaGetItemExtendType(this->classGlueData->getMetaClass(), GExtendTypeCreateFlag_ScriptWrapper).getScriptWrapper());
	if(scriptWrapper) {
//		scriptWrapper->setScriptDataStorage(instance, this->getDataStorage());
	}
}

GObjectGlueData::~GObjectGlueData()
{
	if(this->allowGC) {
		this->classGlueData->getMetaClass()->destroyInstance(this->instance);
	}
}


GBindingContext::GBindingContext(IMetaService * service, const GScriptConfig & config)
	: service(service), config(config), classPool(new GClassPool())
{
}

GBindingContext::~GBindingContext()
{
}

GClassGlueDataPointer GBindingContext::newClassGlueData(const GContextPointer & context, IMetaClass * metaClass)
{
	GClassGlueDataPointer data(new GClassGlueData(context, metaClass));
	this->classPool->addMetaClass(data);
	return data;
}

GClassGlueDataPointer GBindingContext::requireClassGlueData(const GContextPointer & context, IMetaClass * metaClass)
{
	if(this->classPool->hasMetaClass(metaClass)) {
		GClassGlueDataPointer data(new GClassGlueData(context, metaClass));
		return data;
	}
	else {
		return this->newClassGlueData(context, metaClass);
	}
}

GObjectGlueDataPointer GBindingContext::newObjectGlueData(const GContextPointer & context, const GClassGlueDataPointer & classData, void * instance,
	bool allowGC, ObjectPointerCV cv, ObjectGlueDataType dataType)
{
	GObjectGlueDataPointer data(new GObjectGlueData(context, classData, instance, allowGC, cv, dataType));
	return data;
}

GRawGlueDataPointer GBindingContext::newRawGlueData(const GContextPointer & context, const GVariant & data)
{
	GRawGlueDataPointer rawData(new GRawGlueData(context, data));
	return rawData;
}


InvokeParamRank::InvokeParamRank(size_t paramCount) : paramCount(paramCount)
{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->ranks[i] = ParamMatchRank_Unknown;
	}
}

InvokeParamRank::InvokeParamRank(const InvokeParamRank & other) : paramCount(other.paramCount)
{
	for(size_t i = 0; i < this->paramCount; ++i) {
		this->ranks[i] = other.ranks[i];
	}
}

InvokeParamRank & InvokeParamRank::operator = (const InvokeParamRank & other)
{
	this->paramCount = other.paramCount;

	for(size_t i = 0; i < this->paramCount; ++i) {
		this->ranks[i] = other.ranks[i];
	}

	return *this;
}


InvokeCallableParam::InvokeCallableParam(size_t paramCount)
	: paramCount(paramCount), paramsRank(paramCount)
{
	if(this->paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}
}

InvokeCallableParam::~InvokeCallableParam()
{
}


//*********************************************
// Global function implementations
//*********************************************

ObjectPointerCV metaTypeToCV(const GMetaType & type)
{
	if(type.isPointer()) {
		if(type.isPointerToConst()) {
			return opcvConst;
		}
		else if(type.isPointerToVolatile()) {
			return opcvVolatile;
		}
		else if(type.isPointerToConstVolatile()) {
			return opcvConstVolatile;
		}
	}
	else {
		if(type.isConst()) {
			return opcvConst;
		}
		else if(type.isVolatile()) {
			return opcvVolatile;
		}
		else if(type.isConstVolatile()) {
			return opcvConstVolatile;
		}
	}
	
	return opcvNone;
}


bool isParamImplicitConvert(int paramRank)
{
	return paramRank >= ParamMatchRank_Implicit_Begin && paramRank < ParamMatchRank_Implicit_End;
}

int rankImplicitConvert(const GVariant & sourceData, const GMetaType & targetType)
{
	if(variantIsString(sourceData) && targetType.isWideString()) {
		return ParamMatchRank_Implicit_StringToWideString;
	}

	if(variantIsWideString(sourceData) && targetType.isString()) {
		return ParamMatchRank_Implicit_WideStringToString;
	}

	return ParamMatchRank_Unknown;
}

int rankCallableParam(IMetaService * service, IMetaCallable * callable, const InvokeCallableParam * callbackParam, size_t paramIndex)
{
	GMetaType proto = metaGetParamType(callable, paramIndex);
	GScriptDataType sdt = callbackParam->paramsType[paramIndex].dataType;
	
	if(sdt == sdtNull) {
		return ParamMatchRank_Equal;
	}
	
	if(proto.isFundamental() && sdt == sdtFundamental) {
		return ParamMatchRank_Equal;
	}

	if(sdt == sdtScriptMethod && vtIsInterface(proto.getVariantType())) {
		return ParamMatchRank_Convert;
	}

	if(proto.getPointerDimension() > 1) {
		return ParamMatchRank_Unknown;
	}

	int implicitRank = rankImplicitConvert(callbackParam->paramsData[paramIndex], proto);
	if(implicitRank != ParamMatchRank_Unknown) {
		return implicitRank;
	}

	// check for meta class

	if(! callbackParam->paramsType[paramIndex].typeItem) {
		return ParamMatchRank_Unknown;
	}

	if(metaIsClass(callbackParam->paramsType[paramIndex].typeItem->getCategory())) {
		GScopedInterface<IMetaTypedItem> protoType(service->findTypedItemByName(proto.getBaseName()));
		if(! protoType || ! metaIsClass(protoType->getCategory())) {
			return ParamMatchRank_Unknown;
		}

		IMetaClass * paramClass = static_cast<IMetaClass *>(callbackParam->paramsType[paramIndex].typeItem.get());
		IMetaClass * protoClass = static_cast<IMetaClass *>(protoType.get());

		if(paramClass->equals(protoClass)) {
			return ParamMatchRank_Equal;
		}
		else {
			if(paramClass->isInheritedFrom(protoClass)) {
				return ParamMatchRank_Convert;
			}
		}
	}

	return ParamMatchRank_Unknown;
}

int rankCallable(IMetaService * service, IMetaCallable * callable, const InvokeCallableParam * callbackParam, const InvokeParamRank * paramsRank)
{
	if(!! callable->isVariadic()) {
		return 0;
	}

	if(callable->getParamCount() < callbackParam->paramCount) {
		return -1;
	}

	if(callable->getParamCount() - callable->getDefaultParamCount() > callbackParam->paramCount) {
		return -1;
	}

	int rank = 1;
	
	for(size_t i = 0; i < callbackParam->paramCount; ++i) {
		int paramRank = rankCallableParam(service, callable, callbackParam, i);
		rank += paramRank;
		paramsRank->ranks[i] = paramRank;

		if(! isParamImplicitConvert(paramRank)) {
			bool ok = !! callable->checkParam(&callbackParam->paramsData[i].data, static_cast<uint32_t>(i));
			metaCheckError(callable);
			if(! ok) {
				return -1;
			}
		}
	}

	return rank;
}

bool allowAccessData(const GScriptConfig & config, bool isInstance, IMetaAccessible * accessible)
{
	if(isInstance) {
		if(! config.allowAccessStaticDataViaInstance()) {
			if(accessible->isStatic()) {
				return false;
			}
		}
	}
	else {
		if(! accessible->isStatic()) {
			return false;
		}
	}
	
	return true;
}

void convertParam(GVariant * v, int paramRank, GVariant * holder)
{
	switch(paramRank) {
		case ParamMatchRank_Implicit_StringToWideString: {
			*holder = *v;
			*v = GVariant();
			const char * s = fromVariant<char *>(*holder);
			GScopedArray<wchar_t> ws(stringToWideString(s));
			*v = createWideStringVariant(ws.get());
		}
			break;

		case ParamMatchRank_Implicit_WideStringToString: {
			*holder = *v;
			*v = GVariant();
			const wchar_t * ws = fromVariant<wchar_t *>(*holder);
			GScopedArray<char> s(wideStringToString(ws));
			*v = createStringVariant(s.get());
		}
			break;
	}
}

void doInvokeCallable(void * instance, IMetaCallable * callable, InvokeCallableParam * callableParam, InvokeCallableResult * result)
{
	result->resultCount = callable->hasResult() ? 1 : 0;

	GVariant holders[REF_MAX_ARITY];

	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		if(isParamImplicitConvert(callableParam->paramsRank.ranks[i])) {
			convertParam(&callableParam->paramsData[i], callableParam->paramsRank.ranks[i], &holders[i]);
		}
	}

	GVariantData * data[REF_MAX_ARITY];
	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		data[i] = & callableParam->paramsData[i].data;
	}
	callable->executeIndirectly(&result->resultData.data, instance, data, static_cast<uint32_t>(callableParam->paramCount));
	metaCheckError(callable);
}


void * doInvokeConstructor(IMetaService * service, IMetaClass * metaClass, InvokeCallableParam * callableParam)
{
	void * instance = NULL;

	if(callableParam->paramCount == 0 && metaClass->canCreateInstance()) {
		instance = metaClass->createInstance();
	}
	else {
		int maxRankIndex = findAppropriateCallable(service,
			makeCallback(metaClass, &IMetaClass::getConstructorAt), metaClass->getConstructorCount(),
			callableParam, FindCallablePredict());

		if(maxRankIndex >= 0) {
			InvokeCallableResult result;
		
			GScopedInterface<IMetaConstructor> constructor(metaClass->getConstructorAt(static_cast<uint32_t>(maxRankIndex)));
			doInvokeCallable(NULL, constructor.get(), callableParam, &result);
			instance = fromVariant<void *>(GVariant(result.resultData));
		}
	}

	return instance;
}

bool shouldRemoveReference(const GMetaType & type)
{
	return type.isReference()
		&& (type.isPointer() || vtIsFundamental(vtGetBaseType(type.getVariantType())))
		;
}

wchar_t * stringToWideString(const char * s)
{
	size_t len = strlen(s);
	GScopedArray<wchar_t> ws(new wchar_t[len + 1]);
	std::fill(ws.get(), ws.get() + len + 1, 0);
	mbstowcs(ws.get(), s, len);

	return ws.take();
}

char * wideStringToString(const wchar_t * ws)
{
	size_t len = wcslen(ws);
	GScopedArray<char> s(new char[len + 1]);
	std::fill(s.get(), s.get() + len + 1, 0);
	wcstombs(s.get(), ws, len);

	return s.take();
}

GMetaVariant glueDataToVariant(const GGlueDataPointer & glueData)
{
	if(glueData) {

		switch(glueData->getType()) {
			case gdtClass: {
				GClassGlueDataPointer classData = sharedStaticCast<GClassGlueData>(glueData);;
				GMetaTypeData typeData;
				classData->getMetaClass()->getMetaType(&typeData);
				metaCheckError(classData->getMetaClass());
				GMetaType type(typeData);
				type.addPointer();
				return GMetaVariant(classData->getMetaClass(), type);
			}

			case gdtObject: {
				GObjectGlueDataPointer objectData = sharedStaticCast<GObjectGlueData>(glueData);
				GMetaTypeData typeData;
				objectData->getClassData()->getMetaClass()->getMetaType(&typeData);
				metaCheckError(objectData->getClassData()->getMetaClass());
				GMetaType type(typeData);
				type.addPointer();
				switch(objectData->getDataType()) {
					case ogdtNormal:
						return GMetaVariant(pointerToObjectVariant(objectData->getInstance()), type);
					
					case ogdtInterface:
						return GMetaVariant(GVariant(objectData->getInterfaceObject()), type);
				}

				break;
			}

			case gdtRaw: {
				GRawGlueDataPointer rawData = sharedStaticCast<GRawGlueData>(glueData);
				return GMetaVariant(rawData->getData(), GMetaType());
			}

			default:
				break;
		}
	}

	return GMetaVariant();
}

GVariant getAccessibleValueAndType(void * instance, IMetaAccessible * accessible, GMetaType * outType, bool instanceIsConst)
{
	GVariant value;
	GMetaTypeData typeData;

	accessible->getItemType(&typeData);
	metaCheckError(accessible);

	*outType = GMetaType(typeData);

	void * address = accessible->getAddress(instance);
	if(address != NULL && !outType->isPointer() && outType->baseIsClass()) {
		value = address;

		if(instanceIsConst) {
			outType->addPointerToConst();
		}
		else {
			outType->addPointer();
		}
	}
	else {
		accessible->get(&value.data, instance);
		metaCheckError(accessible);
	}

	return value;
}

bool doSetFieldValue(const GGlueDataPointer & glueData, const char * name, const GVariant & value)
{
	if(getGlueDataCV(glueData) == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);

		return false;
	}

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

	const GScriptConfig & config = classData->getContext()->getConfig();
	GContextPointer context = classData->getContext();

	GMetaClassTraveller traveller(classData->getMetaClass(), getGlueDataInstance(glueData));
	
	void * instance = NULL;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			return false;
		}
		
		GMetaMapClassPointer mapClass = classData->getClassMap();
		if(! mapClass) {
			continue;
		}

		GMetaMapItem * mapItem = mapClass->findItem(name);
		if(mapItem == NULL) {
			continue;
		}

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {
				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(config, isInstance, data.get())) {
					metaSetValue(data.get(), getGlueDataInstance(glueData), value);
					return true;
				}
			}
			   break;

//			case mmitMethod:
//			case mmitMethodList:
//				userData->getObjectData()->setScriptValue(name, value);
//				return true;

			case mmitEnum:
			case mmitEnumValue:
			case mmitClass:
				raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);
				return false;

			default:
				break;
		}
	}
}

ObjectPointerCV getGlueDataCV(const GGlueDataPointer & glueData)
{
	if(glueData->getType() == gdtObject) {
		return sharedStaticCast<GObjectGlueData>(glueData)->getCV();
	}
	else {
		return opcvNone;
	}
}

void * getGlueDataInstance(const GGlueDataPointer & glueData)
{
	if(glueData->getType() == gdtObject) {
		return sharedStaticCast<GObjectGlueData>(glueData)->getInstance();
	}
	else {
		return NULL;
	}
}

IMetaClass * getGlueDataMetaClass(const GGlueDataPointer & glueData)
{
	if(glueData->getType() == gdtObject) {
		return sharedStaticCast<GObjectGlueData>(glueData)->getClassData()->getMetaClass();
	}
	else {
		if(glueData->getType() == gdtClass) {
			return sharedStaticCast<GClassGlueData>(glueData)->getMetaClass();
		}
		else {
			return NULL;
		}
	}
}



} // namespace _bind_internal


} // namespace cpgf



