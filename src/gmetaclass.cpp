#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"
#include "gmetatypereg.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>


#include <stdarg.h>
#include <stdio.h>


#if defined(_MSC_VER)
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif
#endif



namespace cpgf {

namespace meta_internal {

class GMetaSuperListImplement
{
public:
	typedef std::vector<GMetaSuperListItem> ListType;
	
public:
	ListType superList;
};


GMetaSuperList::GMetaSuperList() : implement(new GMetaSuperListImplement)
{
}

GMetaSuperList::~GMetaSuperList()
{
	for(GMetaSuperListImplement::ListType::iterator it = this->implement->superList.begin(); it != this->implement->superList.end(); ++it) {
		it->freeList();
	}
}

size_t GMetaSuperList::getCount() const
{
	return this->implement->superList.size();
}

const GMetaClass * GMetaSuperList::getSuper(size_t index) const
{
	return this->implement->superList[index].getBaseClass();
}

const GMetaClassCasterBase * GMetaSuperList::getCaster(size_t index) const
{
	return this->implement->superList[index].getCaster();
}

void GMetaSuperList::doAdd(const GMetaSuperListItem & item)
{
	this->implement->superList.push_back(item);
}


class GMetaItemListImplement
{
public:
	typedef std::vector<GMetaItem *> ListType;
	typedef std::multimap<const std::string, GMetaItem *> MapType;
	typedef std::map<GMetaItem *, bool> PointerMap;

public:
	ListType itemList;
	MapType itemMap;
	PointerMap addressMap;
};



} // namespace meta_internal

class GMetaInternalItemList {

public:
	GMetaInternalItemList();
	~GMetaInternalItemList();

	void setClearOnFree(bool clearOnFree);

	void addItem(GMetaItem * item);

	size_t getCount() const;

	GMetaItem * getItemAt(size_t index) const;
	GMetaItem * getItemByName(const char * name) const;

	size_t getItemListByName(GMetaList * metaList, const char * name, unsigned int filters, void * instance) const;

private:
	void clearList();

private:
	GScopedPointer<meta_internal::GMetaItemListImplement> implement;
	bool clearOnFree;
};


template<typename MetaType>
class GMetaInternalList : public GMetaInternalItemList {
private:
	typedef GMetaInternalItemList super;

public:
	GMetaInternalList() : super() {
	}

	~GMetaInternalList() {
	}

	void add(MetaType * item) {
		super::addItem(static_cast<GMetaItem *>(item));
	}

	MetaType * getAt(size_t index) const {
		return static_cast<MetaType *>(super::getItemAt(index));
	}

	MetaType * getByName(const char * name) const {
		return static_cast<MetaType *>(super::getItemByName(name));
	}
};


GMetaInternalItemList::GMetaInternalItemList() : implement(new meta_internal::GMetaItemListImplement), clearOnFree(true)
{
}

GMetaInternalItemList::~GMetaInternalItemList()
{
	this->clearList();
}

void GMetaInternalItemList::setClearOnFree(bool clearOnFree)
{
	this->clearOnFree = clearOnFree;
}

void GMetaInternalItemList::addItem(GMetaItem * item)
{
	if(this->implement->addressMap.find(item) != this->implement->addressMap.end()) {
		return;
	}

	this->implement->itemList.push_back(item);
	this->implement->itemMap.insert(std::make_pair(item->getName(), item));
	this->implement->addressMap.insert(std::make_pair(item, true));
}

size_t GMetaInternalItemList::getCount() const
{
	return this->implement->itemList.size();
}

GMetaItem * GMetaInternalItemList::getItemAt(size_t index) const
{
	if(index >= this->getCount()) {
		return NULL;
	}
	else {
		return this->implement->itemList[index];
	}
}

GMetaItem * GMetaInternalItemList::getItemByName(const char * name) const
{
	meta_internal::GMetaItemListImplement::MapType::const_iterator it = this->implement->itemMap.find(std::string(name));

	if(it == this->implement->itemMap.end()) {
		return NULL;
	}
	else {
		return it->second;
	}
}

size_t GMetaInternalItemList::getItemListByName(GMetaList * metaList, const char * name, unsigned int filters, void * instance) const
{
	size_t count = 0;

	for(meta_internal::GMetaItemListImplement::ListType::const_iterator it = this->implement->itemList.begin(); it != this->implement->itemList.end(); ++it) {
		if(strcmp((*it)->getName().c_str(), name) == 0) {
			if((filters & MetaFilterInstance) != 0 && !(*it)->isStatic()
				|| (filters & MetaFilterStatic) != 0 && (*it)->isStatic()) {
				metaList->add(*it, instance);
				++count;
			}
		}
	}

	return count;
}

void GMetaInternalItemList::clearList()
{
	if(this->clearOnFree) {
		for(meta_internal::GMetaItemListImplement::ListType::iterator it = this->implement->itemList.begin(); it != this->implement->itemList.end(); ++it) {
			if(!(*it)->hasModifier(metaModifierNoFree)) {
				delete *it;
			}
		}
	}
}


class GMetaClassImplement
{
public:
	GMetaClassImplement() {}
	~GMetaClassImplement() {}

public:
	GMetaInternalItemList * itemLists[mcatCount];

