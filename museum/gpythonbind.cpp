#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

#include "Python.h"

#include "cpgf/scriptbind/gpythonbind.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gstringmap.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"


using namespace cpgf;
using namespace cpgf::bind_internal;


#define ENTER_PYTHON() \
	char local_msg[256]; bool local_error = false; { \
	try {

#define LEAVE_PYTHON(...) \
	} \
	catch(const GException & e) { strncpy(local_msg, e.getMessage(), 256); local_error = true; } \
	catch(...) { strcpy(local_msg, "Unknown exception occurred."); local_error = true; } \
	} if(local_error) { local_msg[255] = 0; error(local_msg); } \
	__VA_ARGS__;


int totalPythonObjectCount = 0;
int pythonObjectCount = 0;
namespace cpgf {

namespace {

class GMapItemObjectData : public GMetaMapItemData
{
public:
	GMapItemObjectData() : object(NULL) {
	}

	virtual ~GMapItemObjectData() {
		Py_XDECREF(this->object);
	}

	PyObject * getObject() const {
		Py_XINCREF(this->object);
		return this->object;
	}

	void setObject(PyObject * object) {
		if(this->object != object) {
			Py_XDECREF(this->object);
			
			this->object = object;
			
			Py_XINCREF(this->object);
		}
	}

private:
	PyObject * object;
};

class GPythonObject : public PyObject
{
public:
	explicit GPythonObject(GScriptUserData * userData);
	virtual ~GPythonObject();

	IMetaService * getService() const;
	const GBindingParamPointer & getParam() const;
	GScriptUserData * getUserData() const;

protected:
	void initType(PyTypeObject * type);

private:
	GScriptUserData * userData;
};

class GPythonScriptFunction : public GScriptFunction
{
public:
	GPythonScriptFunction(const GBindingParamPointer & bindingParam, PyObject * func);
	virtual ~GPythonScriptFunction();

	virtual GMetaVariant invoke(const GMetaVariant * params, size_t paramCount);
	virtual GMetaVariant invokeIndirectly(GMetaVariant const * const * params, size_t paramCount);

private:
	GWeakBindingParamPointer bindingParam;
	PyObject * func;
};

class GPythonScriptObject : public GScriptObject
{
private:
	typedef GScriptObject super;

public:
	GPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config);
	virtual ~GPythonScriptObject();

	virtual void bindClass(const char * name, IMetaClass * metaClass);
	virtual void bindEnum(const char * name, IMetaEnum * metaEnum);

	virtual void bindFundamental(const char * name, const GVariant & value);
	virtual void bindString(const char * stringName, const char * s);
	virtual void bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership);
	virtual void bindRaw(const char * name, const GVariant & value);
	virtual void bindMethod(const char * name, void * instance, IMetaMethod * method);
	virtual void bindMethodList(const char * name, IMetaList * methodList);

	virtual IMetaClass * getClass(const char * className);
	virtual IMetaEnum * getEnum(const char * enumName);

	virtual GVariant getFundamental(const char * name);
	virtual std::string getString(const char * stringName);
	virtual void * getObject(const char * objectName);
	virtual GVariant getRaw(const char * name);
	virtual IMetaMethod * getMethod(const char * methodName, void ** outInstance);
	virtual IMetaList * getMethodList(const char * methodName);

	virtual GScriptDataType getType(const char * name, IMetaTypedItem ** outMetaTypeItem);

	virtual GScriptObject * createScriptObject(const char * name);
	virtual GScriptObject * gainScriptObject(const char * name);

	virtual GScriptFunction * gainScriptFunction(const char * name);

	virtual GMetaVariant invoke(const char * name, const GMetaVariant * params, size_t paramCount);
	virtual GMetaVariant invokeIndirectly(const char * name, GMetaVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);
	virtual bool valueIsNull(const char * name);
	virtual void nullifyValue(const char * name);

	virtual void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible);

	virtual void bindCoreService(const char * name);

public:
	const GBindingParamPointer & getParam() const {
		return this->param;
	}

	PyObject * getObject() const {
		return this->object;
	}

