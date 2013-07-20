#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

#include "Python.h"

#include "cpgf/scriptbind/gpythonbind.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gerrorcode.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"
#include "../pinclude/gstaticuninitializerorders.h"

#include <stdexcept>


using namespace std;
using namespace cpgf;
using namespace cpgf::bind_internal;


#define ENTER_PYTHON() \
	char local_msg[256]; bool local_error = false; { \
	try {

#define LEAVE_PYTHON(...) \
	} \
	catch(const GException & e) { strncpy(local_msg, e.getMessage(), 256); local_error = true; } \
	catch(const exception & e) { strncpy(local_msg, e.what(), 256); local_error = true; } \
	catch(...) { strcpy(local_msg, "Unknown exception occurred."); local_error = true; } \
	} if(local_error) { local_msg[255] = 0; error(local_msg); } \
	__VA_ARGS__;


namespace cpgf {

namespace {

GGlueDataWrapperPool * getPythonDataWrapperPool()
{
	static GGlueDataWrapperPool * pythonDataWrapperPool = NULL;
	if(pythonDataWrapperPool == NULL && isLibraryLive()) {
		pythonDataWrapperPool = new GGlueDataWrapperPool();
		addOrderedStaticUninitializer(suo_ScriptDataWrapperPool, makeUninitializerDeleter(&pythonDataWrapperPool));
	}

	return pythonDataWrapperPool;
}

class GPythonObject : public PyObject, public GGlueDataWrapper
{
public:
	GPythonObject() {}
	explicit GPythonObject(const GGlueDataPointer & glueData);
	virtual ~GPythonObject();

	IMetaService * getService() const;
	GContextPointer getContext() const;
	virtual GGlueDataPointer getData();

	template <typename T>
	GSharedPointer<T> getDataAs() {
		return sharedStaticCast<T>(this->getData());
	}

	void initType(PyTypeObject * type);

private:
	GGlueDataPointer glueData;
};

class GPythonAnyObject : public GPythonObject
{
public:
	GPythonAnyObject();
	virtual ~GPythonAnyObject();
	PyObject * getDict();

private:
	PyObject * dict;
};

class GPythonScriptFunction : public GScriptFunctionBase
{
private:
	typedef GScriptFunctionBase super;

public:
	GPythonScriptFunction(const GContextPointer & context, PyObject * func);
	virtual ~GPythonScriptFunction();

	virtual GVariant invoke(const GVariant * params, size_t paramCount);
	virtual GVariant invokeIndirectly(GVariant const * const * params, size_t paramCount);

private:
	PyObject * func;
};

class GPythonScriptObject : public GScriptObjectBase
{
private:
	typedef GScriptObjectBase super;

public:
	GPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config);
	virtual ~GPythonScriptObject();

	virtual GScriptObject * doCreateScriptObject(const char * name);

	virtual GScriptValue getScriptFunction(const char * name);

	virtual GVariant invoke(const char * name, const GVariant * params, size_t paramCount);
	virtual GVariant invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);

public:
	PyObject * getObject() const {
		return this->object;
	}

protected:
	virtual GScriptValue doGetValue(const char * name);

	virtual void doBindClass(const char * name, IMetaClass * metaClass);
	virtual void doBindEnum(const char * name, IMetaEnum * metaEnum);

	virtual void doBindNull(const char * name);
	virtual void doBindFundamental(const char * name, const GVariant & value);
	virtual void doBindAccessible(const char * name, void * instance, IMetaAccessible * accessible);
	virtual void doBindString(const char * stringName, const char * s);
	virtual void doBindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership);
	virtual void doBindRaw(const char * name, const GVariant & value);
	virtual void doBindMethod(const char * name, void * instance, IMetaMethod * method);
	virtual void doBindMethodList(const char * name, IMetaList * methodList);

	virtual void doBindCoreService(const char * name, IScriptLibraryLoader * libraryLoader);

private:
	GPythonScriptObject(const GPythonScriptObject & other, PyObject * object);

private:
	PyObject * object;
};


GPythonObject * createPythonObject(const GGlueDataPointer & glueData);
void deletePythonObject(GPythonObject * object);

GPythonObject * castFromPython(PyObject * object) {
	return gdynamic_cast<GPythonObject *>(static_cast<GPythonObject *>(object));
}

void commonDealloc(PyObject* p)
{
    deletePythonObject(castFromPython(p));
}

PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * keyWords);

PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * keyWords);

