
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
#include "cpgf/gscopedinterface.h"

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
	// Now let's see how to use interface based API.
	//
	// The APIs we used in tutorial a01 and a02, such as GMetaClass, GMetaField, etc, are class and function based API.
	// They are normal native C++ classes and functions.
	// The problem of the classes and functions, they are not binary compatible across dynamic libraries and compilers.
	// It's not easy, or even not possible, to use classes in another dynamic library compiled by another compiler.
	//
	// That's why interface based API was developed.
	// All interface based APIs are using interface. Or to say, Windows COM (Component Object Model) style interfaces.
	// All interfaces have similar name with their class counterpart. IMetaClass is the interface for GMetaClass, etc.
	//
	// Important reminding, interface based APIs are NOT optional.
	// These kind of APIs are the most important APIs in cpgf library.
	// Also, the serialization and script binding libraries, are developed upon interface based APIs.
	//
	// Now let's start using interface based API.
	// First let's get an instance of IMetaService.
	// IMetaService is a center to obtain all meta data in the system.
	// We call createDefaultMetaService() to get a service to represent the global meta repository.
	// NOTE: All interface instance returned by cpgf must be freed (releaseReference) by the caller.
	// The simplest way to do it is to use GScopedInterface to hold the instance.
	GScopedInterface<IMetaService> service(createDefaultMetaService());

	GScopedInterface<IMetaMethod> method;
	GScopedInterface<IMetaField> field;
	GVariant value;

	// Get the global meta class to use the function "greeting".
	// The service returned by createDefaultMetaService will only hold one module to represent current program.
	// The meta service is designed to work on multiple dynamic libraries.
	// A dynamic library (include the program itself), is called a module.
	// Each module has only one meta repository.
	// Please bear in mind, cpgf was not tested in cross dynamic library environment yet, so the service/module
	// architecture may not be matured.
	GScopedInterface<IMetaModule> globalModule(service->getModuleAt(0));
	GScopedInterface<IMetaClass> globalMetaClass(globalModule->getGlobalMetaClass());
	method.reset(globalMetaClass->getMethod("greeting"));
	metaInvokeMethod(method.get(), NULL, "world");

	GScopedInterface<IMetaClass> pointMetaClass(service->findClassByName("Point"));

	Point point(5, 8);

	field.reset(pointMetaClass->getField("x"));
	// Here we get value from the field.
	// Unlike the class and function based API which passes GVariant and GMetaType around,
	// The interface based API uses GVariantData and GMetaTypeData to gain the binary compatibility.
	// Fortunately, in the header gmetaapiutil.h, there are a bunch of utility functions to hide the conversion
	// between GVariant and GVariantData, GMetaType and GMetaTypeData.
	value = metaGetValue(field.get(), &point);
	cout << "point.x is " << fromVariant<int>(value) << " (should be 5)" << endl;

	method.reset(pointMetaClass->getMethod("extend"));
	// Another handy utility function to invoke the method.
	metaInvokeMethod(method.get(), &point, 2);
	cout << "After extend, point.x is " << fromVariant<int>(metaGetValue(field.get(), &point)) << " (should be 10)" << endl;


	method.reset(pointMetaClass->getMethod("getArea"));
	value = metaInvokeMethod(method.get(), &point);
	cout << "The area is " << fromVariant<int>(value) << " (should be 160)" << endl;

	Point * p;

	p = static_cast<Point *>(pointMetaClass->createInstance());
	cout << "createInstance -- point.x is " << p->x << " (should be 0)" << endl;
	pointMetaClass->destroyInstance(p);

	GScopedInterface<IMetaConstructor> constructor(pointMetaClass->getConstructorAt(0));
	p = static_cast<Point *>(metaInvokeConstructor(constructor.get(), 3, 8));
	cout << "constructor -- point.x is " << p->x << " (should be 3)" << endl;
	pointMetaClass->destroyInstance(p);

}