private:
	GPythonScriptObject(const GPythonScriptObject & other, PyObject * object);

private:
	PyObject * object;
	GBindingParamPointer param;
	bool freeParam;
};


GPythonObject * createPythonObject(GScriptUserData * userData);
void deletePythonObject(GPythonObject * obj);

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

PyObject * doGetAttributeObject(GPythonObject * cppObject, PyObject * attrName);

PyObject * callbackGetEnumValue(PyObject * object, PyObject * attrName);
int callbackSetEnumValue(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackAccessibleDescriptorGet(PyObject * self, PyObject * obj, PyObject * type);
int callbackAccessibleDescriptorSet(PyObject * self, PyObject * obj, PyObject * value);

PyObject * variantToPython(const GBindingParamPointer & param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw);

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
    0                                       /* tp_del */
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
    0                                       /* tp_del */
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
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	&callbackGetAttribute,             /* tp_getattro */
    &callbackSetAttribute,            /* tp_setattro */
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
    0                                       /* tp_del */
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
    0                                       /* tp_del */
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
    0                                       /* tp_del */
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
    0                                       /* tp_del */
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

PyTypeObject * getTypeObject(GScriptUserData * userData)
{
	PyTypeObject * typeObject = NULL;

	switch(static_cast<int>(userData->getType())) {
		case udtObject:
			typeObject = (gdynamic_cast<GObjectUserData *>(userData)->getObjectData()->isInstance() ? &objectType : &classType);
			break;

		case udtMethod:
			typeObject = &functionType;
			break;

		case udtEnum:
			typeObject = &enumType;
			break;

		case udtAccessible:
			typeObject = &accessibleType;
			break;

		case udtRaw:
			typeObject = &rawType;
			break;
	}

	GASSERT(typeObject != NULL);

	return typeObject;
}

GPythonObject * createPythonObject(GScriptUserData * userData)
{
	return new GPythonObject(userData);

//	GPythonObject * obj = PyObject_New(GPythonObject, getTypeObject(userData));
//	new (obj) GPythonObject(userData);
//	return obj;
}

void deletePythonObject(GPythonObject * obj)
{
	delete obj;

//	obj->~GPythonObject();
//	PyObject_Del(obj);
}

GPythonObject::GPythonObject(GScriptUserData * userData)
	: userData(userData)
{
	PyTypeObject * typeObject = getTypeObject(userData);

	GASSERT(typeObject != NULL);

	this->initType(typeObject);

	++pythonObjectCount;
	++totalPythonObjectCount;
}

GPythonObject::~GPythonObject()
{
	--pythonObjectCount;

	delete this->userData;
}

IMetaService * GPythonObject::getService() const
{
	return this->getParam()->getService();
}

const GBindingParamPointer & GPythonObject::getParam() const
{
	return this->userData->getParam();
}

GScriptUserData * GPythonObject::getUserData() const
{
	return this->userData;
}

void GPythonObject::initType(PyTypeObject * type)
{
    if(Py_TYPE(type) == 0) {
        Py_TYPE(type) = &PyType_Type;
        PyType_Ready(type);
    }

	PyObject_INIT(this, type);
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

GScriptDataType getPythonType(PyObject * value, IMetaTypedItem ** typeItem)
{
	if(typeItem != NULL) {
		*typeItem = NULL;
	}

	if(value == Py_None) {
		return sdtNull;
	}

	if(PyNumber_Check(value)) {
		return sdtFundamental;
	}

	if(PyString_Check(value)) {
		return sdtString;
	}

	if(value->ob_type == &classType) {
		if(typeItem != NULL) {
			*typeItem = gdynamic_cast<GObjectUserData *>(castFromPython(value)->getUserData())->getObjectData()->getMetaClass();
			(*typeItem)->addReference();
		}
		return sdtClass;
	}

	if(value->ob_type == &objectType) {
		if(typeItem != NULL) {
			*typeItem = gdynamic_cast<GObjectUserData *>(castFromPython(value)->getUserData())->getObjectData()->getMetaClass();
			(*typeItem)->addReference();
		}
		return sdtObject;
	}

	if(value->ob_type == &enumType) {
		if(typeItem != NULL) {
			*typeItem = gdynamic_cast<GEnumUserData *>(castFromPython(value)->getUserData())->getMetaEnum();
			(*typeItem)->addReference();
		}
		return sdtEnum;
	}

	if(value->ob_type == &rawType) {
		return sdtRaw;
	}

	if(value->ob_type == &functionType) {
		if(gdynamic_cast<GObjectMethodUserData *>(castFromPython(value)->getUserData())->getMethodData().getMethodList()->getCount() > 1) {
			return sdtMethodList;
		}
		else {
			return sdtMethod;
		}
	}

	// should be the last
	if(PyCallable_Check(value)) {
		return sdtScriptMethod;
	}

	return sdtScriptObject;
}

GMetaVariant pythonToVariant(const GBindingParamPointer & param, PyObject * value)
{
	if(value == NULL) {
		return GMetaVariant();
	}

	if(PyNumber_Check(value)) {
		if(PyBool_Check(value)) {
			return PyInt_AsLong(value) != 0;
		}

		if(PyInt_Check(value)) {
			return PyInt_AsLong(value);
		}

		if(PyLong_Check(value)) {
			return PyLong_AsLong(value);
		}

		if(PyFloat_Check(value)) {
			return PyFloat_AsDouble(value);
		}

	}
	else if(PyString_Check(value)) {
		return GMetaVariant(createStringVariant(PyString_AsString(value)), createMetaType<char *>());
	}
	else {
		GPythonObject * object = tryCastFromPython(value);
		if(object != NULL) {
			return userDataToVariant(object->getUserData());
		}

		if(PyCallable_Check(value)) {
			GScopedInterface<IScriptFunction> func(new ImplScriptFunction(new GPythonScriptFunction(param, value), true));
			
			return GMetaVariant(func.get(), GMetaType());
		}

		GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GPythonScriptObject(param->getService(), value, param->getConfig()), true));

		return GMetaVariant(scriptObject.get(), GMetaType());
	}

	return GMetaVariant();
}

