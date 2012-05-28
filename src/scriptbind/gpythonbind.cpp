#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

#include "python.h"

#include "cpgf/gscopedptr.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gstringmap.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"

// test
#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/gmetadefine.h"
#include <iostream>
#include <string>
using namespace std;


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



namespace cpgf {

namespace {

template <typename T>
struct GPythonScopedPointerDeleter
{
	static inline void Delete(T * p) {
		if(p != NULL) {
			Py_DECREF(p);
		}
	}
};

typedef GScopedPointer<PyObject, GPythonScopedPointerDeleter<PyObject> > GPythonScopedPointer;

class GMapItemObjectData : public GMetaMapItemData
{
public:
	GMapItemObjectData() : object(NULL) {
	}

	virtual ~GMapItemObjectData() {
		if(this->object != NULL) {
			Py_DECREF(this->object);
		}
	}

	PyObject * getObject() const {
		Py_INCREF(this->object);
		return this->object;
	}

	void setObject(PyObject * object) {
		if(this->object != object) {
			if(this->object != NULL) {
				Py_DECREF(this->object);
			}
			
			this->object = object;
			
			if(this->object != NULL) {
				Py_INCREF(this->object);
			}
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
	GScriptBindingParam * getParam() const;
	GScriptUserData * getUserData() const;

protected:
	void initType(PyTypeObject * type);

private:
	GScriptUserData * userData;
};

class GPythonScriptFunction : public GScriptFunction
{
public:
	GPythonScriptFunction(GScriptBindingParam * bindingParam, PyObject * func);
	virtual ~GPythonScriptFunction();

	virtual GMetaVariant invoke(const GMetaVariant * params, size_t paramCount);
	virtual GMetaVariant invokeIndirectly(GMetaVariant const * const * params, size_t paramCount);

private:
	GScriptBindingParam * bindingParam;
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

public:
	GScriptBindingParam * getParam() const {
		return this->param;
	}

	PyObject * getObject() const {
		return this->object;
	}

private:
	GPythonScriptObject(const GPythonScriptObject & other, PyObject * object);

private:
	PyObject * object;
	GScriptBindingParam * param;
	bool freeParam;
};


GPythonObject * castFromPython(PyObject * object) {
	return gdynamic_cast<GPythonObject *>(static_cast<GPythonObject *>(object));
}

void commonDealloc(PyObject* p)
{
    delete castFromPython(p);
}

PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * keyWords);

PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * keyWords);

PyObject * callbackGetAttribute(PyObject * object, PyObject * attrName);
int callbackSetAttribute(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * doGetAttributeObject(GPythonObject * cppObject, PyObject * attrName);
bool doSetAttributeObject(GPythonObject * cppObject, PyObject * attrName, PyObject * value);

PyObject * callbackGetEnumValue(PyObject * object, PyObject * attrName);
int callbackSetEnumValue(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackAccessibleDescriptorGet(PyObject * self, PyObject * obj, PyObject * type);
int callbackAccessibleDescriptorSet(PyObject * self, PyObject * obj, PyObject * value);

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
    0, //(reprfunc)func_repr,                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    &callbackCallMethod,                              /* tp_call */
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
    0, //(reprfunc)func_repr,                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    &callbackConstructObject,                              /* tp_call */
    0,                                  /* tp_str */
	&callbackGetAttribute,             /* tp_getattro */
    &callbackSetAttribute,            /* tp_setattro */
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
    0, //(reprfunc)func_repr,                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	&callbackGetAttribute,             /* tp_getattro */
    &callbackSetAttribute,            /* tp_setattro */
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
    0, //(reprfunc)func_repr,                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	&callbackGetEnumValue,             /* tp_getattro */
    &callbackSetEnumValue,            /* tp_setattro */
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
    0, //(reprfunc)func_repr,                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	NULL,             /* tp_getattro */
    NULL,            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_CLASS /* | Py_TPFLAGS_HAVE_GC */,/* tp_flags */
    0,                                  /* tp_doc */
    0, // (traverseproc)func_traverse,          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, //offsetof(PyFunctionObject, func_weakreflist), /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    NULL,                                  /* tp_methods */
    0, // func_memberlist,              /* tp_members */
    NULL, //function_getsetlist,                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    &callbackAccessibleDescriptorGet,           /* tp_descr_get */
    &callbackAccessibleDescriptorSet,                                  /* tp_descr_set */
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
    0, //(reprfunc)func_repr,                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    0,                              /* tp_call */
    0,                                  /* tp_str */
	NULL,             /* tp_getattro */
    NULL,            /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT /* | Py_TPFLAGS_HAVE_GC */,/* tp_flags */
    0,                                  /* tp_doc */
    0, // (traverseproc)func_traverse,          /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, //offsetof(PyFunctionObject, func_weakreflist), /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    NULL,                                  /* tp_methods */
    0, // func_memberlist,              /* tp_members */
    NULL, //function_getsetlist,                /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    NULL,           /* tp_descr_get */
    NULL,                                  /* tp_descr_set */
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

GPythonObject::GPythonObject(GScriptUserData * userData)
	: userData(userData)
{
	PyTypeObject * typeObject = NULL;

	switch(userData->getType()) {
		case udtClass:
			typeObject = (gdynamic_cast<GClassUserData *>(userData)->isInstance ? &objectType : &classType);
			break;

		case udtMethodList:
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

	this->initType(typeObject);
}

GPythonObject::~GPythonObject()
{
}

IMetaService * GPythonObject::getService() const
{
	return this->getParam()->getService();
}

GScriptBindingParam * GPythonObject::getParam() const
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
			*typeItem = gdynamic_cast<GClassUserData *>(castFromPython(value)->getUserData())->metaClass;
			(*typeItem)->addReference();
		}
		return sdtClass;
	}

	if(value->ob_type == &objectType) {
		if(typeItem != NULL) {
			*typeItem = gdynamic_cast<GClassUserData *>(castFromPython(value)->getUserData())->metaClass;
			(*typeItem)->addReference();
		}
		return sdtObject;
	}

	if(value->ob_type == &enumType) {
		if(typeItem != NULL) {
			*typeItem = gdynamic_cast<GEnumUserData *>(castFromPython(value)->getUserData())->metaEnum;
			(*typeItem)->addReference();
		}
		return sdtEnum;
	}

	if(value->ob_type == &rawType) {
		return sdtRaw;
	}

	if(value->ob_type == &functionType) {
		if(gdynamic_cast<GMethodListUserData *>(castFromPython(value)->getUserData())->methodList->getCount() > 1) {
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

GMetaVariant pythonToVariant(GScriptBindingParam * param, PyObject * value)
{
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
		return PyString_AsString(value);
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
	}

	return GMetaVariant();
}

PyObject * objectToPython(GScriptBindingParam * param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv, ClassUserDataType dataType)
{
	if(instance == NULL) {
		return Py_None;
	}

	return new GPythonObject(new GClassUserData(param, metaClass, instance, true, allowGC, cv, dataType));
}

PyObject * rawToPython(GScriptBindingParam * param, const GVariant & value)
{
	GVariantType vt = value.getType();

	if(param->getConfig().allowAccessRawData() && variantIsRawData(vt)) {
		PyObject * rawObject = new GPythonObject(new GRawUserData(param, value));

		return rawObject;
	}

	return NULL;
}

PyObject * variantToPython(GScriptBindingParam * param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
{
	GVariantType vt = static_cast<GVariantType>(value.getType() & ~byReference);
	
	if(vtIsEmpty(vt)) {
		return Py_None;
	}

	if(vtIsBoolean(vt)) {
		return fromVariant<bool>(value) ? Py_True : Py_False;
	}

	if(vtIsInteger(vt)) {
		return PyInt_FromLong(fromVariant<long>(value));
	}

	if(vtIsReal(vt)) {
		return PyFloat_FromDouble(fromVariant<double>(value));
	}

	if(!vtIsByteArray(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == NULL) {
		return Py_None;
	}

	if(variantIsString(value)) {
		return PyString_FromString(fromVariant<char *>(value));
	}

	if(! type.isEmpty() && type.getPointerDimension() <= 1) {
		GScopedInterface<IMetaTypedItem> typedItem(param->getService()->findTypedItemByName(type.getBaseName()));
		if(typedItem) {
			bool isReference = type.isReference();

			if(type.getPointerDimension() == 0 && !isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");
				GASSERT_MSG(type.baseIsClass(), "Unknown type");

				IMetaClass * metaClass = gdynamic_cast<IMetaClass *>(typedItem.get());
				void * instance = metaClass->cloneInstance(objectAddressFromVariant(value));
				return objectToPython(param, instance, gdynamic_cast<IMetaClass *>(typedItem.get()), true, metaTypeToCV(type), cudtNormal);
			}

			if(type.getPointerDimension() == 1 || isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

				if(vtIsByteArray(vt)) {
					GScopedInterface<IByteArray> ba(value.data.valueByteArray); // free the byte array after return
					return objectToPython(param, value.data.valueByteArray, gdynamic_cast<IMetaClass *>(typedItem.get()),
						allowGC, metaTypeToCV(type), cudtByteArray);
				}
				else {
					return objectToPython(param, fromVariant<void *>(value), gdynamic_cast<IMetaClass *>(typedItem.get()),
						allowGC, metaTypeToCV(type), cudtNormal);
				}
			}
		}

		if(bind_internal::shouldRemoveReference(type)) {
			GMetaType newType(type);
			newType.removeReference();

			return variantToPython(param, value, newType, allowGC, allowRaw);
		}
	}

	if(allowRaw) {
		return rawToPython(param, value);
	}

	return NULL;
}

PyObject * converterToPython(GScriptBindingParam * param, const GVariant & value, IMetaConverter * converter)
{
	if(converter == NULL) {
		return NULL;
	}

	if(isMetaConverterCanRead(converter->capabilityForCString())) {
		gapi_bool needFree;
		
		GScopedInterface<IMemoryAllocator> allocator(param->getService()->getAllocator());
		const char * s = converter->readCString(objectAddressFromVariant(value), &needFree, allocator.get());

		if(s != NULL) {
			PyObject * result = PyString_FromString(s);

			if(needFree) {
				allocator->free((void *)s);
			}

			return result;
		}
	}

	return NULL;
}

PyObject * methodResultToPython(GScriptBindingParam * param, IMetaCallable * callable, InvokeCallableResult * result)
{
	if(result->resultCount > 0) {
		GMetaTypeData typeData;

		callable->getResultType(&typeData);
		metaCheckError(callable);

		GVariant value = GVariant(result->resultData);
		PyObject * v;
		v = variantToPython(param, value, GMetaType(typeData), !! callable->isResultTransferOwnership(), false);
		if(v == NULL) {
			GScopedInterface<IMetaConverter> converter(metaGetResultExtendType(callable, GExtendTypeCreateFlag_Converter).getConverter());
			v = converterToPython(param, value, converter.get());
		}
		if(v == NULL) {
			v = rawToPython(param, value);
		}
		if(v == NULL) {
			raiseCoreException(Error_ScriptBinding_FailVariantToScript);
		}
		return v;

	}

	return Py_None;
}

void loadMethodParameters(GScriptBindingParam * p, PyObject * args, GVariantData * outputParams)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
		PyObject * param = PyTuple_GetItem(args, i);
		outputParams[i] = pythonToVariant(p, param).takeData().varData;
	}
}

void loadMethodParamTypes(PyObject * args, GBindDataType * outputTypes)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
		IMetaTypedItem * typeItem;
		PyObject * param = PyTuple_GetItem(args, i);
		outputTypes[i].dataType = getPythonType(param, &typeItem);
		outputTypes[i].typeItem.reset(typeItem);
	}
}

void loadCallableParam(GScriptBindingParam * param, PyObject * args, InvokeCallableParam * callableParam)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	if(paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}

