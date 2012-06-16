#include "../pinclude/gbindcommon.h"

#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gcallback.h"

#include <stdarg.h>


namespace cpgf {

namespace bind_internal {

// such as 2 or more dimensions pointer
const int ParamMatchRank_Unknown = 0;
const int ParamMatchRank_Convert = 50000;
const int ParamMatchRank_Implicit_Begin = 70000;
const int ParamMatchRank_Implicit_WideStringToString = ParamMatchRank_Implicit_Begin + 0;
const int ParamMatchRank_Implicit_StringToWideString = ParamMatchRank_Implicit_Begin + 1;
const int ParamMatchRank_Implicit_End = 80000;
const int ParamMatchRank_Equal = 100000;


GScriptBindingParam::GScriptBindingParam(IMetaService * service, const GScriptConfig & config)
	: service(service), config(config), metaMap(createMetaMap())
{
	this->service->addReference();
}

GScriptBindingParam::~GScriptBindingParam()
{
}


GSharedInstance::GSharedInstance()
	: metaClass(NULL), instance(NULL), isInstance(false), allowGC(false), cv(opcvNone), dataType(cudtNormal)
{
}

GSharedInstance::GSharedInstance(IMetaClass * metaClass, void * instance, bool isInstance,
	bool allowGC, ObjectPointerCV cv, ClassUserDataType dataType)
	: metaClass(metaClass), instance(instance), isInstance(isInstance), allowGC(allowGC), cv(cv), dataType(dataType)
{
	if(this->metaClass != NULL) {
		this->metaClass->addReference();
	}
	
	switch(dataType) {
		case cudtInterface:
			this->allowGC = false;
			if(this->interfaceObject != NULL) {
				this->interfaceObject->addReference();
			}
			break;

		default:
			break;
	}
}

GSharedInstance::~GSharedInstance()
{
	if(this->metaClass != NULL) {
		if(this->allowGC) {
			this->metaClass->destroyInstance(instance);
		}
		this->metaClass->releaseReference();
		this->metaClass = NULL;
	}
	switch(dataType) {
		case cudtInterface:
			if(this->interfaceObject != NULL) {
				this->interfaceObject->releaseReference();
				this->interfaceObject = NULL;
			}
			break;

		default:
			break;
	}
}


GClassUserData::GClassUserData(const GBindingParamPointer & param)
	: super(udtClass, param)
{
}

GClassUserData::GClassUserData(const GBindingParamPointer & param, IMetaClass * metaClass, void * instance, bool isInstance,
	bool allowGC, ObjectPointerCV cv, ClassUserDataType dataType)
	: super(udtClass, param), data(new GSharedInstance(metaClass, instance, isInstance, allowGC, cv, dataType))
{
}

GClassUserData::GClassUserData(const GClassUserData & other)
	: super(udtClass, other.getParam()), data(other.data)
{
}

GClassUserData::~GClassUserData()
{
}


GMetaMap * createMetaMap()
{
	return new GMetaMap;
}

GMetaMapItem::GMetaMapItem()
	: item(NULL), type(mmitNone), enumIndex(0)
{
}

GMetaMapItem::GMetaMapItem(IMetaItem * item, GMetaMapItemType type)
	: item(item), type(type), enumIndex(0)
{
	this->item->addReference();
}

GMetaMapItem::GMetaMapItem(size_t enumIndex, IMetaEnum * item)
	: item(item), type(mmitEnumValue), enumIndex(enumIndex)
{
	this->item->addReference();
}

GMetaMapItem::GMetaMapItem(IMetaList * metaList)
	: item(metaList), type(mmitMethodList), enumIndex(0)
{
	if(this->item != NULL) {
		this->item->addReference();
	}
}

GMetaMapItem::GMetaMapItem(const GMetaMapItem & other)
	: item(other.item), type(other.type), enumIndex(other.enumIndex)
{
	if(this->item != NULL) {
		this->item->addReference();
	}
}

GMetaMapItem::~GMetaMapItem()
{
	if(this->item != NULL) {
		this->item->releaseReference();
	}
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
	if(this->item == NULL) {
		return NULL;
	}
	
	this->item->addReference();
	
	return this->item;
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
				GScopedInterface<IMetaItem> metaItem(static_cast<IMetaItem *>(item.getItem()));
				metaList->add(metaItem.get(), NULL);
				metaList->add(method.get(), NULL);
				this->itemMap[name] = GMetaMapItem(metaList.get());
			}
			else {
				GASSERT(item.getType() == mmitMethodList);
				
				GScopedInterface<IMetaList> metaList(static_cast<IMetaList *>(item.getItem()));
				metaList->add(method.get(), NULL);
			}
		}
	}
}


