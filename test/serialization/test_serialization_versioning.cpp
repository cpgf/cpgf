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
	virtual ~MetaSerializerA() {}

	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * /*archiveWriter*/, const void * /*instance*/, IMetaClass * metaClass) {
		return metaClass->getQualifiedName();
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param) {
		metaSerializerWriteObjectMembers(archiveWriter, serializerWriter, param);
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * metaClass) {
		return metaClass->createInstance();
	}

	virtual void G_API_CC freeObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * metaClass, void * instance) {
		metaClass->destroyInstance(instance);
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

template <typename AR>
void doTestVersioning(IMetaStorageWriter * writer, IMetaStorageReader * reader, const AR & ar)
{
	const char * const serializeObjectName = "versioning";
	const char * const className = "global.TestSerializeClassA";

	
	GDefineMetaNamespace define0 = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define0, 0);

	GMetaModule module0;
	GScopedInterface<IMetaModule> moduleInterface0(createMetaModule(&module0, define0.getMetaClass()));
	GScopedInterface<IMetaService> service0(createMetaService(moduleInterface0.get()));

	GScopedInterface<IMetaClass> metaClass0(service0->findClassByName(className));
	GScopedInterface<IMetaArchiveWriter> archiveWriter0(createMetaArchiveWriter(service0.get(), writer));


	GDefineMetaNamespace define1 = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define1, 1);

	GMetaModule module1;
	GScopedInterface<IMetaModule> moduleInterface1(createMetaModule(&module1, define1.getMetaClass()));
	GScopedInterface<IMetaService> service1(createMetaService(moduleInterface1.get()));

	GScopedInterface<IMetaClass> metaClass1(service1->findClassByName(className));

	GScopedInterface<IMetaArchiveWriter> archiveWriter1(createMetaArchiveWriter(service1.get(), writer));
	

	GDefineMetaNamespace define2 = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define2, 2);

	GMetaModule module2;
	GScopedInterface<IMetaModule> moduleInterface2(createMetaModule(&module2, define2.getMetaClass()));
	GScopedInterface<IMetaService> service2(createMetaService(moduleInterface2.get()));

	GScopedInterface<IMetaClass> metaClass2(service2->findClassByName(className));

	GScopedInterface<IMetaArchiveWriter> archiveWriter2(createMetaArchiveWriter(service2.get(), writer));


	A0 a0;
	A1 a1;
	
	a0.a = 5;

	a1.a = 38;
	a1.b = 78;

	serializeWriteObject(archiveWriter0.get(), serializeObjectName, &a0, metaClass0.get());
	serializeWriteObject(archiveWriter1.get(), serializeObjectName, &a1, metaClass1.get());

	GScopedInterface<IMetaArchiveReader> archiveReader2(createMetaArchiveReader(service2.get(), reader));

	TestArchiveTraits<AR>::rewind(ar);

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

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestVersioning(writer.get(), reader.get(), stream);
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testVersioning_Xml)
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestVersioning(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


GTEST(testVersioning_Json)
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestVersioning(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
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

