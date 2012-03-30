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
void doTestSimpleObject(IMetaService * service, IMetaWriter * writer, IMetaReader * reader, const AR & ar)
{
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(GMetaArchiveConfig().getFlags(), service, writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClass"));

	TestSerializeClass instance;
	instance.set(38);

	serializeWriteObjectValue(archiveWriter.get(), "obj", &instance, metaClass.get());

	ar.rewind();
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(GMetaArchiveConfig().getFlags(), service, reader));
	
	TestSerializeClass readInstance;
	
	GCHECK(instance != readInstance);

	readInstance.pself = NULL;
	readInstance.pnull = (string *)0xbeef;
	serializeReadObject(archiveReader.get(), "", &readInstance, metaClass.get());

	GEQUAL(instance, readInstance);
}

GTEST(TestSimpleObject)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GTextStreamMetaReader<stringstream> inputStream(service.get(), stream);
	
	doTestSimpleObject(service.get(), &outputStream, &inputStream, TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
