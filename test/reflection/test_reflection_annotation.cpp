#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4244)
#endif


#include "test_reflection_common.h"

#define CLASS TestClass_Annotation
#define NAME_CLASS GPP_STRINGIZE(CLASS)

using namespace std;
using namespace cpgf;



namespace Test_Annotation { namespace {

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

class TestData
{
public:
	TestData()
		:	x(1999),
			tag("a tag")
	{
	}

	TestData(int x, std::string tag)
		:	x(x),
			tag(tag)
	{
	}

	TestData(const TestData & other)
		:	x(other.x),
			tag(other.tag)
	{
	}

	TestData & operator = (const TestData & other) {
		this->x = other.x;
		this->tag = other.tag;

		return *this;
	}

	bool operator == (const TestData & other) const {
		return this->x == other.x
			&& this->tag == other.tag;
	}

public:
	int x;
	std::string tag;
};


class CLASS
{
public:
	enum WindowStyle {
		ws0, ws1, ws2, ws3,
	};

public:
	int width;
};

GMETA_DEFINE_CLASS(CLASS, CLASS, NAME_CLASS) {
	using namespace cpgf;

	reflectAnnotation("attribute")
		("name", L"TestClass_Annotation")
		("cat", mcatClass)
		("dog", TestData(mcatClass, NAME_CLASS))
	;
	flushAnnotation();

	reflectAnnotation("attribute")
		("name", L"WindowStyle")
		("cat", mcatEnum)
		("dog", TestData(mcatEnum, "WindowStyle"))
	;
	GMETA_ENUM(WindowStyle, CLASS::ws0, CLASS::ws1, CLASS::ws2, CLASS::ws3);

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

	if(metaIsAnnotation(item->getCategory())) {
		return;
	}

	bool foundAttribute = false;

	for(size_t i = 0; i < item->getAnnotationCount(); ++i) {
		const GMetaAnnotation * anno = item->getAnnotationAt(i);

		GEQUAL(anno->getMetaItem(), item);

		if(string(anno->getName()) == "attribute") {
			foundAttribute = true;

			const GAnnotationValue * value;

			value = anno->getValue("name"); GCHECK(value != NULL);
			GCHECK(value->canToWideString());
			GEQUAL(value->toWideString(), stringToWString(item->getName()));

			value = anno->getValue("cat"); GCHECK(value != NULL);
			GCHECK(value->canToInt());
			GEQUAL(value->toInt(), item->getCategory());

			value = anno->getValue("dog"); GCHECK(value != NULL);
			TestData data = value->toObject<TestData>();
			GEQUAL(data, TestData(item->getCategory(), item->getName()));
		}
	}

	GCHECK(foundAttribute);
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

			value.reset(anno->getValue("name")); GCHECK(value);
			GCHECK(!! value->canToWideString());
			GEQUAL(value->toWideString(), stringToWString(item->getName()));

			value.reset(anno->getValue("cat")); GCHECK(value);
			GCHECK(!! value->canToInt());
			GEQUAL(value->toInt32(), static_cast<int>(item->getCategory()));

			value.reset(anno->getValue("dog")); GCHECK(value);
			TestData data = fromVariant<TestData>(metaGetAnnotationVariant(value));
			GEQUAL(data, TestData(item->getCategory(), item->getName()));
		}
	}

	GCHECK(foundAttribute);
}

GTEST(Lib_Annotation)
{
	const GMetaClass * metaClass;

	metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass != NULL);

	metaClass->getMetaCount(); // ensure registered

	testItem(metaClass);
	for(size_t i = 0; i < metaClass->getMetaCount(); ++i) {
		testItem(metaClass->getMetaAt(i));
	}

}

GTEST(API_Annotation)
{
	GApiScopedPointer<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GApiScopedPointer<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	metaClass->getMetaCount();

	testItem(metaClass.get());

	for(unsigned int i = 0; i < metaClass->getMetaCount(); ++i) {
		GApiScopedPointer<IMetaItem> temp(metaClass->getMetaAt(i));
		testItem(temp.get());
	}

}


} }


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