PyObject * callbackGetAttribute(PyObject * object, PyObject * attrName);
int callbackSetAttribute(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackAnyObjectGetAttribute(PyObject * object, PyObject * attrName);
int callbackAnyObjectSetAttribute(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * doGetAttributeObject(GPythonObject * cppObject, PyObject * attrName);

PyObject * callbackGetEnumValue(PyObject * object, PyObject * attrName);
int callbackSetEnumValue(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackAccessibleDescriptorGet(PyObject * self, PyObject * obj, PyObject * type);
int callbackAccessibleDescriptorSet(PyObject * self, PyObject * obj, PyObject * value);

PyObject * variantToPython(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData);

PyTypeObject functionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.function"),
    sizeof(GPythonObject),
    0,
    (destructor)&commonDealloc,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0,				                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    &callbackCallMethod,                              /* tp_call */
    0,                                  /* tp_str */
    0, 						            /* tp_getattro */
    0, 						            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT, 					/* tp_flags */
    0,                                  /* tp_doc */
    0, 						          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, 									 /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0, 					              /* tp_members */
    NULL, 				                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    NULL, 				                 /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0, 								      /* tp_dictoffset */
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
    0,                                      /* tp_del */
    0                                       /* tp_version_tag */
};


PyTypeObject classType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.class"),
    sizeof(GPythonObject),
    0,
    (destructor)&commonDealloc,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, 				                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    &callbackConstructObject,                              /* tp_call */
    0,                                  /* tp_str */
	&callbackGetAttribute,             /* tp_getattro */
    &callbackSetAttribute,            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,					/* tp_flags */
    0,                                  /* tp_doc */
    0, 					          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, 									 /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0, 					              /* tp_members */
    NULL, 				                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    NULL, 				                 /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0, 							      /* tp_dictoffset */
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
    0,                                      /* tp_del */
    0                                       /* tp_version_tag */
};

template <GMetaOpType op, bool allowRightSelf>
PyObject * binaryOperator(PyObject * a, PyObject * b);

template <GMetaOpType op>
PyObject * unaryOperator(PyObject * a);

PyNumberMethods numberMethods = {
	&binaryOperator<mopAdd, true>, /* nb_add */
	&binaryOperator<mopSub, true>, /* nb_subtract */
	&binaryOperator<mopMul, true>, /* nb_multiply */
	&binaryOperator<mopDiv, true>, /* nb_divide */
	&binaryOperator<mopMod, true>, /* nb_remainder */
	0, /* nb_divmod */
	0, /* nb_power */
	&unaryOperator<mopNeg>, /* nb_negative */
	&unaryOperator<mopPlus>, /* nb_positive */
	0, /* nb_absolute */
	0, /* nb_nonzero */
	&unaryOperator<mopBitNot>, /* nb_invert */
	&binaryOperator<mopBitLeftShift, true>, /* nb_lshift */
	&binaryOperator<mopBitRightShift, true>, /* nb_rshift */
	&binaryOperator<mopBitAnd, true>, /* nb_and */
	&binaryOperator<mopBitXor, true>, /* nb_xor */
	&binaryOperator<mopBitOr, true>, /* nb_or */
	0, /* nb_coerce */
	0, /* nb_int */
	0, /* nb_long */
	0, /* nb_float */
	0, /* nb_oct */
	0, /* nb_hex */
    /* Added in release 2.0 */
	&binaryOperator<mopAddAssign, false>, /* nb_inplace_add */
	&binaryOperator<mopSubAssign, false>, /* nb_inplace_subtract */
	&binaryOperator<mopMulAssign, false>, /* nb_inplace_multiply */
	&binaryOperator<mopDivAssign, false>, /* nb_inplace_divide */
	&binaryOperator<mopModAssign, false>, /* nb_inplace_remainder */
	0, /* nb_inplace_power */
	&binaryOperator<mopBitLeftShiftAssign, false>, /* nb_inplace_lshift */
	&binaryOperator<mopBitRightShiftAssign, false>, /* nb_inplace_rshift */
	&binaryOperator<mopBitAndAssign, false>, /* nb_inplace_and */
	&binaryOperator<mopBitXorAssign, false>, /* nb_inplace_xor */
	&binaryOperator<mopBitOrAssign, false>, /* nb_inplace_or */

    /* Added in release 2.2 */
    /* The following require the Py_TPFLAGS_HAVE_CLASS flag */
	0, /* nb_floor_divide */
	0, /* nb_true_divide */
	0, /* nb_inplace_floor_divide */
	0, /* nb_inplace_true_divide */

    /* Added in release 2.5 */
	0, /* nb_index */
};

PyTypeObject objectType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.object"),
    sizeof(GPythonObject),
    0,
    (destructor)&commonDealloc,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, 				                   /* tp_repr */
    &numberMethods,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	&callbackGetAttribute,             /* tp_getattro */
    &callbackSetAttribute,            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES,	/* tp_flags */
    0,                                  /* tp_doc */
    0, 						          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, 									 /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0, 					              /* tp_members */
    NULL, 			                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    NULL, 			                 /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0, 							      /* tp_dictoffset */
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
    0,                                      /* tp_del */
    0                                       /* tp_version_tag */
};


