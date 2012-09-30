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
#include "../pinclude/gstaticuninitializerorders.h"


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
	explicit GPythonObject(const GGlueDataPointer & glueData);
	virtual ~GPythonObject();

	IMetaService * getService() const;
	GContextPointer getContext() const;
	virtual GGlueDataPointer getData();

	template <typename T>
	GSharedPointer<T> getDataAs() {
		return sharedStaticCast<T>(this->getData());
	}

protected:
	void initType(PyTypeObject * type);

private:
	GGlueDataPointer glueData;
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

	virtual GVariant invoke(const char * name, const GVariant * params, size_t paramCount);
	virtual GVariant invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);
	virtual bool valueIsNull(const char * name);
	virtual void nullifyValue(const char * name);

	virtual void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible);

	virtual void bindCoreService(const char * name);

public:
	PyObject * getObject() const {
		return this->object;
	}

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

PyObject * doGetAttributeObject(GPythonObject * cppObject, PyObject * attrName);

PyObject * callbackGetEnumValue(PyObject * object, PyObject * attrName);
int callbackSetEnumValue(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackAccessibleDescriptorGet(PyObject * self, PyObject * obj, PyObject * type);
int callbackAccessibleDescriptorSet(PyObject * self, PyObject * obj, PyObject * value);

PyObject * variantToPython(const GContextPointer & context, const GVariant & data, bool allowGC, bool allowRaw);

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
			*typeItem = castFromPython(value)->getDataAs<GClassGlueData>()->getMetaClass();
			(*typeItem)->addReference();
		}
		return sdtClass;
	}

	if(value->ob_type == &objectType) {
		if(typeItem != NULL) {
			*typeItem = castFromPython(value)->getDataAs<GObjectGlueData>()->getClassData()->getMetaClass();
			(*typeItem)->addReference();
		}
		return sdtObject;
	}

	if(value->ob_type == &enumType) {
		if(typeItem != NULL) {
			*typeItem = castFromPython(value)->getDataAs<GEnumGlueData>()->getMetaEnum();
			(*typeItem)->addReference();
		}
		return sdtEnum;
	}

	if(value->ob_type == &rawType) {
		return sdtRaw;
	}

	if(value->ob_type == &functionType) {
		return methodTypeToGlueDataType(castFromPython(value)->getDataAs<GObjectAndMethodGlueData>()->getMethodData()->getMethodType());
	}

	// should be the last
	if(PyCallable_Check(value)) {
		return sdtScriptMethod;
	}

	return sdtScriptObject;
}

GVariant pythonToVariant(const GContextPointer & context, PyObject * value)
{
	if(value == NULL) {
		return GVariant();
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
		return createTypedVariant(createStringVariant(PyString_AsString(value)), createMetaType<char *>());
	}
	else {
		GPythonObject * object = tryCastFromPython(value);
		if(object != NULL) {
			return glueDataToVariant(object->getData());
		}

		if(PyCallable_Check(value)) {
			GScopedInterface<IScriptFunction> func(new ImplScriptFunction(new GPythonScriptFunction(context, value), true));
			
			return GVariant(func.get());
		}

		GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GPythonScriptObject(context->getService(), value, context->getConfig()), true));

		return GVariant(scriptObject.get());
	}

	return GVariant();
}

PyObject * objectToPython(const GContextPointer & context, const GClassGlueDataPointer & classData, void * instance, bool allowGC, ObjectPointerCV cv, ObjectGlueDataType dataType)
{
	if(instance == NULL) {
		return pyAddRef(Py_None);
	}

	return createPythonObject(context->newObjectGlueData(classData, instance, allowGC, cv, dataType));
}

