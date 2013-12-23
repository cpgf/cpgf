#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

int readCount = 0;
int writeCount = 0;

class A
{
public:
	A() : a(0), x(0) {}
	virtual ~A() {}

	int getA() const {
		++readCount;
		return a;
	}
	
	void setA(int a) {
		++writeCount;
		this->a = a;
	}

	int a;
	int x;
};

class B : virtual public A
{
public:
	B() : b(0) {}

	int b;
};

class C : virtual public A
{
public:
	C() : c(0) {}

	int c;
};

class D : public B, public C
{
public:
	D() : d(0), x(0) {}

	bool operator == (const D & other) const {
		return
			this->a == other.a
			&& this->b == other.b
			&& this->c == other.c
			&& this->d == other.d
			&& this->A::x == other.A::x
			&& this->D::x == other.D::x
		;
	}
	
	bool operator != (const D & other) const {
		return
			this->a != other.a
			&& this->b != other.b
			&& this->c != other.c
			&& this->d != other.d
			&& this->A::x != other.A::x
			&& this->D::x != other.D::x
		;
	}
	
	int d;
	int x;
};



template <typename Define>
void register_TestSerializeClass(Define define)
{
	GDefineMetaClass<A> classDefineA = GDefineMetaClass<A>::declare("TestSerializeClassA");
	
	classDefineA
		._property("a", &A::getA, &A::setA)
		FIELD(A, x)
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
	
	GDefineMetaClass<D, B, C> classDefineD = GDefineMetaClass<D, B, C>::declare("TestSerializeClassD");
	
	classDefineD
		FIELD(D, d)
		FIELD(D, x)
	;

	define._class(classDefineD);

}

template <typename AR>
void doTestDiamondVirtualBase(IMetaStorageWriter * writer, IMetaStorageReader * reader, const AR & ar)
{
	const char * const serializeObjectName = "diamondVirtualBase";
	
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	readCount = 0;
	writeCount = 0;

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("global.TestSerializeClassD"));

	D instance;
	instance.a = 0x1a;
	instance.b = 0x2b;
	instance.c = 0x3c;
	instance.d = 0x4d;
	instance.A::x = 8;
	instance.D::x = 78;

	serializeWriteObject(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());

	TestArchiveTraits<AR>::rewind(ar);
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader));
	
	D readInstance;
	
	GCHECK(instance != readInstance);

	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());

	GEQUAL(instance, readInstance);
	GEQUAL(8, readInstance.A::x);
	GEQUAL(78, readInstance.D::x);

	GEQUAL(1, readCount);
	GEQUAL(1, writeCount);
}


GTEST(testDiamondVirtualBase_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestDiamondVirtualBase(writer.get(), reader.get(), stream);
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testDiamondVirtualBase_Xml)
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestDiamondVirtualBase(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


GTEST(testDiamondVirtualBase_Json)
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestDiamondVirtualBase(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


} // unnamed namespace
