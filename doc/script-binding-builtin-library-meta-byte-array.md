<!--notoc-->

# Use built-in byte array in cpgf script binding

## Overview

The built-in library builtin.collections.bytearray provides a byte array class to ease script programming.

To load the core library, in script, call "cpgf.import("cpgf", "builtin.collections.bytearray")".

Note:
  * To be consistent, all below sample script code is written in JavaScript.
  * All sample code assumes that cpgf core service is reflected in namespace "cpgf".

## Create a byte array

Though we can allocate a byte array, there is global function createByteArray to create a byte array.
```javascript
var myByteArray = cpgf.createByteArray();
var myAnotherByteArray = cpgf.createByteArray(100);
```
myByteArray is an empty byte array. myAnotherByteArray is a byte array with 100 bytes allocated.

## Class GMetaByteArray

GMetaByteArray is the byte array class. The C++ code is self explained, so below list the class declaration and each member functions.
```c++
class GMetaByteArray {
public:
    GMetaByteArray();
    explicit GMetaByteArray(size_t length);

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

    void writeInt8(int8_t value, const GMetaVariadicParam * moreValues = NULL);
    void writeInt16(int16_t value, const GMetaVariadicParam * moreValues = NULL);
    void writeInt32(int32_t value, const GMetaVariadicParam * moreValues = NULL);
    void writeInt64(int64_t value, const GMetaVariadicParam * moreValues = NULL);

    void writeUint8(uint8_t value, const GMetaVariadicParam * moreValues = NULL);
    void writeUint16(uint16_t value, const GMetaVariadicParam * moreValues = NULL);
    void writeUint32(uint32_t value, const GMetaVariadicParam * moreValues = NULL);
    void writeUint64(uint64_t value, const GMetaVariadicParam * moreValues = NULL);
    
    void writeFloat32(float value, const GMetaVariadicParam * moreValues = NULL);
    void writeFloat64(double value, const GMetaVariadicParam * moreValues = NULL);

    void writeBuffer(const void * buffer, size_t length);
};
```

**The position concept**

Byte array is stream based array. Byte array maintains a current position. When reading or writing, the value is read or written at current position, and then the current position is adjusted forward according to the value size.

Current position can be got with function getPosition, and be set by function setPosition.

**The byte array memory**

Byte array maintains an internal memory buffer. When a value is written beyond the end of buffer, the buffer is grown automatically.

Using function getLength can query the buffer length. Function setLength can adjust the buffer length.

The function getPointer returns the address at the beginning of the buffer, the function getCurrentPointer returns the address at current position. Both functions are usable to pass binary data buffer to C++ from script (that's the essential purpose to introduce the byte array).

**The write functions**

Note most writeXXX functions, such as writeInt8, take an extra parameter which is default to NULL. That's a variadic parameter, so we can write more than one values at ones. For exmaple,
```javascript
myByteArray.writeInt8(1, 2, 3);
```
Will write three characters, 1, 2, and 3 to the byte array.

**Endian unaware**

The byte array is endian unaware. It just stores values in native endian. So don't transfer the data in a byte array across computers with different endian. For example, don't transfer data between Intel PC and ARM computer.