	callableParam->paramCount = paramCount;
	loadMethodParameters(param, args, callableParam->paramsData);
	loadMethodParamTypes(args, callableParam->paramsType);
}


PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * keyWords)
{
	ENTER_PYTHON()

	GPythonObject * methodObject = castFromPython(callableObject);

	GMethodListUserData * userData = gdynamic_cast<GMethodListUserData *>(methodObject->getUserData());
	GScopedInterface<IMetaList> methodList;

	InvokeCallableParam callableParam;
	loadCallableParam(userData->getParam(), args, &callableParam);

	void * instance = NULL;
	if(userData != NULL) {
//		instance = userData->instance;
	}

//	GScopedInterface<IMetaList> methodList;
	if(true) { //namedUserData->metaClass == NULL) {
		methodList.reset(userData->methodList);
		methodList->addReference();
	}
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
		int rank = rankCallable(methodObject->getService(), meta.get(), &callableParam);
		if(rank > maxRank) {
			maxRank = rank;
			maxRankIndex = static_cast<int>(i);
		}
	}

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;
		GScopedInterface<IMetaCallable> callable(gdynamic_cast<IMetaCallable *>(methodList->getAt(maxRankIndex)));
		doInvokeCallable(methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex)), callable.get(), callableParam.paramsData, callableParam.paramCount, &result);
		return methodResultToPython(methodObject->getUserData()->getParam(), callable.get(), &result);
	}

	raiseCoreException(Error_ScriptBinding_CantFindMatchedMethod);

	return Py_None;

	LEAVE_PYTHON(return NULL)
}


PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * keyWords)
{
	ENTER_PYTHON()

	GPythonObject * cppClass = castFromPython(callableObject);
	GClassUserData * userData = gdynamic_cast<GClassUserData *>(cppClass->getUserData());
	
	InvokeCallableParam callableParam;
	loadCallableParam(userData->getParam(), args, &callableParam);

	void * instance = doInvokeConstructor(cppClass->getService(), userData->metaClass, &callableParam);

	if(instance != NULL) {
		return new GPythonObject(new GClassUserData(cppClass->getParam(), userData->metaClass, instance, true, true, opcvNone, cudtNormal));
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

	return Py_None;

	LEAVE_PYTHON(return NULL)
}

int callbackSetAttribute(PyObject * object, PyObject * attrName, PyObject * value)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(object);

	return doSetAttributeObject(cppObject, attrName, value) ? 0 : -1;

	LEAVE_PYTHON(return -1)
}

bool doSetAttributeObject(GPythonObject * cppObject, PyObject * attrName, PyObject * value)
{
	const char * name = PyString_AsString(attrName);

	GClassUserData * userData = gdynamic_cast<GClassUserData *>(cppObject->getUserData());

	if(userData->cv == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);

		return false;
	}

	GMetaClassTraveller traveller(userData->metaClass, userData->instance);

	void * instance = NULL;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
		if(!metaClass) {
			break;
		}

		GMetaMapItem * mapItem = findMetaMapItem(userData->getParam()->getMetaMap(), metaClass.get(), name);
		if(mapItem == NULL) {
			continue;
		}

		bool error = false;

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {
				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(userData, data.get())) {
					GVariant v = pythonToVariant(userData->getParam(), value).getValue();
					metaSetValue(data.get(), userData->instance, v);
					return true;
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList:
			case mmitEnum:
			case mmitEnumValue:
			case mmitClass:
				error = true;
				break;

			default:
				break;
		}

		if(error) {
			raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);
			break;
		}
	}

	return false;
}

