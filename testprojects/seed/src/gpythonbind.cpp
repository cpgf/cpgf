#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

#include "python.h"

#include "cpgf/gscopedptr.h"

#include "pinclude/gbindcommon.h"
#include "pinclude/gscriptbindapiimpl.h"

// test
#include "cpgf/gmetadefine.h"
#include <iostream>
using namespace std;


using namespace cpgf;
using namespace cpgf::bind_internal;

template <typename T>
struct GPythonScopedPointerDeleter
{
	static inline void Delete(T * p) {
		Py_DECREF(p);
	}
};



typedef GScopedPointer<PyObject, GPythonScopedPointerDeleter<PyObject> > GPythonScopedPointer;


namespace {

class GPythonObject : public PyObject
{
public:
	GPythonObject(IMetaService * service, void * instance);

	IMetaService * getService() const;

private:
	GScopedInterface<IMetaService> service;
	void * instance;
};

GPythonObject::GPythonObject(IMetaService * service, void * instance)
	: service(service), instance(instance)
{
}

IMetaService * GPythonObject::getService() const
{
	return this->service.get();
}


class GPythonMethod : public GPythonObject
{
private:
	typedef GPythonObject super;

public:
	GPythonMethod(IMetaService * service, IMetaList * methodList);

	static PyObject * call(PyObject * callableObject, PyObject * args, PyObject * keyWords);
    static void dealloc(PyObject* p);

	IMetaList * getMethodList() const;

private:
	GScopedInterface<IMetaList> methodList;
};

PyTypeObject functionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char*>("cpgf.Python.function"),
    sizeof(GPythonMethod),
    0,
    (destructor)&GPythonMethod::dealloc,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, //(reprfunc)func_repr,                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    &GPythonMethod::call,                              /* tp_call */
    0,                                  /* tp_str */
    0, // PyObject_GenericGetAttr,            /* tp_getattro */
    0, // PyObject_GenericSetAttr,            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT /* | Py_TPFLAGS_HAVE_GC */,/* tp_flags */
    0,                                  /* tp_doc */
    0, // (traverseproc)func_traverse,          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, //offsetof(PyFunctionObject, func_weakreflist), /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0, // func_memberlist,              /* tp_members */
    NULL, //function_getsetlist,                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    NULL, //function_descr_get,                 /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0, //offsetof(PyFunctionObject, func_dict),      /* tp_dictoffset */
    0,                                      /* tp_init */
    0,                                      /* tp_alloc */
    0,                                      /* tp_new */
    0,                                      /* tp_free */
    0,                                      /* tp_is_gc */
    0,                                      /* tp_bases */
    0,                                      /* tp_mro */
    0,                                      /* tp_cache */
    0,                                      /* tp_subclasses */
    0,                                      /* tp_weaklist */
    0                                       /* tp_del */
};


GPythonMethod::GPythonMethod(IMetaService * service, IMetaList * methodList)
	: super(service, NULL), methodList(methodList)
{
	this->methodList->addReference();

    if(Py_TYPE(&functionType) == 0) {
        Py_TYPE(&functionType) = &PyType_Type;
        ::PyType_Ready(&functionType);
    }
    
	PyObject_INIT(this, &functionType);
}

IMetaList * GPythonMethod::getMethodList() const
{
	this->methodList->addReference();
	return this->methodList.get();
}

GMetaVariant pythonToVariant(PyObject * object)
{
	if(PyNumber_Check(object)) {
		if(PyBool_Check(object)) {
			return PyInt_AsLong(object) != 0;
		}

		if(PyInt_Check(object)) {
			return PyInt_AsLong(object);
		}

		if(PyLong_Check(object)) {
			return PyLong_AsLong(object);
		}

		if(PyFloat_Check(object)) {
			return PyFloat_AsDouble(object);
		}

	}
	else if(PyString_Check(object)) {
		return PyString_AsString(object);
	}
	else {
	}

	return GMetaVariant();
}

