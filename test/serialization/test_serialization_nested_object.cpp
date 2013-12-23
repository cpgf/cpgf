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
	A() : na(0) {}
	
	int na;
};

class B
{
public:
	B() : pa(NULL), nb(0) {}
	~B() { delete pa; }

	A * pa;
	A a;
	int nb;
};

class C
{
public:
	C() : pb(NULL), nc(0) {}
	~C() { delete pb; }

	void init() {
		this->a.na = 1;
		this->nc = 3;
		
		this->b.nb = 2;
		this->b.a.na = 5;
		this->b.pa = new A;
		this->b.pa->na = 38;
		
		this->pb = new B;
		this->pb->nb = 32;
		this->pb->a.na = 35;
		this->pb->pa = new A;
		this->pb->pa->na = 78;
	}

	A a;
	B * pb;
	B b;
	int nc;
};

bool operator == (const A & a1, const A & a2)
{
	return a1.na == a2.na;
}

bool operator == (const B & b1, const B & b2)
{
	return b1.a == b2.a && *(b1.pa) == *(b2.pa) && b2.nb == b2.nb;
}

bool operator == (const C & c1, const C & c2)
{
	return c1.a == c2.a && c1.b == c2.b && *(c1.pb) == *(c2.pb) && c1.nc == c2.nc;
}

template <typename Define>
void register_TestSerializeClass(Define define)
{
	GDefineMetaClass<A> classDefineA = GDefineMetaClass<A>::declare("TestSerializeClassA");
	
	classDefineA
		FIELD(A, na)
	;
	define._class(classDefineA);
	
	GDefineMetaClass<B> classDefineB = GDefineMetaClass<B>::declare("TestSerializeClassB");
	
	classDefineB
		FIELD(B, pa)
		FIELD(B, a)
		FIELD(B, nb)
	;
	define._class(classDefineB);

	GDefineMetaClass<C> classDefineC = GDefineMetaClass<C>::declare("TestSerializeClassC");
	
	classDefineC
		FIELD(C, a)
		FIELD(C, pb)
		FIELD(C, b)
		FIELD(C, nc)
	;
	define._class(classDefineC);
	
}

template <typename AR>
void doTestNestedObject(IMetaStorageWriter * writer, IMetaStorageReader * reader, const AR & ar)
{
	const char * const serializeObjectName = "nestedObject";
	
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("global.TestSerializeClassC"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	C instance;
	instance.init();

	C * pinstance = &instance;
	
	serializeWriteData(archiveWriter.get(), serializeObjectName, pinstance, &module);
	// should error
	GBEGIN_EXCEPTION
		serializeWriteData(archiveWriter.get(), serializeObjectName, instance, &module);
	GEND_EXCEPTION(...)

	TestArchiveTraits<AR>::rewind(ar);

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader));
	
	C readInstance;
	
	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());
	
	GCHECK(instance == readInstance);
}

GTEST(testNestedObject_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestNestedObject(writer.get(), reader.get(), stream);
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testNestedObject_Xml)
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestNestedObject(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


GTEST(testNestedObject_Json)
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestNestedObject(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


} // unnamed namespace
