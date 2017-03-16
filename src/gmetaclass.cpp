#include "cpgf/gmetamodule.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafield.h"
#include "cpgf/gmetamethod.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"
#include "cpgf/gstringutil.h"

#include "pinclude/gstaticuninitializerorders.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>


#include <stdarg.h>
#include <stdio.h>


#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8104 //Local Static with constructor dangerous for multi-threaded apps
#endif


namespace cpgf {


GMetaModule * doGetGlobalModule();

namespace meta_internal {


void GMetaClassDataBase::deleteSelf()
{
	this->virtualFunctions->deleteSelf(this);
}

bool GMetaClassDataBase::canCreateInstance() const
{
	return this->virtualFunctions->canCreateInstance();
}

bool GMetaClassDataBase::canCopyInstance() const
{
	return this->virtualFunctions->canCopyInstance();
}

void * GMetaClassDataBase::createInstance() const
{
	return this->virtualFunctions->createInstance(this);
}

void * GMetaClassDataBase::createInplace(void * placement) const
{
	return this->virtualFunctions->createInplace(this, placement);
}

void * GMetaClassDataBase::cloneInstance(const void * instance) const
{
	return this->virtualFunctions->cloneInstance(this, instance);
}

void * GMetaClassDataBase::cloneInplace(const void * instance, void * placement) const
{
	return this->virtualFunctions->cloneInplace(this, instance, placement);
}

void GMetaClassDataBase::destroyInstance(void * instance) const
{
	return this->virtualFunctions->destroyInstance(instance);
}

void GMetaClassDataBase::destroyInplace(void * instance) const
{
	return this->virtualFunctions->destroyInplace(instance);
}

size_t GMetaClassDataBase::getObjectSize() const
{
	return this->virtualFunctions->getObjectSize(this);
}

bool GMetaClassDataBase::isAbstract() const
{
	return this->virtualFunctions->isAbstract(this);
}

bool GMetaClassDataBase::isPolymorphic() const
{
	return this->virtualFunctions->isPolymorphic();
}


GMetaClassCasterBase * GMetaClassCasterBase::clone() const
{
	return this->virtualFunctions->clone();
}

void * GMetaClassCasterBase::downCast(const void * base) const
{
	return this->virtualFunctions->downCast(const_cast<void *>(base));
}

void * GMetaClassCasterBase::upCast(const void * derived) const
{
	return this->virtualFunctions->upCast(const_cast<void *>(derived));
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
		return nullptr;
	}
	else {
		return this->implement->superList[index].getBaseClass();
	}
}

const GMetaClassCasterBase * GMetaSuperList::getCaster(size_t index) const
{
	if(index >= this->getCount()) {
		return nullptr;
	}
	else {
		return this->implement->superList[index].getCaster();
	}
}

bool GMetaSuperList::hasAddedDerived(size_t index) const
{
	if(index >= this->getCount()) {
		return false;
	}
	else {
		return this->implement->superList[index].hasAddedDerived();
	}
}

void GMetaSuperList::setAddedDerived(size_t index, bool addedDerived)
{
	if(index < this->getCount()) {
		this->implement->superList[index].setAddedDerived(addedDerived);
	}
}

GMetaSuperListItem * GMetaSuperList::doAdd(const GMetaSuperListItem & item)
{
	this->implement->superList.push_back(item);
	return &this->implement->superList.back();
}


class GMetaItemListImplement
{
public:
	typedef std::vector<GMetaItem *> ListType;
	typedef std::multimap<const char *, GMetaItem *, GCStringCompare> MapType;

public:
	void clear() {
		this->itemList.clear();
		this->itemMap.clear();
	}

public:
	ListType itemList;
	MapType itemMap;
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

	void extractTo(GMetaInternalItemList * master);

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
		|| this->getItemByName(item->getName().c_str()) == nullptr
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
		return nullptr;
	}
	else {
		return this->implement->itemList[index];
	}
}

GMetaItem * GMetaInternalItemList::getItemByName(const char * name) const
{
	meta_internal::GMetaItemListImplement::MapType::const_iterator it = this->implement->itemMap.find(name);

	if(it == this->implement->itemMap.end()) {
		return nullptr;
	}
	else {
		return it->second;
	}
}

