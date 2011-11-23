#include "cpgf/gmetacommon.h"
#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetatype.h"

#include "pinclude/gmetatypereg.h"

#include <string>
#include <vector>

#include <stdarg.h>
#include <stdio.h>


namespace cpgf {


std::string normalizeReflectName(const char * name)
{
	size_t length = strlen(name);

	if(length == 0) {
		return "";
	}

	const char * p = name + (length - 1);
	const char * end = p;

	while(!isCSymbole(*end) && end > name) {
		--end;
	}
	if(isCSymbole(*end)) {
		++end;
	}

	while(isCSymbole(*p) && p > name) {
		--p;
	}

	if(!isCSymbole(*p)) {
		++p;
	}

	return std::string(p, end - p);
}

namespace meta_internal {

void handleForbidAccessError(bool isRead)
{
	if(isRead) {
		raiseCoreException(Error_Meta_ReadDenied);
	}
	else {
		raiseCoreException(Error_Meta_WriteDenied);
	}

}

void makeFullName(const GMetaItem * item, std::string * outName, const char * delimiter)
{
	*outName = item->getName();
	
	for(;;) {
		item = item->getOwnerItem();
		if(item == NULL) {
			break;
		}

		if(item->getName() == "") {
			break;
		}
		
		*outName = item->getName() + delimiter + *outName;
	}
}


class GMetaItemImplement
{
public:
	typedef std::vector<const GMetaAnnotation *> AnnotationListType;
	
public:
	GMetaItemImplement(const std::string & name, const GMetaType & itemType)
		: name(name), itemType(itemType), annotationList() {
	}

	~GMetaItemImplement() {
	}

