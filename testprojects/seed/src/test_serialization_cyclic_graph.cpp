#include "gmetaarchivereader.h"
#include "gmetaarchivewriter.h"

#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include "gmetatextstreamarchive.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

class C;

class A
{
public:
	A() : a(0), pc(NULL) {}
	~A();
	
	void init();
	bool verify() const;
	
	int a;
	A * pself;
	C * pc;
};

class B
{
public:
	B() : b(0), pa(NULL) {}

	int b;
	B * pself;
	A * pa;
};

class C
{
public:
	C() : c(0), pb(NULL) {}

	int c;
	C * pself;
	B * pb;
};

A::~A()
{
	if(this->pc != NULL) {
		delete this->pc->pb;
		delete this->pc;
	}
}

void A::init()
{
	this->pc = new C;
	this->pc->pb = new B;
	this->pc->pb->pa = this;
	this->pself = this;
	this->pc->pself = this->pc;
	this->pc->pb->pself = this->pc->pb;
	this->a = 1;
	this->pc->c = 3;
	this->pc->pb->b = 2;
}

bool A::verify() const
{
	return
		this->pc != NULL
		&& this->pc->pb != NULL
		&& this->pc->pb->pa == this
		&& this->a == 1
		&& this->pc->c == 3
		&& this->pc->pb->b == 2
	;
}


template <typename Define>
void register_TestSerializeClass(Define define)
{
	GDefineMetaClass<A> classDefineA = GDefineMetaClass<A>::declare("TestSerializeClassA");
	
	classDefineA
		FIELD(A, a)
		FIELD(A, pself)
		FIELD(A, pc)
	;
	define._class(classDefineA);
	
	GDefineMetaClass<B> classDefineB = GDefineMetaClass<B>::declare("TestSerializeClassB");
	
	classDefineB
		FIELD(B, b)
		FIELD(B, pself)
		FIELD(B, pa)
	;
	define._class(classDefineB);

	GDefineMetaClass<C> classDefineC = GDefineMetaClass<C>::declare("TestSerializeClassC");
	
	classDefineC
		FIELD(C, c)
		FIELD(C, pself)
		FIELD(C, pb)
	;
	define._class(classDefineC);
	
}

template <typename AR>
void doTestCyclicGraph(IMetaService * service, IMetaWriter * writer, IMetaReader * reader, const AR & ar)
{
	const char * const serializeObjectName = "cyclicGraph";
	
	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestSerializeClassA"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(0, service, writer));
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(0, service, reader));

	A instance;
	instance.init();
	
	GCHECK(instance.verify());

	serializeWriteObjectValue(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());

	ar.rewind();

	A readInstance;
	
	GCHECK(! readInstance.verify());
	
	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());
	
	GCHECK(readInstance.verify());
}

GTEST(testCyclicGraph)
{
	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GScopedInterface<IMetaModule> module(createMetaModule(define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(module.get()));

	stringstream stream;

	GScopedInterface<IMetaWriter> outputStream(createTextStreamMetaWriter(stream));
	GScopedInterface<IMetaReader> inputStream(createTextStreamMetaReader(service.get(), stream));
	
	doTestCyclicGraph(service.get(), outputStream.get(), inputStream.get(), TestArchiveStream<stringstream>(stream));
	
//	cout << stream.str().c_str() << endl;
}


} // unnamed namespace