PyObject * objectToPython(const GBindingParamPointer & param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv, ObjectUserDataType dataType)
{
	if(instance == NULL) {
		return pyAddRef(Py_None);
	}

	return createPythonObject(new GObjectUserData(param, metaClass, instance, allowGC, cv, dataType));
}

PyObject * rawToPython(const GBindingParamPointer & param, const GVariant & value)
{
	if(param->getConfig().allowAccessRawData()) {
		PyObject * rawObject = createPythonObject(new GRawUserData(param, value));

		return rawObject;
	}

	return NULL;
}

PyObject * createClassObject(const GBindingParamPointer & param, IMetaClass * metaClass)
{
	return createPythonObject(new GObjectUserData(param, metaClass, NULL, false, opcvNone, cudtClass));
}

struct GPythonMethods
{
	typedef PyObject * ResultType;
	
	static ResultType doObjectToScript(const GBindingParamPointer & param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv, ObjectUserDataType dataType)
	{
		return objectToPython(param, instance, metaClass, allowGC, cv, dataType);
	}

	static ResultType doVariantToScript(const GBindingParamPointer & param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
	{
		return variantToPython(param, value, type, allowGC, allowRaw);
	}
	
	static ResultType doRawToScript(const GBindingParamPointer & param, const GVariant & value)
	{
		return rawToPython(param, value);
	}

	static ResultType doConverterToScript(const GBindingParamPointer & param, const GVariant & value, IMetaConverter * converter)
	{
		ResultType result;
		if(converterToScript<GPythonMethods>(&result, param, value, converter)) {
			return result;
		}
		return NULL;
	}

	static ResultType doClassToScript(const GBindingParamPointer & param, IMetaClass * metaClass)
	{
		PyObject * classObject = createClassObject(param, metaClass);
		return classObject;
	}

	static ResultType doStringToScript(const GBindingParamPointer & /*param*/, const char * s)
	{
		return PyString_FromString(s);
	}

	static ResultType doWideStringToScript(const GBindingParamPointer & /*param*/, const wchar_t * ws)
	{
		GScopedArray<char> s(wideStringToString(ws));
		return PyString_FromString(s.get());
	}

	static bool isSuccessResult(const ResultType & result)
	{
		return result != NULL;
	}
};

PyObject * variantToPython(const GBindingParamPointer & param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
{
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

	PyObject * result = NULL;
	if(variantToScript<GPythonMethods>(&result, param, value, type, allowGC, allowRaw)) {
		return result;
	}

	return NULL;
}

PyObject * methodResultToPython(const GBindingParamPointer & param, IMetaCallable * callable, InvokeCallableResult * result)
{
	PyObject * r;
	if(methodResultToScript<GPythonMethods>(&r, param, callable, result)) {
		return r;
	}

	return pyAddRef(Py_None);
}

void loadMethodParameters(const GBindingParamPointer & p, PyObject * args, GVariant * outputParams)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
		PyObject * param = PyTuple_GetItem(args, i);
		outputParams[i] = pythonToVariant(p, param).getValue();
	}
}

