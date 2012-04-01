#include "testserializationcommonclasses.h"
#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include "gmetatextstreamarchive.h"

#include <sstream>
#include <string>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename AR>
void doTestMultipleDimensionArray(IMetaService * service, IMetaWriter * writer, IMetaReader * reader, const AR & ar)
{
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(0, service, writer));

	enum {
		A1 = 3, A2 = 5
	};

	int i[A1][A2];

#define INIT2(v, d1, d2) for(int z1 = 0; z1 < d1; ++z1) for(int z2 = 0; z2 < d2; ++z2) initTestValue(v[z1][z2], getTestSeed(z1 * d2 + z2 + 1));
	INIT2(i, A1, A2)
#undef INIT2

	serializeWriteValue(archiveWriter.get(), "i", i);

	ar.rewind();

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(0, service, reader));

	int ri[A1][A2];

#define INIT2(v, d1, d2) for(int z1 = 0; z1 < d1; ++z1) for(int z2 = 0; z2 < d2; ++z2) initTestValue(v[z1][z2], getTestSeed(0));
	INIT2(ri, A1, A2)
#undef INIT2

	serializeReadValue(archiveReader.get(), "ri", ri);

#define EQ2(v, u, d1, d2) for(int z1 = 0; z1 < d1; ++z1) for(int z2 = 0; z2 < d2; ++z2) cout << v[z1][z2] << "  " << u[z1][z2] << endl;
//GEQUAL(v[z1][z2], u[z1][z2]);
	EQ2(i, ri, A1, A2)
#undef EQ2

}

GTEST(testMultipleDimensionArray)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GTextStreamMetaReader<stringstream> inputStream(NULL, stream);
	
	doTestMultipleDimensionArray(service.get(), &outputStream, &inputStream, TestArchiveStream<stringstream>(stream));

	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
