#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gmetaapiutil.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


namespace {

class A0
{
public:
	A0() : a(0) {}

	int a;
};

class A1
{
public:
	A1() : a(0), b(0) {}

	int a;
	int b;
};

class A2
{
public:
	A2() : a(0), b(0), c(0) {}

	int a;
	int b;
	int c;
};

class MetaSerializerA : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * /*archiveWriter*/, const void * /*instance*/, IMetaClass * metaClass) {
		return metaClass->getTypeName();
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param) {
		metaSerializerWriteObjectMembers(archiveWriter, serializerWriter, param);
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * metaClass) {
		return metaClass->createInstance();
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * /*archiveReader*/, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param) {
		GScopedInterface<IMetaAccessible> accessible;
		uint32_t i;
		uint32_t count;

		count = param->metaClass->getFieldCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(param->metaClass->getFieldAt(i));

			if(param->archiveVersion < 1 && strcmp(accessible->getName(), "b") == 0) {
				continue;
			}

			if(param->archiveVersion < 2 && strcmp(accessible->getName(), "c") == 0) {
				continue;
			}

			serializerReader->readMember(param, accessible.get());
		}
	}
};

template <typename Define>
void register_TestSerializeClass(Define define, int version)
{
	const char * name = "TestSerializeClassA";

	if(version == 0) {
		GDefineMetaClass<A0> classDefineA = GDefineMetaClass<A0>::declare(name);
		classDefineA
			FIELD(A0, a)
		;
		define._class(classDefineA);
	}
	
	if(version == 1) {
		GDefineMetaClass<A1> classDefineA = GDefineMetaClass<A1>::declare(name);
		classDefineA
			._annotation(SerializationAnnotation)
				._element(SerializationAnnotationVersion, 1)

			FIELD(A1, a)
			FIELD(A1, b)
		;
		define._class(classDefineA);
	}
	
	if(version == 2) {
		GDefineMetaClass<A2> classDefineA = GDefineMetaClass<A2>::declare(name);
		classDefineA
			._annotation(SerializationAnnotation)
				._element(SerializationAnnotationVersion, 2)

			FIELD(A2, a)
			FIELD(A2, b)
			FIELD(A2, c)
		;
		define._class(classDefineA);
	}
	
}

template <typename READER, typename AR>
void doTestVersioning(IMetaWriter * writer, const READER & reader, const AR & ar)
{
	const char * const serializeObjectName = "versioning";
	
	GDefineMetaNamespace define0 = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define0, 0);

	GMetaModule module0;
	GScopedInterface<IMetaModule> moduleInterface0(createMetaModule(&module0, define0.getMetaClass()));
	GScopedInterface<IMetaService> service0(createMetaService(moduleInterface0.get()));

	GScopedInterface<IMetaClass> metaClass0(service0->findClassByName("TestSerializeClassA"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter0(createMetaArchiveWriter(service0.get(), writer));
	

	GDefineMetaNamespace define1 = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define1, 1);

	GMetaModule module1;
	GScopedInterface<IMetaModule> moduleInterface1(createMetaModule(&module1, define1.getMetaClass()));
	GScopedInterface<IMetaService> service1(createMetaService(moduleInterface1.get()));

	GScopedInterface<IMetaClass> metaClass1(service1->findClassByName("TestSerializeClassA"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter1(createMetaArchiveWriter(service1.get(), writer));
	

	GDefineMetaNamespace define2 = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define2, 2);

	GMetaModule module2;
	GScopedInterface<IMetaModule> moduleInterface2(createMetaModule(&module2, define2.getMetaClass()));
	GScopedInterface<IMetaService> service2(createMetaService(moduleInterface2.get()));

	GScopedInterface<IMetaClass> metaClass2(service2->findClassByName("TestSerializeClassA"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter2(createMetaArchiveWriter(service2.get(), writer));


	A0 a0;
	A1 a1;
	
	a0.a = 5;

	a1.a = 38;
	a1.b = 78;

	serializeWriteObjectValue(archiveWriter0.get(), serializeObjectName, &a0, metaClass0.get());
	serializeWriteObjectValue(archiveWriter1.get(), serializeObjectName, &a1, metaClass1.get());

	GScopedInterface<IMetaArchiveReader> archiveReader2(createMetaArchiveReader(service2.get(), reader.get(service2.get())));

	ar.rewind();

	A2 readInstance;
	readInstance.a = 9;
	readInstance.b = 10;
	readInstance.c = 11;
	
	serializeReadObject(archiveReader2.get(), serializeObjectName, &readInstance, metaClass2.get());
	
	GEQUAL(5, readInstance.a);
	GEQUAL(10, readInstance.b);
	GEQUAL(11, readInstance.c);

	A2 readInstance2;
	readInstance2.a = 9;
	readInstance2.b = 10;
	readInstance2.c = 68;
	
	serializeReadObject(archiveReader2.get(), serializeObjectName, &readInstance2, metaClass2.get());
	
	GEQUAL(38, readInstance2.a);
	GEQUAL(78, readInstance2.b);
	GEQUAL(68, readInstance2.c);

}

GTEST(testVersioning_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	
	doTestVersioning(writer.get(), MetaReaderGetter(stream), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testVersioning_Xml)
{
	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestVersioning(writer.get(), MetaReaderGetterXml(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


GTEST(testVersioning_Json)
{
	GMetaJsonArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(outputArchive));
	
	doTestVersioning(writer.get(), MetaReaderGetterJson(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


} // unnamed namespace


namespace cpgf {

template <>
struct GMetaTraitsCreateSerializer <A0>
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam &) {
		return new MetaSerializerA();
	}
};

template <>
struct GMetaTraitsCreateSerializer <A1>
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam &) {
		return new MetaSerializerA();
	}
};

template <>
struct GMetaTraitsCreateSerializer <A2>
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam &) {
		return new MetaSerializerA();
	}
};


} // namespace cpgf

