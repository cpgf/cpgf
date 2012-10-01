#ifndef __GBYTEARRAYAPI_H
#define __GBYTEARRAYAPI_H

#include "cpgf/gapi.h"


namespace cpgf {

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


}



#endif


