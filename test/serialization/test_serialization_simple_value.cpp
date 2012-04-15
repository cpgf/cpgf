#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>
#include <string>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename AR>
void doTestSimpleValue(IMetaStorageWriter * writer, IMetaStorageReader * reader, const AR & ar)
{
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(NULL, writer));

	bool b = true;
	char c = 5;
	wchar_t wc = 7;
	signed char sc = -90;
	unsigned char uc = 180;
	short si = -38;
	unsigned short usi = 98;
	int i = 12345;
	unsigned int ui = 98765;
	long l = -1002;
	unsigned long ul = 120909;
	long long ll = -0xbeefdeadLL;
	unsigned long long ull = 0xdeef;
	float f = 1987.5;
	double df = -0.056;
	long double ldf = 13123.123;
	string s = "abc";
	string * ps = &s;

	serializeWriteData(archiveWriter.get(), "b", b);
	serializeWriteData(archiveWriter.get(), "c", c);
	serializeWriteData(archiveWriter.get(), "wc", wc);
	serializeWriteData(archiveWriter.get(), "sc", sc);
	serializeWriteData(archiveWriter.get(), "uc", uc);
	serializeWriteData(archiveWriter.get(), "si", si);
	serializeWriteData(archiveWriter.get(), "usi", usi);
	serializeWriteData(archiveWriter.get(), "i", i);
	serializeWriteData(archiveWriter.get(), "ui", ui);
	serializeWriteData(archiveWriter.get(), "l", l);
	serializeWriteData(archiveWriter.get(), "ul", ul);
	serializeWriteData(archiveWriter.get(), "ll", ll);
	serializeWriteData(archiveWriter.get(), "ull", ull);
	serializeWriteData(archiveWriter.get(), "f", f);
	serializeWriteData(archiveWriter.get(), "df", df);
	serializeWriteData(archiveWriter.get(), "ldf", ldf);
	serializeWriteData(archiveWriter.get(), "s", s);
	serializeWriteData(archiveWriter.get(), "ps", ps);

	TestArchiveTraits<AR>::rewind(ar);
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(NULL, reader));
	
	bool rb = false;
	char rc = 0;
	wchar_t rwc = 0;
	signed char rsc = 0;
	unsigned char ruc = 0;
	short rsi = 0;
	unsigned short rusi = 0;
	int ri = 0;
	unsigned int rui = 0;
	long rl = 0;
	unsigned long rul = 0;
	long long rll = 0;
	unsigned long long rull = 0;
	float rf = 0;
	double rdf = 0;
	long double rldf = 0;
	string rs = "";
	string * rps = NULL;

	serializeReadData(archiveReader.get(), "b", rb);
	serializeReadData(archiveReader.get(), "c", rc);
	serializeReadData(archiveReader.get(), "wc", rwc);
	serializeReadData(archiveReader.get(), "sc", rsc);
	serializeReadData(archiveReader.get(), "uc", ruc);
	serializeReadData(archiveReader.get(), "si", rsi);
	serializeReadData(archiveReader.get(), "usi", rusi);
	serializeReadData(archiveReader.get(), "i", ri);
	serializeReadData(archiveReader.get(), "ui", rui);
	serializeReadData(archiveReader.get(), "l", rl);
	serializeReadData(archiveReader.get(), "ul", rul);
	serializeReadData(archiveReader.get(), "ll", rll);
	serializeReadData(archiveReader.get(), "ull", rull);
	serializeReadData(archiveReader.get(), "f", rf);
	serializeReadData(archiveReader.get(), "df", rdf);
	serializeReadData(archiveReader.get(), "ldf", rldf);
	serializeReadData(archiveReader.get(), "s", rs);
	serializeReadData(archiveReader.get(), "ps", rps);

	GEQUAL(b, rb);
	GEQUAL(c, rc);
	GEQUAL(wc, rwc);
	GEQUAL(sc, rsc);
	GEQUAL(uc, ruc);
	GEQUAL(si, rsi);
	GEQUAL(usi, rusi);
	GEQUAL(i, ri);
	GEQUAL(ui, rui);
	GEQUAL(l, rl);
	GEQUAL(ul, rul);
	GEQUAL(ll, rll);
	GEQUAL(ull, rull);
	GFEQUAL(f, rf);
	GFEQUAL(df, rdf);
	GCHECK(fabs(ldf - rldf) < 0.1);
	GEQUAL(s, rs);
	GEQUAL(rps, &rs);
}

GTEST(testSimpleValue_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestSimpleValue(writer.get(), reader.get(), stream);

//	cout << stream.str().c_str() << endl;
}


GTEST(testSimpleValue_XML)
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestSimpleValue(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


GTEST(testSimpleValue_Json)
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestSimpleValue(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}




} // unnamed namespace
