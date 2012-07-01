#ifndef __GIO_H
#define __GIO_H

#include <stdio.h>

#include "cpgf/game/gcore.h"
#include "cpgf/gscopedptr.h"

namespace cpgf {

typedef enum {
	seekFromBegin = 0,
	seekFromCurrent = 1,
	seekFromEnd = 2
} streamSeekMethod;

struct IGInputStream : virtual public IGInterface {
public:
	virtual int read(void * buffer, int count) = 0;
	virtual int availableByteCount() = 0;
	virtual int seek(int offset, streamSeekMethod method) = 0;
};

struct IGOutputStream : virtual public IGInterface {
public:
	virtual int write(const void * buffer, int count) = 0;
	virtual bool hasSpaceAvailable() = 0;
};

struct IGInOutStream : virtual public IGInterface, public IGInputStream, public IGOutputStream {
public:
	virtual int read(void * buffer, int count) = 0;
	virtual int availableByteCount() = 0;
	virtual int seek(int offset, streamSeekMethod method) = 0;

	virtual int write(const void * buffer, int count) = 0;
	virtual bool hasSpaceAvailable() = 0;
};


class GMemoryStream : public GObject, public IGInOutStream {
public:
	GMemoryStream(int capacity);
	~GMemoryStream();

	int read(void * buffer, int count);
	int availableByteCount();
	int seek(int offset, streamSeekMethod method);
	int write(const void * buffer, int count);
	bool hasSpaceAvailable();
	int loadFromStream(IGInputStream * stream, int count);
	void clear();
	void requireMemory(int needCapacity);

private:
	int capacity;
	int length;
	int offset;
	GScopedArray<gbyte> memory;
};


class GFileInputStream : public GObject, public IGInputStream {
public:
	GFileInputStream(const char * fileName);
	~GFileInputStream();

	int read(void * buffer, int count);
	int availableByteCount();
	int seek(int offset, streamSeekMethod method);

private:
	FILE * fileHandle;
};


class GFileOutputStream : public GObject, public IGOutputStream {
public:
	GFileOutputStream(const char * fileName);
	~GFileOutputStream();

	int write(const void * buffer, int count);
	bool hasSpaceAvailable();

private:
	FILE * fileHandle;
};


class GFileInOutStream : public GObject, public IGInOutStream {
public:
	GFileInOutStream(const char * fileName);
	~GFileInOutStream();

	int read(void * buffer, int count);
	int availableByteCount();
	int seek(int offset, streamSeekMethod method);

	int write(const void * buffer, int count);
	bool hasSpaceAvailable();

private:
	FILE * fileHandle;
};


class GStreamInputStream : public GObject, public IGInputStream {
public:
	GStreamInputStream(IGInputStream * sourceStream, int count, bool autoFreeStream);
	~GStreamInputStream();

	int read(void * buffer, int count);
	int availableByteCount();
	int seek(int offset, streamSeekMethod method);

private:
	bool autoFreeStream;
	IGInputStream * stream;
	int maxCount;
	int originalOffset;
	int offset;
};

bool isFileExist(const char * fileName);

IGInputStream * createInputStreamFromFile(const char *fileName);
IGOutputStream * createOutputStreamFromFile(const char *fileName);
IGInputStream * createInputStreamFromStream(IGInputStream *stream, int maxCount, bool autoFreeStream);

// these functions ensure the endian is correct
void streamWriteBool(IGOutputStream * stream, bool value);
bool streamReadBool(IGInputStream * stream);

void streamWriteByte(IGOutputStream * stream, gbyte value);
gbyte streamReadByte(IGInputStream * stream);

void streamWriteInt16(IGOutputStream * stream, gint16 value);
gint16 streamReadInt16(IGInputStream * stream);

void streamWriteInt32(IGOutputStream * stream, gint32 value);
gint32 streamReadInt32(IGInputStream * stream);

gint64 streamReadInt64(IGInputStream * stream);
void streamWriteInt64(IGOutputStream * stream, gint64 value);

gbyte * streamReadArrayByte(IGInputStream * stream, gbyte * buffer, int count);
void streamWriteArrayByte(IGOutputStream * stream, const gbyte * buffer, int count);

void streamWriteArrayInt16(IGOutputStream * stream, const gint16 * buffer, int count);
gint16 * streamReadArrayInt16(IGInputStream * stream, gint16 * buffer, int count);

void streamWriteArrayInt32(IGOutputStream * stream, const gint32 * buffer, int count);
gint32 * streamReadArrayInt32(IGInputStream * stream, gint32 * buffer, int count);


} // namespace cpgf


#endif

