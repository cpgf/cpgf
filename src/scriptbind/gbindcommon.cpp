#include "../pinclude/gbindcommon.h"

#include "cpgf/gmetaclasstraveller.h"

#include <stdarg.h>


namespace cpgf {

namespace bind_internal {

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

int rankCallable(IMetaService * service, IMetaCallable * callable, InvokeCallableParam * callbackParam)
{
	if(!! callable->isVariadic()) {
		return 0;
	}

	if(callable->getParamCount() != callbackParam->paramCount) {
		return -1;
	}

	for(uint32_t i = 0; i < callbackParam->paramCount; ++i) {
		bool ok = !! callable->checkParam(&callbackParam->paramsData[i], i);
		metaCheckError(callable);
		if(! ok) {
			return -1;
		}
	}

	int rank = 1;
	
	const int RankEqual = 10;
	const int RankConvert = 5;

	for(size_t i = 0; i < callbackParam->paramCount; ++i) {
		GMetaType proto = metaGetParamType(callable, i);
		GScriptDataType sdt = callbackParam->paramsType[i].dataType;
		
		if(sdt == sdtNull) {
			rank += RankEqual;
			continue;
		}
		
		if(proto.isFundamental() && sdt == sdtFundamental) {
			rank += RankEqual;
			continue;
		}

		if(proto.getPointerDimension() > 1) {
			continue;
		}

		if(! callbackParam->paramsType[i].typeItem) {
			continue;
		}

		if(metaIsClass(callbackParam->paramsType[i].typeItem->getCategory())) {
			GScopedInterface<IMetaTypedItem> protoType(service->findTypedItemByName(proto.getBaseName()));
			if(! protoType || ! metaIsClass(protoType->getCategory())) {
				continue;
			}

			IMetaClass * paramClass = static_cast<IMetaClass *>(callbackParam->paramsType[i].typeItem.get());
			IMetaClass * protoClass = static_cast<IMetaClass *>(protoType.get());

			if(paramClass->equals(protoClass)) {
				rank += RankEqual;
			}
			else {
				if(paramClass->isInheritedFrom(protoClass)) {
					rank += RankConvert;
				}
			}
			
			continue;
		}
	}

	return rank;
}

bool allowInvokeCallable(GClassUserData * userData, IMetaCallable * method)
{
	if(userData != NULL && userData->isInstance) {
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
		switch(userData->cv) {
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
	if(userData->isInstance) {
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

void doInvokeCallable(void * instance, IMetaCallable * callable, GVariantData * paramsData, size_t paramCount, InvokeCallableResult * result)
{
	result->resultCount = callable->hasResult() ? 1 : 0;
	vtInit(result->resultData.typeData);

	callable->execute(&result->resultData, instance, paramsData, static_cast<uint32_t>(paramCount));
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
			classData->metaClass->getMetaType(&typeData);
			metaCheckError(classData->metaClass);
			GMetaType type(typeData);
			type.addPointer();
			return GMetaVariant(pointerToObjectVariant(classData->instance), type);
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
		outType->addPointer();
		value = address;

		if(instanceIsConst) {
			outType->addConst();
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


} // namespace bind_internal

} // namespace cpgf



