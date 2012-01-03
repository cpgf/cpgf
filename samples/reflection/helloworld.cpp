#include "cpgf/gtestutil.h" // for test

#include "cpgf/gmetadefine.h"
#include "cpgf/goutmain.h"

#include "cpgf/gmetafield.h"
#include "cpgf/gmetaenum.h"
#include "cpgf/gmetaannotation.h"
#include "cpgf/gmetaoperator.h"
#include "cpgf/gmetaproperty.h"

#include <iostream>
#include <string>


namespace {

class FirstReflection
{
public:
	enum HelloStyle {
		hello = 1, hi = 2, nice = 3,
	};

	FirstReflection() : headCount(0) {
	}

	virtual std::string getSentence(HelloStyle style) const {
		switch(style) {
			case hello:
				return "Hello, guys.";
		
			case hi:
				return "Hi, folks.";
		
		    case nice:
			default:
				return "Nice to meet you.";
		}
	}

	void greet(const std::string & s) {
		std::cout << s << std::endl;
		std::cout << "There are total " << headCount << " persons here." << std::endl;
	}

	int headCount;
};


G_AUTO_RUN_BEFORE_MAIN()
{
	using namespace cpgf;

	GDefineMetaClass<FirstReflection>
		::define("FirstReflection")
		
		._enum<FirstReflection::HelloStyle>("HelloStyle")
			._element("hello", FirstReflection::hello)
			._element("hi", FirstReflection::hi)
			._element("nice", FirstReflection::nice)

		._method("getSentence", &FirstReflection::getSentence)
		._method("greet", &FirstReflection::greet, GMetaPolicyCopyAllConstReference())
		
		._field("headCount", &FirstReflection::headCount)
	;
}


void doTest()
{
	using namespace cpgf;
	using namespace std;

	const GMetaClass * metaClass;
	const GMetaEnum * metaEnum;

	metaClass = findMetaClass("FirstReflection");
	testCheckAssert(metaClass != NULL);

	metaEnum = metaClass->getEnum("HelloStyle");
	testCheckAssert(metaEnum != NULL);

	cout << "There are " << static_cast<unsigned int>(metaEnum->getCount()) << " kind of styles to say hello." << endl;
	for(uint32_t i = 0; i < metaEnum->getCount(); ++i) {
		cout << "The style secret key of No." << i << " is " << metaEnum->getKey(i) << endl;
	}

	const GMetaMethod * methodGetSentence = metaClass->getMethod("getSentence");
	testCheckAssert(methodGetSentence != NULL);

	const GMetaMethod * methodGreet = metaClass->getMethod("greet");
	testCheckAssert(methodGreet != NULL);

	const GMetaField * fieldHeadCount = metaClass->getField("headCount");
	testCheckAssert(fieldHeadCount != NULL);

	cout << "Now let's say hello one by one." << endl;
	void * instance = metaClass->createInstance();
	
	for(size_t i = 0; i < metaEnum->getCount(); ++i) {
		fieldHeadCount->set(instance, static_cast<int>((i + 1) * 10));
		string s = fromVariant<string>(methodGetSentence->invoke(instance, metaEnum->getValue(i)));
		methodGreet->invoke(instance, s);
	}

	metaClass->destroyInstance(instance);
}


gTestCase(doTest);


}
