#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"

#include "pinclude/gmetatypereg.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>


#include <stdarg.h>
#include <stdio.h>


namespace cpgf {

namespace meta_internal {

namespace {
	typedef std::vector<GMetaClass *> MetaDefineClassList;
	MetaDefineClassList * metaDefineClassList = NULL;
} // unnamed namespace

void metaDefineAddMetaClass(GMetaClass * metaClass)
{
	if(metaDefineClassList == NULL) {
		metaDefineClassList = new MetaDefineClassList;
	}

	metaDefineClassList->push_back(metaClass);
}

bool metaDefineRemoveMetaClass(GMetaClass * metaClass)
{
	if(metaDefineClassList != NULL) {
		for(MetaDefineClassList::iterator it = metaDefineClassList->begin(); it != metaDefineClassList->end(); ++it) {
			if(*it == metaClass) {
				metaDefineClassList->erase(it);
				if(metaDefineClassList->empty()) {
					delete metaDefineClassList;
					metaDefineClassList = NULL;
				}

				return true;
			}
		}
	}

	return false;
}



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
	if(index >= this->getCount()) {
		return NULL;
	}
	else {
		return this->implement->superList[index].getBaseClass();
	}
}

const GMetaClassCasterBase * GMetaSuperList::getCaster(size_t index) const
{
	if(index >= this->getCount()) {
		return NULL;
	}
	else {
		return this->implement->superList[index].getCaster();
	}
}

void GMetaSuperList::doAdd(const GMetaSuperListItem & item)
{
	this->implement->superList.push_back(item);
}


class GMetaItemListImplement
{
public:
	typedef std::vector<GMetaItem *> ListType;
	typedef std::multimap<const char *, GMetaItem *, meta_internal::CStringCompare> MapType;

public:
	ListType itemList;
	MapType itemMap;
};


class GGlobalMetaClassList
{
private:
	typedef std::vector<GMetaClass *> ClassList;

public:
	GGlobalMetaClassList();
	~GGlobalMetaClassList();

	GMetaClass * getDefault();
	GMetaClass * getAt(size_t index);
	size_t getCount();
	GMetaClass * getByName(const char * name);

private:
	GMetaClass * createGlobal();

private:
	ClassList classList;
};

GGlobalMetaClassList::GGlobalMetaClassList()
{
}

GGlobalMetaClassList::~GGlobalMetaClassList()
{
	for(ClassList::iterator it = this->classList.begin(); it != this->classList.end(); ++it) {
		delete *it;
	}
}

GMetaClass * GGlobalMetaClassList::getDefault()
{
	return this->getAt(0);
}

GMetaClass * GGlobalMetaClassList::getAt(size_t index)
{
	while(index >= this->classList.size()) {
		this->classList.push_back(NULL);
	}

	if(this->classList.at(index) == NULL) {
		this->classList[index] = this->createGlobal();
	}

	return this->classList.at(index);
}

size_t GGlobalMetaClassList::getCount()
{
	return this->classList.size();
}

GMetaClass * GGlobalMetaClassList::getByName(const char * name)
{
	std::string sname(name);

	for(ClassList::iterator it = this->classList.begin(); it != this->classList.end(); ++it) {
		if((*it)->getName() == sname) {
			return *it;
		}
	}

	return NULL;
}

GMetaClass * GGlobalMetaClassList::createGlobal()
{
	return new GMetaClass((void *)0, new meta_internal::GMetaSuperList, "", NULL, NULL, GMetaPolicyDefault());
}


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
	GASSERT(metaIsAnnotation(item->getCategory())
		|| metaIsConstructor(item->getCategory())
		|| metaIsMethod(item->getCategory())
		|| metaIsOperator(item->getCategory())
		|| this->getItemByName(item->getName().c_str()) == NULL
	);

	this->implement->itemList.push_back(item);
	this->implement->itemMap.insert(std::make_pair(item->getName().c_str(), item));
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
	meta_internal::GMetaItemListImplement::MapType::const_iterator it = this->implement->itemMap.find(name);

	if(it == this->implement->itemMap.end()) {
		return NULL;
	}
	else {
		return it->second;
	}
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

	GMetaInternalItemList metaList;

};

GMetaClass::~GMetaClass()
{
	delete this->implement;
}

void GMetaClass::initializeImplement()
{
	this->implement = new GMetaClassImplement;
	this->implement->metaList.setClearOnFree(false);
}

void GMetaClass::rebindName(const char * name)
{
	this->setName(name);
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

const GMetaConstructor * GMetaClass::getConstructorByParamCount(size_t paramCount) const
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

GMetaField * GMetaClass::addField(GMetaField * field) {
	this->addItem(mcatField, field);
	return field;
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

GMetaProperty * GMetaClass::addProperty(GMetaProperty * prop) {
	this->addItem(mcatProperty, prop);
	return prop;
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

GMetaMethod * GMetaClass::addMethod(GMetaMethod * method) {
	this->addItem(mcatMethod, method);
	return method;
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

GMetaOperator * GMetaClass::addOperator(GMetaOperator * metaOperator)
{
	this->addItem(mcatOperator, metaOperator);
	return metaOperator;
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

GMetaEnum * GMetaClass::addEnum(GMetaEnum * en) {
	this->addItem(mcatEnum, en);

	meta_internal::registerMetaTypedItem(en);

	return en;
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

GMetaClass * GMetaClass::addClass(const GMetaClass * cls)
{
	this->addItem(mcatClass, const_cast<GMetaClass *>(cls));

	meta_internal::registerMetaTypedItem(cls);

	return const_cast<GMetaClass *>(cls);
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

bool GMetaClass::isGlobal() const
{
	return !this->superList;
}

bool GMetaClass::isAbstract() const
{
	return this->baseData->isAbstract();
}

bool GMetaClass::canCreateInstance() const
{
	return this->baseData->canCreateInstance();
}

bool GMetaClass::canCopyInstance() const
{
	return this->baseData->canCopyInstance();
}

bool GMetaClass::isSameClass(const GMetaClass * other) const
{
	return this == other;
}

const GMetaClass * GMetaClass::getBaseClass(size_t baseIndex) const
{
	return this->superList->getSuper(baseIndex);
}

size_t GMetaClass::getBaseCount() const
{
	return this->superList->getCount();
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

void GMetaClass::ensureRegistered() const
{
	if(! this->intialized) {
		if(this->metaRegister) {
			this->metaRegister(const_cast<GMetaClass *>(this));
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
}

void GMetaClass::addItem(GMetaCategory listIndex, GMetaItem * item)
{
	this->implement->itemLists[listIndex]->addItem(item);

	this->implement->metaList.addItem(item);

	item->ownerItem = this;
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

const GMetaClass * findMetaClass(const GMetaType & type)
{
	return meta_internal::findRegisteredMetaClass(type);
}

const GMetaClass * findMetaClass(const char * name)
{
	return meta_internal::findRegisteredMetaClass(name);
}

meta_internal::GGlobalMetaClassList * getGlobalMetaClassList()
{
	static meta_internal::GGlobalMetaClassList classList;

	return &classList;
}

GMetaClass * getGlobalMetaClass()
{
	return getGlobalMetaClassList()->getDefault();
}

GMetaClass * getGlobalMetaClassAt(size_t index)
{
	return getGlobalMetaClassList()->getAt(index);
}

size_t getGlobalMetaClassCount()
{
	return getGlobalMetaClassList()->getCount();
}

GMetaClass * getGlobalMetaClassByName(const char * name)
{
	return getGlobalMetaClassList()->getByName(name);
}


} // namespace cpgf
