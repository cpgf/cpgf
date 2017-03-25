#include "cpgf/gmetacommon.h"
#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetatype.h"
#include "cpgf/gerrorcode.h"

#include "cpgf/private/gmetadefaultparam_p.h"


#include <string>
#include <vector>

#include <stdarg.h>
#include <stdio.h>


namespace cpgf {

namespace meta_internal {


GMetaDefaultParamList::~GMetaDefaultParamList()
{
}

void GMetaDefaultParamList::addDefault(const GVariant & v)
{
	this->defaultValueList.push_back(v);
}

const GVariant & GMetaDefaultParamList::getDefault(size_t index)
{
	return this->defaultValueList.at(index);
}

size_t GMetaDefaultParamList::getDefaultCount() const
{
	return this->defaultValueList.size();
}

size_t GMetaDefaultParamList::loadDefaultParams(
		const GVariant ** paramBuffer,
		size_t passedParamCount,
		size_t prototypeParamCount
	)
{
	if(passedParamCount < prototypeParamCount) {
		const int totalCount = static_cast<int>(this->getDefaultCount());
		int startIndex = totalCount - 1;
		const int needCount = static_cast<int>(prototypeParamCount - passedParamCount);
		if(needCount <= totalCount) {
			startIndex = needCount - 1;
		}
		while(startIndex >= 0) {
			paramBuffer[passedParamCount] = &this->defaultValueList.at(startIndex);
			++passedParamCount;
			--startIndex;
		}
	}

	return passedParamCount;
}

size_t GMetaDefaultParamList::loadDefaultParamsByData(
		const GVariantData ** paramDataBuffer,
		size_t passedParamCount,
		size_t prototypeParamCount
	)
{
	if(passedParamCount < prototypeParamCount) {
		const int totalCount = static_cast<int>(this->getDefaultCount());
		int startIndex = totalCount - 1;
		const int needCount = static_cast<int>(prototypeParamCount - passedParamCount);
		if(needCount <= totalCount) {
			startIndex = needCount - 1;
		}
		while(startIndex >= 0) {
			paramDataBuffer[passedParamCount] = &this->defaultValueList.at(startIndex).refData();
			++passedParamCount;
			--startIndex;
		}
	}

	return passedParamCount;
}

void makeFullName(const GMetaItem * item, std::string * outName, const char * delimiter)
{
	*outName = item->getName();
	
	for(;;) {
		item = item->getOwnerItem();
		if(item == nullptr) {
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
		if(this->annotationList) {
			for(AnnotationListType::iterator it = this->annotationList->begin(); it != this->annotationList->end(); ++it) {
				delete *it;
			}
		}
	}

	std::string name;
	std::string qualifiedName;
	GMetaType itemType;
	GScopedPointer<AnnotationListType> annotationList;
};


void * newZeroBuffer(void * buffer, size_t size, void * copy)
{
	void * result = (buffer == nullptr ? new char[size] : buffer);
	
	if(copy == nullptr) {
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

void checkInvokingArity(size_t invokingParamCount, size_t prototypeParamCount, bool isVariadic)
{
	if((isVariadic && invokingParamCount >= prototypeParamCount)
		|| (! isVariadic && invokingParamCount == prototypeParamCount)) {
	}
	else {
		raiseCoreException(Error_Meta_WrongArity, prototypeParamCount, invokingParamCount);
	}
}

void adjustParamIndex(size_t & index, bool isExplicitThis)
{
	if(index >= abstractParameterIndexBase) {
		index -= abstractParameterIndexBase;
	}
	else {
		if(isExplicitThis) {
			++index;
		}
	}
}


} // namespace meta_internal


GMetaItem::GMetaItem(const char * name, const GMetaType & itemType, GMetaCategory category)
	: implement(new meta_internal::GMetaItemImplement(name, itemType)), modifiers(0), category(category), ownerItem(nullptr)
{
}

GMetaItem::~GMetaItem()
{
}

const GMetaAnnotation * GMetaItem::getAnnotation(const char * name) const
{
	if(! this->implement->annotationList) {
		return nullptr;
	}
	else {
		for(size_t i = 0; i < this->implement->annotationList->size(); ++i) {
			if(strcmp(name, this->implement->annotationList->at(i)->getName().c_str()) == 0) {
				return this->implement->annotationList->at(i);
			}
		}

		return nullptr;
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
		return nullptr;
	}
	else {
		return this->implement->annotationList->at(index);
	}
}

GMetaAnnotation * GMetaItem::addItemAnnotation(GMetaAnnotation * annotation)
{
	if(! this->implement->annotationList) {
		this->implement->annotationList.reset(new meta_internal::GMetaItemImplement::AnnotationListType);
	}

	annotation->setMetaItem(this);
	this->implement->annotationList->push_back(annotation);

	return annotation;
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

void GMetaItem::resetQualifiedName() const
{
	this->implement->qualifiedName = "";
}

std::string GMetaItem::makeQualifiedName(const char * delimiter) const
{
	std::string result;

	meta_internal::makeFullName(this, &result, delimiter);

	return result;
}

bool GMetaItem::equals(const GMetaItem * other) const
{
	return this->getQualifiedName() == other->getQualifiedName();
}

void GMetaItem::setName(const char * name)
{
	this->implement->name = name;
}

	

GMetaTypedItem::GMetaTypedItem(const char * name, const GMetaType & itemType, GMetaCategory category)
	: super(name, itemType, category)
{
}

GMetaTypedItem::~GMetaTypedItem()
{
}

const GMetaType & GMetaTypedItem::getMetaType() const
{
	if(this->implement->itemType.getBaseName() == nullptr) {
		this->implement->itemType = createMetaTypeWithName(this->implement->itemType, this->getQualifiedName().c_str());
	}

	return this->getItemType();
}

const std::string & GMetaTypedItem::getQualifiedName() const
{
	if(this->implement->qualifiedName.empty()) {
		this->implement->qualifiedName = makeQualifiedName(".");
		GMetaTypeData & typeData = this->implement->itemType.refData();
		typeData.baseName = this->implement->qualifiedName.c_str();
	}

	return this->implement->qualifiedName;
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

