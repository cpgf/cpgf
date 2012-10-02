#ifndef __GBYTEARRAY_H
#define __GBYTEARRAY_H

#include "cpgf/gstdint.h"
#include "cpgf/gscopedptr.h"
#include "cpgf/gapi.h"

#include <stddef.h>

namespace cpgf {

class GByteArrayImplement;

// always '\0' ended, and endian unware
class GByteArray {
public:
	GByteArray();
    explicit GByteArray(size_t length);
	~GByteArray();

    void * getMemory() const;
    void * getCurrentMemory() const;

    size_t getPosition() const;
    void setPosition(size_t position);

    size_t getLength() const;
    void setLength(size_t length) const;

    int8_t readInt8() const;
    int16_t readInt16() const;
    int32_t readInt32() const;
    int64_t readInt64() const;

    uint8_t readUint8() const;
    uint16_t readUint16() const;
    uint32_t readUint32() const;
    uint64_t readUint64() const;
    
    float readFloat32() const;
    double readFloat64() const;

    void readBuffer(void * buffer, size_t length) const;

    void writeInt8(int8_t value);
    void writeInt16(int16_t value);
    void writeInt32(int32_t value);
    void writeInt64(int64_t value);

    void writeUint8(uint8_t value);
    void writeUint16(uint16_t value);
    void writeUint32(uint32_t value);
    void writeUint64(uint64_t value);
    
    void writeFloat32(float value);
    void writeFloat64(double value);

    void writeBuffer(const void * buffer, size_t length);
private:
	GScopedPointer<GByteArrayImplement> implement;
};

struct IByteArray : IExtendObject {
    virtual void * G_API_CC getMemory() = 0;
    virtual void * G_API_CC getCurrentMemory() = 0;

    virtual uint32_t G_API_CC getPosition() = 0;
    virtual void G_API_CC setPosition(uint32_t position) = 0;

    virtual uint32_t G_API_CC getLength() = 0;
    virtual void G_API_CC setLength(uint32_t length) = 0;

    virtual int8_t G_API_CC readInt8() = 0;
    virtual int16_t G_API_CC readInt16() = 0;
    virtual int32_t G_API_CC readInt32() = 0;
    virtual int64_t G_API_CC readInt64() = 0;

    virtual uint8_t G_API_CC  readUint8() = 0;
    virtual uint16_t G_API_CC readUint16() = 0;
    virtual uint32_t G_API_CC readUint32() = 0;
    virtual uint64_t G_API_CC readUint64() = 0;
    
    virtual float G_API_CC readFloat32() = 0;
    virtual double G_API_CC readFloat64() = 0;

    virtual void G_API_CC readBuffer(void * buffer, uint32_t length) = 0;

    virtual void G_API_CC writeInt8(int8_t value) = 0;
    virtual void G_API_CC writeInt16(int16_t value) = 0;
    virtual void G_API_CC writeInt32(int32_t value) = 0;
    virtual void G_API_CC writeInt64(int64_t value) = 0;

    virtual void G_API_CC writeUint8(uint8_t value) = 0;
    virtual void G_API_CC writeUint16(uint16_t value) = 0;
    virtual void G_API_CC writeUint32(uint32_t value) = 0;
    virtual void G_API_CC writeUint64(uint64_t value) = 0;

    virtual void G_API_CC writeFloat32(float value) = 0;
    virtual void G_API_CC writeFloat64(double value) = 0;
    
    virtual void G_API_CC writeBuffer(const void * buffer, uint32_t length) = 0;
};


IByteArray * byteArrayToInterface(GByteArray * byteArray, bool freeByteArrayByInterface);

}



#endif


