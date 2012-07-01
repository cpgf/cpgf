#include "cpgf/game/gio.h"

#include <string.h>


namespace cpgf {


bool isFileExist(const char * fileName)
{
	FILE * fileHandle = fopen(fileName, "rb");

	if(NULL == fileHandle) {
		return false;
	}
    fclose(fileHandle);

    return true;
}

IGInputStream * createInputStreamFromFile(const char * fileName)
{
	return new GFileInputStream(fileName);
}

IGOutputStream * createOutputStreamFromFile(const char * fileName)
{
	return new GFileOutputStream(fileName);
}

IGInputStream * createInputStreamFromStream(IGInputStream * stream, int maxCount, bool autoFreeStream)
{
	return new GStreamInputStream(stream, maxCount, autoFreeStream);
}

void streamWriteBool(IGOutputStream * stream, bool value)
{
	streamWriteByte(stream, value ? -1 : 0);
}

bool streamReadBool(IGInputStream * stream)
{
	return streamReadByte(stream) != 0;
}

void streamWriteByte(IGOutputStream * stream, gbyte value)
{
	stream->write(&value, 1);
}

gbyte streamReadByte(IGInputStream * stream)
{
	gbyte result;
	stream->read(&result, 1);
	
	return result;
}

void streamWriteInt16(IGOutputStream * stream, gint16 value)
{
	stream->write(&value, 2);
}

gint16 streamReadInt16(IGInputStream * stream)
{
	gint16 result;
	stream->read(&result, 2);
	
	return result;
}

void streamWriteInt32(IGOutputStream * stream, gint32 value)
{
	stream->write(&value, 4);
}

gint32 streamReadInt32(IGInputStream * stream)
{
	gint32 result;
	stream->read(&result, 4);
	
	return result;
}

void streamWriteInt64(IGOutputStream * stream, gint64 value)
{
	stream->write(&value, 8);
}

gint64 streamReadInt64(IGInputStream * stream)
{
	gint64 result;
	stream->read(&result, 8);
	
	return result;
}

void streamWriteArrayByte(IGOutputStream * stream, const gbyte * buffer, int count)
{
	stream->write(buffer, count * 1);
}

gbyte * streamReadArrayByte(IGInputStream * stream, gbyte * buffer, int count)
{
	if(buffer == NULL) {
		buffer = new gbyte[count];
	}
	
	stream->read(buffer, count * 1);
	
	return buffer;
}

void streamWriteArrayInt16(IGOutputStream * stream, const gint16 * buffer, int count)
{
	stream->write(buffer, count * 2);
}

gint16 * streamReadArrayInt16(IGInputStream * stream, gint16 * buffer, int count)
{
	if(buffer == NULL) {
		buffer = new gint16[count];
	}
	
	stream->read(buffer, count * 2);
	
	return buffer;
}

void streamWriteArrayInt32(IGOutputStream * stream, const gint32 * buffer, int count)
{
	stream->write(buffer, count * 4);
}

gint32 * streamReadArrayInt32(IGInputStream * stream, gint32 * buffer, int count)
{
	if(buffer == NULL) {
		buffer = new gint32[count];
	}
	
	stream->read(buffer, count * 4);
	
	return buffer;
}


GMemoryStream::GMemoryStream(int capacity) : capacity(capacity), length(0), offset(0)
{
}

GMemoryStream::~GMemoryStream()
{
}

int GMemoryStream::read(void * buffer, int count)
{
	this->requireMemory(0);
	
	int availableCount = this->availableByteCount();
	availableCount = (availableCount < count) ? availableCount : count;
	memmove(buffer, memory.get() + offset, availableCount * sizeof(gbyte));
	offset += availableCount;
	
	return availableCount;
}

int GMemoryStream::availableByteCount()
{
	return length - offset;
}

int GMemoryStream::seek(int offset, streamSeekMethod method)
{
	switch(method) {
		case seekFromBegin:
			offset = offset;
			break;
			
		case seekFromCurrent:
			offset += offset;
			break;
			
		case seekFromEnd:
			offset = length - offset - 1;
			break;
	}
	return offset;
}

int GMemoryStream::write(const void * buffer, int count)
{
	this->requireMemory(count);
	memmove(memory.get() + offset, buffer, count * sizeof(gbyte));
	offset += count;
	
	if(length < offset)
		length = offset;
	
	return count;
}

bool GMemoryStream::hasSpaceAvailable()
{
	return true;
}

int GMemoryStream::loadFromStream(IGInputStream * stream, int count)
{
	this->requireMemory(count);
	length = offset + count;
	return stream->read(memory.get() + offset, count);
}

void GMemoryStream::clear()
{
	offset = 0;

	memory.reset(NULL);
}

void GMemoryStream::requireMemory(int needCapacity)
{
	needCapacity += offset;
	if(memory) {
		if(needCapacity > capacity) {
			gbyte * newMemory = new gbyte[needCapacity + needCapacity / 8];
			memmove(newMemory, memory.get(), capacity * sizeof(gbyte));
			capacity = needCapacity;
			memory.reset(newMemory);
		}
	}
	else {
		if(needCapacity < capacity) {
			needCapacity = capacity;
		}
		
		capacity = needCapacity;
		memory.reset(new gbyte[needCapacity]);
	}
}



GFileInputStream::GFileInputStream(const char * fileName)
{
	fileHandle = fopen(fileName, "rb");
}

GFileInputStream::~GFileInputStream()
{
	if(fileHandle != NULL) {
		fclose(fileHandle);
		fileHandle = NULL;
	}
}

int GFileInputStream::read(void * buffer, int count)
{
	if(fileHandle == NULL) {
		return 0;
	}

	int len = fread(buffer, 1, count, fileHandle);

	return len;
}

int GFileInputStream::availableByteCount()
{
	if(fileHandle == NULL) {
		return 0;
	}

	int offset = ftell(fileHandle);
	fseek(fileHandle, 0, SEEK_END);
	int len = ftell(fileHandle);
	fseek(fileHandle, offset, SEEK_SET);

	return len;
}

int GFileInputStream::seek(int offset, streamSeekMethod method)
{
	if(fileHandle == NULL) {
		return 0;
	}

	switch(method) {
		case seekFromBegin:
			fseek(fileHandle, offset, SEEK_SET);
			break;

		case seekFromCurrent:
			fseek(fileHandle, offset, SEEK_CUR);
			break;

		case seekFromEnd:
			fseek(fileHandle, offset, SEEK_END);
			break;
	}
	return ftell(fileHandle);
}




GFileOutputStream::GFileOutputStream(const char * fileName)
{
	fileHandle = fopen(fileName, "wb");
}

GFileOutputStream::~GFileOutputStream()
{
	if(fileHandle != NULL) {
		fclose(fileHandle);
		fileHandle = NULL;
	}
}

int GFileOutputStream::write(const void * buffer, int count)
{
	int len = fwrite(buffer, 1, count, fileHandle);

	return len;
}

bool GFileOutputStream::hasSpaceAvailable()
{
	return true;
}



GFileInOutStream::GFileInOutStream(const char * fileName)
{
	fileHandle = fopen(fileName, "rb+");

	if(NULL == fileHandle) {
    	fileHandle = fopen(fileName, "wb+");
    }
}

GFileInOutStream::~GFileInOutStream()
{
	if(fileHandle != NULL) {
		fclose(fileHandle);
		fileHandle = NULL;
	}
}

int GFileInOutStream::read(void * buffer, int count)
{
	if(fileHandle == NULL) {
		return 0;
	}
	
	int len = fread(buffer, 1, count, fileHandle);
	
	return len;
}

int GFileInOutStream::availableByteCount()
{
	if(fileHandle == NULL) {
		return 0;
	}

	int offset = ftell(fileHandle);
	fseek(fileHandle, 0, SEEK_END);
	int len = ftell(fileHandle);
	fseek(fileHandle, offset, SEEK_SET);

	return len;
}

int GFileInOutStream::seek(int offset, streamSeekMethod method)
{
	if(fileHandle == NULL) {
		return 0;
	}

	switch(method) {
		case seekFromBegin:
			fseek(fileHandle, offset, SEEK_SET);
			break;

		case seekFromCurrent:
			fseek(fileHandle, offset, SEEK_CUR);
			break;

		case seekFromEnd:
			fseek(fileHandle, offset, SEEK_END);
			break;
	}
	return ftell(fileHandle);
}

int GFileInOutStream::write(const void * buffer, int count)
{
	int len = fwrite(buffer, 1, count, fileHandle);

	return len;
}

bool GFileInOutStream::hasSpaceAvailable()
{
    return true;
}



GStreamInputStream::GStreamInputStream(IGInputStream * sourceStream, int count, bool autoFreeStream)
{
	this->offset = 0;
	this->autoFreeStream = autoFreeStream;
	this->stream = sourceStream;
	this->originalOffset = sourceStream->seek(0, seekFromCurrent);
	this->maxCount = count;
	if(this->maxCount <= 0) {
		this->maxCount = this->stream->availableByteCount();
	}
}

GStreamInputStream::~GStreamInputStream()
{
	if(autoFreeStream) {
		delete stream;
	}
}

int GStreamInputStream::read(void * buffer, int count)
{
	int readCount = this->availableByteCount();

	if(readCount >= count) {
		readCount = count;
	}
	readCount = stream->read(buffer, count);

	this->offset = stream->seek(0, seekFromCurrent) - originalOffset;
	
	return readCount;
}

int GStreamInputStream::availableByteCount()
{
	return maxCount - (this->offset - originalOffset);
}

int GStreamInputStream::seek(int offset, streamSeekMethod method)
{
	switch(method) {
		case seekFromBegin:
			offset = offset;
			break;
			
		case seekFromCurrent:
			offset += offset;
			break;
			
		case seekFromEnd:
			offset = maxCount - offset - 1;
			break;
	}
	if(offset < 0) {
		offset = 0;
	}
	if(offset > maxCount) {
		offset = maxCount;
	}
	offset = stream->seek(offset + originalOffset, seekFromBegin) - originalOffset;
	return offset;
}


} // namespace cpgf
