#include "testserializationcommon.h"
#include "testserializationcommonclasses.h"
#include "cpgf/gmetadefine.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename READER, typename AR>
void doTestSimpleObject(IMetaWriter * writer, const READER & reader, const AR & ar)
{
	const char * const serializeObjectName = "simpleObject";
	
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClass"));

	TestSerializeClass instance;
	instance.set(38);

	serializeWriteObject(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());

	ar.rewind();
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader.get(service.get())));
	
	TestSerializeClass readInstance;
	
	GCHECK(instance != readInstance);

	readInstance.pself = NULL;
	readInstance.pnull = (string *)0xbeef;
	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());

	GEQUAL(instance, readInstance);
}

GTEST(TestSimpleObject_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	
	doTestSimpleObject(writer.get(), MetaReaderGetterStream(stream), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}

GTEST(TestSimpleObject_Xml)
{
	GMetaXmlArchive archive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(archive));
	
	doTestSimpleObject(writer.get(), MetaReaderGetterXml(archive), TestArchiveStreamNone());
	
//	archive.saveToStream(cout);
}


GTEST(TestSimpleObject_Json)
{
	GMetaJsonArchive archive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(archive));
	
	doTestSimpleObject(writer.get(), MetaReaderGetterJson(archive), TestArchiveStreamNone());
	
//	archive.saveToStream(cout);
}


} // unnamed namespace
