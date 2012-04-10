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

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClass"));

	TestSerializeClass instance;
	instance.set(38);

	metaArchiveWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());

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
	
	doTestSimpleObject(writer.get(), MetaReaderGetter(stream), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}

GTEST(TestSimpleObject_Xml)
{
	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestSimpleObject(writer.get(), MetaReaderGetterXml(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


GTEST(TestSimpleObject_Json)
{
	GMetaJsonArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(outputArchive));
	
	doTestSimpleObject(writer.get(), MetaReaderGetterJson(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


} // unnamed namespace
