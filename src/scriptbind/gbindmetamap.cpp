#include "gbindmetamap.h"
#include "gbindgluedata.h"

#include "cpgf/gstringutil.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gglobal.h"

namespace cpgf {

namespace bind_internal {

GMetaMapItem::GMetaMapItem()
	: scriptValue(), userData()
{
}

GMetaMapItem::GMetaMapItem(const GScriptValue & scriptValue)
	: scriptValue(scriptValue), userData()
{
}

GMetaMapItem::GMetaMapItem(const GMetaMapItem & other)
	: scriptValue(other.scriptValue)
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

	swap(this->scriptValue, other.scriptValue);
}


GMetaMapClass::GMetaMapClass(IMetaClass * metaClass, void * instance)
{
	this->buildMap(metaClass, instance);
}

GMetaMapClass::~GMetaMapClass()
{
}

GMetaMapItem * GMetaMapClass::findItem(const char * name)
{
	MapType::iterator it = this->itemMap.find(name);
	if(it == this->itemMap.end()) {
		return nullptr;
	}
	else {
		return &this->itemMap[name];
	}
}

void GMetaMapClass::buildMap(IMetaClass * metaClass, void * instance)
{
	using namespace std;

	if(metaClass == nullptr) {
		return;
	}

	uint32_t count;
	uint32_t i;

	count = metaClass->getClassCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaClass> innerClass(metaClass->getClassAt(i));
		const char * name = innerClass->getName();
		this->itemMap[name] = GMetaMapItem(GScriptValue::fromClass(innerClass.get()));
	}

	count = metaClass->getEnumCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaEnum> metaEnum(metaClass->getEnumAt(i));
		const char * name = metaEnum->getName();
		this->itemMap[name] = GMetaMapItem(GScriptValue::fromEnum(metaEnum.get()));

		uint32_t keyCount = metaEnum->getCount();
		for(uint32_t k = 0; k < keyCount; ++k) {
			const char * keyName = metaEnum->getKey(k);
			this->itemMap[keyName] = GMetaMapItem(GScriptValue::fromPrimary(metaGetEnumValue(metaEnum.get(), static_cast<uint32_t>(k))));
		}
	}

	count = metaClass->getFieldCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaField> field(metaClass->getFieldAt(i));
		const char * name = field->getName();
		this->itemMap[name] = GMetaMapItem(GScriptValue::fromAccessible(instance, field.get()));
	}

	count = metaClass->getPropertyCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaProperty> prop(metaClass->getPropertyAt(i));
		const char * name = prop->getName();
		this->itemMap[name] = GMetaMapItem(GScriptValue::fromAccessible(instance, prop.get()));
	}

	std::map<const char *, std::vector<GSharedInterface<IMetaMethod> >, GCStringCompare> methodMap;
	count = metaClass->getMethodCount();
	for(i = 0; i < count; ++i) {
		GScopedInterface<IMetaMethod> method(metaClass->getMethodAt(i));
		const char * name = method->getName();

		methodMap[name].emplace_back(method.get());
	}
	
	for(auto methodPair : methodMap) {
		const char * name = methodPair.first;

		if(methodPair.second.size() == 1) {
			this->itemMap[name] = GMetaMapItem(GScriptValue::fromMethod(instance, methodPair.second.front().get()));
		}
		else {
			GScopedInterface<IMetaList> metaList(createMetaList());
			for(auto i : methodPair.second) {
				metaList->add(i.get(), instance);
			}
			this->itemMap[name] = GMetaMapItem(GScriptValue::fromOverloadedMethods(metaList.get()));
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

GMetaMapClass * GMetaMap::getClassMap(IMetaClass * metaClass)
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


GMapItemMethodData::GMapItemMethodData(const GMethodGlueDataPointer & methodData)
	: methodData(methodData)
{
}

GMapItemMethodData::~GMapItemMethodData()
{
}


} //namespace bind_internal

} //namespace cpgf
