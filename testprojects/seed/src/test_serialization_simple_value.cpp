#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>
#include <string>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename READER, typename AR>
void doTestSimpleValue(IMetaWriter * writer, const READER & reader, const AR & ar)
{
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(0, NULL, writer));

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

	serializeWriteValue(archiveWriter.get(), "b", b);
	serializeWriteValue(archiveWriter.get(), "c", c);
	serializeWriteValue(archiveWriter.get(), "wc", wc);
	serializeWriteValue(archiveWriter.get(), "sc", sc);
	serializeWriteValue(archiveWriter.get(), "uc", uc);
	serializeWriteValue(archiveWriter.get(), "si", si);
	serializeWriteValue(archiveWriter.get(), "usi", usi);
	serializeWriteValue(archiveWriter.get(), "i", i);
	serializeWriteValue(archiveWriter.get(), "ui", ui);
	serializeWriteValue(archiveWriter.get(), "l", l);
	serializeWriteValue(archiveWriter.get(), "ul", ul);
	serializeWriteValue(archiveWriter.get(), "ll", ll);
	serializeWriteValue(archiveWriter.get(), "ull", ull);
	serializeWriteValue(archiveWriter.get(), "f", f);
	serializeWriteValue(archiveWriter.get(), "df", df);
	serializeWriteValue(archiveWriter.get(), "ldf", ldf);
	serializeWriteValue(archiveWriter.get(), "s", s);
	serializeWriteValue(archiveWriter.get(), "ps", ps);

	ar.rewind();
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(0, NULL, reader.get()));
	
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

	serializeReadValue(archiveReader.get(), "b", rb);
	serializeReadValue(archiveReader.get(), "c", rc);
	serializeReadValue(archiveReader.get(), "wc", rwc);
	serializeReadValue(archiveReader.get(), "sc", rsc);
	serializeReadValue(archiveReader.get(), "uc", ruc);
	serializeReadValue(archiveReader.get(), "si", rsi);
	serializeReadValue(archiveReader.get(), "usi", rusi);
	serializeReadValue(archiveReader.get(), "i", ri);
	serializeReadValue(archiveReader.get(), "ui", rui);
	serializeReadValue(archiveReader.get(), "l", rl);
	serializeReadValue(archiveReader.get(), "ul", rul);
	serializeReadValue(archiveReader.get(), "ll", rll);
	serializeReadValue(archiveReader.get(), "ull", rull);
	serializeReadValue(archiveReader.get(), "f", rf);
	serializeReadValue(archiveReader.get(), "df", rdf);
	serializeReadValue(archiveReader.get(), "ldf", rldf);
	serializeReadValue(archiveReader.get(), "s", rs);
	serializeReadValue(archiveReader.get(), "ps", rps);

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

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	GScopedInterface<IMetaReader> reader(createTextStreamMetaReader(NULL, stream));
	
	doTestSimpleValue(writer.get(), MetaReaderGetter(reader.get()), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testSimpleValue_XML)
{
	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestSimpleValue(writer.get(), MetaReaderGetterXml(NULL, outputArchive), TestArchiveStreamNone());
	
//	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
