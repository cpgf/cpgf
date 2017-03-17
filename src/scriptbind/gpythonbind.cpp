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
	static GGlueDataWrapperPool * pythonDataWrapperPool = nullptr;
	if(pythonDataWrapperPool == nullptr && isLibraryLive()) {
		pythonDataWrapperPool = new GGlueDataWrapperPool();
		addOrderedStaticUninitializer(suo_ScriptDataWrapperPool, makeUninitializerDeleter(&pythonDataWrapperPool));
	}

	return pythonDataWrapperPool;
}

class GPythonObject;

class GPythonNative : public GGlueDataWrapper
{
public:
	GPythonNative() {}
	explicit GPythonNative(const GGlueDataPointer & glueData);
	virtual ~GPythonNative();

	virtual GGlueDataPointer getData() const;
	virtual PyObject * toPythonObject() = 0;

private:
	GGlueDataPointer glueData;
};

class GPythonObject : public PyObject, public GPythonNative
{
public:
	GPythonObject() {}
	explicit GPythonObject(const GGlueDataPointer & glueData);
	virtual ~GPythonObject();

	virtual PyObject * toPythonObject();

	void initType(PyTypeObject * type);
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

class GPythonStaticObject : public GPythonObject
{
public:
	GPythonStaticObject(const GGlueDataPointer & glueData, const string & fieldName);
	virtual ~GPythonStaticObject();

	const string & getFieldName() const;

private:
	void initType();

private:
	string fieldName;
};

class GPythonClass : public PyTypeObject, public GPythonNative
{
public:
	GPythonClass() {}
	explicit GPythonClass(const GGlueDataPointer & glueData);
	virtual ~GPythonClass();

	virtual PyObject * toPythonObject();

private:
	void initType();
};

class PythonScriptObjectCacheData : public GScriptObjectCacheData {
public:
	GPythonNative * pythonObject;
	PythonScriptObjectCacheData(GPythonNative *pythonObject) : pythonObject(pythonObject) {}
};

class GPythonBindingContext : public GBindingContext, public GShareFromBase
{
private:
	typedef GBindingContext super;

public:
	GPythonBindingContext(IMetaService * service, const GScriptConfig & config)
		: super(service, config)
	{
	}
};

typedef GSharedPointer<GPythonBindingContext> GPythonContextPointer;

class GPythonScriptFunction : public GScriptFunctionBase
{
private:
	typedef GScriptFunctionBase super;

public:
	GPythonScriptFunction(const GContextPointer & context, PyObject * func);
	virtual ~GPythonScriptFunction();

	virtual GScriptValue invoke(const GVariant * params, size_t paramCount);
	virtual GScriptValue invokeIndirectly(GVariant const * const * params, size_t paramCount);

private:
	PyObject * func;
};

class GPythonScriptArray : public GScriptArrayBase
{
private:
	typedef GScriptArrayBase super;

public:
	GPythonScriptArray(const GContextPointer & context, PyObject * listObject);
	virtual ~GPythonScriptArray();

	virtual size_t getLength();
	virtual GScriptValue getValue(size_t index);
	virtual void setValue(size_t index, const GScriptValue & value);

	virtual bool maybeIsScriptArray(size_t index);
	virtual GScriptValue getAsScriptArray(size_t index);
	virtual GScriptValue createScriptArray(size_t index);

private:
	PyObject * listObject;
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

	virtual GScriptValue invoke(const char * name, const GVariant * params, size_t paramCount);
	virtual GScriptValue invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);

	virtual bool maybeIsScriptArray(const char * name);
	virtual GScriptValue getAsScriptArray(const char * name);
	virtual GScriptValue createScriptArray(const char * name);

	GPythonContextPointer getPythonContext() const {
		return sharedStaticCast<GPythonBindingContext>(this->getBindingContext());
	}

public:
	PyObject * getObject() const {
		return this->object;
	}

protected:
	virtual GScriptValue doGetValue(const char * name);
	virtual void doSetValue(const char * name, const GScriptValue & value);

private:
	GPythonScriptObject(const GPythonScriptObject & other, PyObject * object);

private:
	PyObject * object;
};


GPythonObject * createPythonObject(const GGlueDataPointer & glueData);
void deletePythonObject(GPythonNative * object);
PyObject * createClassObject(const GContextPointer & context, IMetaClass * metaClass);

const char * metaClassTypeName = "_cpgf.Python.metaclass";

bool pythonObjectIsNativeClass(PyObject * object) {
	if(PyType_Check(object)) {
		PyTypeObject * type = (PyTypeObject *)object;
		return type->tp_name[0] == metaClassTypeName[0]
			&& type->tp_name[1] == metaClassTypeName[1]
			&& type->tp_name[2] == metaClassTypeName[2]
			&& strcmp(type->tp_name, metaClassTypeName) == 0
		;
	}
	else {
		return false;
	}
}

GPythonNative * nativeFromPython(PyObject * object) {
	if(pythonObjectIsNativeClass(object)) {
		return static_cast<GPythonNative *>(static_cast<GPythonClass *>((PyTypeObject *)object));
	}
	else {
		return static_cast<GPythonNative *>(static_cast<GPythonObject *>(object));
	}
}

void commonDealloc(PyObject* p)
{
	deletePythonObject(nativeFromPython(p));
}

PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * keyWords);

PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * keyWords);

