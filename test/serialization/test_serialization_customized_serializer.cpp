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
	string bs;
};

class C : public B
{
public:
	C() : c(0) {}

	int c;
};

class R : public C
{
public:
	R() {}
};

A * pa = NULL;
B * pb = NULL;
C * pc = NULL;

class MetaSerializerA : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * /*archiveWriter*/, const void * /*instance*/, IMetaClass * metaClass) {
		return metaClass->getTypeName();
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param) {
		A * instance = static_cast<A *>(const_cast<void *>(param->instance));
		
		GEQUAL(pa, instance);

		int temp = instance->a;
		instance->a = temp * 2;
		metaSerializerWriteObjectMembers(archiveWriter, serializerWriter, param);
		instance->a = temp;
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * metaClass) {
		return metaClass->createInstance();
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param) {
		metaSerializerReadObjectMembers(archiveReader, serializerReader, param);
	}
};

class MetaSerializerB : public IMetaSerializer
{
	G_INTERFACE_IMPL_OBJECT
	G_INTERFACE_IMPL_EXTENDOBJECT
	
public:
	virtual const char * G_API_CC getClassTypeName(IMetaArchiveWriter * /*archiveWriter*/, const void * /*instance*/, IMetaClass * metaClass) {
		return metaClass->getTypeName();
	}
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * /*archiveWriter*/, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param) {
		B * instance = static_cast<B *>(const_cast<void *>(param->instance));
		
		GEQUAL(pb, instance);

		GScopedInterface<IMetaAccessible> accessible;
		uint32_t i;
		uint32_t count;

		count = param->metaClass->getFieldCount();
		for(i = 0; i < count; ++i) {
			accessible.reset(param->metaClass->getFieldAt(i));

			serializerWriter->writeMember(param, accessible.get());
		}
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

			serializerReader->readMember(param, accessible.get());
		}
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
	
	virtual void G_API_CC writeObject(IMetaArchiveWriter * archiveWriter, IMetaSerializerWriter * serializerWriter, GMetaArchiveWriterParam * param) {
		C * instance = static_cast<C *>(const_cast<void *>(param->instance));
		
		GEQUAL(pc, instance);
		
		int temp = instance->c;
		instance->c = temp * 3;
		metaSerializerWriteObjectMembers(archiveWriter, serializerWriter, param);
		instance->c = temp;
	}
	
	virtual void * G_API_CC allocateObject(IMetaArchiveReader * /*archiveReader*/, IMetaClass * metaClass) {
		return metaClass->createInstance();
	}

	virtual void G_API_CC readObject(IMetaArchiveReader * archiveReader, IMetaSerializerReader * serializerReader, GMetaArchiveReaderParam * param) {
		metaSerializerReadObjectMembers(archiveReader, serializerReader, param);
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
		FIELD(B, bs)
	;
	define._class(classDefineB);

	GDefineMetaClass<C, B> classDefineC = GDefineMetaClass<C, B>::declare("TestSerializeClassC");
	
	classDefineC
		FIELD(C, c)
	;
	define._class(classDefineC);
	
	GDefineMetaClass<R, C> classDefineR = GDefineMetaClass<R, C>::declare("TestSerializeClassR");
	
	define._class(classDefineR);
	
}

template <typename READER, typename AR>
void doTestCustomizedSerializer(IMetaWriter * writer, const READER & reader, const AR & ar)
{
	const char * const serializeObjectName = "customizedSerializer";
	
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClassR"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	R instance;
	instance.a = 18;
	instance.b = 28;
	instance.bs = "what is it";
	instance.c = 38;

	pa = &instance;
	pb = &instance;
	pc = &instance;

	metaArchiveWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader.get(service.get())));

	ar.rewind();

	R readInstance;

	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());
	GEQUAL(18 * 2, readInstance.a);
	GEQUAL(28, readInstance.b);
	GEQUAL(string("what is it"), readInstance.bs);
	GEQUAL(38 * 3, readInstance.c);
}

GTEST(testCustomizedSerializer_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	
	doTestCustomizedSerializer(writer.get(), MetaReaderGetter(stream), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testCustomizedSerializer_Xml)
{
	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestCustomizedSerializer(writer.get(), MetaReaderGetterXml(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


GTEST(testCustomizedSerializer_Json)
{
	GMetaJsonArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(outputArchive));
	
	doTestCustomizedSerializer(writer.get(), MetaReaderGetterJson(outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


} // unnamed namespace


namespace cpgf {

template <>
struct GMetaTraitsCreateSerializer <A>
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam &) {
		return new MetaSerializerA();
	}
};

template <>
struct GMetaTraitsCreateSerializer <B>
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam &) {
		return new MetaSerializerB();
	}
};

template <>
struct GMetaTraitsCreateSerializer <C>
{
	static IMetaSerializer * createSerializer(const GMetaTraitsParam &) {
		return new MetaSerializerC();
	}
};


} // namespace cpgf