	GMetaInternalList<GMetaField> fieldList;
	GMetaInternalList<GMetaProperty> propertyList;
	GMetaInternalList<GMetaMethod> methodList;
	GMetaInternalList<GMetaConstructor> constructorList;
	GMetaInternalList<GMetaOperator> operatorList;
	GMetaInternalList<GMetaEnum> enumList;
	GMetaInternalList<GMetaClass> classList;

	GMetaInternalList<GMetaAnnotation> annotationList;

	GMetaInternalItemList metaList;

};

GMetaClass::~GMetaClass()
{
	delete this->implement;
}

void GMetaClass::intializeImplement()
{
	this->implement = new GMetaClassImplement;
	this->implement->metaList.setClearOnFree(false);
}

void * GMetaClass::createInstance() const
{
	return this->baseData->createInstance();
}

void * GMetaClass::createInplace(void * placement) const
{
	return this->baseData->createInplace(placement);
}

void * GMetaClass::cloneInstance(void * instance) const
{
	return this->baseData->cloneInstance(instance);
}

void * GMetaClass::cloneInplace(void * instance, void * placement) const
{
	return this->baseData->cloneInplace(instance, placement);
}

size_t GMetaClass::getTypeSize() const
{
	return this->baseData->getObjectSize();
}

const GMetaConstructor * GMetaClass::getConstructorByParamCount(unsigned int paramCount) const
{
	this->ensureRegistered();

	size_t count = this->implement->constructorList.getCount();

	for(unsigned int i = 0; i < count; ++i) {
		const GMetaConstructor * c = this->implement->constructorList.getAt(i);
		if(c->getParamCount() == paramCount) {
			return c;
		}
	}

	return NULL;
}

size_t GMetaClass::getConstructorCount() const
{
	return this->getItemCount(mcatConstructor);
}

const GMetaConstructor * GMetaClass::getConstructorAt(size_t index) const
{
	return static_cast<const GMetaConstructor *>(this->getItemAt(mcatConstructor, index));
}

void GMetaClass::destroyInstance(void * instance) const {
	this->destroy(instance);
}

void GMetaClass::addField(GMetaField * field) {
	this->addItem(mcatField, field);
}

const GMetaField * GMetaClass::getFieldInHierarchy(const char * name, void ** outInstance) const
{
	return static_cast<const GMetaField *>(this->getItemByName(mcatField, name, true, outInstance));
}

const GMetaField * GMetaClass::getField(const char * name) const
{
	return static_cast<const GMetaField *>(this->getItemByName(mcatField, name, false, NULL));
}

size_t GMetaClass::getFieldCount() const
{
	return this->getItemCount(mcatField);
}

const GMetaField * GMetaClass::getFieldAt(size_t index) const
{
	return static_cast<const GMetaField *>(this->getItemAt(mcatField, index));
}

void GMetaClass::addProperty(GMetaProperty * property) {
	this->addItem(mcatProperty, property);
}

const GMetaProperty * GMetaClass::getPropertyInHierarchy(const char * name, void ** outInstance) const
{
	return static_cast<const GMetaProperty *>(this->getItemByName(mcatProperty, name, true, outInstance));
}

const GMetaProperty * GMetaClass::getProperty(const char * name) const
{
	return static_cast<const GMetaProperty *>(this->getItemByName(mcatProperty, name, false, NULL));
}

size_t GMetaClass::getPropertyCount() const
{
	return this->getItemCount(mcatProperty);
}

const GMetaProperty * GMetaClass::getPropertyAt(size_t index) const
{
	return static_cast<const GMetaProperty *>(this->getItemAt(mcatProperty, index));
}

void GMetaClass::addMethod(GMetaMethod * method) {
	this->addItem(mcatMethod, method);
}

const GMetaMethod * GMetaClass::getMethodInHierarchy(const char * name, void ** outInstance) const
{
	return static_cast<const GMetaMethod *>(this->getItemByName(mcatMethod, name, true, outInstance));
}

const GMetaMethod * GMetaClass::getMethod(const char * name) const
{
	return static_cast<const GMetaMethod *>(this->getItemByName(mcatMethod, name, false, NULL));
}

size_t GMetaClass::getMethodCount() const
{
	return this->getItemCount(mcatMethod);
}

const GMetaMethod * GMetaClass::getMethodAt(size_t index) const
{
	return static_cast<const GMetaMethod *>(this->getItemAt(mcatMethod, index));
}

size_t GMetaClass::getMethodList(GMetaList * metaList, const char * name, unsigned int filters) const
{
	return this->getItemListByName(metaList, mcatMethod, name, false, filters, NULL);
}

size_t GMetaClass::getMethodListInHierarchy(GMetaList * metaList, const char * name, unsigned int filters, void * instance) const
{
	return this->getItemListByName(metaList, mcatMethod, name, true, filters, instance);
}

void GMetaClass::addOperator(GMetaOperator * metaOperator)
{
	this->addItem(mcatOperator, metaOperator);
}

const GMetaOperator * GMetaClass::doGetOperator(GMetaOpType op, bool findSuper, void ** outInstance) const
{
	this->ensureRegistered();

	const GMetaOperator * result = NULL;
	
	for(size_t i = 0; i < this->implement->operatorList.getCount(); ++i) {
		if(this->implement->operatorList.getAt(i)->getOperator() == op) {
			result = this->implement->operatorList.getAt(i);
			break;
		}
	}

	if(!result && findSuper && this->superList) {
		void ** self = outInstance;
		for(unsigned int i = 0; i < this->getBaseCount(); ++i) {
			if(self != NULL) {
				*outInstance = this->castToBase(*self, i);
			}
			result = this->getBaseClass(i)->doGetOperator(op, true, outInstance);

			if(result) {
				break;
			}
		}
	}

	return result;
}

const GMetaOperator * GMetaClass::getOperatorInHierarchy(GMetaOpType op, void ** outInstance) const
{
	return this->doGetOperator(op, true, outInstance);
}

const GMetaOperator * GMetaClass::getOperator(GMetaOpType op) const
{
	return this->doGetOperator(op, false, NULL);
}

size_t GMetaClass::getOperatorCount() const
{
	return this->getItemCount(mcatOperator);
}

const GMetaOperator * GMetaClass::getOperatorAt(size_t index) const
{
	return static_cast<const GMetaOperator *>(this->getItemAt(mcatOperator, index));
}

GMetaEnum & GMetaClass::addEnum(GMetaEnum * en) {
	this->addItem(mcatEnum, en);
	
	meta_internal::registerMetaTypedItem(en);
	
	return *en;
}

const GMetaEnum * GMetaClass::getEnumInHierarchy(const char * name, void ** outInstance) const
{
	return static_cast<const GMetaEnum *>(this->getItemByName(mcatEnum, name, true, outInstance));
}

const GMetaEnum * GMetaClass::getEnum(const char * name) const
{
	return static_cast<const GMetaEnum *>(this->getItemByName(mcatEnum, name, false, NULL));
}

size_t GMetaClass::getEnumCount() const
{
	return this->getItemCount(mcatEnum);
}

const GMetaEnum * GMetaClass::getEnumAt(size_t index) const
{
	return static_cast<const GMetaEnum *>(this->getItemAt(mcatEnum, index));
}

void GMetaClass::addClass(const GMetaClass * cls)
{
	this->addItem(mcatClass, const_cast<GMetaClass *>(cls));
	
	meta_internal::registerMetaTypedItem(cls);
}

const GMetaClass * GMetaClass::getClassInHierarchy(const char * name, void ** outInstance) const
{
	return static_cast<const GMetaClass *>(this->getItemByName(mcatClass, name, true, outInstance));
}

const GMetaClass * GMetaClass::getClass(const char * name) const
{
	return static_cast<const GMetaClass *>(this->getItemByName(mcatClass, name, false, NULL));
}

size_t GMetaClass::getClassCount() const
{
	return this->getItemCount(mcatClass);
}

const GMetaClass * GMetaClass::getClassAt(size_t index) const
{
	return static_cast<const GMetaClass *>(this->getItemAt(mcatClass, index));
}

GMetaAnnotation & GMetaClass::addAnnotation(GMetaAnnotation * annotation)
{
	this->addItem(mcatAnnotation, annotation);

	return *annotation;
}


size_t GMetaClass::getMetaCount() const
{
	this->ensureRegistered();
	
	return this->implement->metaList.getCount();
}

const GMetaItem * GMetaClass::getMetaAt(size_t index) const
{
	this->ensureRegistered();

	return this->implement->metaList.getItemAt(index);
}


bool GMetaClass::isInheritedFrom(const GMetaClass * ancient) const
{
	for(unsigned int i = 0; i < this->getBaseCount(); ++i) {
		const GMetaClass * superClass = this->getBaseClass(i);

		while(superClass != NULL) {
			if(superClass->isSameClass(ancient)) {
				return true;
			}

			superClass = superClass->getBaseClass(i);
		}
	}

	return false;
}

void * GMetaClass::castFromBase(void * base, size_t baseIndex) const
{
	const GMetaClass * baseClass = this->getBaseClass(baseIndex);

	if(baseClass == NULL) {
		return base;
	}

	return this->superList->getCaster(baseIndex)->downCast(base);
}

void * GMetaClass::castToBase(void * self, size_t baseIndex) const
{
	const GMetaClass * baseClass = this->getBaseClass(baseIndex);

	if(baseClass == NULL) {
		return self;
	}

	return this->superList->getCaster(baseIndex)->upCast(self);
}

void GMetaClass::rebindName(const char * name)
{
	this->setName(name);
}

void GMetaClass::ensureRegistered() const
{
	if(! this->intialized) {
		if(this->metaRegister) {
			this->metaRegister();
		}

		this->intialized = true;
	}
}

void GMetaClass::setupItemLists()
{
	this->implement->itemLists[mcatField] = &this->implement->fieldList;
	this->implement->itemLists[mcatProperty] = &this->implement->propertyList;
	this->implement->itemLists[mcatMethod] = &this->implement->methodList;
	this->implement->itemLists[mcatConstructor] = &this->implement->constructorList;
	this->implement->itemLists[mcatOperator] = &this->implement->operatorList;
	this->implement->itemLists[mcatEnum] = &this->implement->enumList;
	this->implement->itemLists[mcatClass] = &this->implement->classList;
	this->implement->itemLists[mcatAnnotation] = &this->implement->annotationList;
}

void GMetaClass::itemAdded(GMetaItem * metaItem)
{
	this->refreshAnnotation(metaItem);

	if(!metaItem->isAnnotation()) {
		this->previousAddedItem = metaItem;
	}
}

void GMetaClass::refreshAnnotation(GMetaItem * metaItem)
{
	if(metaItem->isAnnotation()) {
		return;
	}

	size_t annotationCount = this->implement->annotationList.getCount();
	while(annotationCount > 0) {
		--annotationCount;

		GMetaAnnotation * annotation = this->implement->annotationList.getAt(annotationCount);
		if(annotation->getMetaItem() != NULL) {
			break;
		}

		annotation->setMetaItem(metaItem);
		metaItem->addItemAnnotation(annotation);
	}
}

void GMetaClass::flushAnnotation()
{
	this->refreshAnnotation(this->previousAddedItem == NULL ? this : this->previousAddedItem);

	this->previousAddedItem = NULL;
}

void GMetaClass::addItem(GMetaCategory listIndex, GMetaItem * item)
{
	this->implement->itemLists[listIndex]->addItem(item);
	
	this->implement->metaList.addItem(item);

	item->ownerItem = this;

	this->itemAdded(item);
}

size_t GMetaClass::getItemCount(GMetaCategory listIndex) const
{
	this->ensureRegistered();

	return this->implement->itemLists[listIndex]->getCount();
}

const GMetaItem * GMetaClass::getItemAt(GMetaCategory listIndex, size_t index) const
{
	this->ensureRegistered();

	return this->implement->itemLists[listIndex]->getItemAt(index);
}

const GMetaItem * GMetaClass::getItemByName(GMetaCategory listIndex, const char * name, bool findSuper, void ** outInstance) const
{
	this->ensureRegistered();

	GMetaInternalItemList * itemList = this->implement->itemLists[listIndex];

	const GMetaItem * result = itemList->getItemByName(name);

	if(!result && findSuper && this->superList) {
		void ** self = outInstance;
		for(size_t i = 0; i < this->getBaseCount(); ++i) {
			if(self != NULL) {
				*outInstance = this->castToBase(*self, i);
			}
			result = this->getBaseClass(i)->getItemByName(listIndex, name, true, outInstance);

			if(result) {
				break;
			}
		}
	}

	return result;
}

size_t GMetaClass::getItemListByName(GMetaList * metaList, GMetaCategory listIndex, const char * name, bool findSuper, unsigned int filters, void * instance) const
{
	this->ensureRegistered();

	GMetaInternalItemList * itemList = this->implement->itemLists[listIndex];
	size_t count = itemList->getItemListByName(metaList, name, filters, instance);

	if(findSuper && this->superList) {
		void * self = instance;
		for(size_t i = 0; i < this->getBaseCount(); ++i) {
			if(self != NULL) {
				instance = this->castToBase(self, i);
			}
			count += this->getBaseClass(i)->getItemListByName(metaList, listIndex, name, true, filters, instance);
		}
	}

	return count;
}

GMetaClass * getGlobalMetaClass()
{
	static GMetaClass metaClass((void *)0, new meta_internal::GMetaSuperList, "", NULL, NULL, GMetaPolicyDefault());

	return &metaClass;
}

const GMetaClass * findMetaClass(const GMetaType & type)
{
	return meta_internal::findRegisteredMetaClass(type);
}

const GMetaClass * findMetaClass(const char * name)
{
	return meta_internal::findRegisteredMetaClass(name);
}

void globalAddField(GMetaField * field)
{
	getGlobalMetaClass()->addField(field);
}

void globalAddProperty(GMetaProperty * field)
{
	getGlobalMetaClass()->addProperty(field);
}

void globalAddMethod(GMetaMethod * method)
{
	getGlobalMetaClass()->addMethod(method);
}

void globalAddOperator(GMetaOperator * metaOperator)
{
	getGlobalMetaClass()->addOperator(metaOperator);
}

GMetaEnum & globalAddEnum(GMetaEnum * en)
{
	return getGlobalMetaClass()->addEnum(en);
}

void globalAddClass(const GMetaClass * cls)
{
	getGlobalMetaClass()->addClass(cls);
}

GMetaAnnotation & globaAddAnnotation(GMetaAnnotation * annotation)
{
	return getGlobalMetaClass()->addAnnotation(annotation);
}

void globalFlushAnnotation()
{
	getGlobalMetaClass()->flushAnnotation();
}


} // namespace cpgf