PyObject * callbackGetAttribute(PyObject * object, PyObject * attrName);
int callbackSetAttribute(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackAnyObjectGetAttribute(PyObject * object, PyObject * attrName);
int callbackAnyObjectSetAttribute(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackGetEnumValue(PyObject * object, PyObject * attrName);
int callbackSetEnumValue(PyObject * object, PyObject * attrName, PyObject * value);

PyObject * callbackAccessibleDescriptorGet(PyObject * self, PyObject * obj, PyObject * type);
int callbackAccessibleDescriptorSet(PyObject * self, PyObject * obj, PyObject * value);

PyObject * callbackStaticObjectDescriptorGet(PyObject * self, PyObject * obj, PyObject * type);
int callbackStaticObjectDescriptorSet(PyObject * self, PyObject * obj, PyObject * value);

PyObject * variantToPython(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData);

PyTypeObject functionType = {
	PyVarObject_HEAD_INIT(nullptr, 0)
	const_cast<char *>("cpgf.Python.function"),
	sizeof(GPythonObject),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0,					   /* tp_repr */
	0,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	&callbackCallMethod,							  /* tp_call */
	0,								  /* tp_str */
	0, 				/* tp_getattro */
	0, 				/* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 					/* tp_flags */
	0,								  /* tp_doc */
	0, 			  /* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 									 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	0,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	0,								  /* tp_base */
	0,								  /* tp_dict */
	nullptr, 					 /* tp_descr_get */
	0,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,									  /* tp_init */
	0,									  /* tp_alloc */
	0,									  /* tp_new */
	0,									  /* tp_free */
	0,									  /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
};

int type_is_gc(PyTypeObject * /*python_type*/)
{
  return 0; //python_type->tp_flags & Py_TPFLAGS_HEAPTYPE;
}

PyTypeObject metaClassType = {
	PyVarObject_HEAD_INIT(&PyType_Type, 0)
	const_cast<char *>(metaClassTypeName),
	sizeof(GPythonClass),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0, 					   /* tp_repr */
	0,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	0,							  /* tp_call */
	0,								  /* tp_str */
	0,			 /* tp_getattro */
	0,			/* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	0,								  /* tp_doc */
	0, 			  /* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 									 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	0,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	0,					   /* tp_base */
	0,								  /* tp_dict */
	nullptr, 					 /* tp_descr_get */
	0,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,							   /* tp_init */
	0,					/* tp_alloc */
	0, // &callbackConstructObject2,					 /* tp_new */
	0,									  /* tp_free */
	(inquiry)type_is_gc,						 /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
};

PyTypeObject classType = {
	PyVarObject_HEAD_INIT(&PyType_Type, 0)
	const_cast<char *>("cpgf.Python.class"),
	sizeof(GPythonObject),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0, 					   /* tp_repr */
	0,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	&callbackConstructObject,							  /* tp_call */
	0,								  /* tp_str */
	&callbackGetAttribute,			 /* tp_getattro */
	&callbackSetAttribute,			/* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,	/* tp_flags */
	0,								  /* tp_doc */
	0, 			  /* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 									 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	0,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	&PyBaseObject_Type,					   /* tp_base */
	0,								  /* tp_dict */
	nullptr, 					 /* tp_descr_get */
	0,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,							   /* tp_init */
	0,					/* tp_alloc */
	&PyType_GenericNew,					 /* tp_new */
	0,									  /* tp_free */
	0,						 /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
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
	PyVarObject_HEAD_INIT(nullptr, 0)
	const_cast<char *>("cpgf.Python.object"),
	sizeof(GPythonObject),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0, 					   /* tp_repr */
	&numberMethods,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	0,							  /* tp_call */
	0,								  /* tp_str */
	&callbackGetAttribute,			 /* tp_getattro */
	&callbackSetAttribute,			/* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_CHECKTYPES,	/* tp_flags */
	0,								  /* tp_doc */
	0, 			  /* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 									 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	0,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	0,								  /* tp_base */
	0,								  /* tp_dict */
	nullptr, 					 /* tp_descr_get */
	0,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,									  /* tp_init */
	0,									  /* tp_alloc */
	0,									  /* tp_new */
	0,									  /* tp_free */
	0,									  /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
};


PyTypeObject staticObjectType = {
	PyVarObject_HEAD_INIT(nullptr, 0)
	const_cast<char *>("cpgf.Python.static"),
	sizeof(GPythonStaticObject),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0, 					   /* tp_repr */
	0,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	0,							  /* tp_call */
	0,								  /* tp_str */
	nullptr,		   			  /* tp_getattro */
	nullptr,						/* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_CLASS, 	/* tp_flags */
	0,								  /* tp_doc */
	0, 			  /* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 									 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	nullptr,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	0,								  /* tp_base */
	0,								  /* tp_dict */
	&callbackStaticObjectDescriptorGet,		   /* tp_descr_get */
	&callbackStaticObjectDescriptorSet,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,									  /* tp_init */
	0,									  /* tp_alloc */
	0,									  /* tp_new */
	0,									  /* tp_free */
	0,									  /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
};


PyTypeObject enumType = {
	PyVarObject_HEAD_INIT(nullptr, 0)
	const_cast<char *>("cpgf.Python.enum"),
	sizeof(GPythonObject),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0, 					   /* tp_repr */
	0,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	0,							  /* tp_call */
	0,								  /* tp_str */
	&callbackGetEnumValue,			 /* tp_getattro */
	&callbackSetEnumValue,			/* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			/* tp_flags */
	0,								  /* tp_doc */
	0, 			  			/* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 								 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	0,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	0,								  /* tp_base */
	0,								  /* tp_dict */
	nullptr, 					 /* tp_descr_get */
	0,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,									  /* tp_init */
	0,									  /* tp_alloc */
	0,									  /* tp_new */
	0,									  /* tp_free */
	0,									  /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
};


PyTypeObject accessibleType = {
	PyVarObject_HEAD_INIT(nullptr, 0)
	const_cast<char *>("cpgf.Python.accessible"),
	sizeof(GPythonObject),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0, 					   /* tp_repr */
	0,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	0,							  /* tp_call */
	0,								  /* tp_str */
	nullptr,		   			  /* tp_getattro */
	nullptr,						/* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_CLASS, 	/* tp_flags */
	0,								  /* tp_doc */
	0, 			  /* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 									 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	nullptr,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	0,								  /* tp_base */
	0,								  /* tp_dict */
	&callbackAccessibleDescriptorGet,		   /* tp_descr_get */
	&callbackAccessibleDescriptorSet,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,									  /* tp_init */
	0,									  /* tp_alloc */
	0,									  /* tp_new */
	0,									  /* tp_free */
	0,									  /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
};


PyTypeObject rawType = {
	PyVarObject_HEAD_INIT(nullptr, 0)
	const_cast<char *>("cpgf.Python.raw"),
	sizeof(GPythonObject),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0, 					   /* tp_repr */
	0,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	0,							  /* tp_call */
	0,								  /* tp_str */
	nullptr,		   				  /* tp_getattro */
	nullptr,		 				   /* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 				/* tp_flags */
	0,								  /* tp_doc */
	0, 			  /* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 									 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	nullptr,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	0,								  /* tp_base */
	0,								  /* tp_dict */
	nullptr,		   						/* tp_descr_get */
	nullptr,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,									  /* tp_init */
	0,									  /* tp_alloc */
	0,									  /* tp_new */
	0,									  /* tp_free */
	0,									  /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
};


PyTypeObject emptyObjectType = {
	PyVarObject_HEAD_INIT(nullptr, 0)
	const_cast<char *>("cpgf.Python.emptyObject"),
	sizeof(GPythonAnyObject),
	0,
	(destructor)&commonDealloc,			   /* tp_dealloc */
	0,								  /* tp_print */
	0,								  /* tp_getattr */
	0,								  /* tp_setattr */
	0,								  /* tp_compare */
	0, 					   /* tp_repr */
	0,								  /* tp_as_number */
	0,								  /* tp_as_sequence */
	0,								  /* tp_as_mapping */
	0,								  /* tp_hash */
	0,							  /* tp_call */
	0,								  /* tp_str */
	&callbackAnyObjectGetAttribute,			 /* tp_getattro */
	&callbackAnyObjectSetAttribute,			/* tp_setattro */
	0,								  /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 				/* tp_flags */
	0,								  /* tp_doc */
	0, 			  /* tp_traverse */
	0,								  /* tp_clear */
	0,								  /* tp_richcompare */
	0, 									 /* tp_weaklistoffset */
	0,								  /* tp_iter */
	0,								  /* tp_iternext */
	0,								  /* tp_methods */
	0, 				  /* tp_members */
	nullptr, 					/* tp_getset */
	nullptr,								  /* tp_base */
	0,								  /* tp_dict */
	nullptr, 					 /* tp_descr_get */
	0,								  /* tp_descr_set */
	0, 		  /* tp_dictoffset */
	0,									  /* tp_init */
	0,									  /* tp_alloc */
	0,									  /* tp_new */
	0,									  /* tp_free */
	0,									  /* tp_is_gc */
	0,									  /* tp_bases */
	0,									  /* tp_mro */
	0,									  /* tp_cache */
	0,									  /* tp_subclasses */
	0,									  /* tp_weaklist */
	0,									  /* tp_del */
	0									   /* tp_version_tag */
};


PyTypeObject * const typeObjects[] = {
	&functionType, &classType, &objectType, &enumType, &accessibleType, &rawType
};

const int typeObjectCount = sizeof(typeObjects) / sizeof(typeObjects[0]);


GPythonNative * tryCastFromPython(PyObject * object) {
	for(int i = 0; i <typeObjectCount; ++i) {
		if(object->ob_type == typeObjects[i]) {
			return nativeFromPython(object);
		}
	}
	if(pythonObjectIsNativeClass(object)) {
//		return nativeFromPython(object);
		return static_cast<GPythonNative *>(static_cast<GPythonClass *>((PyTypeObject *)object));
	}

	return nullptr;
}

PyTypeObject * getTypeObject(const GGlueDataPointer & glueData)
{
	PyTypeObject * typeObject = nullptr;

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

	GASSERT(typeObject != nullptr);

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

void deletePythonObject(GPythonNative * object)
{
	if (object->getData()->isValid()) {
		object->getData()->getBindingContext()->getScriptObjectCache()->freeScriptObject(object);
	}
	getPythonDataWrapperPool()->dataWrapperDestroyed(object);
	delete object;
}

GPythonNative::GPythonNative(const GGlueDataPointer & glueData)
	: glueData(glueData)
{
}

GPythonNative::~GPythonNative()
{
}

GGlueDataPointer GPythonNative::getData() const
{
	return this->glueData;
}


GPythonObject::GPythonObject(const GGlueDataPointer & glueData)
	: GPythonNative(glueData)
{
	PyTypeObject * typeObject = getTypeObject(glueData);

	GASSERT(typeObject != nullptr);

	this->initType(typeObject);
}

GPythonObject::~GPythonObject()
{
}

void GPythonObject::initType(PyTypeObject * type)
{
	if(type->tp_dict == 0) {
		Py_TYPE(type) = &PyType_Type;
		if(type == &classType) {
			type->tp_base = &PyBaseObject_Type; //&PyType_Type;
		}
		PyType_Ready(type);
	}

	PyObject_INIT(this, type);
}

PyObject * GPythonObject::toPythonObject()
{
	return this;
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


GPythonStaticObject::GPythonStaticObject(const GGlueDataPointer & glueData, const string & fieldName)
	: GPythonObject(glueData), fieldName(fieldName)
{
	this->initType();
}

GPythonStaticObject::~GPythonStaticObject()
{
}

const string & GPythonStaticObject::getFieldName() const
{
	return this->fieldName;
}

void GPythonStaticObject::initType()
{
	PyTypeObject * type = &staticObjectType;
	if(type->tp_dict == 0) {
		Py_TYPE(type) = &PyType_Type;
		PyType_Ready(type);
	}

	PyObject_INIT(this, type);
}


GPythonClass::GPythonClass(const GGlueDataPointer & glueData)
	: GPythonNative(glueData)
{
	this->initType();
}

GPythonClass::~GPythonClass()
{
}

PyObject * GPythonClass::toPythonObject()
{
	return (PyObject *)static_cast<PyTypeObject *>(this);
}

void bindClassItems(PyObject * dict, const GClassGlueDataPointer & classGlueData)
{
	IMetaClass * metaClass = classGlueData->getMetaClass();
	GScopedInterface<IMetaItem> item;
	uint32_t count = metaClass->getMetaCount();
	for(uint32_t i = 0; i < count; ++i) {
		item.reset(metaClass->getMetaAt(i));
		if(item->isStatic()) {
			PyObject * staticObject;
			if(metaIsClass(item->getCategory())) {
				staticObject = createClassObject(classGlueData->getBindingContext(), static_cast<IMetaClass *>(item.get()));
			}
			else {
				staticObject = new GPythonStaticObject(classGlueData, item->getName());
			}
			PyDict_SetItemString(dict, item->getName(), staticObject);

			if(metaIsEnum(item->getCategory())) {
				IMetaEnum * metaEnum = gdynamic_cast<IMetaEnum *>(item.get());
				uint32_t keyCount = metaEnum->getCount();
				for(uint32_t k = 0; k < keyCount; ++k) {
					const char * name = metaEnum->getKey(k);
					PyObject * staticObject = new GPythonStaticObject(classGlueData, name);
					PyDict_SetItemString(dict, name, staticObject);
				}
			}
		}
		else {
			// to allow override method with script function
			if(metaIsMethod(item->getCategory())) {
				PyObject * staticObject = new GPythonStaticObject(classGlueData, item->getName());
				PyDict_SetItemString(dict, item->getName(), staticObject);
			}
		}
	}
}

void GPythonClass::initType()
{
	PyTypeObject * typeObject = static_cast<PyTypeObject *>(this);
	*typeObject = metaClassType;

//	typeObject->tp_name = this->getAs<GClassGlueData>()->getMetaClass()->getQualifiedName();

	if(typeObject->tp_dict == 0) {
		Py_TYPE(typeObject) = &PyType_Type;
		typeObject->tp_base = &PyType_Type;
		PyType_Ready(typeObject);
	}

	if(typeObject->tp_dict == nullptr) {
		typeObject->tp_dict = PyDict_New();
	}
	bindClassItems(typeObject->tp_dict, this->getAs<GClassGlueData>());

	PyObject_INIT(typeObject, &PyType_Type);
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
	if(value == nullptr) {
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
		GPythonNative * object = tryCastFromPython(value);
		if(object != nullptr) {
			GGlueDataPointer glueData = object->getData();
			if(outputGlueData != nullptr) {
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
	void * instanceAddress = objectAddressFromVariant(instance);

	if(instanceAddress == nullptr) {
		return pyAddRef(Py_None);
	}

	PythonScriptObjectCacheData * cachedPythonObject = context->getScriptObjectCache()->findScriptObject<PythonScriptObjectCacheData>(instance, classData, cv);
	if(cachedPythonObject) {
		return pyAddRef(cachedPythonObject->pythonObject->toPythonObject());
	}

	GObjectGlueDataPointer objectData(context->newOrReuseObjectGlueData(classData, instance, flags, cv));
	if(outputGlueData != nullptr) {
		*outputGlueData = objectData;
	}
	GPythonObject * object = createPythonObject(objectData);
	context->getScriptObjectCache()->addScriptObject(instance, classData, cv, new PythonScriptObjectCacheData(object));
	return object;
}

PyObject * rawToPython(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
{
	if(context->getConfig().allowAccessRawData()) {
		GRawGlueDataPointer rawData(context->newRawGlueData(value));
		if(outputGlueData != nullptr) {
			*outputGlueData = rawData;
		}
		PyObject * rawObject = createPythonObject(rawData);

		return rawObject;
	}

	return nullptr;
}

PyObject * createClassObject(const GContextPointer & context, IMetaClass * metaClass)
{
	return createPythonObject(context->newClassData(metaClass));

//	GPythonClass * object = new GPythonClass(context->newClassData(metaClass));
//	getPythonDataWrapperPool()->dataWrapperCreated(object);
//	return object->toPythonObject();

/*
	if(metaClassType.tp_dict == 0) {
		Py_TYPE(&metaClassType) = &PyType_Type;
		metaClassType.tp_base = &PyType_Type;
		PyType_Ready(&metaClassType);
	}

	PyObject * args = PyTuple_New(3);
	PyTuple_SetItem(args, 0, PyString_FromString(metaClass->getName()));
	PyTuple_SetItem(args, 1, PyTuple_New(0));
	PyTuple_SetItem(args, 1, PyDict_New());

	PyObject * ooo = PyObject_Call((PyObject *)&metaClassType, args, nullptr);
	PyTypeObject * typeObject = (PyTypeObject *)ooo;
	if(typeObject->tp_dict == nullptr) {
		typeObject->tp_dict = PyDict_New();
	}
	bindClassItems(typeObject->tp_dict, context->newClassData(metaClass));

	PyObject_INIT(typeObject, &PyType_Type);
	return ooo;
*/
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
		return result != nullptr;
	}

	static ResultType defaultValue()
	{
		return ResultType();
	}

	static ResultType doMethodsToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getUserData());
		GContextPointer context = classData->getBindingContext();
		if(data == nullptr) {
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));

			GScopedInterface<IMetaList> metaList(getMethodListFromMapItem(mapItem, getGlueDataInstanceAddress(objectData)));
			data = new GMapItemMethodData(context->newMethodGlueData(context->getClassData(boundClass.get()), metaList.get()));

			mapItem->setUserData(data);
		}

		return createPythonObject(context->newObjectAndMethodGlueData(objectData, data->getMethodData()));
	}

	static ResultType doEnumToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem, const char * /*enumName*/)
	{
		GContextPointer context = classData->getBindingContext();
		GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
		return createPythonObject(context->newEnumGlueData(metaEnum.get()));
	}
};

PyObject * variantToPython(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
{
	GVariant value = getVariantRealValue(data);
	GMetaType type = getVariantRealMetaType(data);

	GVariantType vt = static_cast<GVariantType>((uint16_t)value.getType() & ~(uint16_t)GVariantType::maskByReference);

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

	if(!vtIsInterface(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == nullptr) {
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
		callableParam->params[i].value = pythonToScriptValue(context, c, &callableParam->params[i].paramGlueData);
	}
}


PyObject * callbackCallMethod(PyObject * callableObject, PyObject * args, PyObject * /*keyWords*/)
{
	ENTER_PYTHON()

	GPythonNative * methodObject = nativeFromPython(callableObject);

	GObjectAndMethodGlueDataPointer userData = methodObject->getAs<GObjectAndMethodGlueData>();

	GContextPointer bindingContext(userData->getBindingContext());
	InvokeCallableParam callableParam(static_cast<int>(PyTuple_Size(args)), bindingContext->borrowScriptContext());
	loadCallableParam(bindingContext, args, &callableParam);

	InvokeCallableResult result = doInvokeMethodList(bindingContext, userData->getObjectData(), userData->getMethodData(), &callableParam);

	return methodResultToPython(bindingContext, result.callable.get(), &result);

	LEAVE_PYTHON(return nullptr)
}


PyObject * callbackConstructObject(PyObject * callableObject, PyObject * args, PyObject * /*keyWords*/)
{
	ENTER_PYTHON()

	GPythonNative * cppClass = nativeFromPython(callableObject);
	GClassGlueDataPointer classUserData = cppClass->getAs<GClassGlueData>();
	GContextPointer context = classUserData->getBindingContext();

	InvokeCallableParam callableParam(static_cast<int>(PyTuple_Size(args)), context->borrowScriptContext());
	loadCallableParam(context, args, &callableParam);

	void * instance = doInvokeConstructor(context, context->getService(), classUserData->getMetaClass(), &callableParam);

	if(instance != nullptr) {
		return createPythonObject(context->newObjectGlueData(classUserData, instance, GBindValueFlags(bvfAllowGC), opcvNone));
	}
	else {
		raiseCoreException(Error_ScriptBinding_FailConstructObject);
	}

	return nullptr;

	LEAVE_PYTHON(return nullptr)
}

PyObject * callbackGetAttribute(PyObject * object, PyObject * attrName)
{
	ENTER_PYTHON()

	PyObject * attrObject;

// If we enable below code, some common method names such as "get" will be intercepted by Python.
//	if(PyObject_HasAttr(cppObject->ob_type->tp_dict, attrName)) {
//		attrObject = PyObject_GetAttr(cppObject->ob_type->tp_dict, attrName);
//	}
//	else

	GPythonNative * nativeObject = nativeFromPython(object);
	const char * name = PyString_AsString(attrName);

	attrObject = namedMemberToScript<GPythonMethods>(nativeObject->getData(), name);
	if(attrObject != nullptr) {
		return attrObject;
	}

	return pyAddRef(Py_None);

	LEAVE_PYTHON(return nullptr)
}

int callbackSetAttribute(PyObject * object, PyObject * attrName, PyObject * value)
{
	ENTER_PYTHON()

	GPythonNative * nativeObject = nativeFromPython(object);
	const char * name = PyString_AsString(attrName);

	GGlueDataPointer valueGlueData;

	GScriptValue v = pythonToScriptValue(nativeObject->getData()->getBindingContext(), value, &valueGlueData);
	if(setValueOnNamedMember(nativeObject->getData(), name, v, valueGlueData)) {
		return 0;
	}

	return -1;

	LEAVE_PYTHON(return -1)
}

PyObject * callbackGetEnumValue(PyObject * object, PyObject * attrName)
{
	ENTER_PYTHON()

	GPythonNative * cppObject = nativeFromPython(object);

	GEnumGlueDataPointer userData = cppObject->getAs<GEnumGlueData>();

	const char * name = PyString_AsString(attrName);

	int32_t index = userData->getMetaEnum()->findKey(name);
	if(index >= 0) {
		return variantToPython(userData->getBindingContext(), metaGetEnumValue(userData->getMetaEnum(), index), GBindValueFlags(), nullptr);
	}

	raiseCoreException(Error_ScriptBinding_CantFindEnumKey, *name);

	return pyAddRef(Py_None);

	LEAVE_PYTHON(return nullptr)
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

	GPythonNative * cppObject = nativeFromPython(self);

	GAccessibleGlueDataPointer userData = cppObject->getAs<GAccessibleGlueData>();

	return accessibleToScript<GPythonMethods>(userData->getBindingContext(), userData->getAccessible(), userData->getInstanceAddress(), false);

	LEAVE_PYTHON(return nullptr)
}

int callbackAccessibleDescriptorSet(PyObject * self, PyObject * /*obj*/, PyObject * value)
{
	ENTER_PYTHON()

	GPythonNative * cppObject = nativeFromPython(self);

	GAccessibleGlueDataPointer userData = cppObject->getAs<GAccessibleGlueData>();

	GVariant v = pythonToScriptValue(userData->getBindingContext(), value, nullptr).getValue();
	metaSetValue(userData->getAccessible(), userData->getInstanceAddress(), v);

	return 0;

	LEAVE_PYTHON(return 0)
}

PyObject * callbackStaticObjectDescriptorGet(PyObject * self, PyObject * /*obj*/, PyObject * /*type*/)
{
	ENTER_PYTHON()

	GPythonStaticObject * nativeObject = dynamic_cast<GPythonStaticObject *>(nativeFromPython(self));

	PyObject * attrObject = nullptr;
	if(nativeObject != nullptr) {
		attrObject = namedMemberToScript<GPythonMethods>(nativeObject->getData(), nativeObject->getFieldName().c_str());
	}

	if(attrObject != nullptr) {
		return attrObject;
	}

	return pyAddRef(Py_None);

	LEAVE_PYTHON(return nullptr)
}

int callbackStaticObjectDescriptorSet(PyObject * self, PyObject * /*obj*/, PyObject * value)
{
	ENTER_PYTHON()

	GPythonStaticObject * nativeObject = dynamic_cast<GPythonStaticObject *>(nativeFromPython(self));

	GGlueDataPointer valueGlueData;

	GScriptValue v = pythonToScriptValue(nativeObject->getData()->getBindingContext(), value, &valueGlueData);
	if(setValueOnNamedMember(nativeObject->getData(), nativeObject->getFieldName().c_str(), v, valueGlueData)) {
		return 0;
	}

	return -1;

	LEAVE_PYTHON(return -1)
}

PyObject * callbackAnyObjectGetAttribute(PyObject * object, PyObject * attrName)
{
	ENTER_PYTHON()

	PyObject * obj = PyDict_GetItem(static_cast<GPythonAnyObject *>(object)->getDict(), attrName);
	Py_XINCREF(obj);

	return obj;

	LEAVE_PYTHON(return nullptr)
}

int callbackAnyObjectSetAttribute(PyObject * object, PyObject * attrName, PyObject * value)
{
	ENTER_PYTHON()

	return PyDict_SetItem(static_cast<GPythonAnyObject *>(object)->getDict(), attrName, value);

	LEAVE_PYTHON(return -1)
}

GScriptValue invokePythonFunctionIndirectly(const GContextPointer & context, PyObject * object, PyObject * func, GVariant const * const * params, size_t paramCount, const char * name)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(! context) {
		raiseCoreException(Error_ScriptBinding_NoContext);
	}

	if(PyCallable_Check(func)) {
		size_t allParamCount = paramCount;
		if(object != nullptr) {
			++allParamCount;
		}
		GPythonScopedPointer args(PyTuple_New(allParamCount));
		int start = 0;
		if(object != nullptr) {
			start = 1;
			PyTuple_SetItem(args.get(), 0, object);
		}
		for(size_t i = 0; i < paramCount; ++i) {
			GPythonScopedPointer arg(variantToPython(context, *params[i], GBindValueFlags(bvfAllowRaw), nullptr));
			if(!arg) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
			PyTuple_SetItem(args.get(), start + i, arg.take());
		}

		GPythonScopedPointer result;

		result.reset(PyObject_Call(func, args.get(), nullptr));

		return pythonToScriptValue(context, result.get(), nullptr);
	}
	else {
		raiseCoreException(Error_ScriptBinding_CantCallNonfunction);
	}

	return GScriptValue::fromNull();
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
		return nullptr;
	}
}

const char * const signatureName = "cpgf_pYtHon_oBjeCt_Name";
const char * const signatureValue = "cpgf_pYtHon_oBjeCt_vAlue";

void setObjectSignature(PyObject * obj)
{
	setObjectAttr(obj, signatureName, PyString_FromString(signatureValue));
}

PyObject * helperBindMethodList(const GContextPointer & context, IMetaList * methodList)
{
	GMethodGlueDataPointer data = context->newMethodGlueData(GClassGlueDataPointer(), methodList);
	GObjectAndMethodGlueDataPointer methodData = context->newObjectAndMethodGlueData(GObjectGlueDataPointer(), data);
	return createPythonObject(methodData);
}

PyObject * helperBindClass(const GContextPointer & context, IMetaClass * metaClass)
{
	return createClassObject(context, metaClass);
}

PyObject * helperBindEnum(const GContextPointer & context, IMetaEnum * metaEnum)
{
	return createPythonObject(context->newEnumGlueData(metaEnum));
}

PyObject * helperBindAccessible(const GContextPointer & context, void * instance, IMetaAccessible * accessible)
{
	return createPythonObject(context->newAccessibleGlueData(instance, accessible));
}

PyObject * helperBindValue(const GContextPointer & context, const GScriptValue & value)
{
	PyObject * result = nullptr;

	switch(value.getType()) {
		case GScriptValue::typeNull:
			result = Py_None;
			break;

		case GScriptValue::typeFundamental:
			result = variantToPython(context, value.toFundamental(), GBindValueFlags(bvfAllowRaw), nullptr);
			break;

		case GScriptValue::typeString:
			result = PyString_FromString(value.toString().c_str());
			break;

		case GScriptValue::typeClass: {
			GScopedInterface<IMetaClass> metaClass(value.toClass());
			result = helperBindClass(context, metaClass.get());
			break;
		}

		case GScriptValue::typeObject: {
			IMetaClass * metaClass;
			bool transferOwnership;
			void * instance = objectAddressFromVariant(value.toObject(&metaClass, &transferOwnership));
			GScopedInterface<IMetaClass> metaClassGuard(metaClass);

			GBindValueFlags flags;
			flags.setByBool(bvfAllowGC, transferOwnership);
			result = objectToPython(context, context->getClassData(metaClass), instance, flags, opcvNone, nullptr);
			break;
		}

		case GScriptValue::typeMethod: {
			void * instance;
			GScopedInterface<IMetaMethod> method(value.toMethod(&instance));

			if(method->isStatic()) {
				instance = nullptr;
			}

			GScopedInterface<IMetaList> methodList(createMetaList());
			methodList->add(method.get(), instance);

			result = helperBindMethodList(context, methodList.get());
			break;
		}

		case GScriptValue::typeOverloadedMethods: {
			GScopedInterface<IMetaList> methodList(value.toOverloadedMethods());
			result = helperBindMethodList(context, methodList.get());
			break;
		}

		case GScriptValue::typeEnum: {
			GScopedInterface<IMetaEnum> metaEnum(value.toEnum());
			result = helperBindEnum(context, metaEnum.get());
			break;
		}

		case GScriptValue::typeRaw:
			result = rawToPython(context, value.toRaw(), nullptr);
			break;

		case GScriptValue::typeAccessible: {
			void * instance;
			GScopedInterface<IMetaAccessible> accessible(value.toAccessible(&instance));
			result = helperBindAccessible(context, instance, accessible.get());
			break;
		}

	}

	return result;
}

template <GMetaOpType op, bool allowRightSelf>
PyObject * binaryOperator(PyObject * a, PyObject * b)
{
	ENTER_PYTHON()

	PyObject * self = nullptr;
	PyObject * other = nullptr;
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
			return nullptr;
		}
		self = b;
		other = a;
		selfIndex = 1;
		otherIndex = 0;
	}

	GObjectGlueDataPointer objectData = nativeFromPython(self)->getAs<GObjectGlueData>();
	const GContextPointer & context = objectData->getBindingContext();

	InvokeCallableParam callableParam(2, context->borrowScriptContext());

	callableParam.params[selfIndex].value = pythonToScriptValue(context, self, &callableParam.params[selfIndex].paramGlueData);

	callableParam.params[otherIndex].value = pythonToScriptValue(context, other, &callableParam.params[otherIndex].paramGlueData);

	InvokeCallableResult result = doInvokeOperator(context, objectData, objectData->getClassData()->getMetaClass(), op, &callableParam);

	return methodResultToPython(context, result.callable.get(), &result);

	LEAVE_PYTHON(return nullptr)
}

