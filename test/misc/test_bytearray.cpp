#include "test_misc_common.h"

#include "cpgf/gscopedptr.h"
#include "cpgf/metautility/gmetabytearray.h"
#include "cpgf/gvariant.h"

using namespace cpgf;


namespace Test_ByteArray { namespace {


template <typename T>
void testByteArrayLengthAndPosition(T * byteArray)
{
	GEQUAL(byteArray->getLength(), 0);
	GEQUAL(byteArray->getPosition(), 0);

    byteArray->writeInt8(1);
	GEQUAL(byteArray->getLength(), 1);
	GEQUAL(byteArray->getPosition(), 1);

    byteArray->setPosition(0);
	GEQUAL(byteArray->getPosition(), 0);

    byteArray->setPosition(100);
	GEQUAL(byteArray->getLength(), 1);
	GEQUAL(byteArray->getPosition(), 1);

    byteArray->setLength(5);
	GEQUAL(byteArray->getLength(), 5);
	GEQUAL(byteArray->getPosition(), 1);

    byteArray->setLength(0);
	GEQUAL(byteArray->getLength(), 0);
	GEQUAL(byteArray->getPosition(), 0);
}

template <typename T>
void testByteArrayNullEnd(T * byteArray)
{
	char * p;

    byteArray->setLength(0);

    byteArray->writeInt8(1);
    byteArray->writeInt8(2);

    p = (char *)(byteArray->getPointer());
    GDIFF(*(p + 1), 0);

    byteArray->setLength(1);
    byteArray->setPosition(0);

    byteArray->writeInt8(1);

    p = (char *)(byteArray->getPointer());
    GEQUAL(*(p + 1), 0);
}

template <typename T>
void testByteArrayWriteAndRead(T * byteArray)
{
	const char bufferSample[] = {
    	9, 8, 5, 3, 1
    };
    const int bufferSize = sizeof(bufferSample) / sizeof(bufferSample[0]);
    char buffer[bufferSize];

    byteArray->setLength(0);

    byteArray->writeInt8(1);
    byteArray->writeInt16(2);
    byteArray->writeInt32(3);
    byteArray->writeInt64(4);
    byteArray->writeUint8(0x7f);
    byteArray->writeUint16(0x1234);
    byteArray->writeUint32(0xff000);
    byteArray->writeUint64(0x789012345LL);
    byteArray->writeFloat32(1.5f);
    byteArray->writeFloat64(2.8);
    byteArray->writeBuffer(bufferSample, bufferSize);

    byteArray->setPosition(0);
    GEQUAL(byteArray->readInt8(), 1);
    GEQUAL(byteArray->readInt16(), 2);
    GEQUAL(byteArray->readInt32(), 3);
    GEQUAL(byteArray->readInt64(), 4);
    GEQUAL(byteArray->readUint8(), 0x7f);
    GEQUAL(byteArray->readUint16(), 0x1234);
    GEQUAL(byteArray->readUint32(), 0xff000);
    GEQUAL(byteArray->readUint64(), 0x789012345LL);
    GEQUAL(byteArray->readFloat32(), 1.5f);
    GEQUAL(byteArray->readFloat64(), 2.8);
    byteArray->readBuffer(buffer, bufferSize);
    for(int i = 0; i < bufferSize; ++i) {
    	GEQUAL(buffer[i], bufferSample[i]);
    }
}

template <typename T>
void testByteArray(T * byteArray)
{
	testByteArrayLengthAndPosition(byteArray);
    testByteArrayNullEnd(byteArray);
    testByteArrayWriteAndRead(byteArray);
}

GTEST(TestByteArray)
{
	GScopedPointer<GMetaByteArray> byteArray(new GMetaByteArray);

    testByteArray(byteArray.get());
}



} }

