#include "../pinclude/gbindcommon.h"

namespace cpgf {


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
			const char * keyName = metaEnum->getName();
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
				metaList->add(static_cast<IMetaItem *>(item.getItem()), NULL);
				metaList->add(method.get(), NULL);
				this->itemMap[name] = GMetaMapItem(metaList.get());
			}
			else {
				GASSERT(item.getType() == mmitMethodList);
				
				static_cast<IMetaList *>(item.getItem())->add(method.get(), NULL);
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

int rankCallable(IMetaService * service, IMetaCallable * callable, GVariantData * paramsData, GBindDataType * paramsType, size_t paramCount)
{
	if(!! callable->isVariadic()) {
		return 0;
	}

	if(callable->getParamCount() != paramCount) {
		return -1;
	}

	for(uint32_t i = 0; i < paramCount; ++i) {
		if(! callable->checkParam(&paramsData[i], i)) {
			return -1;
		}
	}

	int rank = 1;
	
	const int RankEqual = 10;
	const int RankConvert = 5;

	for(size_t i = 0; i < paramCount; ++i) {
		GMetaType proto = metaGetParamType(callable, i);
		GScriptDataType sdt = paramsType[i].dataType;
		
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

		if(! paramsType[i].typeItem) {
			continue;
		}

		if(metaIsClass(paramsType[i].typeItem->getCategory())) {
			GScopedInterface<IMetaTypedItem> protoType(service->findTypedItemByName(proto.getBaseName()));
			if(! protoType || ! metaIsClass(protoType->getCategory())) {
				continue;
			}

			IMetaClass * paramClass = static_cast<IMetaClass *>(paramsType[i].typeItem.get());
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

bool checkCallable(IMetaCallable * callable, GVariantData * paramsData, size_t paramCount)
{
	if(!! callable->isVariadic()) {
		return true;
	}

	if(callable->getParamCount() != paramCount) {
		return false;
	}

	for(uint32_t i = 0; i < paramCount; ++i) {
		if(!callable->checkParam(&paramsData[i], i)) {
			return false;
		}
	}

	return true;
}



} // namespace cpgf



