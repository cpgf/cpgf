#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include "gmetatextstreamarchive.h"

#include "cpgf/ginterface.h"
#include "cpgf/gflags.h"
#include "cpgf/gfunctionextractor.h"

#include "gmetaarchivetypemap.h"

#include <sstream>
#include <string>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename AR>
void doTestSimpleValue(IMetaWriter * writer, IMetaReader * reader, const AR & ar)
{
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(GMetaArchiveConfig().getFlags(), NULL, writer));

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
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(GMetaArchiveConfig().getFlags(), NULL, reader));
	
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

	serializeReadValue(archiveReader.get(), "rb", &rb);
	serializeReadValue(archiveReader.get(), "rc", &rc);
	serializeReadValue(archiveReader.get(), "rwc", &rwc);
	serializeReadValue(archiveReader.get(), "rsc", &rsc);
	serializeReadValue(archiveReader.get(), "ruc", &ruc);
	serializeReadValue(archiveReader.get(), "rsi", &rsi);
	serializeReadValue(archiveReader.get(), "rusi", &rusi);
	serializeReadValue(archiveReader.get(), "ri", &ri);
	serializeReadValue(archiveReader.get(), "rui", &rui);
	serializeReadValue(archiveReader.get(), "rl", &rl);
	serializeReadValue(archiveReader.get(), "rul", &rul);
	serializeReadValue(archiveReader.get(), "rll", &rll);
	serializeReadValue(archiveReader.get(), "rull", &rull);
	serializeReadValue(archiveReader.get(), "rf", &rf);
	serializeReadValue(archiveReader.get(), "rdf", &rdf);
	serializeReadValue(archiveReader.get(), "rldf", &rldf);
	serializeReadValue(archiveReader.get(), "rs", &rs);
	serializeReadValue(archiveReader.get(), "rps", &rps);

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

GTEST(testSimpleValue)
{
	stringstream stream;

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GTextStreamMetaReader<stringstream> inputStream(NULL, stream);
	
	doTestSimpleValue(&outputStream, &inputStream, TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