PyTypeObject enumType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.enum"),
    sizeof(GPythonObject),
    0,
    (destructor)&commonDealloc,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, 				                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	&callbackGetEnumValue,             /* tp_getattro */
    &callbackSetEnumValue,            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT, 			/* tp_flags */
    0,                                  /* tp_doc */
    0, 				          			/* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, 								 /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0, 					              /* tp_members */
    NULL, 				                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    NULL, 				                 /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0, 							      /* tp_dictoffset */
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
    0,                                      /* tp_del */
    0                                       /* tp_version_tag */
};


PyTypeObject accessibleType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.accessible"),
    sizeof(GPythonObject),
    0,
    (destructor)&commonDealloc,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, 				                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	NULL,           			  /* tp_getattro */
    NULL,            			/* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_CLASS, 	/* tp_flags */
    0,                                  /* tp_doc */
    0, 						          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, 									 /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    NULL,                                  /* tp_methods */
    0, 						              /* tp_members */
    NULL, 				                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    &callbackAccessibleDescriptorGet,           /* tp_descr_get */
    &callbackAccessibleDescriptorSet,                                  /* tp_descr_set */
    0, 								      /* tp_dictoffset */
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
    0,                                      /* tp_del */
    0                                       /* tp_version_tag */
};


PyTypeObject rawType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.raw"),
    sizeof(GPythonObject),
    0,
    (destructor)&commonDealloc,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, 				                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	NULL,           				  /* tp_getattro */
    NULL,         				   /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT, 				/* tp_flags */
    0,                                  /* tp_doc */
    0, 						          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, 									 /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    NULL,                                  /* tp_methods */
    0, 					              /* tp_members */
    NULL, 				                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    NULL,           						/* tp_descr_get */
    NULL,                                  /* tp_descr_set */
    0, 								      /* tp_dictoffset */
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
    0,                                      /* tp_del */
    0                                       /* tp_version_tag */
};


PyTypeObject emptyObjectType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.emptyObject"),
    sizeof(GPythonAnyObject),
    0,
    (destructor)&commonDealloc,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, 				                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	&callbackAnyObjectGetAttribute,             /* tp_getattro */
    &callbackAnyObjectSetAttribute,            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT, 				/* tp_flags */
    0,                                  /* tp_doc */
    0, 						          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, 									 /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0, 					              /* tp_members */
    NULL, 			                /* tp_getset */
	NULL,                                  /* tp_base */
    0,                                  /* tp_dict */
    NULL, 			                 /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0, 							      /* tp_dictoffset */
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
    0,                                      /* tp_del */
    0                                       /* tp_version_tag */
};


PyTypeObject * const typeObjects[] = {
	&functionType, &classType, &objectType, &enumType, &accessibleType, &rawType
};

const int typeObjectCount = sizeof(typeObjects) / sizeof(typeObjects[0]);


GPythonObject * tryCastFromPython(PyObject * object) {
	for(int i = 0; i <typeObjectCount; ++i) {
		if(object->ob_type == typeObjects[i]) {
			return castFromPython(object);
		}
	}

	return NULL;
}

PyTypeObject * getTypeObject(const GGlueDataPointer & glueData)
{
	PyTypeObject * typeObject = NULL;

	switch(static_cast<int>(glueData->getType())) {
		case gdtObject:
			typeObject = &objectType;
			break;

		case gdtClass:
			typeObject = &classType;
			break;

		case gdtObjectAndMethod:
			typeObject = &functionType;
			break;

		case gdtEnum:
			typeObject = &enumType;
			break;

		case gdtAccessible:
			typeObject = &accessibleType;
			break;

		case gdtRaw:
			typeObject = &rawType;
			break;
	}

	GASSERT(typeObject != NULL);

	return typeObject;
}

GPythonObject * createPythonObject(const GGlueDataPointer & glueData)
{
	GPythonObject * object = new GPythonObject(glueData);
	getPythonDataWrapperPool()->dataWrapperCreated(object);
	return object;

//	GPythonObject * obj = PyObject_New(GPythonObject, getTypeObject(glueData));
//	new (obj) GPythonObject(glueData);
//	return obj;
}

GPythonObject * createEmptyPythonObject()
{
	GPythonObject * object = new GPythonAnyObject();
	getPythonDataWrapperPool()->dataWrapperCreated(object);
	return object;
}

void deletePythonObject(GPythonObject * object)
{
	getPythonDataWrapperPool()->dataWrapperDestroyed(object);
	delete object;

//	object->~GPythonObject();
//	PyObject_Del(object);
}

GPythonObject::GPythonObject(const GGlueDataPointer & glueData)
	: glueData(glueData)
{
	PyTypeObject * typeObject = getTypeObject(glueData);

	GASSERT(typeObject != NULL);

	this->initType(typeObject);
}

GPythonObject::~GPythonObject()
{
}

