#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

#include "python.h"

#include "cpgf/gscopedptr.h"

// test
#include "cpgf/gmetadefine.h"
#include <iostream>
using namespace std;


using namespace cpgf;

template <typename T>
struct GPythonScopedPointerDeleter
{
	static inline void Delete(T * p) {
		Py_DECREF(p);
	}
};



typedef GScopedPointer<PyObject, GPythonScopedPointerDeleter<PyObject> > GPythonScopedPointer;


namespace {

int xxx = 5;

template <typename D>
void register_Meta(D define)
{
	define._field("xxx", &xxx);
}

}


void doBindField(PyObject * object, IMetaField * field)
{
	PyObject_SetAttrString(object, field->getName(), object);
}

void doTestPythonBind()
{
	Py_Initialize();

	GPythonScopedPointer mainObject(PyImport_ImportModule("__main__"));
	
	GDefineMetaGlobal global;
	register_Meta(global);

	GScopedInterface<IMetaService> service(createDefaultMetaService());
	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> globalClass(module->getGlobalMetaClass());

	GScopedInterface<IMetaField> field;

	field.reset(globalClass->getField("xxx"));

}