void loadMethodParameters(PyObject * args, GScriptBindingParam * param, GVariantData * outputParams)
{
	int paramCount = PyTuple_Size(args);

	for(int i = 0; i < paramCount; ++i) {
		PyObject * param = PyTuple_GetItem(args, i);
		outputParams[i] = pythonToVariant(param).takeData().varData;
	}
}

void loadMethodParamTypes(PyObject * args, GBindDataType * outputTypes)
{
	int paramCount = PyTuple_Size(args);

	for(int i = 0; i < paramCount; ++i) {
//		IMetaTypedItem * typeItem;
//		outputTypes[i].dataType = getV8Type(args[i], &typeItem);
//		outputTypes[i].typeItem.reset(typeItem);
	}
}

void loadCallableParam(PyObject * args, GScriptBindingParam * param, InvokeCallableParam * callableParam)
{
	int paramCount = PyTuple_Size(args);

	if(paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}

	callableParam->paramCount = paramCount;
	loadMethodParameters(args, param, callableParam->paramsData);
	loadMethodParamTypes(args, callableParam->paramsType);
}

PyObject * GPythonMethod::call(PyObject * callableObject, PyObject * args, PyObject * keyWords)
{
	bool isGlobal = true;

	InvokeCallableParam callableParam;
	loadCallableParam(args, NULL, &callableParam);

	GPythonMethod * methodObject = static_cast<GPythonMethod *>(callableObject);

	void * instance = NULL;
//	if(userData != NULL) {
//		instance = userData->instance;
//	}

	GScopedInterface<IMetaList> methodList(methodObject->getMethodList());

	//if(namedUserData->metaClass == NULL) {
	//	methodList.reset(namedUserData->methodList);
	//	methodList->addReference();
	//}
	//else {
	//	methodList.reset(createMetaList());
	//	loadMethodList(methodList.get(), namedUserData->getParam()->getMetaMap(), userData == NULL? namedUserData->metaClass : userData->metaClass,
	//		instance,  userData, namedUserData->name.c_str());
	//}

	int maxRank = -1;
	int maxRankIndex = -1;

	uint32_t count = methodList->getCount();

	for(uint32_t i = 0; i < count; ++i) {
		GScopedInterface<IMetaCallable> meta(gdynamic_cast<IMetaCallable *>(methodList->getAt(i)));
//		if(isGlobal || allowInvokeCallable(userData, meta.get())) {
		{
			int rank = rankCallable(methodObject->getService(), meta.get(), &callableParam);
			if(rank > maxRank) {
				maxRank = rank;
				maxRankIndex = static_cast<int>(i);
			}
		}
	}

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;
		GScopedInterface<IMetaCallable> callable(gdynamic_cast<IMetaCallable *>(methodList->getAt(maxRankIndex)));
		doInvokeCallable(methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex)), callable.get(), callableParam.paramsData, callableParam.paramCount, &result);
//		return methodResultToV8(namedUserData->getParam(), callable.get(), &result);
	}

//	raiseCoreException(Error_ScriptBinding_CantFindMatchedMethod);

	return Py_None; //Py_BuildValue("");
}

void GPythonMethod::dealloc(PyObject * p)
{
    delete static_cast<GPythonMethod *>(p);
}


}


void doBindMethodList(IMetaService * service, PyObject * object, const char * name, IMetaList * methodList, GUserDataMethodType methodType)
{
	PyObject * method = new GPythonMethod(service, methodList);

	PyObject_SetAttrString(object, name, method);
}

void xxx(int a, const char * b) {
	cout << "XXX a: " << a << " b: " << b << endl;
}

template <typename D>
void register_Meta(D define)
{
	define._method("xxx", &xxx);
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

	GScopedInterface<IMetaMethod> method;

	method.reset(globalClass->getMethod("xxx"));

	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method.get(), NULL);
	doBindMethodList(service.get(), mainObject.get(), "xxx", methodList.get(), udmtMethod);

	const char * code = ""
		"xxx(5, 'abc') \n"
	;
	PyRun_SimpleString(code);

	Py_Finalize();
}
