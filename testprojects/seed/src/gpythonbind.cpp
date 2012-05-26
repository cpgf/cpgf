#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

#include "python.h"

#include "cpgf/gscopedptr.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gstringmap.h"

#include "pinclude/gbindcommon.h"
#include "pinclude/gscriptbindapiimpl.h"

// test
#include "cpgf/gmetadefine.h"
#include <iostream>
using namespace std;


using namespace cpgf;
using namespace cpgf::bind_internal;

namespace {

template <typename T>
struct GPythonScopedPointerDeleter
{
	static inline void Delete(T * p) {
		Py_DECREF(p);
	}
};

typedef GScopedPointer<PyObject, GPythonScopedPointerDeleter<PyObject> > GPythonScopedPointer;

class GMapItemObjectData : public GMetaMapItemData
{
public:
	GMapItemObjectData() : object(NULL) {
	}

	virtual ~GMapItemObjectData() {
	}

	PyObject * getObject() const {
		return this->object;
	}

	void setObject(PyObject * object) {
		this->object = object;
	}

private:
	PyObject * object;
};

class GPythonBase : public PyObject
{
public:
	explicit GPythonBase(IMetaService * service, GScriptBindingParam * param);
	virtual ~GPythonBase();

	IMetaService * getService() const;
	GScriptBindingParam * getParam() const;

protected:
	void initType(PyTypeObject * type);

private:
	GScopedInterface<IMetaService> service;
	GScriptBindingParam * param;
};

class GPythonMethod : public GPythonBase
{
private:
	typedef GPythonBase super;

public:
	GPythonMethod(IMetaService * service, GScriptBindingParam * param, IMetaList * methodList);

	static PyObject * call(PyObject * callableObject, PyObject * args, PyObject * keyWords);

	IMetaList * getMethodList() const;

private:
	GScopedInterface<IMetaList> methodList;
};

class GPythonObject : public GPythonBase
{
private:
	typedef GPythonBase super;

public:
	GPythonObject(IMetaService * service, GScriptBindingParam * param, void * instance, IMetaClass * metaClass, bool freeObject);
	virtual ~GPythonObject();

	static PyObject * construct(PyObject * callableObject, PyObject * args, PyObject * keyWords);
	
	static PyObject * getAttribute(PyObject * object, PyObject * attrName);
	static int setAttribute(PyObject * object, PyObject * attrName, PyObject * value);

	IMetaClass * getMetaClass() const;
	void * getInstance() const;
	bool isInstance() const;

private:
	PyObject * doGetAttribute(PyObject * attrName);
	int doSetAttribute(PyObject * attrName, PyObject * value);

	PyObject * getAttributeObject(PyObject * attrName);

private:
	void * instance;
	GScopedInterface<IMetaClass> metaClass;
	bool freeObject;
};

template <typename T>
void commonDealloc(PyObject* p)
{
    delete castFromPython<T *>(p);
}

PyTypeObject functionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.function"),
    sizeof(GPythonMethod),
    0,
    (destructor)&commonDealloc<GPythonMethod>,               /* tp_dealloc */
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


PyTypeObject classType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.class"),
    sizeof(GPythonObject),
    0,
    (destructor)&commonDealloc<GPythonObject>,               /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, //(reprfunc)func_repr,                   /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    &GPythonObject::construct,                              /* tp_call */
    0,                                  /* tp_str */
	&GPythonObject::getAttribute,             /* tp_getattro */
    &GPythonObject::setAttribute,            /* tp_setattro */
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
    (destructor)&commonDealloc<GPythonObject>,               /* tp_dealloc */
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
	&GPythonObject::getAttribute,             /* tp_getattro */
    &GPythonObject::setAttribute,            /* tp_setattro */
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
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
		PyObject * param = PyTuple_GetItem(args, i);
		outputParams[i] = pythonToVariant(param).takeData().varData;
	}
}

void loadMethodParamTypes(PyObject * args, GBindDataType * outputTypes)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	for(int i = 0; i < paramCount; ++i) {
//		IMetaTypedItem * typeItem;
//		outputTypes[i].dataType = getV8Type(args[i], &typeItem);
//		outputTypes[i].typeItem.reset(typeItem);
	}
}

void loadCallableParam(PyObject * args, GScriptBindingParam * param, InvokeCallableParam * callableParam)
{
	int paramCount = static_cast<int>(PyTuple_Size(args));

	if(paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}

	callableParam->paramCount = paramCount;
	loadMethodParameters(args, param, callableParam->paramsData);
	loadMethodParamTypes(args, callableParam->paramsType);
}


template <typename T>
T castFromPython(PyObject * object) {
	return gdynamic_cast<T>(static_cast<GPythonBase *>(object));
}



GPythonBase::GPythonBase(IMetaService * service, GScriptBindingParam * param)
	: service(service), param(param)
{
	this->service->addReference();
}

GPythonBase::~GPythonBase()
{
}

IMetaService * GPythonBase::getService() const
{
	return this->service.get();
}

