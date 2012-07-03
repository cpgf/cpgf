
/*
This tutorial is the very first step to learn how to build meta data.
All the libraries, cpgf reflection, cpgf serialization, cpgf script binding,
are heavily depending on meta data.

This tutorial demonstrates:
1, Reflect global method.
2, Reflect class, object method, and object field.
3, Add meta data to global repository.
4, Use the meta data.

Please keep in mind, though the first step is to add meta data to global repository,
it's highly recommended to avoid polluting the global repository when possible.
We will learn how to use meta data locally in other tutorials.
*/

// This header must be included to build meta data
#include "cpgf/gmetadefine.h"

#include <iostream>
using namespace std;

// The namespace for cpgf library.
using namespace cpgf;

// We use unnamed namespace to avoid name clash with other tutorials.
// The namespace has nothing with the tutorial.
namespace {

// The global function we will reflect for.
void greeting(const char * message)
{
	cout << "Hello, " << message << endl;
}

// The class we are going to reflect for.
class Point
{
public:
	Point() : x(0), y(0) {
	}

	Point(int x, int y) : x(x), y(y) {
	}

	void extend(int scale) {
		x *= scale;
		y *= scale;
	}

	int getArea() const {
		return x * y;
	}

public:
	int x;
	int y;
};

// Reflect for class Point.
// "define" is a meta define to hold the meta data. We will explain it later.
template <typename Define>
void reflectPoint(Define define)
{
	// Reflect the method "extend".	
	define._method("extend", &Point::extend);
	
	// Reflect the method "getArea".
	// The type Define::ClassType is just Point.
	// So this line is equivalent to
	// 	define._method("getArea", &Point::getArea);
	define._method("getArea", &Define::ClassType::getArea);

	// Reflect the non-default constructor.
	// The constructor prototype is a function which always returns void *. This is required by the reflection system.
	define.template _constructor<void * (int, int)>();

	// Reflect the field "x" and "y".
	define._field("x", &Point::x);
	define._field("y", &Point::y);
}


} // unnamed namespace


void run_a01()
{
	// Create meta define for class Point.
	// For a class C, it's meta define type is GDefineMetaClass<C>.
	// GDefineMetaClass has two static function to create a define, GDefineMetaClass::define and GDefineMetaClass::declare.
	// GDefineMetaClass::define create the meta class and insert it to global meta data repository.
	// GDefineMetaClass::declare create the meta class, but doesn't insert it to global.
	// Here we use "declare", and will insert the meta class to global later manually.
	GDefineMetaClass<Point> defineForPoint = GDefineMetaClass<Point>::declare("Point");

	// Now call our reflection function to build meta data.
	reflectPoint(defineForPoint);

	// Now defineForPoint contains all meta data for class Point.

	// Now get the global meta data repository.
	// Don't misunderstand! The global meta data repository is not a must.
	// It's quite possible to build and use meta data locally.
	// We will explain that in other tutorials.
	// Also it's encouraged to avoid global meta data whenever you can.
	GDefineMetaGlobal defineForGlobal;

	// Reflect global function to global meta class.
	defineForGlobal._method("greeting", &greeting);

	// Register meta class for Point to global.
	defineForGlobal._class(defineForPoint);

	// Now we have build all meta data and register to global meta data repository.
	// We don't need defineForGlobal any more.
	// We can get back the global meta class in a global way.

	// Now let's start using the meta data.

	// Get the global meta class.
	// In one program, there is only one global meta class.
	// But a shared library may have its own global meta class.
	const GMetaClass * globalMetaClass = getGlobalMetaClass();

	// Get the meta method for global function "greeting".
	const GMetaMethod * greetingMethod = globalMetaClass->getMethod("greeting");
	// Invoke the global function.
	// Most meta functions need an object instance to work with.
	// For global functions and fields, just pass the instance as NULL.
	greetingMethod->invoke(NULL, "world");

	// Now let's use the class Point via reflection system.
	const GMetaClass * pointMetaClass = globalMetaClass->getClass("Point");
	const GMetaField * field;
	const GMetaMethod * method;
	GVariant value;

	// Create an instance for Point to be used by meta functions.
	Point point(5, 8);

	// Get the meta field for "x".	
	field = pointMetaClass->getField("x");
	// Get the value of "x". The result is a GVariant.
	value = field->get(&point);
	// Print the value. We use fromVariant to convert a GVariant to relevant C++ type.
	cout << "point.x is " << fromVariant<int>(value) << " (should be 5)" << endl;

	// Get the meta method for "extend".
	method = pointMetaClass->getMethod("extend");
	// Invoke Point::extend.
	method->invoke(&point, 2);
	cout << "After extend, point.x is " << fromVariant<int>(field->get(&point)) << " (should be 10)" << endl;

	// Get the meta method for "getArea".
	method = pointMetaClass->getMethod("getArea");
	// Invoke Point::getArea, and obtain the return value.
	value = method->invoke(&point);
	// Print the return value.
	cout << "The area is " << fromVariant<int>(value) << " (should be 160)" << endl;

	// Now let's learn something about constructing object and meta constructor.

	Point * p;

	// Create an instance with default constructor.
	// createInstance doesn't need the default ctor to be reflected.
	p = static_cast<Point *>(pointMetaClass->createInstance());
	cout << "createInstance -- point.x is " << p->x << " (should be 0)" << endl;
	// Don't forget to free the object once we don't need it.
	pointMetaClass->destroyInstance(p);

	// Get the meta constructor of void *(int, int). 0 is the index.
	// If there is another meta constructor in front of this one in the meta data, the index should be 1.
	const GMetaConstructor * constructor = pointMetaClass->getConstructorAt(0);
	p = static_cast<Point *>(constructor->invoke(3, 8));
	cout << "constructor -- point.x is " << p->x << " (should be 3)" << endl;
	pointMetaClass->destroyInstance(p);
}


