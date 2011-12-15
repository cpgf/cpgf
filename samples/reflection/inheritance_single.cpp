#include "reflect_common.h"


namespace {


class TestObject_InheritanceSingle
{
public:
	static const int ID = 1;

public:
	virtual std::string getName() = 0;

	int getID() {
		return ID;
	}

public:
	int value;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestObject_InheritanceSingle>
		::define("inheritance_single::TestObject_InheritanceSingle")
	
		._method("getName", &TestObject_InheritanceSingle::getName)
		._method("getID", &TestObject_InheritanceSingle::getID)
	;
}


class TestAnimal : public TestObject_InheritanceSingle
{
public:
	static const int ID = 2;
	static const std::string Name;

public:
	virtual std::string getName() {
		return Name;
	}

	int getID() {
		return ID;
	}

	virtual int born() {
		return 100;
	}

};
const std::string TestAnimal::Name = "animal";


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestAnimal, TestObject_InheritanceSingle>
		::define("inheritance_single::TestAnimal")
	
		._method("getID", &TestAnimal::getID)
		._method("born", &TestAnimal::born)
	;
}


class TestCat : public TestAnimal
{
public:
	static const int ID = 3;
	static const std::string Name;

public:
	virtual std::string getName() {
		return Name;
	}

	int getID() {
		return ID;
	}

	virtual int born() {
		return 101;
	}

};
const std::string TestCat::Name = "cat";


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<TestCat, TestAnimal>
		::define("inheritance_single::TestCat")
	
		._method("getID", &TestCat::getID)
	;
}


void doTest()
{
    using namespace cpgf;

	std::cout << "inheritance_single" << std::endl;

	const GMetaClass * catClass;
	const GMetaClass * animalClass;
	const GMetaMethod * method;

	catClass = findMetaClass("inheritance_single::TestCat"); testCheckAssert(catClass != NULL);
	testCheckEqual(catClass, findMetaClass(catClass->getItemType()));
	animalClass = findMetaClass("inheritance_single::TestAnimal"); testCheckAssert(animalClass != NULL);

	testCheckAssert(catClass->getBaseClass(0) != NULL);
	testCheckAssert(animalClass->getBaseClass(0) != NULL);

	{
		void * cat = catClass->createInstance();
		method = catClass->getMethodInHierarchy("getName", &cat); testCheckAssert(method != NULL);
		testCheckStringEqual(fromVariant<std::string>(method->invoke(cat)), TestCat::Name);

		// for single inheritance, animal == cat
		void * animal = catClass->castToBase(cat, 0);
		method = animalClass->getMethodInHierarchy("getName", &animal); testCheckAssert(method != NULL);
		// it's still cat's name, because animal is still an instance of Cat
		testCheckStringEqual(fromVariant<std::string>(method->invoke(animal)), TestCat::Name);

		catClass->destroyInstance(cat);
	}

	{
		method = catClass->getMethod("getID"); testCheckAssert(method != NULL);
		void * cat = catClass->createInstance();
		testCheckEqual(fromVariant<int>(method->invoke(cat)), TestCat::ID);

		void * animal = catClass->castToBase(cat, 0);
		method = animalClass->getMethodInHierarchy("getID", &animal); testCheckAssert(method != NULL);
		// getID is not virtual, so no polymorphism happens
		testCheckEqual(fromVariant<int>(method->invoke(animal)), TestAnimal::ID);

		catClass->destroyInstance(cat);
	}



}


gTestCase(doTest);


}

