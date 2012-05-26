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

template <typename T>
void commonDealloc(PyObject* p)
{
    delete castFromPython<T *>(p);
}

PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * keyWords);

PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * keyWords);

PyObject * callbackGetAttribute(PyObject * object, PyObject * attrName);
int callbackSetAttribute(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackDoGetAttribute(GPythonObject * cppObject, PyObject * attrName);
int callbackDoSetAttribute(GPythonObject * cppObject, PyObject * attrName, PyObject * value);

PyObject * doGetAttributeObject(GPythonObject * cppObject, PyObject * attrName);
bool doSetAttributeObject(GPythonObject * cppObject, PyObject * attrName, PyObject * value);

PyTypeObject functionType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char *>("cpgf.Python.function"),
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


template <typename T>
T castFromPython(PyObject * object) {
	return gdynamic_cast<T>(static_cast<GPythonObject *>(object));
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
        ::PyType_Ready(type);
    }

	PyObject_INIT(this, type);
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

PyObject * rawToPython(GScriptBindingParam * param, const GVariant & value)
{
	GVariantType vt = value.getType();

	//if(param->getConfig().allowAccessRawData() && variantIsRawData(vt)) {
	//	void * userData = lua_newuserdata(L, sizeof(GRawUserData));
	//	new (userData) GRawUserData(param, value);

	//	lua_newtable(L);

	//	setMetaTableSignature(L);
	//	setMetaTableGC(L);

	//	lua_setmetatable(L, -2);

	//	return true;
	//}

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
/*
	if(vtIsInteger(vt)) {
		lua_pushinteger(L, fromVariant<lua_Integer>(value));

		return true;
	}

	if(vtIsReal(vt)) {
		lua_pushnumber(L, fromVariant<lua_Number>(value));

		return true;
	}

	if(!vtIsByteArray(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == NULL) {
		lua_pushnil(L);

		return true;
	}

	if(variantIsString(value)) {
		lua_pushstring(L, fromVariant<char *>(value));

		return true;
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
				objectToLua(L, param, instance, gdynamic_cast<IMetaClass *>(typedItem.get()), true, metaTypeToCV(type), cudtNormal);

				return true;
			}

			if(type.getPointerDimension() == 1 || isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

				if(vtIsByteArray(vt)) {
					GScopedInterface<IByteArray> ba(value.data.valueByteArray); // free the byte array after return
					objectToLua(L, param, value.data.valueByteArray, gdynamic_cast<IMetaClass *>(typedItem.get()),
						allowGC, metaTypeToCV(type), cudtByteArray);
				}
				else {
					objectToLua(L, param, fromVariant<void *>(value), gdynamic_cast<IMetaClass *>(typedItem.get()),
						allowGC, metaTypeToCV(type), cudtNormal);
				}

				return true;
			}
		}

		if(bind_internal::shouldRemoveReference(type)) {
			GMetaType newType(type);
			newType.removeReference();

			return variantToLua(L, param, value, newType, allowGC, allowRaw);
		}
	}
*/
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

	//if(isMetaConverterCanRead(converter->capabilityForCString())) {
	//	gapi_bool needFree;
	//	
	//	GScopedInterface<IMemoryAllocator> allocator(param->getService()->getAllocator());
	//	const char * s = converter->readCString(objectAddressFromVariant(value), &needFree, allocator.get());

	//	if(s != NULL) {
	//		lua_pushstring(L, s);

	//		if(needFree) {
	//			allocator->free((void *)s);
	//		}

	//		return true;
	//	}
	//}

	return NULL;
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


PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * keyWords)
{
	InvokeCallableParam callableParam;
	loadCallableParam(args, NULL, &callableParam);

	GPythonObject * methodObject = castFromPython<GPythonObject *>(callableObject);

	GMethodListUserData * userData = gdynamic_cast<GMethodListUserData *>(methodObject->getUserData());
	IMetaList * methodList = userData->methodList;

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
//		return methodResultToV8(namedUserData->getParam(), callable.get(), &result);
	}

//	raiseCoreException(Error_ScriptBinding_CantFindMatchedMethod);

	return Py_None; //Py_BuildValue("");
}


PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * keyWords)
{
	InvokeCallableParam callableParam;
	loadCallableParam(args, NULL, &callableParam);

	GPythonObject * cppClass = castFromPython<GPythonObject *>(callableObject);
	GClassUserData * userData = gdynamic_cast<GClassUserData *>(cppClass->getUserData());
	
	void * instance = doInvokeConstructor(cppClass->getService(), userData->metaClass, &callableParam);

	if(instance != NULL) {
		return new GPythonObject(new GClassUserData(cppClass->getParam(), userData->metaClass, instance, true, true, opcvNone, cudtNormal));
	}
	else {
		raiseCoreException(Error_ScriptBinding_FailConstructObject);
	}

	return NULL;
}

PyObject * callbackGetAttribute(PyObject * object, PyObject * attrName)
{
	GPythonObject * cppObject = castFromPython<GPythonObject *>(object);

	return callbackDoGetAttribute(cppObject, attrName);
}

int callbackSetAttribute(PyObject * object, PyObject * attrName, PyObject * value)
{
	GPythonObject * cppObject = castFromPython<GPythonObject *>(object);

	return callbackDoSetAttribute(cppObject, attrName, value);
}

PyObject * callbackDoGetAttribute(GPythonObject * cppObject, PyObject * attrName)
{
	PyObject * attrObject = doGetAttributeObject(cppObject, attrName);
	if(attrObject != NULL) {
		return attrObject;
	}

	return Py_None;
}

int callbackDoSetAttribute(GPythonObject * cppObject, PyObject * attrName, PyObject * value)
{
	return doSetAttributeObject(cppObject, attrName, value) ? 0 : -1;
}

bool doSetAttributeObject(GPythonObject * cppObject, PyObject * attrName, PyObject * value)
{
	const char * name = PyString_AsString(attrName);

	GClassUserData * userData = gdynamic_cast<GClassUserData *>(cppObject->getUserData());
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
					GVariant v = pythonToVariant(value).getValue();
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


void doBindMethodList(IMetaService * service, GScriptBindingParam * param, PyObject * owner, const char * name, IMetaList * methodList)
{
	PyObject * method = new GPythonObject(new GMethodListUserData(param, methodList, udmtMethodList));

	PyObject_SetAttrString(owner, name, method);
}

void doBindClass(IMetaService * service, GScriptBindingParam * param, PyObject * owner, const char * name, IMetaClass * metaClass)
{
	PyObject * pyClass = new GPythonObject(new GClassUserData(param, metaClass, NULL, false, false, opcvNone, cudtNormal));

	PyObject_SetAttrString(owner, name, pyClass);
}

void xxx(int a, const char * b) {
	cout << "XXX a: " << a << " b: " << b << endl;
}

struct Hello
{
	Hello() : n(0), b(false) {
		cout << "Construct Hello()" << endl;
	}

	explicit Hello(int n) : n(n), b(false) {
		cout << "Construct Hello(int n) " << n << endl;
	}

	void greet() {
		cout << "Hello " << this->n << endl;
	}

	int n;
	bool b;
};

template <typename D>
void register_Meta(D define)
{
	define._method("xxx", &xxx);

	GDefineMetaClass<Hello> hello = GDefineMetaClass<Hello>::declare("Hello");
	hello._constructor<void *()>();
	hello._constructor<void *(int)>();
	
	hello._method("greet", &Hello::greet);
	
	hello._field("b", &Hello::b);

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
	doBindMethodList(service.get(), &param, mainObject.get(), "xxx", methodList.get());

	GScopedInterface<IMetaClass> metaClass(globalClass->getClass("Hello"));
	doBindClass(service.get(), &param, mainObject.get(), metaClass->getName(), metaClass.get());

	const char * code = ""
		"xxx(5, 'abc') \n"
		"h = Hello() \n"
		"h = Hello(38) \n"
		"print h.b \n"
		"h.b = True \n"
		"print h.b \n"
		"h.greet() \n"
	;
	PyRun_SimpleString(code);

	Py_Finalize();
}
