#include "cpgf/metautility/gmetabytearray.h"
#include "cpgf/gexception.h"
#include "cpgf/gapiutil.h"
#include "cpgf/gvariant.h"
#include "cpgf/gmetacommon.h"

#include <vector>

#include <string.h>


using namespace std;

namespace cpgf {

class GMetaByteArrayImplement
{
public:
	typedef unsigned char ByteType;
	typedef vector<ByteType> ArrayType;

public:
	GMetaByteArrayImplement()
    	: byteArray(), length(0), position(0) {
    }

    explicit GMetaByteArrayImplement(size_t length)
    	: byteArray(length + 1), length(length), position(0) {
        this->byteArray[0] = 0;
    }

    void * getPointer() const {
    	return length > 0 ? const_cast<ByteType *>(&byteArray[0]) : nullptr;
    }

    void * getCurrentPointer() const {
       	return static_cast<uint8_t *>(this->getPointer()) + this->position;
    }

    void setPosition(size_t position) {
    	if(this->position != position) {
	    	this->position = position;
    	    this->checkAndLimitPosition();
        }
    }

    void setLength(size_t length) {
    	if(this->length != length) {
	    	this->length = length;
    		this->byteArray.resize(this->length + 1);
        	this->checkAndLimitPosition();
        }
    }

    template <typename T>
    T read() {
    	size_t currentPosition = this->position;
        this->position += sizeof(T);
        return *reinterpret_cast<T *>(
        	static_cast<uint8_t *>(this->getPointer()) + currentPosition
        );
    }

    template <typename T>
    void write(T value, const GMetaVariadicParam * moreValues) {
    	size_t valueCount = 1;
    	if(moreValues != nullptr) {
    		valueCount += moreValues->paramCount;
    	}

    	this->needSpace(sizeof(T) * valueCount);

    	int i = 0;
    	for(;;) {
	        *reinterpret_cast<T *>(
    	    	static_cast<uint8_t *>(this->getPointer()) + this->position
        	) = value;
	        this->position += sizeof(T);

	        --valueCount;
	        if(valueCount == 0) {
	        	break;
	        }
	    	if(moreValues != nullptr) {
    			value = fromVariant<T>(*(moreValues->params[i]));
    			++i;
    		}
        }

        if(this->position == this->length) {
        	*static_cast<uint8_t *>(this->getCurrentPointer()) = 0;
        }
    }

    void readBuffer(void * buffer, size_t length)
    {
        memmove(buffer, this->getCurrentPointer(), length);
        this->position += length;
    }
    void writeBuffer(const void * buffer, size_t length)
    {
        this->needSpace(length);
        memmove(this->getCurrentPointer(), buffer, length);
        this->position += length;
        if(this->position == this->length) {
        	*static_cast<uint8_t *>(this->getCurrentPointer()) = 0;
        }
    }

    void needSpace(size_t space) {
    	if(this->position + space > this->length) {
        	this->setLength(this->position + space);
        }
    }

private:
	void checkAndLimitPosition() {
        if(this->position > this->length) {
        	this->position = this->length;
            this->byteArray[0] = 0;
        }
    }

public:
	ArrayType byteArray;
    size_t length;
    size_t position;
};

GMetaByteArray::GMetaByteArray()
	: implement(new GMetaByteArrayImplement())
{
}

GMetaByteArray::GMetaByteArray(size_t length)
	: implement(new GMetaByteArrayImplement(length))
{
}

GMetaByteArray::~GMetaByteArray()
{
}

void * GMetaByteArray::getPointer() const
{
	return this->implement->getPointer();
}

void * GMetaByteArray::getCurrentPointer() const
{
	return this->implement->getCurrentPointer();
}

size_t GMetaByteArray::getPosition() const
{
	return this->implement->position;
}

void GMetaByteArray::setPosition(size_t position)
{
	this->implement->setPosition(position);
}

size_t GMetaByteArray::getLength() const
{
	return this->implement->length;
}

void GMetaByteArray::setLength(size_t length) const
{
	this->implement->setLength(length);
}

int8_t GMetaByteArray::readInt8() const
{
	return this->implement->read<int8_t>();
}

int16_t GMetaByteArray::readInt16() const
{
	return this->implement->read<int16_t>();
}

int32_t GMetaByteArray::readInt32() const
{
	return this->implement->read<int32_t>();
}

int64_t GMetaByteArray::readInt64() const
{
	return this->implement->read<int64_t>();
}


uint8_t GMetaByteArray::readUint8() const
{
	return this->implement->read<uint8_t>();
}

uint16_t GMetaByteArray::readUint16() const
{
	return this->implement->read<uint16_t>();
}

uint32_t GMetaByteArray::readUint32() const
{
	return this->implement->read<uint32_t>();
}

uint64_t GMetaByteArray::readUint64() const
{
	return this->implement->read<uint64_t>();
}

float GMetaByteArray::readFloat32() const
{
	return this->implement->read<float>();
}

double GMetaByteArray::readFloat64() const
{
	return this->implement->read<double>();
}

void GMetaByteArray::readBuffer(void * buffer, size_t length) const
{
    this->implement->readBuffer(buffer, length);
}

void GMetaByteArray::writeInt8(int8_t value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<int8_t>(value, moreValues);
}

void GMetaByteArray::writeInt16(int16_t value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<int16_t>(value, moreValues);
}

void GMetaByteArray::writeInt32(int32_t value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<int32_t>(value, moreValues);
}

void GMetaByteArray::writeInt64(int64_t value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<int64_t>(value, moreValues);
}

void GMetaByteArray::writeUint8(uint8_t value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<uint8_t>(value, moreValues);
}

void GMetaByteArray::writeUint16(uint16_t value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<uint16_t>(value, moreValues);
}

void GMetaByteArray::writeUint32(uint32_t value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<uint32_t>(value, moreValues);
}

void GMetaByteArray::writeUint64(uint64_t value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<uint64_t>(value, moreValues);
}

void GMetaByteArray::writeFloat32(float value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<float>(value, moreValues);
}

void GMetaByteArray::writeFloat64(double value, const GMetaVariadicParam * moreValues)
{
	this->implement->write<double>(value, moreValues);
}

void GMetaByteArray::writeBuffer(const void * buffer, size_t length)
{
    this->implement->writeBuffer(buffer, length);
}


}

