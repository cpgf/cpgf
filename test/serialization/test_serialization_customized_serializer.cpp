#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;

namespace {

class A
{
public:
	A() : a(0) {}

	int a;
};

class B : public A
{
public:
	B() : b(0) {}

	int b;
};

class C : public A
{
public:
	C() : c(0) {}

	int c;
};

class D : public C
{
public:
	D() : d(0) {}

	int d;
};

class MetaSerializerA : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * /*archiveWriter*/, const void * /*instance*/, IMetaClass * metaClass) {
		return metaClass->getTypeName();
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, GMetaArchiveWriterParam * param) {
		A * instance = static_cast<A *>(const_cast<void *>(param->instance));
		int temp = instance->a;
		instance->a = temp * 2;
		archiveWriter->writeObjectMembers(param);
		instance->a = temp;
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * metaClass) {
		return metaClass->createInstance();
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, GMetaArchiveReaderParam * param) {
		archiveReader->readObjectMembers(param);
	}
};

class MetaSerializerC : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * /*archiveWriter*/, const void * /*instance*/, IMetaClass * metaClass) {
		return metaClass->getTypeName();
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaWriter * metaWriter, GMetaArchiveWriterParam * param) {
		C * instance = static_cast<C *>(const_cast<void *>(param->instance));
		int temp = instance->c;
		instance->c = temp * 3;
		archiveWriter->writeObjectMembers(param);
		instance->c = temp;
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * metaClass) {
		return metaClass->createInstance();
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaReader * metaReader, GMetaArchiveReaderParam * param) {
		archiveReader->readObjectMembers(param);
	}
};

template <typename Define>
void register_TestSerializeClass(Define define)
{
	GDefineMetaClass<A> classDefineA = GDefineMetaClass<A>::declare("TestSerializeClassA");
	
	classDefineA
		FIELD(A, a)
	;
	define._class(classDefineA);
	
	GDefineMetaClass<B, A> classDefineB = GDefineMetaClass<B, A>::declare("TestSerializeClassB");
	
	classDefineB
		FIELD(B, b)
	;
	define._class(classDefineB);

	GDefineMetaClass<C, A> classDefineC = GDefineMetaClass<C, A>::declare("TestSerializeClassC");
	
	classDefineC
		FIELD(C, c)
	;
	define._class(classDefineC);
	
	GDefineMetaClass<D, C> classDefineD = GDefineMetaClass<D, C>::declare("TestSerializeClassD");
	
	classDefineD
		FIELD(D, d)
	;
	define._class(classDefineD);
	
}

template <typename READER, typename AR>
void doTestCustomizedSerializer(IMetaService * service, IMetaWriter * writer, const READER & reader, const AR & ar)
{

	const char * const serializeObjectName = "customizedSerializer";
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClassD"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(GMetaArchiveConfig(), service, writer));

	D instance;
	instance.a = 18;
	instance.c = 38;

	serializeWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(GMetaArchiveConfig(), service, reader.get()));

	ar.rewind();

	D readInstance;

	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());
	GEQUAL(18 * 2, readInstance.a);
	GEQUAL(38 * 3, readInstance.c);
}

GTEST(testCustomizedSerializer_TextStream)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	GScopedInterface<IMetaReader> reader(createTextStreamMetaReader(service.get(), stream));
	
	doTestCustomizedSerializer(service.get(), writer.get(), MetaReaderGetter(reader.get()), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testCustomizedSerializer_Xml)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestCustomizedSerializer(service.get(), writer.get(), MetaReaderGetterXml(service.get(), outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}



} // unnamed namespace


namespace cpgf {

template <>
struct GMetaTraitsCreateSerializer <A>
{
	static IMetaSerializer * createSerializer() {
		return new MetaSerializerA();
	}
};

template <>
struct GMetaTraitsCreateSerializer <C>
{
	static IMetaSerializer * createSerializer() {
		return new MetaSerializerC();
	}
};


} // namespace cpgf