	std::string name;
	std::string qualifiedName;
	GMetaType itemType;
	GScopedPointer<AnnotationListType> annotationList;
};


void * newZeroBuffer(void * buffer, size_t size, void * copy)
{
	void * result = (buffer == NULL ? new char[size] : buffer);
	
	if(copy == NULL) {
		switch(size) {
			case 1:
				*static_cast<uint8_t *>(result) = 0;
				break;
				
			case 2:
				*static_cast<uint16_t *>(result) = 0;
				break;
				
			case 4:
				*static_cast<uint32_t *>(result) = 0;
				break;
				
			case 8:
				*static_cast<uint64_t *>(result) = 0;
				break;
				
			default:
				memset(result, 0, size);
				break;
		}
	}
	else {
		switch(size) {
			case 1:
				*static_cast<uint8_t *>(result) = *static_cast<uint8_t *>(copy);
				break;
				
			case 2:
				*static_cast<uint16_t *>(result) = *static_cast<uint16_t *>(copy);
				break;
				
			case 4:
				*static_cast<uint32_t *>(result) = *static_cast<uint32_t *>(copy);
				break;
				
			case 8:
				*static_cast<uint64_t *>(result) = *static_cast<uint64_t *>(copy);
				break;
				
			default:
				memmove(result, copy, size);
				break;
		}
	}
	
	return result;
}

} // namespace meta_internal


GMetaItem::GMetaItem(const char * name, const GMetaType & itemType, GMetaCategory category)
	: implement(new meta_internal::GMetaItemImplement(name, itemType)), modifiers(0), category(category), ownerItem(NULL)
{
}

GMetaItem::~GMetaItem()
{
}

const GMetaAnnotation * GMetaItem::getAnnotation(const char * name) const
{
	if(! this->implement->annotationList) {
		return NULL;
	}
	else {
		for(size_t i = 0; i < this->implement->annotationList->size(); ++i) {
			if(strcmp(name, this->implement->annotationList->at(i)->getName().c_str()) == 0) {
				return this->implement->annotationList->at(i);
			}
		}

		return NULL;
	}
}

size_t GMetaItem::getAnnotationCount() const
{
	if(! this->implement->annotationList) {
		return 0;
	}
	else {
		return this->implement->annotationList->size();
	}
}

const GMetaAnnotation * GMetaItem::getAnnotationAt(size_t index) const
{
	if(! this->implement->annotationList) {
		return NULL;
	}
	else {
		return this->implement->annotationList->at(index);
	}
}

void GMetaItem::addItemAnnotation(const GMetaAnnotation * annotation)
{
	if(! this->implement->annotationList) {
		this->implement->annotationList.reset(new meta_internal::GMetaItemImplement::AnnotationListType);
	}

	this->implement->annotationList->push_back(annotation);
}

const GMetaType & GMetaItem::getItemType() const
{
	return this->implement->itemType;
}

const std::string & GMetaItem::getName() const
{
	return this->implement->name;
}

const std::string & GMetaItem::getQualifiedName() const
{
	if(this->implement->qualifiedName.empty()) {
		this->implement->qualifiedName = makeQualifiedName(".");
	}

	return this->implement->qualifiedName;
}

std::string GMetaItem::makeQualifiedName(const char * delimiter) const
{
	std::string result;

	meta_internal::makeFullName(this, &result, delimiter);

	return result;
}

void GMetaItem::setName(const char * name)
{
	this->implement->name = name;
}

	

GMetaTypedItem::GMetaTypedItem(const char * name, const GMetaType & itemType, GMetaCategory category)
	: super(name, itemType, category)
{
}

const GMetaType & GMetaTypedItem::getMetaType() const
{
	if(this->implement->itemType.getBaseName() == NULL) {
		this->implement->itemType = createMetaTypeWithName(this->implement->itemType, this->getTypeName().c_str());
	}

	return this->getItemType();
}

const std::string & GMetaTypedItem::getTypeName() const
{
	if(this->typeName == "") {
		meta_internal::makeFullName(this, &this->typeName, "-");
	}
	return this->typeName;
}



GMetaAccessible::GMetaAccessible(const char * name, const GMetaType & itemType, GMetaCategory category)
	: super(name, itemType, category)
{
}


GMetaCallable::GMetaCallable(const char * name, const GMetaType & itemType, GMetaCategory category)
	: super(name, itemType, category)
{
}


GMetaList::GMetaList()
{
}

GMetaList::~GMetaList()
{
}

GMetaList::GMetaList(const GMetaList & other)
	: itemList(other.itemList)
{
}

GMetaList & GMetaList::operator = (const GMetaList & other)
{
	if(this != &other) {
		this->itemList = other.itemList;
	}

	return *this;
}

void GMetaList::add(const GMetaItem * item, void * instance)
{
	this->itemList.push_back(item);
	this->instanceList.push_back(instance);
}

size_t GMetaList::getCount() const
{
	return this->itemList.size();
}

const GMetaItem * GMetaList::getAt(size_t index) const
{
	return this->itemList.at(index);
}

void * GMetaList::getInstanceAt(size_t index) const
{
	return this->instanceList.at(index);
}

void GMetaList::clear()
{
	this->itemList.clear();
	this->instanceList.clear();
}


bool GMetaConverterDefault::canToCString()
{
	return false;
}

const char * GMetaConverterDefault::toCString(const void * instance, int * needFree)
{
	(void)instance;

	*needFree = false;

	return NULL;
}



const GMetaTypedItem * findMetaType(const GMetaType & type)
{
	return meta_internal::findRegisteredMetaType(type.getBaseType());
}

const GMetaTypedItem * findMetaType(const char * name)
{
	return meta_internal::findRegisteredMetaType(name);
}


const GMetaEnum * findMetaEnum(const GMetaType & type)
{
	return meta_internal::findRegisteredMetaEnum(type);
}

const GMetaEnum * findMetaEnum(const char * name)
{
	return meta_internal::findRegisteredMetaEnum(name);
}

const GMetaFundamental * findMetaFundamental(const GMetaType & type)
{
	return meta_internal::findRegisteredMetaFundamental(type);
}

const GMetaFundamental * findMetaFundamental(const char * name)
{
	return meta_internal::findRegisteredMetaFundamental(name);
}

const GMetaFundamental * findMetaFundamental(GVariantType vt)
{
	return meta_internal::findRegisteredMetaFundamental(vt);
}


bool metaIsField(int category)
{
	return category == mcatField;
}

bool metaIsProperty(int category)
{
	return category == mcatProperty;
}

bool metaIsMethod(int category)
{
	return category == mcatMethod;
}

bool metaIsEnum(int category)
{
	return category == mcatEnum;
}

bool metaIsOperator(int category)
{
	return category == mcatOperator;
}

bool metaIsConstructor(int category)
{
	return category == mcatConstructor;
}

bool metaIsClass(int category)
{
	return category == mcatClass;
}

bool metaIsAnnotation(int category)
{
	return category == mcatAnnotation;
}

bool metaIsFundamental(int category)
{
	return category == mcatFundamental;
}



} // namespace cpgf