void loadMethodParamTypes(PyObject * args, CallableParamDataType * outputTypes)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
		IMetaTypedItem * typeItem;
		PyObject * param = PyTuple_GetItem(args, i);
		outputTypes[i].dataType = getPythonType(param, &typeItem);
		outputTypes[i].typeItem.reset(typeItem);
	}
}

void loadCallableParam(const GBindingParamPointer & param, PyObject * args, InvokeCallableParam * callableParam)
{
	loadMethodParameters(param, args, callableParam->paramsData);
	loadMethodParamTypes(args, callableParam->paramsType);
}


PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * /*keyWords*/)
{
	ENTER_PYTHON()

	GPythonObject * methodObject = castFromPython(callableObject);

	GObjectMethodUserData * userData = gdynamic_cast<GObjectMethodUserData *>(methodObject->getUserData());

	InvokeCallableParam callableParam(static_cast<int>(PyTuple_Size(args)));
	loadCallableParam(userData->getParam(), args, &callableParam);

	GObjectUserData data(userData->getParam(), userData->getObjectData());
	InvokeCallableResult result = doInvokeMethodList(userData->getParam(), &data, userData->getMethodData(), &callableParam);

	return methodResultToPython(methodObject->getUserData()->getParam(), result.callable.get(), &result);

	LEAVE_PYTHON(return NULL)
}


PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * /*keyWords*/)
{
	ENTER_PYTHON()

	GPythonObject * cppClass = castFromPython(callableObject);
	GObjectUserData * classUserData = gdynamic_cast<GObjectUserData *>(cppClass->getUserData());
	
	InvokeCallableParam callableParam(static_cast<int>(PyTuple_Size(args)));
	loadCallableParam(classUserData->getParam(), args, &callableParam);

	void * instance = doInvokeConstructor(cppClass->getService(), classUserData->getObjectData()->getMetaClass(), &callableParam);

	if(instance != NULL) {
		GObjectUserData * instanceUserData = new GObjectUserData(cppClass->getParam(), classUserData->getObjectData()->getMetaClass(), instance, true, opcvNone, cudtObject);
		instanceCreated(instanceUserData, classUserData);
		return createPythonObject(instanceUserData);
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
	GObjectUserData * userData = gdynamic_cast<GObjectUserData *>(cppObject->getUserData());
	const char * name = PyString_AsString(attrName);

	if(doSetFieldValue(userData, name, pythonToVariant(userData->getParam(), value).getValue())) {
		return 0;
	}

	return -1;

	LEAVE_PYTHON(return -1)
}

PyObject * doGetAttributeObject(GPythonObject * cppObject, PyObject * attrName)
{
	if(PyObject_HasAttr(cppObject->ob_type->tp_dict, attrName)) {
		return PyObject_GetAttr(cppObject->ob_type->tp_dict, attrName);
	}

	const char * name = PyString_AsString(attrName);

	GObjectUserData * userData = gdynamic_cast<GObjectUserData *>(cppObject->getUserData());
	GMetaClassTraveller traveller(userData->getObjectData()->getMetaClass(), userData->getObjectData()->getInstance());

	void * instance = NULL;
	IMetaClass * outDerived;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance, &outDerived));
		GScopedInterface<IMetaClass> derived(outDerived);

		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = cppObject->getParam()->getMetaMap()->findClassMap(metaClass.get());
		GMetaMapItem * mapItem = mapClass->findItem(name);
		if(mapItem == NULL) {
			continue;
		}

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {

				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(userData->getParam()->getConfig(), getObjectData(userData), data.get())) {
					PyObject * r;
					if(accessibleToScript<GPythonMethods>(&r, cppObject->getParam(), data.get(), instance, userData->getObjectData()->getCV() == opcvConst)) {
						return r;
					}
					return r;
				}

			}
			   break;

			case mmitMethod:
			case mmitMethodList: {
				GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getData());
				if(data == NULL) {
					GScopedInterface<IMetaList> methodList(createMetaList());
					loadMethodList(&traveller, methodList.get(), userData->getParam()->getMetaMap(), mapItem, instance, name);

					GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass.get(), derived.get()));

					data = new GMapItemMethodData(GMethodData(boundClass.get(), methodList.get(), name, udmtInternal));

					mapItem->setData(data);
				}

				return createPythonObject(new GObjectMethodUserData(userData->getParam(), userData->getObjectData(), data->getMethodData()));
			}
				break;

			case mmitEnum:
				if(! userData->getObjectData()->isInstance() || userData->getParam()->getConfig().allowAccessEnumTypeViaInstance()) {
					GMapItemObjectData * data = gdynamic_cast<GMapItemObjectData *>(mapItem->getData());
					if(data == NULL) {
						data = new GMapItemObjectData;
						mapItem->setData(data);
						GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
						GPythonScopedPointer enumObject(createPythonObject(new GEnumUserData(userData->getParam(), metaEnum.get())));
						data->setObject(enumObject.get());
					}

					return data->getObject();
				}
				break;

			case mmitEnumValue:
				if(! userData->getObjectData()->isInstance() || userData->getParam()->getConfig().allowAccessEnumValueViaInstance()) {
					GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
					return variantToPython(userData->getParam(), metaGetEnumValue(metaEnum, static_cast<uint32_t>(mapItem->getEnumIndex())), GMetaType(), false, true);
				}
				break;

			case mmitClass:
				if(! userData->getObjectData()->isInstance() || userData->getParam()->getConfig().allowAccessClassViaInstance()) {
					GMapItemObjectData * data = gdynamic_cast<GMapItemObjectData *>(mapItem->getData());
					if(data == NULL) {
						data = new GMapItemObjectData;
						mapItem->setData(data);
						GScopedInterface<IMetaClass> innerMetaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
						GPythonScopedPointer classObject(createClassObject(userData->getParam(), innerMetaClass.get()));
						data->setObject(classObject.get());
					}

					return data->getObject();
				}
				break;

			default:
				break;
		}

	}

	return NULL;
}


PyObject * callbackGetEnumValue(PyObject * object, PyObject * attrName)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(object);
	
	GEnumUserData * userData = gdynamic_cast<GEnumUserData *>(cppObject->getUserData());

	const char * name = PyString_AsString(attrName);

	int32_t index = userData->getMetaEnum()->findKey(name);
	if(index >= 0) {
		return variantToPython(userData->getParam(), metaGetEnumValue(userData->getMetaEnum(), index), GMetaType(), true, false);
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
	
	GAccessibleUserData * userData = gdynamic_cast<GAccessibleUserData *>(cppObject->getUserData());

	PyObject * r;
	if(accessibleToScript<GPythonMethods>(&r, userData->getParam(), userData->getAccessible(), userData->getInstance(), false)) {
		return r;
	}
	return r;

	LEAVE_PYTHON(return NULL)
}

int callbackAccessibleDescriptorSet(PyObject * self, PyObject * /*obj*/, PyObject * value)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(self);
	
	GAccessibleUserData * userData = gdynamic_cast<GAccessibleUserData *>(cppObject->getUserData());

	GMetaVariant v = pythonToVariant(userData->getParam(), value);
	metaSetValue(userData->getAccessible(), userData->getInstance(), v.getValue());

	return 0;

	LEAVE_PYTHON(return 0)
}

