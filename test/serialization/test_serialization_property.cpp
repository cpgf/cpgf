#include "testserializationcommon.h"
#include "cpgf/gmetadefine.h"

#include <sstream>

#include <iostream>

using namespace std;
using namespace cpgf;


namespace {

class TestPropertyData
{
public:
	TestPropertyData() : n(0), s() {
	}

	bool operator == (const TestPropertyData & other) const {
		return this->n == other.n && this->s == other.s;
	}

	bool operator != (const TestPropertyData & other) const {
		return ! (*this == other);
	}

	void init(long long seed) {
		this->n = (int)seed;
		this->s = "I'm a property";
	}

	int n;
	string s;
};

class TestPropertyClass
{
public:
	TestPropertyClass() : dataByValue(), dataByConstRef(), dataByPointer(NULL), value(0), str() {
	}

	~TestPropertyClass() {
		delete this->dataByPointer;
	}

	bool operator == (const TestPropertyClass & other) const {
		return this->value == other.value && this->str == other.str
			&& this->dataByValue == other.dataByValue
			&& this->dataByConstRef == other.dataByConstRef
			&& (this->dataByPointer == other.dataByPointer || (this->dataByPointer != NULL && other.dataByPointer != NULL && *(this->dataByPointer) == *(other.dataByPointer)))
		;
	}

	bool operator != (const TestPropertyClass & other) const {
		return ! (*this == other);
	}

	void init() {
		this->value = 99;
		this->str = "I'm another property";
		this->dataByValue.init(38);
		this->dataByConstRef.init(1999);
		if(this->dataByPointer == NULL) {
			this->dataByPointer = new TestPropertyData;
		}
		this->dataByPointer->init(2013);
	}

public:
	TestPropertyData getDataByValue() const {
		return this->dataByValue;
	}

	void setDataByValue(TestPropertyData v) {
		this->dataByValue = v;
	}

	const TestPropertyData & getDataByConstRef() const {
		return this->dataByConstRef;
	}

	void setDataByConstRef(const TestPropertyData & v) {
		this->dataByConstRef = v;
	}

	TestPropertyData * getDataByPointer() {
		return this->dataByPointer;
	}

	void setDataByPointer(TestPropertyData * v) {
		if(this->dataByPointer != NULL) {
			delete this->dataByPointer;
		}
		this->dataByPointer = v;
	}

	int getValue() const {
		return this->value;
	}

	void setValue(int v) {
		this->value = v;
	}

	const string & getStr() const {
		return this->str;
	}

	void setStr(const string & s) {
		this->str = s;
	}

private:
	TestPropertyData dataByValue;
	TestPropertyData dataByConstRef;
	TestPropertyData * dataByPointer;
	int value;
	string str;
};

template <typename Define>
void register_TestSerializeClass(Define define)
{
	GDefineMetaClass<TestPropertyData> classDefineData = GDefineMetaClass<TestPropertyData>::declare("TestPropertyData");
	
	classDefineData
		FIELD(TestPropertyData, n)
		FIELD(TestPropertyData, s)
	;
	define._class(classDefineData);
	
	GDefineMetaClass<TestPropertyClass> classDefineClass = GDefineMetaClass<TestPropertyClass>::declare("TestPropertyClass");
	
	classDefineClass
		._property("dataByValue", &TestPropertyClass::getDataByValue, &TestPropertyClass::setDataByValue)
		._property("dataByConstRef", &TestPropertyClass::getDataByConstRef, &TestPropertyClass::setDataByConstRef)
		._property("dataByPointer", &TestPropertyClass::getDataByPointer, &TestPropertyClass::setDataByPointer)
		._property("value", &TestPropertyClass::getValue, &TestPropertyClass::setValue)
		._property("str", &TestPropertyClass::getStr, &TestPropertyClass::setStr, GMetaPolicyCopyAllConstReference())
	;
	define._class(classDefineClass);

}

template <typename AR>
void doTestProperty(IMetaStorageWriter * writer, IMetaStorageReader * reader, const AR & ar)
{
	const char * const serializeObjectName = "testProperty";

	GDefineMetaNamespace define = GDefineMetaNamespace::declare("global");
	register_TestSerializeClass(define);

	GMetaModule module;
	GScopedInterface<IMetaModule> moduleInterface(createMetaModule(&module, define.getMetaClass()));
	GScopedInterface<IMetaService> service(createMetaService(moduleInterface.get()));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("global.TestPropertyClass"));

	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	TestPropertyClass instance;
	instance.init();
	
	serializeWriteObject(archiveWriter.get(), serializeObjectName, &instance, metaClass.get());

	TestArchiveTraits<AR>::rewind(ar);

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader));
	
	TestPropertyClass readInstance;
	
	GCHECK(readInstance != instance);
	
	serializeReadObject(archiveReader.get(), serializeObjectName, &readInstance, metaClass.get());
	
	GCHECK(readInstance == instance);
}

GTEST(testProperty_TextStream)
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestProperty(writer.get(), reader.get(), stream);
	
//	cout << stream.str().c_str() << endl;
}


GTEST(testProperty_Xml)
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestProperty(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


GTEST(testProperty_Json)
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestProperty(writer.get(), reader.get(), storage);
	
//	storage.saveToStream(cout);
}


} // unnamed namespace