IMetaService * GPythonObject::getService() const
{
	return this->getContext()->getService();
}

GContextPointer GPythonObject::getContext() const
{
	return this->glueData->getContext();
}

GGlueDataPointer GPythonObject::getData()
{
	return this->glueData;
}

void GPythonObject::initType(PyTypeObject * type)
{
    if(Py_TYPE(type) == 0) {
        Py_TYPE(type) = &PyType_Type;
        PyType_Ready(type);
    }

	PyObject_INIT(this, type);
}


GPythonAnyObject::GPythonAnyObject()
	: dict(PyDict_New())
{
	this->initType(&emptyObjectType);
}

GPythonAnyObject::~GPythonAnyObject()
{
//		Py_XDECREF(this->dict);
}

PyObject * GPythonAnyObject::getDict()
{
	return this->dict;
}


void error(const char * message)
{
	PyErr_SetString(PyExc_RuntimeError, message);
}

PyObject * pyAddRef(PyObject * obj)
{
	Py_XINCREF(obj);

	return obj;
}

GScriptValue pythonToScriptValue(const GContextPointer & context, PyObject * value, GGlueDataPointer * outputGlueData)
{
	if(value == NULL) {
		return GScriptValue();
	}

	if(value == Py_None) {
		return GScriptValue::fromNull();
	}

	if(PyNumber_Check(value)) {
		if(PyBool_Check(value)) {
			return GScriptValue::fromFundamental(bool(PyInt_AsLong(value) != 0));
		}

		if(PyInt_Check(value)) {
			return GScriptValue::fromFundamental(PyInt_AsLong(value));
		}

		if(PyLong_Check(value)) {
			return GScriptValue::fromFundamental(PyLong_AsLong(value));
		}

		if(PyFloat_Check(value)) {
			return GScriptValue::fromFundamental(PyFloat_AsDouble(value));
		}

	}
	else if(PyString_Check(value)) {
		return GScriptValue::fromAndCopyString(PyString_AsString(value));
	}
	else {
		GPythonObject * object = tryCastFromPython(value);
		if(object != NULL) {
			GGlueDataPointer glueData = object->getData();
			if(outputGlueData != NULL) {
				*outputGlueData = glueData;
			}
			return glueDataToScriptValue(glueData);
		}

		if(PyCallable_Check(value)) {
			GScopedInterface<IScriptFunction> func(new ImplScriptFunction(new GPythonScriptFunction(context, value), true));
			
			return GScriptValue::fromScriptFunction(func.get());
		}

		GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GPythonScriptObject(context->getService(), value, context->getConfig()), true));

		return GScriptValue::fromScriptObject(scriptObject.get());
	}

	return GScriptValue();
}

PyObject * objectToPython(const GContextPointer & context, const GClassGlueDataPointer & classData,
						  const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
{
	if(objectAddressFromVariant(instance) == NULL) {
		return pyAddRef(Py_None);
	}

	GObjectGlueDataPointer objectData(context->newOrReuseObjectGlueData(classData, instance, flags, cv));
	if(outputGlueData != NULL) {
		*outputGlueData = objectData;
	}
	return createPythonObject(objectData);
}

PyObject * rawToPython(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
{
	if(context->getConfig().allowAccessRawData()) {
		GRawGlueDataPointer rawData(context->newRawGlueData(value));
		if(outputGlueData != NULL) {
			*outputGlueData = rawData;
		}
		PyObject * rawObject = createPythonObject(rawData);

		return rawObject;
	}

	return NULL;
}

PyObject * createClassObject(const GContextPointer & context, IMetaClass * metaClass)
{
	return createPythonObject(context->newClassData(metaClass));
}

struct GPythonMethods
{
	typedef PyObject * ResultType;
	
	static ResultType doObjectToScript(const GContextPointer & context, const GClassGlueDataPointer & classData,
		const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
	{
		return objectToPython(context, classData, instance, flags, cv, outputGlueData);
	}

	static ResultType doVariantToScript(const GContextPointer & context, const GVariant & value, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
	{
		return variantToPython(context, value, flags, outputGlueData);
	}
	
	static ResultType doRawToScript(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
	{
		return rawToPython(context, value, outputGlueData);
	}

	static ResultType doClassToScript(const GContextPointer & context, IMetaClass * metaClass)
	{
		PyObject * classObject = createClassObject(context, metaClass);
		return classObject;
	}

	static ResultType doStringToScript(const GContextPointer & /*context*/, const char * s)
	{
		return PyString_FromString(s);
	}

	static ResultType doWideStringToScript(const GContextPointer & /*context*/, const wchar_t * ws)
	{
		GScopedArray<char> s(wideStringToString(ws));
		return PyString_FromString(s.get());
	}

	static bool isSuccessResult(const ResultType & result)
	{
		return result != NULL;
	}

	static ResultType defaultValue()
	{
		return ResultType();
	}

	static ResultType doMethodsToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem,
		const char * methodName, GMetaClassTraveller * /*traveller*/,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getUserData());
		GContextPointer context = classData->getContext();
		if(data == NULL) {
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));

			GScopedInterface<IMetaList> metaList(getMethodListFromMapItem(mapItem, getGlueDataInstance(objectData)));
			data = new GMapItemMethodData(context->newMethodGlueData(context->getClassData(boundClass.get()), metaList.get(), methodName));

			mapItem->setUserData(data);
		}

		return createPythonObject(context->newObjectAndMethodGlueData(objectData, data->getMethodData()));
	}

	static ResultType doEnumToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem, const char * /*enumName*/)
	{
		GContextPointer context = classData->getContext();
		GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
		return createPythonObject(context->newEnumGlueData(metaEnum.get()));
	}
};

