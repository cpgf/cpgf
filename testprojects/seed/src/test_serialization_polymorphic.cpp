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

const int CA = 1;
const int CB = 2;
const int CC = 3;

class A
{
public:
	A() : a(0) {}
	virtual ~A() {}

	virtual int get() const { return CA; }

	int a;
};

class B : public A
{
public:
	B() : b(0) {}

	virtual int get() const { return CB; }

	int b;
};

class C : public A
{
public:
	C() : c(0) {}

	virtual int get() const { return CC; }

	int c;
};

class R
{
public:
	R() : r(0), pa(NULL) {}
	~R() { delete pa; }

	int r;
	A * pa;
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
	
	GDefineMetaClass<R> classDefineR = GDefineMetaClass<R>::declare("TestSerializeClassR");
	
	classDefineR
		FIELD(R, r)
		FIELD(R, pa)
	;

	define._class(classDefineR);

}

template <typename SEEK>
void doTestPolymorphic(IMetaWriter * writer, IMetaReader * reader, const SEEK & seek)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaService> service(createMetaService(createMetaModule(define.getMetaClass())));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(GMetaArchiveConfig().getFlags(), service.get(), writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClassR"));

	R instance;
	instance.r = 58;
	B * pb = new B;
	instance.pa = pb;
	pb->a = 15;
	pb->b = 16;

	archiveWriter->writeObjectValue("obj", &instance, metaClass.get());

	seek(0);
	
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(GMetaArchiveConfig().getFlags(), service.get(), reader));
	
	R readInstance;
	
	archiveReader->readObject("", &readInstance, metaClass.get());

	GCHECK(readInstance.pa != NULL);
	GEQUAL(CB, readInstance.pa->get());
}

GTEST(testPolymorphic)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaService> service(createMetaService(createMetaModule(define.getMetaClass())));

	stringstream stream;

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GTextStreamMetaReader<stringstream> inputStream(service.get(), stream);
	
	doTestPolymorphic(&outputStream, &inputStream, makeCallback(&stream, extractFunction1(&stringstream::seekg)));
	
	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
