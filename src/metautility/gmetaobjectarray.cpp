#include "cpgf/metautility/gmetaobjectarray.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetacommon.h"
#include "cpgf/gsharedinterface.h"

#include <algorithm>

#include <stdlib.h>

namespace {

struct CFreeGuard
{
public:
	explicit CFreeGuard(void * p) : p(p) {}
	~CFreeGuard() {
		free(this->p);
	}
private:
	void * p;
};

} // unnamed namespace

namespace cpgf {

class GMetaObjectArrayImplement
{
public:
	explicit GMetaObjectArrayImplement(IMetaClass * metaClass);
	~GMetaObjectArrayImplement();

	void setObject(uint32_t index, void * object);
	void * getObject(uint32_t index) const;
	
	uint32_t getObjectCount() const;
	const GMetaType & getMetaType() const;
	
	void * getBuffer() const;
	
private:
	void * doGetObject(uint32_t index) const;
	void * doGetAddress(void * head, uint32_t index) const;
	void requireMemory(uint32_t count);
	void doSetCapacity(uint32_t newCapacity);
	void doFreeObjects(void * head, uint32_t startIndex, uint32_t endIndex); // [startIndex, endIndex)

private:
	GSharedInterface<IMetaClass> metaClass;
	GMetaType metaType;
	uint32_t objectSize;
	uint32_t objectCount;
	uint32_t capacity;
	void * buffer;
};


GMetaObjectArrayImplement::GMetaObjectArrayImplement(IMetaClass * metaClass)
	: metaClass(metaClass), metaType(metaGetItemType(metaClass)), objectSize(metaClass->getTypeSize()), objectCount(0), capacity(0), buffer(nullptr)
{
}

GMetaObjectArrayImplement::~GMetaObjectArrayImplement()
{
	if(this->buffer != nullptr) {
		this->doFreeObjects(this->buffer, 0, this->objectCount);

		free(this->buffer);
	}
}

void GMetaObjectArrayImplement::setObject(uint32_t index, void * object)
{
	if(object == nullptr) {
		return;
	}

	this->requireMemory(index + 1);
	if(this->objectCount <= index) {
		this->objectCount = index + 1;
	}
	void * newObject = this->getObject(index);
	this->metaClass->destroyInplace(newObject);
	this->metaClass->cloneInplace(object, newObject);
}

void * GMetaObjectArrayImplement::getObject(uint32_t index) const
{
	if(index < this->objectCount) {
		return this->doGetObject(index);
	}
	else {
		return nullptr;
	}
}

uint32_t GMetaObjectArrayImplement::getObjectCount() const
{
	return this->objectCount;
}

const GMetaType & GMetaObjectArrayImplement::getMetaType() const
{
	return this->metaType;
}

void * GMetaObjectArrayImplement::getBuffer() const
{
	return this->buffer;
}

void * GMetaObjectArrayImplement::doGetObject(uint32_t index) const
{
	return this->doGetAddress(this->buffer, index);
}

void * GMetaObjectArrayImplement::doGetAddress(void * head, uint32_t index) const
{
	return static_cast<char *>(head) + (index * this->objectSize);
}

void GMetaObjectArrayImplement::requireMemory(uint32_t count)
{
	if(count > this->capacity) {
		uint32_t newCapacity = count;
		if(count > 16) {
			newCapacity += count / 2;
		}
		else {
			newCapacity += count;
		}
		
		this->doSetCapacity(newCapacity);
	}

	while(count > this->objectCount) {
		--count;
		void * object = this->doGetObject(count);
		this->metaClass->createInplace(object);
	}
}

void GMetaObjectArrayImplement::doSetCapacity(uint32_t newCapacity)
{
	if(newCapacity == this->capacity) {
		return;
	}
	
	if(newCapacity < this->objectCount) {
		this->doFreeObjects(this->buffer, newCapacity, this->objectCount);
		this->objectCount = newCapacity;
	}
	
	void * newBuffer = malloc(newCapacity * this->objectSize);

	if(this->buffer != nullptr) {
		CFreeGuard freeGuard(this->buffer);
		for(uint32_t i = 0; i < this->objectCount; ++i) {
			this->metaClass->cloneInplace(this->doGetAddress(this->buffer, i), this->doGetAddress(newBuffer, i));
		}
		
		this->doFreeObjects(this->buffer, 0, this->objectCount);
	}
	
	this->buffer = newBuffer;
	this->capacity = newCapacity;
}

void GMetaObjectArrayImplement::doFreeObjects(void * head, uint32_t startIndex, uint32_t endIndex)
{
	while(startIndex < endIndex) {
		void * object = this->doGetAddress(head, startIndex);
		this->metaClass->destroyInplace(object);
		++startIndex;
	}
}


GMetaObjectArray::GMetaObjectArray(IMetaClass * metaClass)
	: implement(new GMetaObjectArrayImplement(metaClass))
{
}

GMetaObjectArray::~GMetaObjectArray()
{
}

GVariant GMetaObjectArray::getItem(uint32_t index) const
{
	void * object = this->implement->getObject(index);
	if(object == nullptr) {
		return object;
	}
	else {
		return createTypedVariant(object, this->implement->getMetaType());
	}
}

void GMetaObjectArray::setItem(uint32_t index, const GVariant & value, const GMetaVariadicParam * moreValues)
{
	void * object = objectAddressFromVariant(value);
	this->implement->setObject(index, object);

	if(moreValues != nullptr) {
		for(uint32_t i = 0; i < moreValues->paramCount; ++i) {
			++index;
			object = objectAddressFromVariant(*(moreValues->params[i]));
			this->implement->setObject(index, object);
		}
	}
}

uint32_t GMetaObjectArray::getCount() const
{
	return this->implement->getObjectCount();
}


} // namespace cpgf