GScriptBindingParam * GPythonBase::getParam() const
{
	return this->param;
}

void GPythonBase::initType(PyTypeObject * type)
{
    if(Py_TYPE(type) == 0) {
        Py_TYPE(type) = &PyType_Type;
        ::PyType_Ready(type);
    }

	PyObject_INIT(this, type);
}

GPythonMethod::GPythonMethod(IMetaService * service, GScriptBindingParam * param, IMetaList * methodList)
	: super(service, param), methodList(methodList)
{
	this->methodList->addReference();

	this->initType(&functionType);
}

IMetaList * GPythonMethod::getMethodList() const
{
	return this->methodList.get();
}

PyObject * GPythonMethod::call(PyObject * callableObject, PyObject * args, PyObject * keyWords)
{
	bool isGlobal = true;

	InvokeCallableParam callableParam;
	loadCallableParam(args, NULL, &callableParam);

	GPythonMethod * methodObject = castFromPython<GPythonMethod *>(callableObject);

	void * instance = NULL;
//	if(userData != NULL) {
//		instance = userData->instance;
//	}

	IMetaList * methodList = methodObject->getMethodList();

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


GPythonObject::GPythonObject(IMetaService * service, GScriptBindingParam * param, void * instance, IMetaClass * metaClass, bool freeObject)
	: super(service, param), instance(instance), metaClass(metaClass), freeObject(freeObject)
{
	this->metaClass->addReference();

	this->initType(this->isInstance() ? &objectType : &classType);
}

GPythonObject::~GPythonObject()
{
	if(this->freeObject) {
		this->metaClass->destroyInstance(this->instance);
	}
}

PyObject * GPythonObject::construct(PyObject * callableObject, PyObject * args, PyObject * keyWords)
{
	InvokeCallableParam callableParam;
	loadCallableParam(args, NULL, &callableParam);

	GPythonObject * cppClass = castFromPython<GPythonObject *>(callableObject);
	
	void * instance = doInvokeConstructor(cppClass->getService(), cppClass->getMetaClass(), &callableParam);

	if(instance != NULL) {
		return new GPythonObject(cppClass->getService(), cppClass->getParam(), instance, cppClass->getMetaClass(), true);
	}
	else {
		raiseCoreException(Error_ScriptBinding_FailConstructObject);
	}

	return NULL;
}

PyObject * GPythonObject::getAttribute(PyObject * object, PyObject * attrName)
{
	GPythonObject * cppObject = castFromPython<GPythonObject *>(object);

	return cppObject->doGetAttribute(attrName);
}

int GPythonObject::setAttribute(PyObject * object, PyObject * attrName, PyObject * value)
{
	GPythonObject * cppObject = castFromPython<GPythonObject *>(object);

	PyObject * attrObject = cppObject->getAttributeObject(attrName);

	return cppObject->doSetAttribute(attrName, value);;
}

PyObject * GPythonObject::doGetAttribute(PyObject * attrName)
{
	PyObject * attrObject = this->getAttributeObject(attrName);
	if(attrObject != NULL) {
		return attrObject;
	}

	return PyInt_FromLong(1978);
}

int GPythonObject::doSetAttribute(PyObject * attrName, PyObject * value)
{
	PyObject * attrObject = this->getAttributeObject(attrName);

	return 1;
}

IMetaClass * GPythonObject::getMetaClass() const
{
	return this->metaClass.get();
}

void * GPythonObject::getInstance() const
{
	return this->instance;
}

bool GPythonObject::isInstance() const
{
	return this->instance != NULL;
}

PyObject * GPythonObject::getAttributeObject(PyObject * attrName)
{
	if(PyObject_HasAttr(this->ob_type->tp_dict, attrName)) {
		return PyObject_GetAttr(this->ob_type->tp_dict, attrName);
	}

	const char * name = PyString_AsString(attrName);

	GMetaClassTraveller traveller(this->getMetaClass(), this->getInstance());

	void * inst = NULL;
	IMetaClass * outDerived;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&inst, &outDerived));
		GScopedInterface<IMetaClass> derived(outDerived);

		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = this->getParam()->getMetaMap()->findClassMap(metaClass.get());
		if(mapClass->getData() == NULL) {
//			continue;
		}
		GMetaMapItem * mapItem = mapClass->findItem(name);
		if(mapItem == NULL) {
			continue;
		}

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {
/*
				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(userData, data.get())) {
					GMetaType type;
					GVariant result = getAccessibleValueAndType(inst, data.get(), &type, userData->cv == opcvConst);
					Handle<Value> v = variantToV8(userData->getParam(), result, type, false, false);
					if(v.IsEmpty()) {
						GScopedInterface<IMetaConverter> converter(metaGetItemExtendType(data, GExtendTypeCreateFlag_Converter).getConverter());
						v = converterToV8(userData->getParam(), result, converter.get());
					}
					if(v.IsEmpty()) {
						v = rawToV8(userData->getParam(), result);
					}
					if(v.IsEmpty()) {
						raiseCoreException(Error_ScriptBinding_FailVariantToScript);
					}
					return v;
				}
*/
			}
			   break;

			case mmitMethod:
			case mmitMethodList: {
				GMapItemObjectData * data = gdynamic_cast<GMapItemObjectData *>(mapItem->getData());
/*
				if(data == NULL) {
					GScopedInterface<IMetaList> methodList(createMetaList());
					loadMethodList(&traveller, methodList.get(), userData->getParam()->getMetaMap(), mapItem, inst, userData, name, true);

					data = new GMapItemMethodData;
					mapItem->setData(data);
					GExtendMethodUserData * newUserData;

					// select the class to bind to the method (i.e, to call the method, an object must be the class or the class' derived)
					// that to ensure Arguments::Holder is correct
					GScopedInterface<IMetaClass> boundClass;
					if(!derived) {
						boundClass.reset(metaClass.get());
						boundClass->addReference();
					}
					else {
						if(derived->getBaseCount() > 0 && derived->getBaseClass(0)) {
							// always choose first base because we only support single inheritance in JS
							boundClass.reset(derived->getBaseClass(0));
						}
						else {
							boundClass.reset(derived.get());
							boundClass->addReference();
						}
					}

					GMetaMapClass * baseMapClass = getMetaClassMap(userData->getParam(), boundClass.get());
					data->setTemplate(createMethodTemplate(userData->getParam(), userData->metaClass, userData->inst == NULL, methodList.get(), name,
						gdynamic_cast<GMapItemClassData *>(baseMapClass->getData())->functionTemplate, udmtInternal, &newUserData));
					newUserData->baseInstance = userData->inst;
					newUserData->name = name;
					data->setUserData(newUserData);
				}

				return data->functionTemplate->GetFunction();
*/
			}