PyObject * rawToPython(const GContextPointer & context, const GVariant & value)
{
	if(context->getConfig().allowAccessRawData()) {
		PyObject * rawObject = createPythonObject(context->newRawGlueData(value));

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
	
	static ResultType doObjectToScript(const GContextPointer & context, const GClassGlueDataPointer & classData, void * instance, bool allowGC, ObjectPointerCV cv, ObjectGlueDataType dataType)
	{
		return objectToPython(context, classData, instance, allowGC, cv, dataType);
	}

	static ResultType doVariantToScript(const GContextPointer & context, const GVariant & value, bool allowGC, bool allowRaw)
	{
		return variantToPython(context, value, allowGC, allowRaw);
	}
	
	static ResultType doRawToScript(const GContextPointer & context, const GVariant & value)
	{
		return rawToPython(context, value);
	}

	static ResultType doConverterToScript(const GContextPointer & context, const GVariant & value, IMetaConverter * converter)
	{
		return converterToScript<GPythonMethods>(context, value, converter);
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

	static ResultType doMethodsToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem,
		const char * methodName, GMetaClassTraveller * /*traveller*/,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getUserData());
		GContextPointer context = classData->getContext();
		if(data == NULL) {
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));

			data = new GMapItemMethodData(context->newMethodGlueData(context->getClassData(boundClass.get()), NULL, methodName, gdmtInternal));

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

PyObject * variantToPython(const GContextPointer & context, const GVariant & data, bool allowGC, bool allowRaw)
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

	return variantToScript<GPythonMethods>(context, value, type, allowGC, allowRaw);
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

void loadMethodParameters(const GContextPointer & context, PyObject * args, GVariant * outputParams)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
		PyObject * c = PyTuple_GetItem(args, i);
		outputParams[i] = getVariantRealValue(pythonToVariant(context, c));
	}
}

void loadMethodParamTypes(PyObject * args, CallableParamDataType * outputTypes)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
		IMetaTypedItem * typeItem;
		PyObject * context = PyTuple_GetItem(args, i);
		outputTypes[i].dataType = getPythonType(context, &typeItem);
		outputTypes[i].typeItem.reset(typeItem);
	}
}

void loadCallableParam(const GContextPointer & context, PyObject * args, InvokeCallableParam * callableParam)
{
	loadMethodParameters(context, args, callableParam->paramsData);
	loadMethodParamTypes(args, callableParam->paramsType);
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

	void * instance = doInvokeConstructor(cppClass->getService(), classUserData->getMetaClass(), &callableParam);

	if(instance != NULL) {
		return createPythonObject(context->newObjectGlueData(classUserData, instance, true, opcvNone, ogdtNormal));
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
	GGlueDataPointer userData = cppObject->getDataAs<GGlueData>();
	const char * name = PyString_AsString(attrName);

	if(doSetFieldValue(userData, name, pythonToVariant(userData->getContext(), value))) {
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
		return variantToPython(userData->getContext(), metaGetEnumValue(userData->getMetaEnum(), index), true, false);
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

	return accessibleToScript<GPythonMethods>(userData->getContext(), userData->getAccessible(), userData->getInstance(), false);

	LEAVE_PYTHON(return NULL)
}

int callbackAccessibleDescriptorSet(PyObject * self, PyObject * /*obj*/, PyObject * value)
{
	ENTER_PYTHON()

	GPythonObject * cppObject = castFromPython(self);
	
	GAccessibleGlueDataPointer userData = cppObject->getDataAs<GAccessibleGlueData>();

	GVariant v = pythonToVariant(userData->getContext(), value);
	metaSetValue(userData->getAccessible(), userData->getInstance(), v);

	return 0;

	LEAVE_PYTHON(return 0)
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
			GPythonScopedPointer arg(variantToPython(context, *params[i], false, true));
			if(!arg) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
			PyTuple_SetItem(args.get(), start + i, arg.take());
		}

		GPythonScopedPointer result;

		result.reset(PyObject_Call(func, args.get(), NULL));

		return pythonToVariant(context, result.get());
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
		return (strcmp(signatureValue, PyString_AsString(signature.get())) == 0);
	}
	else {
		return false;
	}
}

void doBindMethodList(const GContextPointer & context, PyObject * owner, const char * name, IMetaList * methodList, GGlueDataMethodType methodType)
{
	GMethodGlueDataPointer data = context->newMethodGlueData(GClassGlueDataPointer(), methodList, name, methodType);
	GObjectAndMethodGlueDataPointer methodData = context->newObjectAndMethodGlueData(GObjectGlueDataPointer(), data);
	PyObject * methodObject = createPythonObject(methodData);

	setObjectAttr(owner, name, methodObject);
}

