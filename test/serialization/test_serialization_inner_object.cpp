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
	class B {
	public:
		class C {
		public:
			C() : nc(0) {}
			
			int nc;
		};
		
		B() : nb(0) {}

		int nb;
		C c;
	};

	A() : na(0) {}
	
	int na;
	B b;
};

bool operator == (const A & a1, const A & a2)
{
	return a1.na == a2.na && a1.b.nb == a2.b.nb && a1.b.c.nc == a2.b.c.nc;
}

template <typename Define>
void register_TestSerializeClass(Define define)
{
	GDefineMetaClass<A> classDefineA = GDefineMetaClass<A>::declare("TestSerializeClassA");
	
	classDefineA
		._class(
			GDefineMetaClass<A::B>::declare("TestSerializeClassB")
				._class(
					GDefineMetaClass<A::B::C>::declare("TestSerializeClassC")
						FIELD(A::B::C, nc)
				)

				FIELD(A::B, nb)
				FIELD(A::B, c)
		)

		FIELD(A, na)
		FIELD(A, b)
	;
	define._class(classDefineA);
}

template <typename AR>
void doTestInnerObject(IMetaStorageWriter * writer, IMetaStorageReader * reader, const AR & ar)
{
	const char * const serializeObjectName = "innerObject";
	
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("global.TestSerializeClassA"));

	GCHECK(metaClass);

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	A instance;
	instance.na = 38;
	instance.b.nb = 78;
	instance.b.c.nc = 99;

	serializeWriteData(archiveWriter.get(), serializeObjectName, instance, &module);

	TestArchiveTraits<AR>::rewind(ar);

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader));
	
	A readInstance;
	
	GCHECK(! (instance == readInstance));
	
	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());
	
	GCHECK(instance == readInstance);
}

GTEST(testInnerObject_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestInnerObject(writer.get(), reader.get(), stream);
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testInnerObject_Xml)
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestInnerObject(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


GTEST(testInnerObject_Json)
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestInnerObject(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


} // unnamed namespace
