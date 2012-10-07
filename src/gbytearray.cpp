#include "cpgf/gbytearray.h"
#include "cpgf/gexception.h"
#include "cpgf/gapiutil.h"

#include <vector>

#include <string.h>


using namespace std;

namespace cpgf {

class GByteArrayImplement
{
public:
	typedef unsigned char ByteType;
	typedef vector<ByteType> ArrayType;

public:
	GByteArrayImplement()
    	: byteArray(), length(0), position(0) {
    }

    explicit GByteArrayImplement(size_t length)
    	: byteArray(length + 1), length(length), position(0) {
        this->byteArray[0] = 0;
    }

    void * getMemory() const {
    	return length > 0 ? const_cast<ByteType *>(&byteArray[0]) : NULL;
    }

    void * getCurrentMemory() const {
       	return static_cast<uint8_t *>(this->getMemory()) + this->position;
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
        	static_cast<uint8_t *>(this->getMemory()) + currentPosition
        );
    }

    template <typename T>
    void write(T value) {
    	this->needSpace(sizeof(T));

        *reinterpret_cast<T *>(
        	static_cast<uint8_t *>(this->getMemory()) + this->position
        ) = value;
        this->position += sizeof(T);
        if(this->position == this->length) {
        	*static_cast<uint8_t *>(this->getCurrentMemory()) = 0;
        }
    }

    void readBuffer(void * buffer, size_t length)
    {
        memmove(buffer, this->getCurrentMemory(), length);
        this->position += length;
    }
    void writeBuffer(const void * buffer, size_t length)
    {
        this->needSpace(length);
        memmove(this->getCurrentMemory(), buffer, length);
        this->position += length;
        if(this->position == this->length) {
        	*static_cast<uint8_t *>(this->getCurrentMemory()) = 0;
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

GByteArray::GByteArray()
	: implement(new GByteArrayImplement())
{
}

GByteArray::GByteArray(size_t length)
	: implement(new GByteArrayImplement(length))
{
}

GByteArray::~GByteArray()
{
}

void * GByteArray::getMemory() const
{
	return this->implement->getMemory();
}

void * GByteArray::getCurrentMemory() const
{
	return this->implement->getCurrentMemory();
}

size_t GByteArray::getPosition() const
{
	return this->implement->position;
}

void GByteArray::setPosition(size_t position)
{
	this->implement->setPosition(position);
}

size_t GByteArray::getLength() const
{
	return this->implement->length;
}

void GByteArray::setLength(size_t length) const
{
	this->implement->setLength(length);
}

int8_t GByteArray::readInt8() const
{
	return this->implement->read<int8_t>();
}

int16_t GByteArray::readInt16() const
{
	return this->implement->read<int16_t>();
}

int32_t GByteArray::readInt32() const
{
	return this->implement->read<int32_t>();
}

int64_t GByteArray::readInt64() const
{
	return this->implement->read<int64_t>();
}


uint8_t GByteArray::readUint8() const
{
	return this->implement->read<uint8_t>();
}

uint16_t GByteArray::readUint16() const
{
	return this->implement->read<uint16_t>();
}

uint32_t GByteArray::readUint32() const
{
	return this->implement->read<uint32_t>();
}

uint64_t GByteArray::readUint64() const
{
	return this->implement->read<uint64_t>();
}

float GByteArray::readFloat32() const
{
	return this->implement->read<float>();
}

double GByteArray::readFloat64() const
{
	return this->implement->read<double>();
}

void GByteArray::readBuffer(void * buffer, size_t length) const
{
    this->implement->readBuffer(buffer, length);
}

void GByteArray::writeInt8(int8_t value)
{
	this->implement->write<int8_t>(value);
}

void GByteArray::writeInt16(int16_t value)
{
	this->implement->write<int16_t>(value);
}

void GByteArray::writeInt32(int32_t value)
{
	this->implement->write<int32_t>(value);
}

void GByteArray::writeInt64(int64_t value)
{
	this->implement->write<int64_t>(value);
}

void GByteArray::writeUint8(uint8_t value)
{
	this->implement->write<uint8_t>(value);
}

void GByteArray::writeUint16(uint16_t value)
{
	this->implement->write<uint16_t>(value);
}

void GByteArray::writeUint32(uint32_t value)
{
	this->implement->write<uint32_t>(value);
}

void GByteArray::writeUint64(uint64_t value)
{
	this->implement->write<uint64_t>(value);
}

void GByteArray::writeFloat32(float value)
{
	this->implement->write<float>(value);
}

void GByteArray::writeFloat64(double value)
{
	this->implement->write<double>(value);
}

void GByteArray::writeBuffer(const void * buffer, size_t length)
{
    this->implement->writeBuffer(buffer, length);
}


}

