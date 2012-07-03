
/*
NOTE: This tutorial must be executed after tutorial a01.
This tutorial uses the meta data registered in a01.

This tutorial demonstrates how to use interface based API
to access meta data in global repository.
*/

// We must include this header to use interface based API.
#include "cpgf/gmetaapi.h"

#include "cpgf/gmetaapiutil.h"

#include "cpgf/gmetaclass.h"

#include <iostream>
using namespace std;

using namespace cpgf;

namespace {

// Duplicated the class prototype here 
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

} // unnamed namespace


void run_a03()
{
	GScopedInterface<IMetaService> service(createDefaultMetaService());

	GScopedInterface<IMetaModule> globalModule(service->getModuleAt(0));
	GScopedInterface<IMetaClass> pointMetaClass(service->findClassByName("Point"));

	GScopedInterface<IMetaMethod> method;
	GScopedInterface<IMetaField> field;
	GVariant value;

	GScopedInterface<IMetaClass> globalMetaClass(globalModule->getGlobalMetaClass());
	method.reset(globalMetaClass->getMethod("greeting"));
	metaInvokeMethod(method, NULL, "world");

	Point point(5, 8);

	// Get the meta field for "x".	
	field.reset(pointMetaClass->getField("x"));
	// Get the value of "x". The result is a GVariant.
	value = metaGetValue(field, &point);
	// Print the value. We use fromVariant to convert a GVariant to relevant C++ type.
	cout << "point.x is " << fromVariant<int>(value) << " (should be 5)" << endl;

	// Get the meta method for "extend".
	method.reset(pointMetaClass->getMethod("extend"));
	// Invoke Point::extend.
	metaInvokeMethod(method, &point, 2);
	cout << "After extend, point.x is " << fromVariant<int>(metaGetValue(field, &point)) << " (should be 10)" << endl;


	// Get the meta method for "getArea".
	method.reset(pointMetaClass->getMethod("getArea"));
	// Invoke Point::getArea, and obtain the return value.
	value = metaInvokeMethod(method, &point);
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
	GScopedInterface<IMetaConstructor> constructor(pointMetaClass->getConstructorAt(0));
	p = static_cast<Point *>(metaInvokeConstructor(constructor, 3, 8));
	cout << "constructor -- point.x is " << p->x << " (should be 3)" << endl;
	pointMetaClass->destroyInstance(p);

}


