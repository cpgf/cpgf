#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

const int CA = 1;
const int CB = 2;
const int CC = 3;
const int CD = 4;

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

class D : public C
{
public:
	D() : d(0) {}

	virtual int get() const { return CD; }

	int d;
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
	
	GDefineMetaClass<D, C> classDefineD = GDefineMetaClass<D, C>::declare("TestSerializeClassD");
	
	classDefineD
		FIELD(D, d)
	;
	define._class(classDefineD);
	
	GDefineMetaClass<R> classDefineR = GDefineMetaClass<R>::declare("TestSerializeClassR");
	
	classDefineR
		FIELD(R, r)
		FIELD(R, pa)
	;
	define._class(classDefineR);

}

template <typename READER, typename AR>
void doTestPolymorphic(IMetaService * service, IMetaWriter * writer, const READER & reader, const AR & ar)
{
	const char * const serializeObjectName = "polymorphic";
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClassR"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(0, service, writer));

	// write

	R instance1;
	instance1.r = 58;
	B * pb1 = new B;
	instance1.pa = pb1;
	pb1->a = 15;
	pb1->b = 16;

	serializeWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance1, metaClass.get());

	R instance2;
	instance2.r = 68;
	D * pd2 = new D;
	instance2.pa = pd2;
	pd2->a = 25;
	pd2->d = 26;

	serializeWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance2, metaClass.get());

	writer->flush();

	// read
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(0, service, reader.get()));

	ar.rewind();

	R readInstance1;

	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance1, metaClass.get());

	GCHECK(readInstance1.pa != NULL);
	GEQUAL(CB, readInstance1.pa->get());
	GEQUAL(58, readInstance1.r);
	GEQUAL(15, readInstance1.pa->a);
	GDIFF(NULL, dynamic_cast<B *>(readInstance1.pa));
	GEQUAL(16, dynamic_cast<B *>(readInstance1.pa)->b);

	R readInstance2;

	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance2, metaClass.get());

	GCHECK(readInstance2.pa != NULL);
	GEQUAL(CD, readInstance2.pa->get());
	GEQUAL(68, readInstance2.r);
	GEQUAL(25, readInstance2.pa->a);
	GDIFF(NULL, dynamic_cast<D *>(readInstance2.pa));
	GEQUAL(26, dynamic_cast<D *>(readInstance2.pa)->d);
}

GTEST(testPolymorphic_TextStream)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createTextStreamMetaWriter(stream));
	GScopedInterface<IMetaReader> reader(createTextStreamMetaReader(service.get(), stream));
	
	doTestPolymorphic(service.get(), writer.get(), MetaReaderGetter(reader.get()), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}

GTEST(testPolymorphic_Xml)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GScopedInterface<IMetaWriter> writer(createXmlMetaWriter(stream));
	
	doTestPolymorphic(service.get(), writer.get(), MetaReaderGetterXml(service.get(), stream), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
