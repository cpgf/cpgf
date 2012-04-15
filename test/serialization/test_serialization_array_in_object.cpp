#include "testserializationcommon.h"
#include "testserializationcommonclasses.h"
#include "cpgf/gmetadefine.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

template <typename READER, typename AR>
void doTestArrayInObject(IMetaWriter * writer, const READER & reader, const AR & ar)
{
	const char * const serializeObjectName = "arrayInObject";

	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);
	register_TestSerializeArray(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeArray"));

	TestSerializeArray instance;
	instance.init();

	serializeWriteObject(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());
	
	ar.rewind();
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader.get(service.get())));
	
	TestSerializeArray readInstance;
	
	GCHECK(! (instance == readInstance));

	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());

	GEQUAL(instance, readInstance);
}

GTEST(testArrayInObject_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	
	doTestArrayInObject(writer.get(), MetaReaderGetterStream(stream), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testArrayInObject_Xml)
{
	GMetaXmlArchive archive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(archive));
	
	doTestArrayInObject(writer.get(), MetaReaderGetterXml(archive), TestArchiveStreamNone());
	
//	archive.saveToStream(cout);
}


GTEST(testArrayInObject_Json)
{
	GMetaJsonArchive archive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(archive));
	
	doTestArrayInObject(writer.get(), MetaReaderGetterJson(archive), TestArchiveStreamNone());
	
//	archive.saveToStream(cout);
}


} // unnamed namespace