GMetaMap::GMetaMap()
{
}

GMetaMap::~GMetaMap()
{
	for(MapType::iterator it = this->classMap.begin(); it != this->classMap.end(); ++it) {
		delete it->second;
	}
}

GMetaMapClass * GMetaMap::findClassMap(IMetaClass * metaClass)
{
	using namespace std;
	
	const char * name = metaClass->getQualifiedName();
	MapType::iterator it = this->classMap.find(name);
	
	if(it != this->classMap.end()) {
		return it->second;
	}
	else {
		return this->classMap.insert(MapType::value_type(name, new GMetaMapClass(metaClass))).first->second;
	}
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


InvokeCallableResult::InvokeCallableResult()
{
	initializeVarData(&this->resultData);
}

InvokeCallableResult::~InvokeCallableResult()
{
	freeVarData(&this->resultData);
}


bool variantIsScriptRawData(GVariantType /*vt*/)
{
	return true;
//	vt = vtGetBaseType(vt);
//	return vt == vtPointer || vt == vtObject || vt == vtShadow || vt == vtVoid;
}

bool metaMapItemIsAccessible(GMetaMapItemType type)
{
	return type == mmitField || type == mmitProperty;
}

bool metaMapItemIsInvokable(GMetaMapItemType type)
{
	return type == mmitMethod || type == mmitMethodList;
}

GMetaMapItem * findMetaMapItem(GMetaMap * metaMap, IMetaClass * metaClass, const char * itemName)
{
	GMetaMapClass * mapClass = metaMap->findClassMap(metaClass);

	if(mapClass != NULL) {
		return mapClass->findItem(itemName);
	}
	else {
		return NULL;
	}
}


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

int rankCallableParam(IMetaService * service, IMetaCallable * callable, InvokeCallableParam * callbackParam, size_t paramIndex)
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

int rankCallable(IMetaService * service, IMetaCallable * callable, InvokeCallableParam * callbackParam, InvokeParamRank * paramsRank)
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

bool allowInvokeCallable(GClassUserData * userData, IMetaCallable * method)
{
	if(userData != NULL && userData->data->isInstance) {
		if(! userData->getParam()->getConfig().allowAccessStaticMethodViaInstance()) {
			if(method->isStatic()) {
				return false;
			}
		}
	}
	else {
		if(! method->isStatic()) {
			return false;
		}
	}
	
	if(userData != NULL) {
		const GMetaType & methodType = metaGetItemType(method);
		switch(userData->data->cv) {
			case opcvConst:
				return methodType.isConstFunction();
				
			case opcvVolatile:
				return methodType.isVolatileFunction();
				
			case opcvConstVolatile:
				return methodType.isConstVolatileFunction();
				
			default:
				break;
		}
	}
	
	return true;
}

bool allowAccessData(GClassUserData * userData, IMetaAccessible * accessible)
{
	if(userData->data->isInstance) {
		if(! userData->getParam()->getConfig().allowAccessStaticDataViaInstance()) {
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

void doInvokeCallable(void * instance, IMetaCallable * callable, InvokeCallableParam * callableParam, InvokeCallableResult * result)
{
	result->resultCount = callable->hasResult() ? 1 : 0;
	vtInit(result->resultData.typeData);

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
	callable->executeIndirectly(&result->resultData, instance, data, static_cast<uint32_t>(callableParam->paramCount));
	metaCheckError(callable);
}

void loadMethodList(GMetaClassTraveller * traveller,
	IMetaList * methodList, GMetaMap * metaMap, GMetaMapItem * mapItem,
	void * instance, GClassUserData * userData, const char * methodName, bool allowAny)
{
	while(mapItem != NULL) {
		if(mapItem->getType() == mmitMethod) {
			GScopedInterface<IMetaMethod> method(gdynamic_cast<IMetaMethod *>(mapItem->getItem()));
			if(allowAny || allowInvokeCallable(userData, method.get())) {
				methodList->add(method.get(), instance);
			}
		}
		else {
			if(mapItem->getType() == mmitMethodList) {
				GScopedInterface<IMetaList> newMethodList(gdynamic_cast<IMetaList *>(mapItem->getItem()));
				for(uint32_t i = 0; i < newMethodList->getCount(); ++i) {
					GScopedInterface<IMetaItem> item(newMethodList->getAt(i));
					if(allowAny || allowInvokeCallable(userData, gdynamic_cast<IMetaMethod *>(item.get()))) {
						methodList->add(item.get(), instance);
					}
				}
			}
		}
		
		GScopedInterface<IMetaClass> metaClass(traveller->next(&instance));
		if(!metaClass) {
			break;
		}
		mapItem = findMetaMapItem(metaMap, metaClass.get(), methodName);
	}
}

void loadMethodList(GMetaClassTraveller * traveller,
	IMetaList * methodList, GMetaMap * metaMap, GMetaMapItem * mapItem,
	void * instance, GClassUserData * userData, const char * methodName)
{
	loadMethodList(traveller, methodList, metaMap, mapItem, instance, userData, methodName, false);
}

void loadMethodList(IMetaList * methodList, GMetaMap * metaMap, IMetaClass * objectMetaClass,
	void * objectInstance, GClassUserData * userData, const char * methodName)
{
	GMetaClassTraveller traveller(objectMetaClass, objectInstance);
	void * instance = NULL;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = metaMap->findClassMap(metaClass.get());
		if(mapClass == NULL) {
			continue;
		}
		GMetaMapItem * mapItem = mapClass->findItem(methodName);
		if(mapItem == NULL) {
			continue;
		}

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty:
			   return;

			case mmitMethod:
			case mmitMethodList: {
				loadMethodList(&traveller, methodList, metaMap, mapItem, instance, userData, methodName);
				return;
			}

			case mmitEnum:
			case mmitEnumValue:
			case mmitClass:
				return;

			default:
				break;
		}
	}
}

GScriptDataType methodTypeToUserDataType(GUserDataMethodType methodType)
{
	switch(methodType) {
		case udmtMethod:
			return sdtMethod;

		case udmtMethodList:
			return sdtMethodList;

		case udmtInternal:
			return sdtScriptMethod;

		default:
			return sdtUnknown;
	}
}

GMetaVariant userDataToVariant(GScriptUserData * userData)
{
	switch(userData->getType()) {
		case udtClass: {
			GClassUserData * classData = static_cast<GClassUserData *>(userData);
			GMetaTypeData typeData;
			classData->data->metaClass->getMetaType(&typeData);
			metaCheckError(classData->data->metaClass);
			GMetaType type(typeData);
			type.addPointer();
			switch(classData->data->dataType) {
				case cudtNormal: {
					return GMetaVariant(pointerToObjectVariant(classData->getInstance()), type);
				}
				
				case cudtInterface: {
					return GMetaVariant(GVariant(classData->data->interfaceObject), type);
				}
			}

			break;
		}

		case udtRaw: {
			GRawUserData * rawData = static_cast<GRawUserData *>(userData);
			return GMetaVariant(rawData->data, GMetaType());
		}

		default:
			break;
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


} // namespace bind_internal

} // namespace cpgf