PyObject * variantToPython(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
{
	GVariant value = getVariantRealValue(data);
	GMetaType type = getVariantRealMetaType(data);

	GVariantType vt = static_cast<GVariantType>(value.getType() & ~byReference);
	
	if(vtIsEmpty(vt)) {
		return pyAddRef(Py_None);
	}

	if(vtIsBoolean(vt)) {
		return pyAddRef(fromVariant<bool>(value) ? Py_True : Py_False);
	}

	if(vtIsInteger(vt)) {
		return PyInt_FromLong(fromVariant<long>(value));
	}

	if(vtIsReal(vt)) {
		return PyFloat_FromDouble(fromVariant<double>(value));
	}

	if(!vtIsInterface(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == NULL) {
		return pyAddRef(Py_None);
	}

	if(variantIsString(value)) {
		return PyString_FromString(fromVariant<char *>(value));
	}

	if(variantIsWideString(value)) {
		const wchar_t * ws = fromVariant<wchar_t *>(value);
		GScopedArray<char> s(wideStringToString(ws));
		return PyString_FromString(s.get());
	}

	return complexVariantToScript<GPythonMethods>(context, value, type, flags, outputGlueData);
}

PyObject * methodResultToPython(const GContextPointer & context, IMetaCallable * callable, InvokeCallableResult * result)
{
	PyObject * r = methodResultToScript<GPythonMethods>(context, callable, result);
	if(r) {
		return r;
	}
	else {
		return pyAddRef(Py_None);
	}
}

void loadCallableParam(const GContextPointer & context, PyObject * args, InvokeCallableParam * callableParam)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
		PyObject * c = PyTuple_GetItem(args, i);
		callableParam->params[i].value = pythonToScriptValue(context, c, &callableParam->params[i].glueData);
	}
}


PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * /*keyWords*/)
{
	ENTER_PYTHON()

	GPythonObject * methodObject = castFromPython(callableObject);

	GObjectAndMethodGlueDataPointer userData = methodObject->getDataAs<GObjectAndMethodGlueData>();

	InvokeCallableParam callableParam(static_cast<int>(PyTuple_Size(args)));
	loadCallableParam(userData->getContext(), args, &callableParam);

	InvokeCallableResult result = doInvokeMethodList(userData->getContext(), userData->getObjectData(), userData->getMethodData(), &callableParam);

	return methodResultToPython(userData->getContext(), result.callable.get(), &result);

	LEAVE_PYTHON(return NULL)
}


PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * /*keyWords*/)
{
	ENTER_PYTHON()

	GPythonObject * cppClass = castFromPython(callableObject);
	GClassGlueDataPointer classUserData = cppClass->getDataAs<GClassGlueData>();
	GContextPointer context = classUserData->getContext();
	
	InvokeCallableParam callableParam(static_cast<int>(PyTuple_Size(args)));
	loadCallableParam(context, args, &callableParam);

	void * instance = doInvokeConstructor(context, cppClass->getService(), classUserData->getMetaClass(), &callableParam);

	if(instance != NULL) {
		return createPythonObject(context->newObjectGlueData(classUserData, instance, GBindValueFlags(bvfAllowGC), opcvNone));
	}
	else {
		raiseCoreException(Error_ScriptBinding_FailConstructObject);
	}

	return NULL;

	LEAVE_PYTHON(return NULL)
}

PyObject * callbackGetAttribute(PyObject * object, PyObject * attrName)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(object);

	PyObject * attrObject = doGetAttributeObject(cppObject, attrName);
	if(attrObject != NULL) {
		return attrObject;
	}

	return pyAddRef(Py_None);

	LEAVE_PYTHON(return NULL)
}

int callbackSetAttribute(PyObject * object, PyObject * attrName, PyObject * value)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(object);
	GGlueDataPointer instanceGlueData = cppObject->getDataAs<GGlueData>();
	const char * name = PyString_AsString(attrName);

	GVariant v;
	GGlueDataPointer valueGlueData;

	v = pythonToScriptValue(instanceGlueData->getContext(), value, &valueGlueData).getValue();
	if(setValueOnNamedMember(instanceGlueData, name, v, valueGlueData)) {
		return 0;
	}

	return -1;

	LEAVE_PYTHON(return -1)
}