GMetaVariant invokePythonFunctionIndirectly(const GBindingParamPointer & bindingParam, PyObject * object, PyObject * func, GMetaVariant const * const * params, size_t paramCount, const char * name)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(! bindingParam) {
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
			GPythonScopedPointer arg(variantToPython(bindingParam, params[i]->getValue(), params[i]->getType(), false, true));
			if(!arg) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
			PyTuple_SetItem(args.get(), start + i, arg.take());
		}

		GPythonScopedPointer result;

		result.reset(PyObject_Call(func, args.get(), NULL));

		return pythonToVariant(bindingParam, result.get());
	}
	else {
		raiseCoreException(Error_ScriptBinding_CantCallNonfunction);
	}

	return GMetaVariant();
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
		return (strcmp(signatureValue, PyString_AsString(signature.get())) == 0);
	}
	else {
		return false;
	}
}

void doBindMethodList(const GBindingParamPointer & param, PyObject * owner, const char * name, IMetaList * methodList)
{
	GMethodData data = GMethodData(NULL, methodList, name, udmtMethodList);
	GObjectMethodUserData * methodData = new GObjectMethodUserData(param, GSharedObjectData(), data);
	PyObject * methodObject = createPythonObject(methodData);

	setObjectAttr(owner, name, methodObject);
}

void doBindClass(const GBindingParamPointer & param, PyObject * owner, const char * name, IMetaClass * metaClass)
{
	PyObject * classObject = createClassObject(param, metaClass);

	setObjectAttr(owner, name, classObject);
}

void doBindEnum(const GBindingParamPointer & param, PyObject * owner, const char * name, IMetaEnum * metaEnum)
{
	PyObject * enumObject = createPythonObject(new GEnumUserData(param, metaEnum));

	setObjectAttr(owner, name, enumObject);
}

void doBindAccessible(const GBindingParamPointer & param, PyObject * owner, const char * name, void * instance, IMetaAccessible * accessible)
{
	PyObject * accessibleObject = createPythonObject(new GAccessibleUserData(param, instance, accessible));

	setObjectAttr(owner, name, accessibleObject);
}


GPythonScriptFunction::GPythonScriptFunction(const GBindingParamPointer & bindingParam, PyObject * func)
	: bindingParam(bindingParam), func(func)
{
	Py_XINCREF(this->func);
}

GPythonScriptFunction::~GPythonScriptFunction()
{
	Py_XDECREF(this->func);
}

GMetaVariant GPythonScriptFunction::invoke(const GMetaVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GMetaVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GMetaVariant GPythonScriptFunction::invokeIndirectly(GMetaVariant const * const * params, size_t paramCount)
{
	ENTER_PYTHON()

	return invokePythonFunctionIndirectly(this->bindingParam.get(), NULL, this->func, params, paramCount, "");

	LEAVE_PYTHON(return GMetaVariant())
}


GPythonScriptObject::GPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config)
	: super(config), object(object), param(new GScriptBindingParam(service, config)), freeParam(true)
{
}

GPythonScriptObject::GPythonScriptObject(const GPythonScriptObject & other, PyObject * object)
	: super(other.param->getConfig()), object(object), param(other.param), freeParam(false)
{
}

GPythonScriptObject::~GPythonScriptObject()
{
	if(this->freeParam) {
//		delete this->param;
	}
}

GScriptDataType GPythonScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	ENTER_PYTHON()

	if(outMetaTypeItem != NULL) {
		*outMetaTypeItem = NULL;
	}

	GPythonScopedPointer obj(getObjectAttr(this->object, name));
	
	if(obj) {
		return getPythonType(obj.get(), outMetaTypeItem);
	}
	else {
		return sdtNull;
	}

	LEAVE_PYTHON(return sdtUnknown)
}

void GPythonScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	ENTER_PYTHON()

	doBindClass(this->param, this->object, name, metaClass);

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	ENTER_PYTHON()

	doBindEnum(this->param, this->object, name, metaEnum);

	LEAVE_PYTHON()
}

