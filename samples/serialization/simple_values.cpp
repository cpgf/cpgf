// to read object from archive
#include "cpgf/serialization/gmetaarchivereader.h"

// to write object to archive
#include "cpgf/serialization/gmetaarchivewriter.h"

// storage of text stream
#include "cpgf/serialization/gmetastorage_textstream.h"

// storage of XML
#include "cpgf/serialization/gmetastorage_xml.h"

// storage of JSON
#include "cpgf/serialization/gmetastorage_json.h"

// meta traits -- to support STL string
#include "cpgf/metatraits/gmetaserializer_string.h"

// meta traits -- to support C++ array (T[])
#include "cpgf/metatraits/gmetaserializer_array.h"


#include <sstream>
#include <string>

#include <iostream>

#include <math.h>

using namespace std;
using namespace cpgf;

void check(bool b);


void doTestSimpleValue(IMetaStorageWriter * writer, IMetaStorageReader * reader)
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

	serializeWriteData(archiveWriter.get(), "b", b); // we can also pass the constant directly, such as, serializeWriteData(archiveWriter.get(), "b", true);
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

	check(b == rb);
	check(c == rc);
	check(wc == rwc);
	check(sc == rsc);
	check(uc == ruc);
	check(si == rsi);
	check(usi == rusi);
	check(i == ri);
	check(ui == rui);
	check(l == rl);
	check(ul == rul);
	check(ll == rll);
	check(ull == rull);
	check(f == rf);
	check(fabs(df - rdf) < 0.1);
	check(fabs(ldf - rldf) < 0.1);
	check(s == rs);
	check(rps == &rs);
}

void testSimpleValue_TextStream()
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestSimpleValue(writer.get(), reader.get());

	cout << "Text stream" << endl;
	cout << stream.str().c_str() << endl;
}


void testSimpleValue_XML()
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestSimpleValue(writer.get(), reader.get());
	
	cout << "XML" << endl;
	storage.saveToStream(cout);
}


void testSimpleValue_Json()
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestSimpleValue(writer.get(), reader.get());
	
	cout << "JSON" << endl;
	storage.saveToStream(cout);
}


void testSimpleValue()
{
	cout << "Simple values: " << endl;
	
	testSimpleValue_TextStream();
	testSimpleValue_XML();
	testSimpleValue_Json();
}

