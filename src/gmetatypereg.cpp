#include "cpgf/gmetaclass.h"
#include "cpgf/gmetafundamental.h"
#include "cpgf/gstringmap.h"

#include <vector>

#ifdef G_COMPILER_CPPBUILDER
#pragma warn -8104 //Local Static with constructor dangerous for multi-threaded apps
#endif


namespace cpgf {


namespace meta_internal {


namespace {
	static bool isInFinalizing = false;

	class GMetaTypedItemListXXX
	{
	public:
		typedef GStringMap<const GMetaTypedItem *, GStringMapReuseKey> MapType;

	public:
		GMetaTypedItemListXXX() : freeItems(false) {
		}

		explicit GMetaTypedItemListXXX(bool freeItems) : freeItems(freeItems) {
		}

		~GMetaTypedItemListXXX() {
			isInFinalizing = true;

			if(this->freeItems) {
				for(MapType::const_iterator it = this->itemMap.begin(); it != this->itemMap.end(); ++it) {
					delete it->second;
				}
			}
		}

		void add(const GMetaTypedItem * item) {
			this->itemMap.set(item->getTypeName().c_str(), item);
		}

		void remove(const GMetaTypedItem * item) {
			this->itemMap.remove(item->getTypeName().c_str());
		}

		const GMetaTypedItem * findByType(const GTypeInfo & type) const {
			for(MapType::const_iterator it = this->itemMap.begin(); it != this->itemMap.end(); ++it) {
				if(it->second->getMetaType().getBaseType() == type) {
					return it->second;
				}
			}

			return NULL;
		}

		const GMetaTypedItem * findByName(const char * name) const {
			if(name == NULL || !*name) {
				return NULL;
			}

			MapType::const_iterator it = this->itemMap.find(name);
			return it == this->itemMap.end() ? NULL : it->second;
		}

		const MapType * getItemMap() const {
			return &this->itemMap;
		}

	private:
		MapType itemMap;
		bool freeItems;
	};

	GMetaTypedItemListXXX * getMetaClassList() {
		static GMetaTypedItemListXXX metaClassList;

		return &metaClassList;
	}

	GMetaTypedItemListXXX * getMetaEnumList() {
		static GMetaTypedItemListXXX metaEnumList;

		return &metaEnumList;
	}

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

	bool metaTypeRegIntialized = false;

	void intializeAllMetaTypes(const GMetaClass * metaClass)
	{
		for(size_t i = 0; i < metaClass->getClassCount(); ++i) {
			intializeAllMetaTypes(metaClass->getClassAt(i));
		}
	}

	void initializeMetaTypeReg()
	{
		if(!metaTypeRegIntialized) {
			registerFundamentalTypes();
			intializeAllMetaTypes(getGlobalMetaClass());

			metaTypeRegIntialized = true;
		}
	}
}

const GMetaTypedItem * findRegisteredMetaType(const GTypeInfo & type)
{
	const GMetaTypedItem * item;

	item = getMetaClassList()->findByType(type);

	if(item == NULL) {
		item = getMetaFundamentalList()->findByType(type);
	}

	if(item == NULL) {
		item = getMetaEnumList()->findByType(type);
	}

	return item;
}

const GMetaTypedItem * findRegisteredMetaType(const char * name)
{
	const GMetaTypedItem * item;

	item = getMetaClassList()->findByName(name);

	if(item == NULL) {
		item = getMetaFundamentalList()->findByName(name);
	}

	if(item == NULL) {
		item = getMetaEnumList()->findByName(name);
	}

	return item;
}


const GMetaClass * findRegisteredMetaClass(const GMetaType & type)
{
	return static_cast<const GMetaClass *>(getMetaClassList()->findByType(type.getBaseType()));
}

const GMetaClass * findRegisteredMetaClass(const char * name)
{
	return static_cast<const GMetaClass *>(getMetaClassList()->findByName(name));
}


const GMetaEnum * findRegisteredMetaEnum(const GMetaType & type)
{
	return static_cast<const GMetaEnum *>(getMetaEnumList()->findByType(type.getBaseType()));
}

const GMetaEnum * findRegisteredMetaEnum(const char * name)
{
	return static_cast<const GMetaEnum *>(getMetaEnumList()->findByName(name));
}


const GMetaFundamental * findRegisteredMetaFundamental(const GMetaType & type)
{
	return static_cast<const GMetaFundamental *>(getMetaFundamentalList()->findByType(type.getBaseType()));
}

const GMetaFundamental * findRegisteredMetaFundamental(const char * name)
{
	return static_cast<const GMetaFundamental *>(getMetaFundamentalList()->findByName(name));
}

const GMetaFundamental * findRegisteredMetaFundamental(GVariantType vt)
{
	const GMetaTypedItemListXXX::MapType * itemMap = getMetaFundamentalList()->getItemMap();

	for(GMetaTypedItemListXXX::MapType::const_iterator it = itemMap->begin(); it != itemMap->end(); ++it) {
		if(static_cast<const GMetaFundamental *>(it->second)->getVariantType() == vt) {
			return static_cast<const GMetaFundamental *>(it->second);
		}
	}

	return NULL;
}




} // namespace meta_internal



} // namespace cpgf

