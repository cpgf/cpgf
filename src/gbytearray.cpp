#include "cpgf/gbytearray.h"
#include "cpgf/gbytearrayapi.h"
#include "cpgf/gexception.h"

#include "pinclude/gapiimpl.h"

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

void GByteArray::writeBuffer(const void * buffer, size_t length)
{
    this->implement->writeBuffer(buffer, length);
}


class ImplByteArray : public ImplExtendObject, public IByteArray
{
public:
	ImplByteArray();
    ImplByteArray(GByteArray * byteArray, bool freeByteArray);
    ~ImplByteArray();

    IMPL_OBJECT
	IMPL_EXTENDOBJECT

protected:
    virtual void * G_API_CC getMemory();

    virtual uint32_t G_API_CC getPosition();
    virtual void G_API_CC setPosition(uint32_t position);

    virtual uint32_t G_API_CC getLength();
    virtual void G_API_CC setLength(uint32_t length);

    virtual int8_t G_API_CC readInt8();
    virtual int16_t G_API_CC readInt16();
    virtual int32_t G_API_CC readInt32();
    virtual int64_t G_API_CC readInt64();

    virtual uint8_t G_API_CC  readUint8();
    virtual uint16_t G_API_CC readUint16();
    virtual uint32_t G_API_CC readUint32();
    virtual uint64_t G_API_CC readUint64();

    virtual void G_API_CC readBuffer(void * buffer, uint32_t length);

    virtual void G_API_CC writeInt8(int8_t value);
    virtual void G_API_CC writeInt16(int16_t value);
    virtual void G_API_CC writeInt32(int32_t value);
    virtual void G_API_CC writeInt64(int64_t value);

    virtual void G_API_CC writeUint8(uint8_t value);
    virtual void G_API_CC writeUint16(uint16_t value);
    virtual void G_API_CC writeUint32(uint32_t value);
    virtual void G_API_CC writeUint64(uint64_t value);

    virtual void G_API_CC writeBuffer(const void * buffer, uint32_t length);

private:
	GByteArray * byteArray;
    bool freeByteArray;
};

#define ENTER_BYTEARRAY_API() \
	this->clearError(); \
	try {

#define LEAVE_BYTEARRAY_API(...) \
	} \
	catch(const GException & e) { this->handleError(e.getCode(), e.getMessage()); __VA_ARGS__; } \
	catch(const std::exception & e) { this->handleError(0, e.what()); __VA_ARGS__; } \
	catch(...) { this->handleError(0, "Unknow exception."); __VA_ARGS__; }

ImplByteArray::ImplByteArray()
	: byteArray(new GByteArray), freeByteArray(true)
{
}

ImplByteArray::ImplByteArray(GByteArray * byteArray, bool freeByteArray)
	: byteArray(byteArray), freeByteArray(freeByteArray)
{
}

ImplByteArray::~ImplByteArray()
{
	if(this->freeByteArray) {
    	delete this->byteArray;
    }
}

void * G_API_CC ImplByteArray::getMemory()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->getMemory();

    LEAVE_BYTEARRAY_API(return NULL)
}

uint32_t G_API_CC ImplByteArray::getPosition()
{
	ENTER_BYTEARRAY_API()

    return static_cast<uint32_t>(this->byteArray->getPosition());

    LEAVE_BYTEARRAY_API(return 0)
}

void G_API_CC ImplByteArray::setPosition(uint32_t position)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->setPosition(position);

    LEAVE_BYTEARRAY_API()
}

uint32_t G_API_CC ImplByteArray::getLength()
{
	ENTER_BYTEARRAY_API()

    return static_cast<uint32_t>(this->byteArray->getLength());

    LEAVE_BYTEARRAY_API(return 0)
}

void G_API_CC ImplByteArray::setLength(uint32_t length)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->setLength(length);

    LEAVE_BYTEARRAY_API()
}

int8_t G_API_CC ImplByteArray::readInt8()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readInt8();

    LEAVE_BYTEARRAY_API(return 0)
}

int16_t G_API_CC ImplByteArray::readInt16()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readInt16();

    LEAVE_BYTEARRAY_API(return 0)
}

int32_t G_API_CC ImplByteArray::readInt32()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readInt32();

    LEAVE_BYTEARRAY_API(return 0)
}

int64_t G_API_CC ImplByteArray::readInt64()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readInt64();

    LEAVE_BYTEARRAY_API(return 0)
}

uint8_t G_API_CC ImplByteArray:: readUint8()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readUint8();

    LEAVE_BYTEARRAY_API(return 0)
}

uint16_t G_API_CC ImplByteArray::readUint16()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readUint16();

    LEAVE_BYTEARRAY_API(return 0)
}

uint32_t G_API_CC ImplByteArray::readUint32()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readUint32();

    LEAVE_BYTEARRAY_API(return 0)
}

uint64_t G_API_CC ImplByteArray::readUint64()
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readUint64();

    LEAVE_BYTEARRAY_API(return 0)
}

void G_API_CC ImplByteArray::readBuffer(void * buffer, uint32_t length)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->readBuffer(buffer, length);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeInt8(int8_t value)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeInt8(value);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeInt16(int16_t value)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeInt16(value);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeInt32(int32_t value)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeInt32(value);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeInt64(int64_t value)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeInt64(value);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeUint8(uint8_t value)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeUint8(value);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeUint16(uint16_t value)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeUint16(value);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeUint32(uint32_t value)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeUint32(value);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeUint64(uint64_t value)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeUint64(value);

    LEAVE_BYTEARRAY_API()
}

void G_API_CC ImplByteArray::writeBuffer(const void * buffer, uint32_t length)
{
	ENTER_BYTEARRAY_API()

    return this->byteArray->writeBuffer(buffer, length);

    LEAVE_BYTEARRAY_API()
}


IByteArray * byteArrayToInterface(GByteArray * byteArray, bool freeByteArrayByInterface)
{
	return new ImplByteArray(byteArray, freeByteArrayByInterface);
}


}