PyObject * doGetAttributeObject(GPythonObject * cppObject, PyObject * attrName)
{
// If we enable blow code, some common method names such as "get" will be intercepted by Python.
//	if(PyObject_HasAttr(cppObject->ob_type->tp_dict, attrName)) {
//		return PyObject_GetAttr(cppObject->ob_type->tp_dict, attrName);
//	}

	const char * name = PyString_AsString(attrName);
	return namedMemberToScript<GPythonMethods>(cppObject->getData(), name);
}

PyObject * callbackGetEnumValue(PyObject * object, PyObject * attrName)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(object);
	
	GEnumGlueDataPointer userData = cppObject->getDataAs<GEnumGlueData>();

	const char * name = PyString_AsString(attrName);

	int32_t index = userData->getMetaEnum()->findKey(name);
	if(index >= 0) {
		return variantToPython(userData->getContext(), metaGetEnumValue(userData->getMetaEnum(), index), GBindValueFlags(), NULL);
	}

	raiseCoreException(Error_ScriptBinding_CantFindEnumKey, *name);

	return pyAddRef(Py_None);

	LEAVE_PYTHON(return NULL)
}

int callbackSetEnumValue(PyObject * /*object*/, PyObject * /*attrName*/, PyObject * /*value*/)
{
	ENTER_PYTHON()

	raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);

	return -1;

	LEAVE_PYTHON(return -1)
}

PyObject * callbackAccessibleDescriptorGet(PyObject * self, PyObject * /*obj*/, PyObject * /*pyType*/)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(self);
	
	GAccessibleGlueDataPointer userData = cppObject->getDataAs<GAccessibleGlueData>();

	return accessibleToScript<GPythonMethods>(userData->getContext(), userData->getAccessible(), userData->getInstanceAddress(), false);

	LEAVE_PYTHON(return NULL)
}

int callbackAccessibleDescriptorSet(PyObject * self, PyObject * /*obj*/, PyObject * value)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(self);
	
	GAccessibleGlueDataPointer userData = cppObject->getDataAs<GAccessibleGlueData>();

	GVariant v = pythonToScriptValue(userData->getContext(), value, NULL).getValue();
	metaSetValue(userData->getAccessible(), userData->getInstanceAddress(), v);

	return 0;

	LEAVE_PYTHON(return 0)
}

PyObject * callbackAnyObjectGetAttribute(PyObject * object, PyObject * attrName)
{
	ENTER_PYTHON()

	PyObject * obj = PyDict_GetItem(static_cast<GPythonAnyObject *>(object)->getDict(), attrName);
	Py_XINCREF(obj);

	return obj;

	LEAVE_PYTHON(return NULL)
}

int callbackAnyObjectSetAttribute(PyObject * object, PyObject * attrName, PyObject * value)
{
	ENTER_PYTHON()

	return PyDict_SetItem(static_cast<GPythonAnyObject *>(object)->getDict(), attrName, value);

	LEAVE_PYTHON(return -1)
}

GVariant invokePythonFunctionIndirectly(const GContextPointer & context, PyObject * object, PyObject * func, GVariant const * const * params, size_t paramCount, const char * name)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(! context) {
		raiseCoreException(Error_ScriptBinding_NoContext);
	}

	if(PyCallable_Check(func)) {
		size_t allParamCount = paramCount;
		if(object != NULL) {
			++allParamCount;
		}
		GPythonScopedPointer args(PyTuple_New(allParamCount));
		int start = 0;
		if(object != NULL) {
			start = 1;
			PyTuple_SetItem(args.get(), 0, object);
		}
		for(size_t i = 0; i < paramCount; ++i) {
			GPythonScopedPointer arg(variantToPython(context, *params[i], GBindValueFlags(bvfAllowRaw), NULL));
			if(!arg) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
			PyTuple_SetItem(args.get(), start + i, arg.take());
		}

		GPythonScopedPointer result;

		result.reset(PyObject_Call(func, args.get(), NULL));

		return pythonToScriptValue(context, result.get(), NULL).getValue();
	}
	else {
		raiseCoreException(Error_ScriptBinding_CantCallNonfunction);
	}

	return GVariant();
}


void setObjectAttr(PyObject * owner, const char * name, PyObject * value)
{
	if(PyDict_Check(owner)) {
		PyDict_SetItemString(owner, name, value);
	}
	else {
		PyObject_SetAttrString(owner, name, value);
	}
}

PyObject * getObjectAttr(PyObject * owner, const char * name)
{
	if(PyObject_HasAttrString(owner, name)) {
		return PyObject_GetAttrString(owner, name);
	}
	else {
		if(PyDict_Check(owner)) {
			PyObject * obj = PyDict_GetItemString(owner, name);
			Py_XINCREF(obj);

			return obj;
		}
		return NULL;
	}
}