PyObject * doGetAttributeObject(GPythonObject * cppObject, PyObject * attrName)
{
	if(PyObject_HasAttr(cppObject->ob_type->tp_dict, attrName)) {
		return PyObject_GetAttr(cppObject->ob_type->tp_dict, attrName);
	}

	const char * name = PyString_AsString(attrName);

	GClassUserData * userData = gdynamic_cast<GClassUserData *>(cppObject->getUserData());
	GMetaClassTraveller traveller(userData->metaClass, userData->instance);

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
				if(allowAccessData(userData, data.get())) {
					GMetaType type;
					GVariant result = getAccessibleValueAndType(instance, data.get(), &type, userData->cv == opcvConst);
					PyObject * fieldObject = variantToPython(userData->getParam(), result, type, false, false);
					if(fieldObject == NULL) {
						GScopedInterface<IMetaConverter> converter(metaGetItemExtendType(data, GExtendTypeCreateFlag_Converter).getConverter());
						fieldObject = converterToPython(userData->getParam(), result, converter.get());
					}
					if(fieldObject == NULL) {
						fieldObject = rawToPython(userData->getParam(), result);
					}
					if(fieldObject == NULL) {
						raiseCoreException(Error_ScriptBinding_FailVariantToScript);
					}
					return fieldObject;
				}

			}
			   break;

			case mmitMethod:
			case mmitMethodList: {
				GMapItemObjectData * data = gdynamic_cast<GMapItemObjectData *>(mapItem->getData());
				if(data == NULL) {
					GScopedInterface<IMetaList> methodList(createMetaList());
					loadMethodList(&traveller, methodList.get(), userData->getParam()->getMetaMap(), mapItem, instance, userData, name, true);

					data = new GMapItemObjectData();
					mapItem->setData(data);
					PyObject * methodObject = new GPythonObject(new GMethodListUserData(userData->getParam(), methodList.get(), udmtMethodList));
					data->setObject(methodObject);
				}

				return data->getObject();
			}
				break;

			case mmitEnum:
				if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumTypeViaInstance()) {
					GMapItemObjectData * data = gdynamic_cast<GMapItemObjectData *>(mapItem->getData());
					if(data == NULL) {
						data = new GMapItemObjectData;
						mapItem->setData(data);
						GEnumUserData * newUserData;
						GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
						PyObject * enumObject = new GPythonObject(new GEnumUserData(userData->getParam(), metaEnum.get()));
						data->setObject(enumObject);
					}

					return data->getObject();
				}
				break;

			case mmitEnumValue:
				if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumValueViaInstance()) {
					GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
					return variantToPython(userData->getParam(), metaGetEnumValue(metaEnum, static_cast<uint32_t>(mapItem->getEnumIndex())), GMetaType(), false, true);
				}
				break;

			case mmitClass:
				if(! userData->isInstance || userData->getParam()->getConfig().allowAccessClassViaInstance()) {
					GMapItemObjectData * data = gdynamic_cast<GMapItemObjectData *>(mapItem->getData());
					if(data == NULL) {
						data = new GMapItemObjectData;
						mapItem->setData(data);
						GScopedInterface<IMetaClass> innerMetaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
						PyObject * classObject = new GPythonObject(new GClassUserData(userData->getParam(), innerMetaClass.get(), NULL, false, false, opcvNone, cudtNormal));
						data->setObject(classObject);
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

	int32_t index = userData->metaEnum->findKey(name);
	if(index >= 0) {
		return variantToPython(userData->getParam(), metaGetEnumValue(userData->metaEnum, index), GMetaType(), true, false);
	}

	raiseCoreException(Error_ScriptBinding_CantFindEnumKey, *name);

	return Py_None;

	LEAVE_PYTHON(return NULL)
}

int callbackSetEnumValue(PyObject * object, PyObject * attrName, PyObject * value)
{
	ENTER_PYTHON()

	raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);

	return -1;

	LEAVE_PYTHON(return -1)
}

