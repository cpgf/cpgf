#include "pinclude/gmetatypereg.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetafundamental.h"

#include <vector>


namespace cpgf {


namespace meta_internal {


namespace {
	class GMetaTypedItemList
	{
	public:
		typedef std::vector<const GMetaTypedItem *> ListType;

	public:
		GMetaTypedItemList() : freeItems(false) {
		}

		GMetaTypedItemList(bool freeItems) : freeItems(freeItems) {
		}

		~GMetaTypedItemList() {
			if(this->freeItems) {
				for(ListType::const_iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
					delete *it;
				}
			}
		}

		void add(const GMetaTypedItem * item) {
			if(std::find(this->itemList.begin(), this->itemList.end(), item) == this->itemList.end()) {
				this->itemList.push_back(item);
			}
		}

		const GMetaTypedItem * findByType(const GTypeInfo & type) const {
			for(ListType::const_iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
				if((*it)->getMetaType().getBaseType() == type) {
					return *it;
				}
			}

			return NULL;
		}

		const GMetaTypedItem * findByName(const char * name) const {
			if(name == NULL || !*name) {
				return NULL;
			}

			for(ListType::const_iterator it = this->itemList.begin(); it != this->itemList.end(); ++it) {
				if(strcmp((*it)->getName().c_str(), name) == 0) {
					return *it;
				}
			}

			return NULL;
		}

		bool isEmpty() const {
			return this->itemList.empty();
		}

		const ListType * getItemList() const {
			return &this->itemList;
		}

	private:
		ListType itemList;
		bool freeItems;
	};

	GMetaTypedItemList * getMetaClassList() {
		static GMetaTypedItemList metaClassList;

		if(metaClassList.isEmpty()) {
			metaClassList.add(getGlobalMetaClass());
		}

		return &metaClassList;
	}

	GMetaTypedItemList * getMetaEnumList() {
		static GMetaTypedItemList metaEnumList;

		return &metaEnumList;
	}

	GMetaTypedItemList * getMetaFundamentalList() {
		static GMetaTypedItemList metaFundamentalList(true);

		return &metaFundamentalList;
	}

	void registerFundamentalTypes()
	{
		GMetaTypedItemList * itemList = getMetaFundamentalList();
		
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
	const GMetaTypedItemList::ListType * itemList = getMetaFundamentalList()->getItemList();

	for(GMetaTypedItemList::ListType::const_iterator it = itemList->begin(); it != itemList->end(); ++it) {
		if(static_cast<const GMetaFundamental *>(*it)->getVariantType() == vt) {
			return static_cast<const GMetaFundamental *>(*it);
		}
	}

	return NULL;
}


void registerMetaTypedItem(const GMetaTypedItem * typedItem)
{
	switch(typedItem->getCategory()) {
		case mcatClass:
			getMetaClassList()->add(typedItem);
			break;
		
		case mcatEnum:
			getMetaEnumList()->add(typedItem);
			break;
			
		default:
			GASSERT_MSG(false, "Registering non-type item.");
			break;
	}
}



} // namespace meta_internal



} // namespace cpgf
