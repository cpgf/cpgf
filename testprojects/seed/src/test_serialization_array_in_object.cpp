#include "testserializationcommon.h"
#include "testserializationcommonclasses.h"
#include "cpgf/gmetadefine.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename AR>
void doTestArrayInObject(IMetaService * service, IMetaWriter * writer, IMetaReader * reader, const AR & ar)
{
	const char * const serializeObjectName = "arrayInObject";

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(0, service, writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeArray"));

	TestSerializeArray instance;
	instance.init();

	serializeWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());
	
	writer->flush();

	ar.rewind();
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(0, service, reader));
	
	TestSerializeArray readInstance;
	
	GCHECK(! (instance == readInstance));

	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());

	GEQUAL(instance, readInstance);
}

GTEST(testArrayInObject)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);
	register_TestSerializeArray(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	GScopedInterface<IMetaReader> reader(createTextStreamMetaReader(service.get(), stream));
	
	doTestArrayInObject(service.get(), writer.get(), reader.get(), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