PyObject * callbackAccessibleDescriptorGet(PyObject * self, PyObject * obj, PyObject * pyType)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(self);
	
	GAccessibleUserData * userData = gdynamic_cast<GAccessibleUserData *>(cppObject->getUserData());

	GMetaType type;
	GVariant result = getAccessibleValueAndType(userData->instance, userData->accessible, &type, false);

	PyObject * v = NULL;
	v = variantToPython(userData->getParam(), result, type, false, false);
	if(v == NULL) {
		GScopedInterface<IMetaConverter> converter(metaGetItemExtendType(userData->accessible, GExtendTypeCreateFlag_Converter).getConverter());
		v = converterToPython(userData->getParam(), result, converter.get());
	}
	if(v == NULL) {
		v = rawToPython(userData->getParam(), result);
	}
	if(v == NULL) {
		v = Py_None;
	}
	return v;

	LEAVE_PYTHON(return NULL)
}

int callbackAccessibleDescriptorSet(PyObject * self, PyObject * obj, PyObject * value)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(self);
	
	GAccessibleUserData * userData = gdynamic_cast<GAccessibleUserData *>(cppObject->getUserData());

	GMetaVariant v = pythonToVariant(userData->getParam(), value);
	metaSetValue(userData->accessible, userData->instance, v.getValue());

	return 0;

	LEAVE_PYTHON(return 0)
}

