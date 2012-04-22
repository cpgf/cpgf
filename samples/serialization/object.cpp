// to read object from archive
#include "cpgf/serialization/gmetaarchivereader.h"

// to write object to archive
#include "cpgf/serialization/gmetaarchivewriter.h"

// storage of text stream
#include "cpgf/serialization/gmetastorage_textstream.h"

// storage of XML
#include "cpgf/serialization/gmetastorage_xml.h"

// storage of JSON
#include "cpgf/serialization/gmetastorage_json.h"

// meta traits -- to support STL string
#include "cpgf/metatraits/gmetaserializer_string.h"

// meta traits -- to support C++ array (T[])
#include "cpgf/metatraits/gmetaserializer_array.h"

#include "cpgf/gmetadefine.h"

#include <sstream>
#include <string>

#include <iostream>

using namespace std;
using namespace cpgf;


// uncomment this line to see how to disable serialization for a whole class
//#define DISABLE_MYDATA


void check(bool b);


class MyData
{
public:
	MyData()
		: d(0)
	{
	}
	
public:
	int d;
};

class MyObject
{
public:
	MyObject()
		: n(0), s(""), pdata(new MyData), notSerializedN(0), pn(NULL), pp(NULL)
	{
		arr[0] = 0;
		arr[1] = 0;
		arr[2] = 0;
	}
	
	~MyObject()
	{
		delete this->pdata;
	}
	
public:
	int n;
	string s;
	int arr[3];
	MyData data;
	MyData * pdata;

	// will disable it in annotation so it's not serialized
	int notSerializedN;
	
	// will not be serialized, no need annotation
	int * pn;
	// will not be serialized, no need annotation
	void ** pp;
};

template <typename D>
void register_MyObject(D define)
{
	GDefineMetaClass<MyData> dataDefine = GDefineMetaClass<MyData>::declare("MyData");

	// meta data for MyData	
	dataDefine
#ifdef DISABLE_MYDATA	
		// This annotation disable serialization for MyData, any object of MyData will not be serialized.
		._annotation(SerializationAnnotation)
			._element(SerializationAnnotationEnable, false)
#endif

		._field("d", &MyData::d)
	;

	define._class(dataDefine);
	
	GDefineMetaClass<MyObject> classDefine = GDefineMetaClass<MyObject>::declare("MyObject");
	
	classDefine
		._field("n", &MyObject::n)
		._field("s", &MyObject::s)
		._field("arr", &MyObject::arr)
		._field("data", &MyObject::data)
		._field("pdata", &MyObject::pdata)

		._field("notSerializedN", &MyObject::notSerializedN)
			._annotation(SerializationAnnotation)
				._element(SerializationAnnotationEnable, false)

		._field("pn", &MyObject::pn)
		._field("pp", &MyObject::pp)
	;

	define._class(classDefine);
}

void doTestObject(IMetaStorageWriter * writer, IMetaStorageReader * reader)
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	
	GScopedInterface<IMetaArchiveWriter> archiveWriter(createMetaArchiveWriter(service.get(), writer));

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("MyObject"));

	MyObject instance;
	
	instance.n = 5;
	instance.s = "cpgf serialization library";
	instance.data.d = 18;
	instance.pdata->d = 78;
	instance.arr[0] = 1999;
	instance.arr[1] = 2008;
	instance.arr[2] = 2012;
	instance.notSerializedN = 38;
	instance.pp = (void **)50;

	serializeWriteData(archiveWriter.get(), "mystring", string("This is a string"));
	serializeWriteObject(archiveWriter.get(), "myobj", &instance, metaClass.get());

	GScopedInterface<IMetaArchiveReader> archiveReader(createMetaArchiveReader(service.get(), reader));
	
	MyObject readInstance;
	string s;

	// In real code, we don't need to set the values before deserialization
	// We set the values here just for demonstration purpose,
	// and to see which value is changed which is not.
	readInstance.n = 98; // will be overwritten after deserialization
	readInstance.s = "abc"; // will be overwritten after deserialization
	readInstance.data.d = 3;
	readInstance.pdata->d = 36;
	readInstance.notSerializedN = 123;
	readInstance.pn = (int *)0xfead;
	readInstance.pp = (void **)0xbed;
	
	serializeReadData(archiveReader.get(), "mystring", s);
	serializeReadObject(archiveReader.get(), "myobj", &readInstance, metaClass.get());

	check(s == "This is a string"); // has read string
	
	check(readInstance.n == 5); // has read n
	check(readInstance.s == "cpgf serialization library"); // has read s

#ifdef DISABLE_MYDATA
	check(readInstance.data.d == 3); // not read data because MyData is disabled
	check(readInstance.pdata->d == 36); // not read pdata because MyData is disabled
#else
	check(readInstance.data.d == 18); // has read data
	check(readInstance.pdata->d == 78); // has read pdata
#endif

	check(readInstance.arr[0] == 1999); // has read arr
	check(readInstance.arr[1] == 2008); // has read arr
	check(readInstance.arr[2] == 2012); // has read arr
	check(readInstance.notSerializedN == 123); // not read
	check(readInstance.pn == (int *)0xfead); // not read
	check(readInstance.pp == (void **)0xbed); // not read
}

void testObject_TextStream()
{
	stringstream stream;

	GScopedInterface<IMetaStorageWriter> writer(createTextStreamStorageWriter(stream));
	GScopedInterface<IMetaStorageReader> reader(createTextStreamStorageReader(stream));
	
	doTestObject(writer.get(), reader.get());

	cout << "Text stream" << endl;
	cout << stream.str().c_str() << endl;
}


void testObject_XML()
{
	GMetaXmlStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createXmlStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createXmlStorageReader(storage));
	
	doTestObject(writer.get(), reader.get());
	
	cout << "XML" << endl;
	storage.saveToStream(cout);
}


void testObject_Json()
{
	GMetaJsonStorage storage;

	GScopedInterface<IMetaStorageWriter> writer(createJsonStorageWriter(storage));
	GScopedInterface<IMetaStorageReader> reader(createJsonStorageReader(storage));
	
	doTestObject(writer.get(), reader.get());
	
	cout << "JSON" << endl;
	storage.saveToStream(cout);
}


void testObject()
{
	// Here we just register the meta data to global.
	// This is the easest but may pollute the global namespace.
	// To register the meta data to temporary meta class, see how we did in unit tests.
	register_MyObject(GDefineMetaGlobal());
	
	cout << "Object: " << endl;
	
	testObject_TextStream();
	testObject_XML();
	testObject_Json();
}

