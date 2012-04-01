#include "testserializationcommon.h"
#include "testserializationcommonclasses.h"
#include "cpgf/gmetadefine.h"

#include "gmetatextstreamarchive.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename AR>
void doTestArrayInObject(IMetaService * service, IMetaWriter * writer, IMetaReader * reader, const AR & ar)
{
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(0, service, writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeArray"));

	TestSerializeArray instance;
	instance.init();

	serializeWriteObjectValue(archiveWriter.get(), "obj", &instance, metaClass.get());

	ar.rewind();
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(0, service, reader));
	
	TestSerializeArray readInstance;
	
	GCHECK(! (instance == readInstance));

	serializeReadObject(archiveReader.get(), "obj", &readInstance, metaClass.get());

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

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GTextStreamMetaReader<stringstream> inputStream(service.get(), stream);
	
	doTestArrayInObject(service.get(), &outputStream, &inputStream, TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
