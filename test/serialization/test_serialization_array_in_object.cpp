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

	serializeWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());
	
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
	
	doTestArrayInObject(writer.get(), MetaReaderGetter(stream), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testArrayInObject_Xml)
{
	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestArrayInObject(writer.get(), MetaReaderGetterXml(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


GTEST(testArrayInObject_Json)
{
	GMetaJsonArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(outputArchive));
	
	doTestArrayInObject(writer.get(), MetaReaderGetterJson(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


} // unnamed namespace
