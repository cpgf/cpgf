#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#endif


#include "reflect_common.h"

#include <algorithm>


namespace {

std::wstring stringToWString(const std::string & s)
{
    std::wstring temp(s.length(),L' ');

    std::copy(s.begin(), s.end(), temp.begin());

    return temp;
}


std::string wstringToString(const std::wstring & s)
{
    std::string temp(s.length(), ' ');

    std::copy(s.begin(), s.end(), temp.begin());

    return temp;
}

class TestObject
{
public:
	enum WindowStyle {
		ws0, ws1, ws2, ws3,
	};

public:
	int width;
};

GMETA_DEFINE_CLASS(TestObject, TestObject, "annotation::TestObject") {
	using namespace cpgf;

	reflectAnnotation("attribute")
		("name", L"annotation::TestObject")
		("cat", mcatClass)
		("dog", TestData(mcatClass, "annotation::TestObject"))
	;
	flushAnnotation();

	reflectAnnotation("attribute")
		("name", L"WindowStyle")
		("cat", mcatEnum)
		("dog", TestData(mcatEnum, "WindowStyle"))
	;
	GMETA_ENUM(WindowStyle, TestObject::ws0, TestObject::ws1, TestObject::ws2, TestObject::ws3);

	GMETA_FIELD(width);
	reflectAnnotation("attribute")
		("name", L"width")
		("cat", mcatField)
		("dog", TestData(mcatField, "width"))
	;
	flushAnnotation();
}

void testItem(const cpgf::GMetaItem * item)
{
	using namespace cpgf;
	using namespace std;

	if(item->isAnnotation()) {
		return;
	}

	bool foundAttribute = false;

	for(size_t i = 0; i < item->getAnnotationCount(); ++i) {
		const GMetaAnnotation * anno = item->getAnnotationAt(i);

		testCheckEqual(anno->getMetaItem(), item);

		if(string(anno->getName()) == "attribute") {
			foundAttribute = true;

			const GAnnotationValue * value;

			value = anno->getValue("name"); testCheckAssert(value != NULL);
			testCheckAssert(value->canToWideString());
			testCheckEqual(value->toWideString(), stringToWString(item->getName()));

			value = anno->getValue("cat"); testCheckAssert(value != NULL);
			testCheckAssert(value->canToInt());
			testCheckEqual(value->toInt(), item->getCategory());

			value = anno->getValue("dog"); testCheckAssert(value != NULL);
			TestData data = value->toObject<TestData>();
			testCheckEqual(data, TestData(item->getCategory(), item->getName()));
		}
	}

	testCheckAssert(foundAttribute);
}

void testItem(cpgf::IMetaItem * item)
{
	using namespace cpgf;
	using namespace std;

//	if(item->isAnnotation()) {
	if(item->getCategory() == mcatAnnotation) {
		return;
	}

	bool foundAttribute = false;
	for(unsigned int i = 0; i < item->getAnnotationCount(); ++i) {
		GApiScopedPointer<IMetaAnnotation> anno(item->getAnnotationAt(i));

		GApiScopedPointer<IMetaItem> tempItem(anno->getMetaItem());

		if(string(anno->getName()) == "attribute") {
			foundAttribute = true;

			GApiScopedPointer<IMetaAnnotationValue> value;

			value.reset(anno->getValue("name")); testCheckAssert(value);
			testCheckAssert(!! value->canToWideString());
			testCheckEqual(value->toWideString(), stringToWString(item->getName()));

			value.reset(anno->getValue("cat")); testCheckAssert(value);
			testCheckAssert(!! value->canToInt());
			testCheckEqual(value->toInt32(), static_cast<int>(item->getCategory()));

			value.reset(anno->getValue("dog")); testCheckAssert(value);
			TestData data = fromVariant<TestData>(metaGetAnnotationVariant(value));
			testCheckEqual(data, TestData(item->getCategory(), item->getName()));
		}
	}

	testCheckAssert(foundAttribute);
}

void doTestLib()
{
	using namespace cpgf;
	using namespace std;

	const GMetaClass * metaClass;

	metaClass = findMetaClass("annotation::TestObject");
	testCheckAssert(metaClass != NULL);
	std::cout << "Lib: " << metaClass->getName() << std::endl;

	metaClass->getMetaCount(); // ensure registered

	testItem(metaClass);
	for(size_t i = 0; i < metaClass->getMetaCount(); ++i) {
		testItem(metaClass->getMetaAt(i));
	}

}

void doTestAPI()
{
	using namespace cpgf;
	using namespace std;

	GApiScopedPointer<IMetaService> service(createDefaultMetaService());
	testCheckAssert(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName("annotation::TestObject"));
	testCheckAssert(metaClass);

	std::cout << "API: " << metaClass->getName() << std::endl;

	metaClass->getMetaCount();

	testItem(metaClass.get());

	for(unsigned int i = 0; i < metaClass->getMetaCount(); ++i) {
		GApiScopedPointer<IMetaItem> temp(metaClass->getMetaAt(i));
		testItem(temp.get());
	}

}


void doTest()
{
	doTestLib();
	doTestAPI();
}


gTestCase(doTest);


}


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