const char * const signatureName = "cpgf_pYtHon_oBjeCt_Name";
const char * const signatureValue = "cpgf_pYtHon_oBjeCt_vAlue";

void setObjectSignature(PyObject * obj)
{
	setObjectAttr(obj, signatureName, PyString_FromString(signatureValue));
}

bool isValidObject(PyObject * obj)
{
	GPythonScopedPointer signature(getObjectAttr(obj, signatureName));

	if(signature) {
		const char * name = PyString_AsString(signature.get());
		if(name != NULL) {
			return (strcmp(signatureValue, name) == 0);
		}
	}
	return false;
}

void helperBindMethodList(const GContextPointer & context, PyObject * owner, const char * name, IMetaList * methodList)
{
	GMethodGlueDataPointer data = context->newMethodGlueData(GClassGlueDataPointer(), methodList, name);
	GObjectAndMethodGlueDataPointer methodData = context->newObjectAndMethodGlueData(GObjectGlueDataPointer(), data);
	PyObject * methodObject = createPythonObject(methodData);

	setObjectAttr(owner, name, methodObject);
}

void helperBindClass(const GContextPointer & context, PyObject * owner, const char * name, IMetaClass * metaClass)
{
	PyObject * classObject = createClassObject(context, metaClass);

	setObjectAttr(owner, name, classObject);
}

void helperBindEnum(const GContextPointer & context, PyObject * owner, const char * name, IMetaEnum * metaEnum)
{
	PyObject * enumObject = createPythonObject(context->newEnumGlueData(metaEnum));

	setObjectAttr(owner, name, enumObject);
}

void helperBindAccessible(const GContextPointer & context, PyObject * owner, const char * name, void * instance, IMetaAccessible * accessible)
{
	PyObject * accessibleObject = createPythonObject(context->newAccessibleGlueData(instance, accessible));

	setObjectAttr(owner, name, accessibleObject);
}

template <GMetaOpType op, bool allowRightSelf>
PyObject * binaryOperator(PyObject * a, PyObject * b)
{
	ENTER_PYTHON()
	
	PyObject * self = NULL;
	PyObject * other = NULL;
	int selfIndex = 0;
	int otherIndex = 0;

	if(a->ob_type == &objectType) {
		self = a;
		other = b;
		selfIndex = 0;
		otherIndex = 1;
	}
	else {
		if(! allowRightSelf) {
			return NULL;
		}
		self = b;
		other = a;
		selfIndex = 1;
		otherIndex = 0;
	}
	
	GObjectGlueDataPointer objectData = castFromPython(self)->getDataAs<GObjectGlueData>();
	const GContextPointer & context = objectData->getContext();

	InvokeCallableParam callableParam(2);

	callableParam.params[selfIndex].value = pythonToScriptValue(context, self, &callableParam.params[selfIndex].glueData);
	
	callableParam.params[otherIndex].value = pythonToScriptValue(context, other, &callableParam.params[otherIndex].glueData);
	
	InvokeCallableResult result = doInvokeOperator(context, objectData, objectData->getClassData()->getMetaClass(), op, &callableParam);
	
	return methodResultToPython(context, result.callable.get(), &result);

	LEAVE_PYTHON(return NULL)
}

template <GMetaOpType op>
PyObject * unaryOperator(PyObject * a)
{
	ENTER_PYTHON()
	
	PyObject * self = a;
	
	GObjectGlueDataPointer objectData = castFromPython(self)->getDataAs<GObjectGlueData>();
	const GContextPointer & context = objectData->getContext();

	InvokeCallableParam callableParam(1);

	callableParam.params[0].value = pythonToScriptValue(context, self, &callableParam.params[0].glueData);
	
	InvokeCallableResult result = doInvokeOperator(context, objectData, objectData->getClassData()->getMetaClass(), op, &callableParam);
	
	return methodResultToPython(context, result.callable.get(), &result);

	LEAVE_PYTHON(return NULL)
}


GPythonScriptFunction::GPythonScriptFunction(const GContextPointer & context, PyObject * func)
	: super(context), func(func)
{
	Py_XINCREF(this->func);
}

GPythonScriptFunction::~GPythonScriptFunction()
{
	if(isLibraryLive()) {
		Py_XDECREF(this->func);
	}
}

GVariant GPythonScriptFunction::invoke(const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GVariant GPythonScriptFunction::invokeIndirectly(GVariant const * const * params, size_t paramCount)
{
	return invokePythonFunctionIndirectly(this->getContext(), NULL, this->func, params, paramCount, "");
}


GPythonScriptObject::GPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config)
	: super(GContextPointer(new GBindingContext(service, config)), config), object(object)
{
}

