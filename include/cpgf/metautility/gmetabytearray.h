#ifndef CPGF_GMETABYTEARRAY_H
#define CPGF_GMETABYTEARRAY_H

#include "cpgf/gscopedptr.h"
#include "cpgf/gapi.h"

#include <cstdint>
#include <stddef.h>

namespace cpgf {

struct GMetaVariadicParam;

class GMetaByteArrayImplement;

// always '\0' ended, and endian unware
class GMetaByteArray {
public:
	GMetaByteArray();
    explicit GMetaByteArray(size_t length);
	~GMetaByteArray();

    void * getPointer() const;
    void * getCurrentPointer() const;

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

    void writeInt8(int8_t value, const GMetaVariadicParam * moreValues = nullptr);
    void writeInt16(int16_t value, const GMetaVariadicParam * moreValues = nullptr);
    void writeInt32(int32_t value, const GMetaVariadicParam * moreValues = nullptr);
    void writeInt64(int64_t value, const GMetaVariadicParam * moreValues = nullptr);

    void writeUint8(uint8_t value, const GMetaVariadicParam * moreValues = nullptr);
    void writeUint16(uint16_t value, const GMetaVariadicParam * moreValues = nullptr);
    void writeUint32(uint32_t value, const GMetaVariadicParam * moreValues = nullptr);
    void writeUint64(uint64_t value, const GMetaVariadicParam * moreValues = nullptr);
    
    void writeFloat32(float value, const GMetaVariadicParam * moreValues = nullptr);
    void writeFloat64(double value, const GMetaVariadicParam * moreValues = nullptr);

    void writeBuffer(const void * buffer, size_t length);
private:
	GScopedPointer<GMetaByteArrayImplement> implement;
};


}



#endif


