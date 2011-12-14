#include "test_reflection_common.h"

#define CLASS TestClass_Template
#define NAME_CLASS GPP_STRINGIZE(CLASS)

using namespace std;
using namespace cpgf;


namespace Test_Template { namespace {


template <typename T>
class TestBase
{
public:
	virtual ~TestBase() {
	}

	int getDouble() const {
		return sizeof(T) * 2;
	}

	int value;
};

template <typename T, typename P>
class CLASS : public TestBase<T>
{
public:
	int getSize() const {
		return sizeof(T);
	}

	int n;
};


template <typename ClassType>
void lazyDefineClass(GDefineMetaClass<ClassType> define)
{
	define
		._method("getSize", &ClassType::getSize)
		._method("getDouble", &ClassType::getDouble)
	;
}

G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<CLASS<char, int> >::lazy(NAME_CLASS, &lazyDefineClass<CLASS<char, int> >);
}


GTEST(ReflectTemplate)
{
	using namespace cpgf;

	const GMetaClass * metaClass;
	const GMetaMethod * method;

	metaClass = findMetaClass(NAME_CLASS);
	GCHECK(metaClass != NULL);

	{
		void * obj = metaClass->createInstance();
		method = metaClass->getMethodInHierarchy("getSize", &obj);
		GCHECK(method != NULL);
		GEQUAL(fromVariant<unsigned int>(method->invoke(obj)), sizeof(char));
		metaClass->destroyInstance(obj);
	}

	{
		void * obj = metaClass->createInstance();
		method = metaClass->getMethodInHierarchy("getDouble", &obj);
		GCHECK(method != NULL);
		GEQUAL(fromVariant<unsigned int>(method->invoke(obj)), 2 * sizeof(char));
		metaClass->destroyInstance(obj);
	}

}



} }
