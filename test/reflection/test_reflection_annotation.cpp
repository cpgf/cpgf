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

namespace {

void lazyDefineClass(GDefineMetaClass<CLASS> define)
{
	define
		._annotation("attribute")
			._element("name", L"TestClass_Annotation")
			._element("cat", mcatClass)
			._element("dog", TestData(mcatClass, NAME_CLASS))
			._element("desc", "Description")

		._enum<CLASS::WindowStyle>("WindowStyle")
			._element("CLASS:ws0", CLASS::ws0)
			._element("CLASS:ws1", CLASS::ws1)
			._element("CLASS:ws2", CLASS::ws2)
			._element("CLASS:ws3", CLASS::ws3)
			._annotation("attribute")
				._element("name", L"WindowStyle")
				._element("cat", mcatEnum)
				._element("dog", TestData(mcatEnum, "WindowStyle"))
				._element("desc", "Description")

		._field("width", &CLASS::width)
			._annotation("attribute")
				._element("name", L"width")
				._element("cat", mcatField)
				._element("dog", TestData(mcatField, "width"))
				._element("desc", "Description")
	;
}

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS>::lazy(NAME_CLASS, &lazyDefineClass);
}

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
		GEQUAL(anno->getValue("doesntExist"), NULL);
		GEQUAL(anno->getCount(), 4);

		if(string(anno->getName()) == "attribute") {
			foundAttribute = true;

			const GAnnotationValue * value;

			value = anno->getValue("name"); GCHECK(value != NULL);
			GEQUAL(std::string(anno->getNameAt(0)), "name");
			GEQUAL(anno->getValueAt(0), value);
			GCHECK(value->canToWideString());
			GEQUAL(value->toWideString(), stringToWString(item->getName()));

			value = anno->getValue("cat"); GCHECK(value != NULL);
			GEQUAL(std::string(anno->getNameAt(1)), "cat");
			GEQUAL(anno->getValueAt(1), value);
			GCHECK(value->canToInt());
			GEQUAL(value->toInt(), item->getCategory());

			value = anno->getValue("dog"); GCHECK(value != NULL);
			GEQUAL(std::string(anno->getNameAt(2)), "dog");
			GEQUAL(anno->getValueAt(2), value);
			TestData data = value->toObject<TestData>();
			GEQUAL(data, TestData(item->getCategory(), item->getName()));

			value = anno->getValue("desc"); GCHECK(value != NULL);
			GEQUAL(std::string(anno->getNameAt(3)), "desc");
			GEQUAL(anno->getValueAt(3), value);
			GCHECK(value->canToString());
			GEQUAL(value->toString(), std::string("Description"));
		}
	}

	GCHECK(foundAttribute);
}

void testItem(cpgf::IMetaItem * item)
{
	using namespace cpgf;
	using namespace std;

	if(item->getCategory() == mcatAnnotation) {
		return;
	}

	bool foundAttribute = false;
	for(unsigned int i = 0; i < item->getAnnotationCount(); ++i) {
		GScopedInterface<IMetaAnnotation> anno(item->getAnnotationAt(i));
		
		GEQUAL(anno->getValue("doesntExist"), NULL);

		GScopedInterface<IMetaItem> tempItem(anno->getMetaItem());

		if(string(anno->getName()) == "attribute") {
			foundAttribute = true;

			GScopedInterface<IMetaAnnotationValue> value;

			value.reset(anno->getValue("name")); GCHECK(value);
			GCHECK(!! value->canToWideString());
			GEQUAL(value->toWideString(), stringToWString(item->getName()));

			value.reset(anno->getValue("cat")); GCHECK(value);
			GCHECK(!! value->canToInt());
			GEQUAL(value->toInt32(), static_cast<int>(item->getCategory()));

			value.reset(anno->getValue("dog")); GCHECK(value);
			TestData data = fromVariant<TestData>(metaGetAnnotationVariant(value.get()));
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
	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GCHECK(service);

	GScopedInterface<IMetaClass> metaClass(service->findClassByName(NAME_CLASS));
	GCHECK(metaClass);

	metaClass->getMetaCount();

	testItem(metaClass.get());

	for(unsigned int i = 0; i < metaClass->getMetaCount(); ++i) {
		GScopedInterface<IMetaItem> temp(metaClass->getMetaAt(i));
		testItem(temp.get());
	}

}


} }


#if defined(_MSC_VER)
#pragma warning(pop)
#endif

