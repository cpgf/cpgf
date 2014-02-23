
/*
This tutorial demonstrates how to reflect meta data to a temporary meta class.
After used the meta data, the meta class will be destroyed so nothing pollutes
the global repository.
*/

#include "cpgf/gmetadefine.h"

#include <iostream>
using namespace std;

using namespace cpgf;

namespace {

void greeting(const char * message)
{
	cout << "I love local data, " << message << endl;
}

class Single
{
public:
	Single() : x(0) {
	}

	explicit Single(int x) : x(x) {
	}

public:
	int x;
};

template <typename Define>
void reflectPoint(Define define)
{
	define._field("x", &Single::x);
}


} // unnamed namespace


void run_a02()
{
	GDefineMetaClass<Single> defineForSingle = GDefineMetaClass<Single>::declare("Single");

	reflectPoint(defineForSingle);

	// Now we come to the key point in this tutorial.
	// We will create a virtual namespace and add meta data there.
	// The virtual namespace has no connection with C++ namespace.
	// A virtual namespace is just yet another meta class.
	// In fact cpgf reflection system is not aware of C++ namespace.
	// That means, we can add meta data for classes in different C++ namespace to same virtual namespace.
	//
	// Virtual namespace is a very powerful tool to avoid name clash.
	// For example, assume we are adding meta data for different libraries, such as OpenGL, Box2D.
	// Then we can add all meta data for OpenGL to a virtual namespace with name "gl", Box2D
	// to namespace with name "box2d". Then the symbol names in the two namespaces won't conflict
	// with the other, and the logic is very clean.
	// The meta data in cpgf library for various libraries, such as OpenGL, Box2D, SFML, etc,
	// heavily uses virtual namespaces.
	GDefineMetaNamespace defineForNamespace = GDefineMetaNamespace::declare("ns");

	// Reflect global function to the virtual namespace "ns".
	defineForNamespace._method("greeting", &greeting);

	// Insert meta class for Single.
	defineForNamespace._class(defineForSingle);

	// Now we have build all meta data and insert to the virtual namespace.

	// Now let's start using the meta data.

	// Get the meta class for virtual namespace.
	const GMetaClass * nsMetaClass = defineForNamespace.getMetaClass();

	const GMetaMethod * greetingMethod = nsMetaClass->getMethod("greeting");
	greetingMethod->invoke(NULL, "yes, exactly");

	const GMetaClass * singleMetaClass = nsMetaClass->getClass("Single");
	const GMetaField * field;
	GVariant value;

	Single single(6);

	field = singleMetaClass->getField("x");
	value = field->get(&single);
	cout << "single.x is " << fromVariant<int>(value) << " (should be 6)" << endl;
}


