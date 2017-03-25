#include "cpgf/gmetamodule.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetafundamental.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gcompiler.h"

#include <map>

namespace cpgf {

namespace {

inline std::size_t getTypeInfoHash(const GTypeInfo &type) {
	if (type.isEmpty()) {
		throw std::runtime_error("missing type information");
	}
	return type.getStdTypeInfo().hash_code();
}

}

class GMetaTypedItemList
{
	typedef std::map<std::size_t, const GMetaTypedItem *> ItemsByTypeHashMap;
public:
	typedef GStringMap<const GMetaTypedItem *, GStringMapReuseKey> MapType;

public:
	GMetaTypedItemList() : freeItems(false) {
	}

	explicit GMetaTypedItemList(bool freeItems) : freeItems(freeItems) {
	}

	~GMetaTypedItemList() {
		if(this->freeItems) {
			for(MapType::const_iterator it = this->itemMap.begin(); it != this->itemMap.end(); ++it) {
				delete it->second;
			}
		}
	}

	void add(const GMetaTypedItem * item) {
		const char * name = item->getQualifiedName().c_str();
//		MapType::const_iterator it = this->itemMap.find(name);
//		if(it == this->itemMap.end()) {
			this->itemMap.set(name, item);
			itemsByTypeHash[getTypeInfoHash(item->getMetaType().getBaseType())] = item;
//		}
	}

	void remove(const GMetaTypedItem * item) {
		this->itemMap.remove(item->getQualifiedName().c_str());
		itemsByTypeHash.erase(getTypeInfoHash(item->getMetaType().getBaseType()));
	}

	const GMetaTypedItem * findByType(const GTypeInfo & type) const {
		ItemsByTypeHashMap::const_iterator it = itemsByTypeHash.find(getTypeInfoHash(type));
		if (it == itemsByTypeHash.end()) {
			return nullptr;
		}
		return it->second;
	}

	const GMetaTypedItem * findByName(const char * name) const {
		if(name == nullptr || !*name) {
			return nullptr;
		}

		MapType::const_iterator it = this->itemMap.find(name);
		return it == this->itemMap.end() ? nullptr : it->second;
	}