GScriptObject * GPythonScriptObject::createScriptObject(const char * name)
{
	ENTER_PYTHON()

	GPythonScopedPointer existObject(getObjectAttr(this->object, name));
	if(existObject) {
		return NULL;
	}

	PyObject * dict = PyDict_New();
	setObjectAttr(this->object, name, dict);
	setObjectSignature(dict);
	return new GPythonScriptObject(*this, dict);

	LEAVE_PYTHON(return NULL)
}

GScriptObject * GPythonScriptObject::gainScriptObject(const char * name)
{
	ENTER_PYTHON()

	PyObject * attr = getObjectAttr(this->object, name);
	if(attr != NULL) {
		return new GPythonScriptObject(*this, attr);
	}
	else {
		return NULL;
	}

	LEAVE_PYTHON(return NULL)
}

GScriptFunction * GPythonScriptObject::gainScriptFunction(const char * name)
{
	ENTER_PYTHON()

	GPythonScopedPointer func(getObjectAttr(this->object, name));
	if(func) {
		if(PyCallable_Check(func.get())) {
			return new GPythonScriptFunction(this->param, func.take());
		}
	}

	return NULL;

	LEAVE_PYTHON(return NULL)
}

GMetaVariant GPythonScriptObject::invoke(const char * name, const GMetaVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GMetaVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GMetaVariant GPythonScriptObject::invokeIndirectly(const char * name, GMetaVariant const * const * params, size_t paramCount)
{
	ENTER_PYTHON()

	GPythonScopedPointer func(getObjectAttr(this->object, name));
	return invokePythonFunctionIndirectly(this->param, NULL, func.get(), params, paramCount, name);

	LEAVE_PYTHON(return GMetaVariant())
}

void GPythonScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.data.typeData)), "Only fundamental value can be bound via bindFundamental");

	ENTER_PYTHON()

	setObjectAttr(this->object, name, variantToPython(this->param, value, GMetaType(), false, true));

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	ENTER_PYTHON()

	doBindAccessible(this->param, this->object, name, instance, accessible);

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindString(const char * stringName, const char * s)
{
	ENTER_PYTHON()

	setObjectAttr(this->object, stringName, PyString_FromString(s));

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	ENTER_PYTHON()

	setObjectAttr(this->object, objectName, objectToPython(this->param, instance, type, transferOwnership, opcvNone, cudtObject));

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindRaw(const char * name, const GVariant & value)
{
	ENTER_PYTHON()

	setObjectAttr(this->object, name, rawToPython(this->param, value));

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindMethod(const char * name, void * instance, IMetaMethod * method)
{
	ENTER_PYTHON()

	if(method->isStatic()) {
		instance = NULL;
	}

	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method, instance);

	doBindMethodList(this->param, this->object, name, methodList.get());

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	ENTER_PYTHON()

	doBindMethodList(this->param, this->object, name, methodList);

	LEAVE_PYTHON()
}

IMetaClass * GPythonScriptObject::getClass(const char * className)
{
	IMetaTypedItem * typedItem = NULL;

	GScriptDataType sdt = this->getType(className, &typedItem);
	GScopedInterface<IMetaTypedItem> item(typedItem);
	if(sdt == sdtClass) {
		return gdynamic_cast<IMetaClass *>(item.take());
	}

	return NULL;
}

IMetaEnum * GPythonScriptObject::getEnum(const char * enumName)
{
	IMetaTypedItem * typedItem = NULL;

	GScriptDataType sdt = this->getType(enumName, &typedItem);
	GScopedInterface<IMetaTypedItem> item(typedItem);
	if(sdt == sdtEnum) {
		return gdynamic_cast<IMetaEnum *>(item.take());
	}

	return NULL;
}

GVariant GPythonScriptObject::getFundamental(const char * name)
{
	ENTER_PYTHON()

	GPythonScopedPointer obj(getObjectAttr(this->object, name));
	if(obj && getPythonType(obj.get(), NULL) == sdtFundamental) {
		return pythonToVariant(this->getParam(), obj.get()).getValue();;
	}
	else {
		return GVariant();
	}

	LEAVE_PYTHON(return GVariant())
}

std::string GPythonScriptObject::getString(const char * stringName)
{
	ENTER_PYTHON()

	GPythonScopedPointer obj(getObjectAttr(this->object, stringName));
	if(obj && PyString_Check(obj.get())) {
		return PyString_AS_STRING(obj.get());
	}
	else {
		return "";
	}

	LEAVE_PYTHON(return "")
}

void * GPythonScriptObject::getObject(const char * objectName)
{
	ENTER_PYTHON()

	GPythonScopedPointer obj(getObjectAttr(this->object, objectName));
	if(obj && obj->ob_type == &objectType) {
		return gdynamic_cast<GObjectUserData *>(castFromPython(obj.get())->getUserData())->getObjectData()->getInstance();
	}
	else {
		return NULL;
	}

	LEAVE_PYTHON(return NULL)
}

GVariant GPythonScriptObject::getRaw(const char * name)
{
	ENTER_PYTHON()

	GPythonScopedPointer obj(getObjectAttr(this->object, name));
	if(obj && getPythonType(obj.get(), NULL) == sdtRaw) {
		return gdynamic_cast<GRawUserData *>(castFromPython(obj.get())->getUserData())->getData();
	}
	else {
		return GVariant();
	}


	LEAVE_PYTHON(return GVariant())
}

IMetaMethod * GPythonScriptObject::getMethod(const char * methodName, void ** outInstance)
{
	ENTER_PYTHON()

	if(outInstance != NULL) {
		*outInstance = NULL;
	}

	GPythonScopedPointer obj(getObjectAttr(this->object, methodName));
	if(obj && obj->ob_type == &functionType) {
		GObjectMethodUserData * userData = gdynamic_cast<GObjectMethodUserData *>(castFromPython(obj.get())->getUserData());
		if(outInstance != NULL) {
			*outInstance = userData->getMethodData().getMethodList()->getInstanceAt(0);
		}
		return gdynamic_cast<IMetaMethod *>(userData->getMethodData().getMethodList()->getAt(0));
	}
	else {
		return NULL;
	}

	LEAVE_PYTHON(return NULL)
}

IMetaList * GPythonScriptObject::getMethodList(const char * methodName)
{
	ENTER_PYTHON()

	GPythonScopedPointer obj(getObjectAttr(this->object, methodName));
	if(obj && obj->ob_type == &functionType) {
		GObjectMethodUserData * userData = gdynamic_cast<GObjectMethodUserData *>(castFromPython(obj.get())->getUserData());
		userData->getMethodData().getMethodList()->addReference();
		return userData->getMethodData().getMethodList();
	}
	else {
		return NULL;
	}

	LEAVE_PYTHON(return NULL)
}

void GPythonScriptObject::assignValue(const char * fromName, const char * toName)
{
	ENTER_PYTHON()

	GPythonScopedPointer obj(getObjectAttr(this->object, fromName));
	if(obj) {
		setObjectAttr(this->object, toName, obj.get());
	}

	LEAVE_PYTHON()
}

bool GPythonScriptObject::valueIsNull(const char * name)
{
	ENTER_PYTHON()

	GPythonScopedPointer obj(getObjectAttr(this->object, name));

	return !obj || obj.get() == Py_None;

	LEAVE_PYTHON(return false)
}

void GPythonScriptObject::nullifyValue(const char * name)
{
	ENTER_PYTHON()

	setObjectAttr(this->object, name, Py_None);

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindCoreService(const char * name)
{
	ENTER_PYTHON()

	this->getParam()->bindScriptCoreService(this, name);

	LEAVE_PYTHON()
}



} // unamed namespace


GScriptObject * old_createPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config)
{
	return new GPythonScriptObject(service, object, config);
}

IScriptObject * old_createPythonScriptInterface(IMetaService * service, PyObject * object, const GScriptConfig & config)
{
	return new ImplScriptObject(new GPythonScriptObject(service, object, config), true);
}

} // namespace cpgf