void doBindClass(const GContextPointer & context, PyObject * owner, const char * name, IMetaClass * metaClass)
{
	PyObject * classObject = createClassObject(context, metaClass);

	setObjectAttr(owner, name, classObject);
}

void doBindEnum(const GContextPointer & context, PyObject * owner, const char * name, IMetaEnum * metaEnum)
{
	PyObject * enumObject = createPythonObject(context->newEnumGlueData(metaEnum));

	setObjectAttr(owner, name, enumObject);
}

void doBindAccessible(const GContextPointer & context, PyObject * owner, const char * name, void * instance, IMetaAccessible * accessible)
{
	PyObject * accessibleObject = createPythonObject(context->newAccessibleGlueData(instance, accessible));

	setObjectAttr(owner, name, accessibleObject);
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
	ENTER_PYTHON()

	return invokePythonFunctionIndirectly(this->getContext(), NULL, this->func, params, paramCount, "");

	LEAVE_PYTHON(return GVariant())
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

	doBindClass(this->getContext(), this->object, name, metaClass);

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	ENTER_PYTHON()

	doBindEnum(this->getContext(), this->object, name, metaEnum);

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
	GPythonScriptObject * newScriptObject = new GPythonScriptObject(*this, dict);
	newScriptObject->setOwner(this);
	newScriptObject->setName(name);
	return newScriptObject;

	LEAVE_PYTHON(return NULL)
}

GScriptObject * GPythonScriptObject::gainScriptObject(const char * name)
{
	ENTER_PYTHON()

	PyObject * attr = getObjectAttr(this->object, name);
	if(attr != NULL) {
		GPythonScriptObject * newScriptObject = new GPythonScriptObject(*this, attr);
		newScriptObject->setOwner(this);
		newScriptObject->setName(name);
		return newScriptObject;
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
			return new GPythonScriptFunction(this->getContext(), func.take());
		}
	}

	return NULL;

	LEAVE_PYTHON(return NULL)
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
	ENTER_PYTHON()

	GPythonScopedPointer func(getObjectAttr(this->object, name));
	return invokePythonFunctionIndirectly(this->getContext(), NULL, func.get(), params, paramCount, name);

	LEAVE_PYTHON(return GVariant())
}

void GPythonScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.refData().typeData)), "Only fundamental value can be bound via bindFundamental");

	ENTER_PYTHON()

	setObjectAttr(this->object, name, variantToPython(this->getContext(), value, false, true));

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	ENTER_PYTHON()

	doBindAccessible(this->getContext(), this->object, name, instance, accessible);

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

	setObjectAttr(this->object, objectName, objectToPython(this->getContext(), this->getContext()->getClassData(type), instance, transferOwnership, opcvNone, ogdtNormal));

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindRaw(const char * name, const GVariant & value)
{
	ENTER_PYTHON()

	setObjectAttr(this->object, name, rawToPython(this->getContext(), value));

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

	doBindMethodList(this->getContext(), this->object, name, methodList.get(), gdmtMethod);

	LEAVE_PYTHON()
}

void GPythonScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	ENTER_PYTHON()

	doBindMethodList(this->getContext(), this->object, name, methodList, gdmtMethodList);

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
		return pythonToVariant(this->getContext(), obj.get());
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
		return castFromPython(obj.get())->getDataAs<GObjectGlueData>()->getInstance();
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
		return castFromPython(obj.get())->getDataAs<GRawGlueData>()->getData();
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
		GObjectAndMethodGlueDataPointer userData = castFromPython(obj.get())->getDataAs<GObjectAndMethodGlueData>();
		if(outInstance != NULL) {
			*outInstance = userData->getMethodData()->getMethodList()->getInstanceAt(0);
		}
		return gdynamic_cast<IMetaMethod *>(userData->getMethodData()->getMethodList()->getAt(0));
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
		GObjectAndMethodGlueDataPointer userData = castFromPython(obj.get())->getDataAs<GObjectAndMethodGlueData>();
		userData->getMethodData()->getMethodList()->addReference();
		return userData->getMethodData()->getMethodList();
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

	this->getContext()->bindScriptCoreService(this, name);

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


G_GUARD_LIBRARY_LIFE


} // namespace cpgf


