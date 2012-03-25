#include "testserializationcommon.h"

#include "gmetatextstreamarchive.h"

#include "cpgf/ginterface.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gflags.h"

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "gmetaarchivetypemap.h"

#include <string.h>

#include <map>


#include <iostream>
#include <sstream>
using namespace std;


using namespace cpgf;

class TestClassSerializeBase {
public:
	TestClassSerializeBase() : baseA(5.8) {
	}

	double baseA;
};

class TestClassSerialize : public TestClassSerializeBase {
public:
	TestClassSerialize() : a(NULL), object(NULL) {
		this->arr[0] = 138;
		this->arr[1] = 139;
		this->arr[2] = 168;
	}

	~TestClassSerialize() {
		delete object;
	}

	const char * a;
	int fieldInt;
	string fieldString;
	
	int fieldReadonlyInt;
	string fieldWriteonlyString;

	TestClassSerialize * object;
	int arr[3];
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestClassSerializeBase>
		::define("TestClassSerializeBase")

		._field("baseA", &TestClassSerializeBase::baseA)
	;

	GDefineMetaClass<TestClassSerialize, TestClassSerializeBase>
		::define("TestClassSerialize")

		._field("a", &TestClassSerialize::a)
			._annotation("serialize")
				._element("enable", false)

//		._field("fieldInt", &TestClassSerialize::fieldInt)
//		._field("fieldString", &TestClassSerialize::fieldString)
//		._field("fieldReadonlyInt", &TestClassSerialize::fieldReadonlyInt, GMetaPolicyReadOnly())
//		._field("fieldWriteonlyString", &TestClassSerialize::fieldWriteonlyString, GMetaPolicyWriteOnly())
//		._field("object", &TestClassSerialize::object)
		._field("arr", &TestClassSerialize::arr)
	;
}

void testSer()
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());

	stringstream stream;

	GTextStreamMetaWriter<stringstream> outputStream(stream);
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(0, service.get(), &outputStream));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("TestClassSerialize"));

	GScopedInterface<IMetaField> field;

	TestClassSerialize instance;
	TestClassSerialize * pobj = &instance;
	instance.fieldInt = 38;
	instance.a = "abcdef";
	instance.fieldString = "STL string";
	instance.object = new TestClassSerialize;
	instance.object->fieldInt = 98;

	field.reset(metaClass->getField("fieldInt"));
//	archiveWriter->writeField(field->getName(), pobj, field.get());

	archiveWriter->writeObjectValue("obj", pobj, metaClass.get());

	cout << stream.str().c_str();

	stream.seekg(0);

	GTextStreamMetaReader<stringstream> inputStream(service.get(), stream);
	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(0, service.get(), &inputStream));
	TestClassSerialize readInstance;
	archiveReader->readObject("", &readInstance, metaClass.get());
	cout << endl << "Read object" << endl;
	cout << readInstance.fieldInt << endl;
//	cout << readInstance.object->fieldInt << endl;
//	cout << readInstance.fieldString << endl;
	cout << "ArraySize: " << ArraySize<int [3][5]>::Result << endl;
}

