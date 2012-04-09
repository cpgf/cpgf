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
	A() : a(0) {}
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

class D : public B, public C
{
public:
	D() : d(0) {}

	bool operator == (const D & other) const {
		return
			this->B::a == other.B::a
			&& this->C::a == other.C::a
			&& this->b == other.b
			&& this->c == other.c
			&& this->d == other.d
		;
	}
	
	bool operator != (const D & other) const {
		return
			this->B::a != other.B::a
			&& this->C::a != other.C::a
			&& this->b != other.b
			&& this->c != other.c
			&& this->d != other.d
		;
	}
	
	int d;
};



template <typename Define>
void register_TestSerializeClass(Define define)
{
	GDefineMetaClass<A> classDefineA = GDefineMetaClass<A>::declare("TestSerializeClassA");
	
	classDefineA
		._property("a", &A::getA, &A::setA)
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
	;

	define._class(classDefineD);

}

template <typename READER, typename AR>
void doTestDiamondNonvirtualBase(IMetaService * service, IMetaWriter * writer, const READER & reader, const AR & ar)
{
	const char * const serializeObjectName = "diamondNonvirtualBase";
	
	readCount = 0;
	writeCount = 0;

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service, writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClassD"));

	D instance;
	instance.B::a = 0x1a;
	instance.C::a = 0x1b;
	instance.b = 0x2b;
	instance.c = 0x3c;
	instance.d = 0x4d;

	metaArchiveWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());

	ar.rewind();
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service, reader.get()));
	
	D readInstance;
	
	GCHECK(instance != readInstance);

	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());

	GEQUAL(instance, readInstance);

	GEQUAL(2, readCount);
	GEQUAL(2, writeCount);
}

GTEST(testDiamondNonvirtualBase_TextStream)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	GScopedInterface<IMetaReader> reader(createTextStreamMetaReader(service.get(), stream));
	
	doTestDiamondNonvirtualBase(service.get(), writer.get(), MetaReaderGetter(reader.get()), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testDiamondNonvirtualBase_Xml)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	GMetaXmlArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(outputArchive));
	
	doTestDiamondNonvirtualBase(service.get(), writer.get(), MetaReaderGetterXml(service.get(), outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


GTEST(testDiamondNonvirtualBase_Json)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	GMetaJsonArchive outputArchive;

	GScopedInterface<IMetaWriter> writer(createJsonMetaWriter(outputArchive));
	
	doTestDiamondNonvirtualBase(service.get(), writer.get(), MetaReaderGetterJson(service.get(), outputArchive), TestArchiveStreamNone());
	
//	outputArchive.saveToStream(cout);
}


} // unnamed namespace
