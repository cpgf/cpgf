#ifndef __GBYTEARRAY_H
#define __GBYTEARRAY_H

#include "cpgf/gstdint.h"
#include "cpgf/gscopedptr.h"

#include <stddef.h>

namespace cpgf {

class GByteArrayImplement;

// always '\0' ended, and endian unware
class GByteArray {
public:
	GByteArray();
    explicit GByteArray(size_t length);

    void * getMemory() const;

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

    void readBuffer(void * buffer, size_t length) const;

    void writeInt8(int8_t value);
    void writeInt16(int16_t value);
    void writeInt32(int32_t value);
    void writeInt64(int64_t value);

    void writeUint8(uint8_t value);
    void writeUint16(uint16_t value);
    void writeUint32(uint32_t value);
    void writeUint64(uint64_t value);

    void writeBuffer(const void * buffer, size_t length);
private:
	GScopedPointer<GByteArrayImplement> implement;
};

struct IByteArray;

IByteArray * byteArrayToInterface(GByteArray * byteArray, bool freeByteArrayByInterface);

}



#endif


