// must include this header to define meta data
#include "cpgf/gmetadefine.h"

// optional. Include if you want to auto define before "main" function.
#include "cpgf/goutmain.h"

#include <string>


class MyFirstBaseClass
{
private:
	int aaa;
};

class MySecondBaseClass
{
private:
	int bbb[5];
};

struct TestData
{
	TestData() : x(0), tag("")
	{
	}

	TestData(int x, const std::string & tag) : x(x), tag(tag)
	{
	}

	int x;
	std::string tag;
};

struct TestNoncopyable
{
};

class MyClass : virtual public MyFirstBaseClass, virtual public MySecondBaseClass
{
public:
	enum WindowStyle {
		ws0, ws1, ws2, ws3,
	};

	class AnotherInner {
	public:
		int y;

	};

public:
	MyClass() {}
	MyClass(int) {}
	MyClass(int, const std::string &) {}

	int getWidth() const { return width; }
	void incWidth(int, std::string &) {}

	const std::string & getName() const { return name; }
	TestData getData() const { return data; }

	MyClass operator + (int n) const { return MyClass(this->width + n); }

private:
	int width;
	TestNoncopyable nocopy;
	std::string name;
	TestData data;

private:
	friend void myMetaBuild(); // friend, so the function can build meta data for private members
};

int myStats;

int myAddNumber(int n, int delta)
{
	return n + delta;
}

template <typename T>
class MyTemplateBase
{
public:
	virtual ~MyTemplateBase() {
	}

	int getDouble() const {
		return sizeof(T) * 2;
	}

	int value;
};

template <typename T, typename P>
class MyTemplate : public MyTemplateBase<T>
{
public:
	int getSize() const {
		return sizeof(T);
	}

	int n;
};


template <typename ClassType>
void lazyDefineClass(cpgf::GDefineMetaClass<ClassType> define)
{
	define
		._method("getSize", &ClassType::getSize)
		._method("getDouble", &ClassType::getDouble)
	;
}

void myMetaBuild()
{
	using namespace cpgf;

	// define meta class
	GDefineMetaClass<MyClass, MyFirstBaseClass, MySecondBaseClass> // which class to define (MyClass), and give the base classes
		::define("MyClass_TheNameCanBeDiffWithClassName") // the class name

		// The meta class for MyClass has two annotations.
		// First's name is attribute, and has three elements.
		// Second's name is style.
		._annotation("attribute")
			._element("name", "What's my name") // element name, value is an Ansi string
			._element("cat", mcatClass) // element cat, value is int
			._element("dog", TestData(mcatClass, "A cute dog")) // element dog, value is TestData
		._annotation("style")
			._element("fit", L"fit me") // element fit, value is a Wide string.

	    // constructors
		._constructor<void * ()>()
		._constructor<void * (int)>()
		._constructor<void * (int, const std::string &)>(GMetaPolicyCopyAllConstReference())

		._field("width", &MyClass::width) // reflect a normal field
		._field("nocopy", &MyClass::nocopy, GMetaPolicyNoncopyable()) // reflect a field with policy

		._property("name", &MyClass::getName, 0) // a property has only getter, no setter
		._property("data", &MyClass::getData, &MyClass::data) // a property has both getter and setter.
		// also can reflect a property with policy
		
		._method("getWidth", &MyClass::getWidth) // reflect a normal method
		._method("incWidth", &MyClass::incWidth, GMetaPolicyCopyAllConstReference()) // reflect a method with policy
			._annotation("attr") // the method incWidth has an annotation
				._element("what", "ok")

		// operator
		._operator<MyClass (const GMetaSelf &, int)>(mopHolder + mopHolder)

		// enumerator
		._enum<MyClass::WindowStyle>("WindowStyle")
			._element("ws0", MyClass::ws0)
			._element("ws1", MyClass::ws1)
			._element("ws2", MyClass::ws2)
			._element("ws3", MyClass::ws3)

		// nested class
		._class(
			GDefineMetaClass<MyClass::AnotherInner>
				::declare("AnotherInner")
				._field("y", &MyClass::AnotherInner::y)
		)
	;

	// define global meta data

	GDefineMetaGlobal()
		._field("myStats", &myStats)
		._method("myAddNumber", &myAddNumber)
	;

	// define meta data for template
	GDefineMetaClass<MyTemplate<char, int> >::lazy("MyTemplate_char_int", &lazyDefineClass<MyTemplate<char, int> >); // instantiation
	GDefineMetaClass<MyTemplate<double, short> >::lazy("MyTemplate_double_short", &lazyDefineClass<MyTemplate<double, short> >); // another instantiation
}


G_RUN_BEFORE_MAIN(myMetaBuild())