void GMetaInternalItemList::extractTo(GMetaInternalItemList * master)
{
	for(meta_internal::GMetaItemListImplement::ListType::iterator it = this->implement->itemList.begin(); it != this->implement->itemList.end(); ++it) {
		master->addItem(*it);
	}
	this->implement->clear();
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
	this->implement->clear();
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
	this->setModule(nullptr);

	delete this->implement;
}

void GMetaClass::initialize()
{
	this->addModifier(metaModifierStatic);

	this->module = nullptr;

	this->implement = new GMetaClassImplement;
	this->implement->metaList.setClearOnFree(false);

	this->setupItemLists();

	this->setModule(doGetGlobalModule());

	this->fixupHierarchy();
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

void * GMetaClass::cloneInstance(const void * instance) const
{
	return this->baseData->cloneInstance(instance);
}

void * GMetaClass::cloneInplace(const void * instance, void * placement) const
{
	return this->baseData->cloneInplace(instance, placement);
}

void GMetaClass::destroyInstance(void * instance) const {
	this->baseData->destroyInstance(instance);
}

void GMetaClass::destroyInplace(void * instance) const {
	this->baseData->destroyInplace(instance);
}

size_t GMetaClass::getTypeSize() const
{
	return this->baseData->getObjectSize();
}

GMetaExtendType GMetaClass::getItemExtendType(uint32_t flags) const
{
	return this->baseData->getItemExtendType(flags, this);
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

	return nullptr;
}

size_t GMetaClass::getConstructorCount() const
{
	return this->getItemCount(mcatConstructor);
}

const GMetaConstructor * GMetaClass::getConstructorAt(size_t index) const
{
	return static_cast<const GMetaConstructor *>(this->getItemAt(mcatConstructor, index));
}

void GMetaClass::addDerivedClass(const GMetaClass * derived)
{
	if(! this->derivedList) {
		this->derivedList.reset(new DerivedListType);
	}
	this->derivedList->push_back(derived);
}

GMetaConstructor * GMetaClass::addConstructor(GMetaConstructor * constructor)
{
	this->addItem(mcatConstructor, constructor);
	return constructor;
}

GMetaField * GMetaClass::addField(GMetaField * field)
{
	this->addItem(mcatField, field);
	return field;
}

const GMetaField * GMetaClass::getFieldInHierarchy(const char * name, void ** outInstance) const
{
	return static_cast<const GMetaField *>(this->getItemByName(mcatField, name, true, outInstance));
}

const GMetaField * GMetaClass::getField(const char * name) const
{
	return static_cast<const GMetaField *>(this->getItemByName(mcatField, name, false, nullptr));
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
	return static_cast<const GMetaProperty *>(this->getItemByName(mcatProperty, name, false, nullptr));
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
	return static_cast<const GMetaMethod *>(this->getItemByName(mcatMethod, name, false, nullptr));
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

	const GMetaOperator * result = nullptr;

	for(size_t i = 0; i < this->implement->operatorList.getCount(); ++i) {
		if(this->implement->operatorList.getAt(i)->getOperator() == op) {
			result = this->implement->operatorList.getAt(i);
			break;
		}
	}

	if(!result && findSuper && this->superList) {
		void ** self = outInstance;
		for(unsigned int i = 0; i < this->getBaseCount(); ++i) {
			if(self != nullptr) {
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
	return this->doGetOperator(op, false, nullptr);
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

	return en;
}

const GMetaEnum * GMetaClass::getEnumInHierarchy(const char * name, void ** outInstance) const
{
	return static_cast<const GMetaEnum *>(this->getItemByName(mcatEnum, name, true, outInstance));
}

const GMetaEnum * GMetaClass::getEnum(const char * name) const
{
	return static_cast<const GMetaEnum *>(this->getItemByName(mcatEnum, name, false, nullptr));
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

	if (cls->module == this->module) {
		// will re-register with different name
		cls->setModule(nullptr);
		cls->resetQualifiedName();
	}
	cls->setModule(this->module);

	return const_cast<GMetaClass *>(cls);
}

void GMetaClass::extractTo(GMetaClass * master)
{
	for(int i = 0; i < static_cast<int>(mcatCount); ++i) {
		GMetaCategory c = static_cast<GMetaCategory>(i);
		if(this->implement->itemLists[c] != nullptr) {
			this->implement->itemLists[c]->extractTo(master->implement->itemLists[c]);
		}
	}
	for(size_t i = 0; i < this->implement->metaList.getCount(); ++i) {
		GMetaItem * item = this->implement->metaList.getItemAt(i);
		if(item != nullptr) {
			item->ownerItem = master;
		}
	}
	this->implement->metaList.extractTo(&master->implement->metaList);
}

const GMetaClass * GMetaClass::getClassInHierarchy(const char * name, void ** outInstance) const
{
	return static_cast<const GMetaClass *>(this->getItemByName(mcatClass, name, true, outInstance));
}

const GMetaClass * GMetaClass::getClass(const char * name) const
{
	return static_cast<const GMetaClass *>(this->getItemByName(mcatClass, name, false, nullptr));
}

const GMetaClass * GMetaClass::doGetClass(const char * name) const
{
	return static_cast<const GMetaClass *>(this->doGetItemByName(mcatClass, name, false, nullptr));
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

void GMetaClass::setModule(GMetaModule * module) const
{
	if(this->module != module) {
		if(this->module != nullptr) {
			this->module->unregisterMetaClass(this);
		}

		this->module = module;

		if(this->module != nullptr) {
			this->module->registerMetaClass(this);
		}
	}

	size_t count;
	size_t i;

	count = this->doGetItemCount(mcatClass);
	for(i = 0; i < count; ++i) {
		static_cast<const GMetaClass *>(this->doGetItemAt(mcatClass, i))->setModule(module);
	}
}

GMetaModule * GMetaClass::getModule() const
{
	if(this->module == nullptr) {
		this->setModule(doGetGlobalModule());
	}

	return this->module;
}

bool GMetaClass::isGlobal() const
{
	// Even a meta class doesn't has any parent class, we require an empty superList.
	// Only global class has no superList.
	return !this->superList;
}

bool GMetaClass::isAbstract() const
{
	return this->baseData->isAbstract();
}

bool GMetaClass::isPolymorphic() const
{
	return this->baseData->isPolymorphic();
}

bool GMetaClass::canCreateInstance() const
{
	return this->baseData->canCreateInstance();
}

bool GMetaClass::canCopyInstance() const
{
	return this->baseData->canCopyInstance();
}

bool GMetaClass::equals(const GMetaClass * other) const
{
	return this == other;
}

const GMetaClass * GMetaClass::getBaseClass(size_t baseIndex) const
{
	this->ensureRegistered();

	return const_cast<GMetaClass *>(this->superList->getSuper(baseIndex));
}

size_t GMetaClass::getBaseCount() const
{
	this->ensureRegistered();

	return this->superList->getCount();
}

const GMetaClass * GMetaClass::getDerivedClass(size_t derivedIndex) const
{
	this->ensureRegistered();

	if(this->derivedList) {
		return this->derivedList->at(derivedIndex);
	}
	else {
		return nullptr;
	}
}

size_t GMetaClass::getDerivedCount() const
{
	this->ensureRegistered();

	if(this->derivedList) {
		return this->derivedList->size();
	}
	else {
		return 0;
	}
}

bool GMetaClass::fixupHierarchy()
{
	bool didIt = false;

	if(this->superList && this->superList->getCount() > 0) {
		size_t baseCount = this->superList->getCount();
		for(size_t i = 0; i < baseCount; ++i) {
			GMetaClass * baseClass = const_cast<GMetaClass *>(this->superList->getSuper(i));
			if(baseClass != nullptr && ! this->superList->hasAddedDerived(i)) {
				baseClass->addDerivedClass(this);
				this->superList->setAddedDerived(i, true);

				didIt = true;
			}
		}
	}

	return didIt;
}

bool GMetaClass::isInheritedFrom(const GMetaClass * ancient) const
{
	for(unsigned int i = 0; i < this->getBaseCount(); ++i) {
		const GMetaClass * superClass = this->getBaseClass(i);

		if(superClass != nullptr) {
			if(superClass->equals(ancient)) {
				return true;
			}

			if(superClass->isInheritedFrom(ancient)) {
				return true;
			}
		}
	}

	return false;
}

void * GMetaClass::castFromBase(const void * base, size_t baseIndex) const
{
	const GMetaClass * baseClass = this->getBaseClass(baseIndex);

	if(baseClass == nullptr) {
		return const_cast<void *>(base);
	}

	return this->superList->getCaster(baseIndex)->downCast(base);
}

void * GMetaClass::castToBase(const void * self, size_t baseIndex) const
{
	const GMetaClass * baseClass = this->getBaseClass(baseIndex);

	if(baseClass == nullptr) {
		return const_cast<void *>(self);
	}

	return this->superList->getCaster(baseIndex)->upCast(self);
}

void * GMetaClass::castFromDerived(const void * derived, size_t derivedIndex) const
{
	const GMetaClass * derivedClass = this->getDerivedClass(derivedIndex);

	if(derivedClass == nullptr) {
		return const_cast<void *>(derived);
	}

	size_t derivedBaseCount = derivedClass->getBaseCount();
	for(size_t i = 0; i < derivedBaseCount; ++i) {
		const GMetaClass * baseClass = derivedClass->getBaseClass(i);
		if(baseClass != nullptr && baseClass->equals(this)) {
			return derivedClass->castToBase(derived, i);
		}
	}

	return const_cast<void *>(derived);
}

void * GMetaClass::castToDerived(const void * self, size_t derivedIndex) const
{
	const GMetaClass * derivedClass = this->getDerivedClass(derivedIndex);

	if(derivedClass == nullptr) {
		return const_cast<void *>(self);
	}

	size_t derivedBaseCount = derivedClass->getBaseCount();
	for(size_t i = 0; i < derivedBaseCount; ++i) {
		const GMetaClass * baseClass = derivedClass->getBaseClass(i);
		if(baseClass != nullptr && baseClass->equals(this)) {
			return derivedClass->castFromBase(self, i);
		}
	}

	return const_cast<void *>(self);
}

void GMetaClass::ensureRegistered() const
{
	if(! this->intialized) {
		if(this->metaRegister) {
			this->metaRegister(const_cast<GMetaClass *>(this));
		}

		this->intialized = true;
	}

	if(this->module != nullptr && ! this->module->hasInitializedMetaClasses()) {
		this->module->initializeMetaClasses();
	}
}

void GMetaClass::setupItemLists()
{
	this->implement->itemLists[mcatAnnotation] = nullptr;
	this->implement->itemLists[mcatFundamental] = nullptr;

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

	return this->doGetItemCount(listIndex);
}

size_t GMetaClass::doGetItemCount(GMetaCategory listIndex) const
{
	return this->implement->itemLists[listIndex]->getCount();
}

const GMetaItem * GMetaClass::getItemAt(GMetaCategory listIndex, size_t index) const
{
	this->ensureRegistered();

	return this->doGetItemAt(listIndex, index);
}

const GMetaItem * GMetaClass::doGetItemAt(GMetaCategory listIndex, size_t index) const
{
	return this->implement->itemLists[listIndex]->getItemAt(index);
}

const GMetaItem * GMetaClass::getItemByName(GMetaCategory listIndex, const char * name, bool findSuper, void ** outInstance) const
{
	this->ensureRegistered();

	return this->doGetItemByName(listIndex, name, findSuper, outInstance);
}

const GMetaItem * GMetaClass::doGetItemByName(GMetaCategory listIndex, const char * name, bool findSuper, void ** outInstance) const
{
	GMetaInternalItemList * itemList = this->implement->itemLists[listIndex];

	const GMetaItem * result = itemList->getItemByName(name);

	if(!result && findSuper && this->superList) {
		void ** self = outInstance;
		for(size_t i = 0; i < this->getBaseCount(); ++i) {
			if(self != nullptr) {
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
	return getGlobalMetaClass()->getModule()->findClassByType(type.getBaseType());
}

const GMetaClass * findMetaClass(const char * name)
{
	return getGlobalMetaClass()->getModule()->findClassByName(name);
}

GMetaModule * doGetGlobalModule()
{
	static GMetaModule * globalModule = nullptr;

	if(globalModule == nullptr && isLibraryLive()) {
		globalModule = new GMetaModule();
		addOrderedStaticUninitializer(suo_MetaModuleForGlobalMetaclass, makeUninitializerDeleter(&globalModule));
	}

	return globalModule;
}

GMetaClass * getGlobalMetaClass()
{
	static GMetaClass * global = nullptr;
	if(global == nullptr && isLibraryLive()) {
		global = new GMetaClass((void *)0, nullptr, "", nullptr, GMetaPolicyDefault());
		addOrderedStaticUninitializer(suo_GlobalMetaClass, makeUninitializerDeleter(&global));
	}

	return global;
}


G_GUARD_LIBRARY_LIFE


} // namespace cpgf