/*
			case mmitEnum:
				if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumTypeViaInstance()) {
					GMapItemEnumData * data = gdynamic_cast<GMapItemEnumData *>(mapItem->getData());
					if(data == NULL) {
						data = new GMapItemEnumData;
						mapItem->setData(data);
						GEnumUserData * newUserData;
						GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
						data->setTemplate(createEnumTemplate(userData->getParam(), metaEnum.get(), name, &newUserData));
						data->setUserData(newUserData);
					}
					Local<Object> obj = data->objectTemplate->NewInstance();
					obj->SetPointerInInternalField(0, data->getUserData());
					setObjectSignature(&obj);
					return obj;
				}
				break;

			case mmitEnumValue:
				if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumValueViaInstance()) {
					GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
					return variantToV8(userData->getParam(), metaGetEnumValue(metaEnum, static_cast<uint32_t>(mapItem->getEnumIndex())), GMetaType(), false, true);
				}
				break;

			case mmitClass:
				if(! userData->isInstance || userData->getParam()->getConfig().allowAccessClassViaInstance()) {
					GScopedInterface<IMetaClass> innerMetaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
					Handle<FunctionTemplate> functionTemplate = createClassTemplate(userData->getParam(), name, innerMetaClass.get());
					return functionTemplate->GetFunction();
				}
				break;
*/
			default:
				break;
		}

	}

	return NULL;
}



}


void doBindMethodList(IMetaService * service, GScriptBindingParam * param, PyObject * owner, const char * name, IMetaList * methodList, GUserDataMethodType methodType)
{
	PyObject * method = new GPythonMethod(service, param, methodList);

	PyObject_SetAttrString(owner, name, method);
}

void doBindClass(IMetaService * service, GScriptBindingParam * param, PyObject * owner, const char * name, IMetaClass * metaClass)
{
	PyObject * pyClass = new GPythonObject(service, param, NULL, metaClass, false);

	PyObject_SetAttrString(owner, name, pyClass);
}

void xxx(int a, const char * b) {
	cout << "XXX a: " << a << " b: " << b << endl;
}

struct Hello
{
	Hello() : n(0) {
		cout << "Construct Hello()" << endl;
	}

	explicit Hello(int n) : n(n) {
		cout << "Construct Hello(int n) " << n << endl;
	}

	void greet() {
		cout << "Hello " << endl;
	}

	int n;
};

template <typename D>
void register_Meta(D define)
{
	define._method("xxx", &xxx);

	GDefineMetaClass<Hello> hello = GDefineMetaClass<Hello>::declare("Hello");
	hello._constructor<void *()>();
	hello._constructor<void *(int)>();
	hello._method("greet", &Hello::greet);

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
	doBindMethodList(service.get(), &param, mainObject.get(), "xxx", methodList.get(), udmtMethod);

	GScopedInterface<IMetaClass> metaClass(globalClass->getClass("Hello"));
	doBindClass(service.get(), &param, mainObject.get(), metaClass->getName(), metaClass.get());

	const char * code = ""
		"xxx(5, 'abc') \n"
		"h = Hello() \n"
		"h = Hello(38) \n"
		"print h.aaa \n"
	;
	PyRun_SimpleString(code);

	Py_Finalize();
}
