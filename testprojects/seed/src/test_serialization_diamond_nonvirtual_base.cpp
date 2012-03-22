#include "cpgf/metatraits/gmetaserializer_string.h"

#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include "gmetatextstreamarchive.h"

#include "cpgf/ginterface.h"
#include "cpgf/gflags.h"
#include "cpgf/gfunctionextractor.h"

#include "gmetaarchivetypemap.h"

#include <sstream>
#include <string>

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
	
	GDefineMetaClass<D, B, C> classDefineD = GDefineMetaClass<D, B, C>::declare("TestSerializeClassD");
	
	classDefineD
		FIELD(D, d)
	;

	define._class(classDefineD);

}

template <typename SEEK>
void doTestMultipleInheritance(IMetaWriter * writer, IMetaReader * reader, const SEEK & seek)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaService> service(createMetaService(createMetaModule(define.getMetaClass())));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(GMetaArchiveConfig().getFlags(), service.get(), writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClassD"));

	D instance;
	instance.B::a = 0x1a;
	instance.C::a = 0x1b;
	instance.b = 0x2b;
	instance.c = 0x3c;
	instance.d = 0x4d;

	archiveWriter->writeObjectValue("obj", &instance, metaClass.get());

	seek(0);
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(GMetaArchiveConfig().getFlags(), service.get(), reader));
	
	D readInstance;
	
	GCHECK(instance != readInstance);

	archiveReader->readObject("", &readInstance, metaClass.get());

	GEQUAL(instance, readInstance);
}

GTEST(testMultipleInheritance)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaService> service(createMetaService(createMetaModule(define.getMetaClass())));

	stringstream stream;

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GTextStreamMetaReader<stringstream> inputStream(service.get(), stream);
	
	doTestMultipleInheritance(&outputStream, &inputStream, makeCallback(&stream, extractFunction1(&stringstream::seekg)));
	
	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
