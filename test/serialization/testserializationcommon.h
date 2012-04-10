#ifndef __TESTSERIALIZATIONCOMMON_H
#define __TESTSERIALIZATIONCOMMON_H

#include "cpgf/serialization/gmetaarchivereader.h"
#include "cpgf/serialization/gmetaarchivewriter.h"

#include "cpgf/serialization/gmetatextstreamarchive.h"
#include "cpgf/serialization/gmetaxmlarchive.h"
#include "cpgf/serialization/gmetajsonarchive.h"

#include "cpgf/metatraits/gmetaserializer_string.h"
#include "cpgf/metatraits/gmetaserializer_array.h"

#include "cpgf/serialization/gmetaarchivecommon.h"

#include "../unittestbase.h"

#include <string>
#include <sstream>


#define FIELD(cls, n) ._field(# n, &cls::n)

template <typename STREAM>
class TestArchiveStream
{
public:
	explicit TestArchiveStream(STREAM & stream) : stream(stream) {}
	
	void rewind() const {
		this->stream.seekg(0);
	}

private:
	TestArchiveStream & operator = (const TestArchiveStream &);
	
private:
	STREAM & stream;
};

class TestArchiveStreamNone
{
public:
	void rewind() const {
	}
};


class MetaReaderGetter
{
public:
	explicit MetaReaderGetter(std::stringstream & outputStream);

	cpgf::IMetaReader * get(cpgf::IMetaService * service) const;

private:
	std::stringstream & outputStream;
	mutable cpgf::GScopedInterface<cpgf::IMetaReader> reader;
};

class MetaReaderGetterXml
{
public:
	explicit MetaReaderGetterXml(cpgf::GMetaXmlArchive & outputArchive);

	cpgf::IMetaReader * get(cpgf::IMetaService * service) const;

private:
	cpgf::GMetaXmlArchive & outputArchive;
	cpgf::GMetaXmlArchive inputArchive;
	mutable cpgf::GScopedInterface<cpgf::IMetaReader> reader;
};

class MetaReaderGetterJson
{
public:
	explicit MetaReaderGetterJson(cpgf::GMetaJsonArchive & outputArchive);

	cpgf::IMetaReader * get(cpgf::IMetaService * service) const;

private:
	cpgf::GMetaJsonArchive & outputArchive;
	cpgf::GMetaJsonArchive inputArchive;
	mutable cpgf::GScopedInterface<cpgf::IMetaReader> reader;
};


template <typename T>
void initTestValue(T & value, long long seed)
{
	value = (T)(seed);
}

void initTestValue(bool & value, long long seed);
void initTestValue(char & value, long long seed);
void initTestValue(wchar_t & value, long long seed);
void initTestValue(signed char & value, long long seed);
void initTestValue(unsigned char & value, long long seed);
void initTestValue(float & value, long long seed);
void initTestValue(double & value, long long seed);
void initTestValue(long double & value, long long seed);
void initTestValue(std::string & value, long long seed);

long long getTestSeed(int n);


#endif