GMetaVariant invokePythonFunctionIndirectly(GScriptBindingParam * bindingParam, PyObject * object, PyObject * func, GMetaVariant const * const * params, size_t paramCount, const char * name)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(PyCallable_Check(func)) {
		int allParamCount = paramCount;
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
Py_INCREF(arg.get());
			if(!arg) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
			PyTuple_SetItem(args.get(), start + i, arg.get());
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
	PyObject_SetAttrString(owner, name, value);
}

PyObject * getObjectAttr(PyObject * owner, const char * name)
{
	if(PyObject_HasAttrString(owner, name)) {
		return PyObject_GetAttrString(owner, name);
	}
	else {
		return NULL;
	}
}

void doBindMethodList(GScriptBindingParam * param, PyObject * owner, const char * name, IMetaList * methodList)
{
	PyObject * methodObject = new GPythonObject(new GMethodListUserData(param, methodList, udmtMethodList));

	setObjectAttr(owner, name, methodObject);
}

void doBindClass(GScriptBindingParam * param, PyObject * owner, const char * name, IMetaClass * metaClass)
{
	PyObject * classObject = new GPythonObject(new GClassUserData(param, metaClass, NULL, false, false, opcvNone, cudtNormal));

	setObjectAttr(owner, name, classObject);
}

void doBindEnum(GScriptBindingParam * param, PyObject * owner, const char * name, IMetaEnum * metaEnum)
{
	PyObject * enumObject = new GPythonObject(new GEnumUserData(param, metaEnum));

	setObjectAttr(owner, name, enumObject);
}

void doBindAccessible(GScriptBindingParam * param, PyObject * owner, const char * name, void * instance, IMetaAccessible * accessible)
{
	PyObject * accessibleObject = new GPythonObject(new GAccessibleUserData(param, instance, accessible));

	setObjectAttr(owner, name, accessibleObject);
}


GPythonScriptFunction::GPythonScriptFunction(GScriptBindingParam * bindingParam, PyObject * func)
	: bindingParam(bindingParam), func(func)
{
	Py_INCREF(func);
}