template <GMetaOpType op>
PyObject * unaryOperator(PyObject * a)
{
	ENTER_PYTHON()

	PyObject * self = a;

	GObjectGlueDataPointer objectData = nativeFromPython(self)->getAs<GObjectGlueData>();
	const GContextPointer & context = objectData->getBindingContext();

	InvokeCallableParam callableParam(1, context->borrowScriptContext());

	callableParam.params[0].value = pythonToScriptValue(context, self, &callableParam.params[0].paramGlueData);

	InvokeCallableResult result = doInvokeOperator(context, objectData, objectData->getClassData()->getMetaClass(), op, &callableParam);

	return methodResultToPython(context, result.callable.get(), &result);

	LEAVE_PYTHON(return nullptr)
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

GScriptValue GPythonScriptFunction::invoke(const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GScriptValue GPythonScriptFunction::invokeIndirectly(GVariant const * const * params, size_t paramCount)
{
	return invokePythonFunctionIndirectly(this->getBindingContext(), nullptr, this->func, params, paramCount, "");
}


GPythonScriptArray::GPythonScriptArray(const GContextPointer & context, PyObject * listObject)
	: super(context), listObject(listObject)
{
	Py_XINCREF(this->listObject);
}

GPythonScriptArray::~GPythonScriptArray()
{
	if(isLibraryLive()) {
		Py_XDECREF(this->listObject);
	}
}

size_t GPythonScriptArray::getLength()
{
	return PyList_Size(this->listObject);
}

GScriptValue GPythonScriptArray::getValue(size_t index)
{
	size_t length = this->getLength();
	if(index >= length) {
		return GScriptValue();
	}

	PyObject * obj = PyList_GetItem(this->listObject, index); // borrowed reference!

	if(obj != nullptr) {
		return pythonToScriptValue(this->getBindingContext(), obj, nullptr);
	}
	else {
		return GScriptValue();
	}
}

void GPythonScriptArray::setValue(size_t index, const GScriptValue & value)
{
	if(value.isAccessible()) {
		raiseCoreException(Error_ScriptBinding_NotSupportedFeature, "Set Accessible Into Array", "Python");
	}
	else {
		size_t length = this->getLength();
		while(index >= length) {
			Py_XINCREF(Py_None);
			PyList_Append(this->listObject, Py_None);
			++length;
		}
		PyObject * valueObject = helperBindValue(this->getBindingContext(), value);
		GASSERT(valueObject != nullptr);

		PyList_SetItem(this->listObject, index, valueObject);
	}
}

bool GPythonScriptArray::maybeIsScriptArray(size_t index)
{
	PyObject * obj = PyList_GetItem(this->listObject, index); // borrowed reference!
	if(obj != nullptr) {
		return !! PyList_Check(obj);
	}
	else {
		return false;
	}
}
GScriptValue GPythonScriptArray::getAsScriptArray(size_t index)
{
	PyObject * obj = PyList_GetItem(this->listObject, index); // borrowed reference!
	if(obj != nullptr && !! PyList_Check(obj)) {
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GPythonScriptArray(this->getBindingContext(), obj), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
	else {
		return GScriptValue();
	}
}
GScriptValue GPythonScriptArray::createScriptArray(size_t index)
{
	PyObject * obj = PyList_GetItem(this->listObject, index); // borrowed reference!
	if(obj != nullptr) {
		if(!! PyList_Check(obj)) {
			GScopedInterface<IScriptArray> scriptArray(
				new ImplScriptArray(new GPythonScriptArray(this->getBindingContext(), obj), true)
			);
			return GScriptValue::fromScriptArray(scriptArray.get());
		}
	}
	else {
		PyObject * obj = PyList_New(0);
		PyList_SetItem(this->listObject, index, obj);
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GPythonScriptArray(this->getBindingContext(), obj), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}

	return GScriptValue();
}

GPythonScriptObject::GPythonScriptObject(IMetaService * service, PyObject * object, const GScriptConfig & config)
	: super(GContextPointer(new GPythonBindingContext(service, config)), config), object(object)
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
		return pythonToScriptValue(this->getBindingContext(), obj.get(), nullptr);
	}
	else {
		return GScriptValue();
	}
}

void GPythonScriptObject::doSetValue(const char * name, const GScriptValue & value)
{
	PyObject * valueObject = helperBindValue(this->getBindingContext(), value);
	GASSERT(valueObject != nullptr);

	setObjectAttr(this->object, name, valueObject);
}

GScriptObject * GPythonScriptObject::doCreateScriptObject(const char * name)
{
	PyObject * attr = getObjectAttr(this->object, name);
	if(attr != nullptr) {
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

	return nullptr;
}

GScriptValue GPythonScriptObject::getScriptFunction(const char * name)
{
	GPythonScopedPointer func(getObjectAttr(this->object, name));
	if(func) {
		if(PyCallable_Check(func.get())) {
			GScopedInterface<IScriptFunction> scriptFunction(new ImplScriptFunction(new GPythonScriptFunction(this->getBindingContext(), func.take()), true));
			return GScriptValue::fromScriptFunction(scriptFunction.get());
		}
	}

	return GScriptValue();
}

GScriptValue GPythonScriptObject::invoke(const char * name, const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GScriptValue GPythonScriptObject::invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount)
{
	GPythonScopedPointer func(getObjectAttr(this->object, name));
	return invokePythonFunctionIndirectly(this->getBindingContext(), nullptr, func.get(), params, paramCount, name);
}

void GPythonScriptObject::assignValue(const char * fromName, const char * toName)
{
	GPythonScopedPointer obj(getObjectAttr(this->object, fromName));
	if(obj) {
		setObjectAttr(this->object, toName, obj.get());
	}
}

bool GPythonScriptObject::maybeIsScriptArray(const char * name)
{
	GPythonScopedPointer obj(getObjectAttr(this->object, name));
	if(obj) {
		return !! PyList_Check(obj.get());
	}
	else {
		return false;
	}
}

GScriptValue GPythonScriptObject::getAsScriptArray(const char * name)
{
	GPythonScopedPointer obj(getObjectAttr(this->object, name));
	if(obj && !! PyList_Check(obj.get())) {
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GPythonScriptArray(this->getBindingContext(), obj.get()), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
	else {
		return GScriptValue();
	}
}

GScriptValue GPythonScriptObject::createScriptArray(const char * name)
{
	PyObject * attr = getObjectAttr(this->object, name);
	if(attr != nullptr) {
		if(this->getValue(name).getType() == GScriptValue::typeScriptArray) {
			GScopedInterface<IScriptArray> scriptArray(
				new ImplScriptArray(new GPythonScriptArray(this->getBindingContext(), attr), true)
			);
			return GScriptValue::fromScriptArray(scriptArray.get());
		}
	}
	else {
		PyObject * obj = PyList_New(0);
		setObjectAttr(this->object, name, obj);
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GPythonScriptArray(this->getBindingContext(), obj), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}

	return GScriptValue();
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