GPythonScriptObject::GPythonScriptObject(const GPythonScriptObject & other, PyObject * object)
	: super(other), object(object)
{
}

GPythonScriptObject::~GPythonScriptObject()
{
}

GScriptValue GPythonScriptObject::doGetValue(const char * name)
{
	GPythonScopedPointer obj(getObjectAttr(this->object, name));
	
	if(obj) {
		return pythonToScriptValue(this->getContext(), obj.get(), NULL);
	}
	else {
		return GScriptValue();
	}
}

void GPythonScriptObject::doBindClass(const char * name, IMetaClass * metaClass)
{
	helperBindClass(this->getContext(), this->object, name, metaClass);
}

void GPythonScriptObject::doBindEnum(const char * name, IMetaEnum * metaEnum)
{
	helperBindEnum(this->getContext(), this->object, name, metaEnum);
}

GScriptObject * GPythonScriptObject::doCreateScriptObject(const char * name)
{
	PyObject * attr = getObjectAttr(this->object, name);
	if(attr != NULL) {
		if(this->getValue(name).getType() == GScriptValue::typeScriptObject) {
			GPythonScriptObject * newScriptObject = new GPythonScriptObject(*this, attr);
			newScriptObject->setOwner(this);
			newScriptObject->setName(name);
			return newScriptObject;
		}
	}
	else {
		PyObject * dict = createEmptyPythonObject(); // PyDict_New();
		setObjectAttr(this->object, name, dict);
		setObjectSignature(dict);
		GPythonScriptObject * newScriptObject = new GPythonScriptObject(*this, dict);
		newScriptObject->setOwner(this);
		newScriptObject->setName(name);
		return newScriptObject;
	}

	return NULL;
}

GScriptValue GPythonScriptObject::getScriptFunction(const char * name)
{
	GPythonScopedPointer func(getObjectAttr(this->object, name));
	if(func) {
		if(PyCallable_Check(func.get())) {
			GScopedInterface<IScriptFunction> scriptFunction(new ImplScriptFunction(new GPythonScriptFunction(this->getContext(), func.take()), true));
			return GScriptValue::fromScriptFunction(scriptFunction.get());
		}
	}

	return GScriptValue();
}

GVariant GPythonScriptObject::invoke(const char * name, const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GVariant GPythonScriptObject::invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount)
{
	GPythonScopedPointer func(getObjectAttr(this->object, name));
	return invokePythonFunctionIndirectly(this->getContext(), NULL, func.get(), params, paramCount, name);
}

void GPythonScriptObject::doBindNull(const char * name)
{
	setObjectAttr(this->object, name, Py_None);
}

void GPythonScriptObject::doBindFundamental(const char * name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.refData().typeData)), "Only fundamental value can be bound via bindFundamental");

	setObjectAttr(this->object, name, variantToPython(this->getContext(), value, GBindValueFlags(bvfAllowRaw), NULL));
}

void GPythonScriptObject::doBindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	helperBindAccessible(this->getContext(), this->object, name, instance, accessible);
}

void GPythonScriptObject::doBindString(const char * stringName, const char * s)
{
	setObjectAttr(this->object, stringName, PyString_FromString(s));
}

void GPythonScriptObject::doBindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	GBindValueFlags flags;
	flags.setByBool(bvfAllowGC, transferOwnership);
	setObjectAttr(this->object, objectName, objectToPython(this->getContext(), this->getContext()->getClassData(type), instance, flags, opcvNone, NULL));
}

void GPythonScriptObject::doBindRaw(const char * name, const GVariant & value)
{
	setObjectAttr(this->object, name, rawToPython(this->getContext(), value, NULL));
}

void GPythonScriptObject::doBindMethod(const char * name, void * instance, IMetaMethod * method)
{
	if(method->isStatic()) {
		instance = NULL;
	}

	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method, instance);

	helperBindMethodList(this->getContext(), this->object, name, methodList.get());
}

void GPythonScriptObject::doBindMethodList(const char * name, IMetaList * methodList)
{
	helperBindMethodList(this->getContext(), this->object, name, methodList);
}

void GPythonScriptObject::assignValue(const char * fromName, const char * toName)
{
	GPythonScopedPointer obj(getObjectAttr(this->object, fromName));
	if(obj) {
		setObjectAttr(this->object, toName, obj.get());
	}
}

void GPythonScriptObject::doBindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	this->getContext()->bindScriptCoreService(this, name, libraryLoader);
}


} // unamed namespace


GScriptObject * createPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config)
{
	return new GPythonScriptObject(service, object, config);
}

IScriptObject * createPythonScriptInterface(IMetaService * service, PyObject * object, const GScriptConfig & config)
{
	return new ImplScriptObject(new GPythonScriptObject(service, object, config), true);
}


G_GUARD_LIBRARY_LIFE


} // namespace cpgf