GPythonScriptFunction::~GPythonScriptFunction()
{
	Py_DECREF(func);
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

	return invokePythonFunctionIndirectly(this->bindingParam, NULL, this->func, params, paramCount, "");

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
		delete this->param;
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

	PyObject * dict = PyDict_New();
	setObjectAttr(this->object, name, dict);
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

	setObjectAttr(this->object, objectName, objectToPython(this->param, instance, type, transferOwnership, opcvNone, cudtNormal));

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindRaw(const char * name, const GVariant & value)
{
	GASSERT_MSG(variantIsRawData(vtGetType(value.data.typeData)), "Only raw data (pointer, object) can be bound via bindRaw");

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
		return gdynamic_cast<GClassUserData *>(castFromPython(obj.get())->getUserData())->instance;
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
		return gdynamic_cast<GRawUserData *>(castFromPython(obj.get())->getUserData())->data;
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
		GMethodListUserData * userData = gdynamic_cast<GMethodListUserData *>(castFromPython(obj.get())->getUserData());
		if(outInstance != NULL) {
			*outInstance = userData->methodList->getInstanceAt(0);
		}
		return gdynamic_cast<IMetaMethod *>(userData->methodList->getAt(0));
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
		GMethodListUserData * userData = gdynamic_cast<GMethodListUserData *>(castFromPython(obj.get())->getUserData());
		userData->methodList->addReference();
		return userData->methodList;
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



} // unamed namespace


GScriptObject * createPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config)
{
	return new GPythonScriptObject(service, object, config);
}

IScriptObject * createPythonScriptInterface(IMetaService * service, PyObject * object, const GScriptConfig & config)
{
	return new ImplScriptObject(new GPythonScriptObject(service, object, config), true);
}

} // namespace cpgf


int nnn = 58;
void xxx(int a, const char * b) {
	cout << "XXX a: " << a << " b: " << b << endl;
}

struct Hello
{
	enum En { ena = 2001, enb };

	Hello() : n(0), b(false), s(""), pobj(NULL) {
		cout << "Construct Hello()" << endl;
	}

	explicit Hello(int n) : n(n), b(false), s(""), pobj(NULL) {
		cout << "Construct Hello(int n) " << n << endl;
	}

	~Hello() {
		delete this->pobj;
	}

	int greet(int i) {
		cout << "Hello " << this->n << endl;
		if(this->pobj == NULL) {
			this->pobj = new Hello(78);
		}

		if(i != this->n) {
			cout << "!!! Greet wrong " << endl;
		}

		return 1999;
	}

	int n;
	bool b;
	string s;
	Hello * pobj;
};

template <typename D>
void register_Meta(D define)
{
	define._field("nnn", &nnn);
	define._method("xxx", &xxx);

	GDefineMetaClass<Hello> hello = GDefineMetaClass<Hello>::declare("Hello");
	hello._constructor<void *()>();
	hello._constructor<void *(int)>();
	
	hello._method("greet", &Hello::greet);
	
	hello
		._field("b", &Hello::b)
		._field("n", &Hello::n)
		._field("s", &Hello::s)
		._field("pobj", &Hello::pobj)
	;

	hello._enum<Hello::En>("En")
		._element("ena", Hello::ena)
		._element("enb", Hello::enb)
	;

	define._class(hello);
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

	GScriptBindingParam param(service.get(), GScriptConfig());

	GScopedInterface<IMetaMethod> method;

	method.reset(globalClass->getMethod("xxx"));

	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method.get(), NULL);
	doBindMethodList(&param, mainObject.get(), "xxx", methodList.get());

	GScopedInterface<IMetaClass> metaClass(globalClass->getClass("Hello"));
	doBindClass(&param, mainObject.get(), metaClass->getName(), metaClass.get());

	GScopedInterface<IMetaAccessible> accessible;

	accessible.reset(globalClass->getField("nnn"));
	doBindAccessible(&param, mainObject.get(), accessible->getName(), NULL, accessible.get());

	const char * code = ""
		"xxx(5, 'abc') \n"
		"h = Hello() \n"
		"h = Hello(38) \n"

		"h.s = 'This is a string.' \n"
		"print h.s \n"

		"print h.pobj \n"
		"print h.greet(38) \n"
		"print Hello(99).greet(99) \n"
		"print h.pobj.n \n"

		"print h.En.ena \n"
		"print h.ena \n"

		"print nnn.__get__(0) \n"
		"nnn.__set__(0, 10000) \n"
	;
	PyRun_SimpleString(code);

	cout << nnn << endl;

	Py_Finalize();
}


