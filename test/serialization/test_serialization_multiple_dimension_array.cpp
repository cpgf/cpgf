#include "testserializationcommonclasses.h"
#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>
#include <string>

#include <iostream>

using namespace std;
using namespace cpgf;


#define LOOP2(d1, d2) for(int z1 = 0; z1 < d1; ++z1) for(int z2 = 0; z2 < d2; ++z2)
#define LOOP3(d1, d2, d3) for(int z1 = 0; z1 < d1; ++z1) for(int z2 = 0; z2 < d2; ++z2) for(int z3 = 0; z3 < d3; ++z3)


namespace {

template <typename AR>
void doTestMultipleDimensionArray(IMetaStorageWriter * writer, IMetaStorageReader * reader, const AR & ar)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	enum {
		A1 = 3, A2 = 5,
		B1 = 5, B2 = 3, B3 = 2,
	};

	int i[A1][A2];
	string s2[A1][A2];
	long l[B1][B2][B3];
	string s[B1][B2][B3];
	string * ps[B1][B2][B3];
	TestSerializeClass o[B1][B2][B3];
	TestSerializeClass * po[B1][B2][B3];
	TestSerializeClass * npo[B1][B2][B3];

#define INIT2(v, d1, d2) LOOP2(d1, d2) initTestValue(v[z1][z2], getTestSeed(z1 * d2 + z2 + 1));
	INIT2(i, A1, A2)
	INIT2(s2, A1, A2)
#undef INIT2

#define INIT3(v, d1, d2, d3) LOOP3(d1, d2, d3) initTestValue(v[z1][z2][z3], getTestSeed(z1 * d2 * d3 + z2 * d3 + z3 + 1));
	INIT3(l, B1, B2, B3)
	INIT3(s, B1, B2, B3)
	INIT3(o, B1, B2, B3)
#undef INIT3
	LOOP3(B1, B2, B3) ps[z1][z2][z3] = &s[z1][z2][z3];
	LOOP3(B1, B2, B3) po[z1][z2][z3] = &o[z1][z2][z3];
	LOOP3(B1, B2, B3) { npo[z1][z2][z3] = new TestSerializeClass(); initTestValue(*npo[z1][z2][z3], getTestSeed(z1 * B2 * B3 + z2 * B3 + z3 + 1)); }

	serializeWriteData(archiveWriter.get(), "i", i);
	serializeWriteData(archiveWriter.get(), "l", l);
	serializeWriteData(archiveWriter.get(), "s2", s2);
	serializeWriteData(archiveWriter.get(), "s", s);
	serializeWriteData(archiveWriter.get(), "ps", ps);
	serializeWriteData(archiveWriter.get(), "o", o, &module);
	serializeWriteData(archiveWriter.get(), "po", po, &module);
	serializeWriteData(archiveWriter.get(), "npo", npo, &module);

	TestArchiveTraits<AR>::rewind(ar);

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader));

	int ri[A1][A2];
	string rs2[A1][A2];
	long rl[B1][B2][B3];
	string rs[B1][B2][B3];
	string * rps[B1][B2][B3];
	TestSerializeClass ro[B1][B2][B3];
	TestSerializeClass * rpo[B1][B2][B3];
	TestSerializeClass * rnpo[B1][B2][B3];

#define INIT2(v, d1, d2) LOOP2(d1, d2) initTestValue(v[z1][z2], getTestSeed(0));
	INIT2(ri, A1, A2)
	INIT2(rs2, A1, A2)
#undef INIT2

#define INIT3(v, d1, d2, d3) LOOP3(d1, d2, d3) initTestValue(v[z1][z2][z3], getTestSeed(0));
	INIT3(rl, B1, B2, B3)
	INIT3(rs, B1, B2, B3)
	INIT3(ro, B1, B2, B3)
#undef INIT3
	LOOP3(B1, B2, B3) rps[z1][z2][z3] = NULL;
	LOOP3(B1, B2, B3) rpo[z1][z2][z3] = NULL;
	LOOP3(B1, B2, B3) rnpo[z1][z2][z3] = NULL;

	serializeReadData(archiveReader.get(), "i", ri);
	serializeReadData(archiveReader.get(), "l", rl);
	serializeReadData(archiveReader.get(), "s2", rs2);
	serializeReadData(archiveReader.get(), "s", rs);
	serializeReadData(archiveReader.get(), "ps", rps);
	serializeReadData(archiveReader.get(), "o", ro, &module);
	serializeReadData(archiveReader.get(), "po", rpo, &module);
	serializeReadData(archiveReader.get(), "npo", rnpo, &module);

#define EQ2(v, u, d1, d2) LOOP2(d1, d2) GEQUAL(v[z1][z2], u[z1][z2]);
	EQ2(i, ri, A1, A2)
	EQ2(s2, rs2, A1, A2)
#undef EQ2

#define EQ3(v, u, d1, d2, d3) LOOP3(d1, d2, d3) GEQUAL(v[z1][z2][z3], u[z1][z2][z3]);
	EQ3(s, rs, B1, B2, B3)
	EQ3(l, rl, B1, B2, B3)
	EQ3(o, ro, B1, B2, B3)
	EQ3(*npo, *rnpo, B1, B2, B3)
#undef EQ3
	LOOP3(B1, B2, B3) GEQUAL(rps[z1][z2][z3], &rs[z1][z2][z3]);
	LOOP3(B1, B2, B3) GEQUAL(rpo[z1][z2][z3], &ro[z1][z2][z3]);

	LOOP3(B1, B2, B3) delete npo[z1][z2][z3];
	LOOP3(B1, B2, B3) delete rnpo[z1][z2][z3];
}

GTEST(testMultipleDimensionArray_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestMultipleDimensionArray(writer.get(), reader.get(), stream);

//	cout << stream.str().c_str() << endl;
}


GTEST(testMultipleDimensionArray_Xml)
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestMultipleDimensionArray(writer.get(), reader.get(), storage);

//	storage.saveToStream(cout);
}


GTEST(testMultipleDimensionArray_Json)
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestMultipleDimensionArray(writer.get(), reader.get(), storage);

//	storage.saveToStream(cout);
}


} // unnamed namespace