	const MapType * getItemMap() const {
		return &this->itemMap;
	}

private:
	MapType itemMap;
	ItemsByTypeHashMap itemsByTypeHash;
	bool freeItems;
};

class GMetaModuleImplement
{
public:
	GScopedPointer<GMetaTypedItemList> classList;
	GScopedPointer<GMetaTypedItemList> enumList;
};

GMetaModule::GMetaModule()
	: implement(new GMetaModuleImplement), initializedMetaClasses(false)
{
}

GMetaModule::~GMetaModule()
{
	this->unregisterAll();
}

void GMetaModule::unregisterAll()
{
	if(this->implement->classList) {
		GScopedPointer<GMetaTypedItemList> tempClassList(this->implement->classList.take());
		for(GMetaTypedItemList::MapType::const_iterator it = tempClassList->getItemMap()->begin(); it != tempClassList->getItemMap()->end(); ++it) {
			this->unregisterMetaClass(static_cast<const GMetaClass *>(it->second));
			static_cast<const GMetaClass *>(it->second)->setModule(nullptr);
		}
	}
}

void GMetaModule::registerMetaClass(const GMetaClass * metaClass)
{
	this->doRegisterMetaClass(metaClass);

	size_t count;
	size_t i;

	count = metaClass->doGetItemCount(mcatEnum);
	for(i = 0; i < count; ++i) {
		doRegisterMetaEnum(static_cast<const GMetaEnum *>(metaClass->doGetItemAt(mcatEnum, i)));
	}
}

void GMetaModule::unregisterMetaClass(const GMetaClass * metaClass)
{
	this->doUnregisterMetaClass(metaClass);

	size_t count;
	size_t i;

	count = metaClass->doGetItemCount(mcatEnum);
	for(i = 0; i < count; ++i) {
		doUnregisterMetaEnum(static_cast<const GMetaEnum *>(metaClass->doGetItemAt(mcatEnum, i)));
	}
}

void GMetaModule::doRegisterMetaClass(const GMetaClass * metaClass)
{
	if(metaClass->isGlobal()) {
		return;
	}

	if(! this->implement->classList) {
		this->implement->classList.reset(new GMetaTypedItemList);
	}

	this->implement->classList->add(metaClass);
}

void GMetaModule::doRegisterMetaEnum(const GMetaEnum * metaEnum)
{
	if(! this->implement->enumList) {
		this->implement->enumList.reset(new GMetaTypedItemList);
	}

	this->implement->enumList->add(metaEnum);
}

void GMetaModule::doUnregisterMetaClass(const GMetaClass * metaClass)
{
	if(metaClass->isGlobal()) {
		return;
	}

	if(this->implement->classList) {
		this->implement->classList->remove(metaClass);
	}
}

void GMetaModule::doUnregisterMetaEnum(const GMetaEnum * metaEnum)
{
	if(this->implement->enumList) {
		this->implement->enumList->remove(metaEnum);
	}
}

const GMetaTypedItem * GMetaModule::findItemByType(const GTypeInfo & type) const
{
	const GMetaTypedItem * item = nullptr;

	if(this->implement->classList) {
		item = this->implement->classList->findByType(type);
	}

	if(item == nullptr && this->implement->enumList) {
		item = this->implement->enumList->findByType(type);
	}

	return item;
}

const GMetaTypedItem * GMetaModule::findItemByName(const char * name) const
{
	const GMetaTypedItem * item = nullptr;

	if(this->implement->classList) {
		item = this->implement->classList->findByName(name);
	}

	if(item == nullptr && this->implement->enumList) {
		item = this->implement->enumList->findByName(name);
	}

	return item;
}

const GMetaClass * GMetaModule::findClassByType(const GTypeInfo & type) const
{
	if(this->implement->classList) {
		return static_cast<const GMetaClass *>(this->implement->classList->findByType(type));
	}
	else {
		return nullptr;
	}
}

const GMetaClass * GMetaModule::findClassByName(const char * name) const
{
	if(this->implement->classList) {
		return static_cast<const GMetaClass *>(this->implement->classList->findByName(name));
	}
	else {
		return nullptr;
	}
}

void GMetaModule::initializeMetaClasses()
{
	if(! hasInitializedMetaClasses() && this->implement->classList) {
		bool didIt = false;
		do {
			didIt = false;
			for(GMetaTypedItemList::MapType::const_iterator it = this->implement->classList->getItemMap()->begin(); it != this->implement->classList->getItemMap()->end(); ++it) {
				if(const_cast<GMetaClass *>(static_cast<const GMetaClass *>(it->second))->fixupHierarchy()) {
					didIt = true;
				}
			}
		} while(didIt);

		initializedMetaClasses = true;
	}
}

GMetaModule * getItemModule(const GMetaItem * metaItem)
{
	while(metaItem != nullptr) {
		if(metaIsClass(metaItem->getCategory())) {
			return static_cast<const GMetaClass *>(metaItem)->getModule();
		}
		metaItem = metaItem->getOwnerItem();
	}

	return nullptr;
}


} // namespace cpgf



/*  not used

	GMetaTypedItemListXXX * getMetaFundamentalList() {
		static GMetaTypedItemListXXX metaFundamentalList(true);

		return &metaFundamentalList;
	}

	void registerFundamentalTypes()
	{
		GMetaTypedItemListXXX * itemList = getMetaFundamentalList();

		itemList->add(new GMetaFundamental((bool *)0, "bool"));
		itemList->add(new GMetaFundamental((char *)0, "char"));
		itemList->add(new GMetaFundamental((wchar_t *)0, "wchar"));
		itemList->add(new GMetaFundamental((signed char *)0, "s_char"));
		itemList->add(new GMetaFundamental((unsigned char *)0, "u_char"));
		itemList->add(new GMetaFundamental((signed short *)0, "s_short"));
		itemList->add(new GMetaFundamental((unsigned short *)0, "u_short"));
		itemList->add(new GMetaFundamental((signed int *)0, "s_int"));
		itemList->add(new GMetaFundamental((unsigned int *)0, "u_int"));
		itemList->add(new GMetaFundamental((signed long *)0, "s_long"));
		itemList->add(new GMetaFundamental((unsigned long *)0, "u_long"));
		itemList->add(new GMetaFundamental((signed long long *)0, "s_longlong"));
		itemList->add(new GMetaFundamental((unsigned long long *)0, "u_longlong"));
		itemList->add(new GMetaFundamental((float *)0, "float"));
		itemList->add(new GMetaFundamental((double *)0, "double"));
		itemList->add(new GMetaFundamental((long double *)0, "longdouble"));
	}

*/
