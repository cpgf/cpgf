#if defined(_MSC_VER)
#define _CRT_SECURE_NO_WARNINGS

#pragma warning(push)
#pragma warning(disable:4127) // warning C4127: conditional expression is constant
#endif

// Include cmath to avoid error: '::hypot' has not been declared
#include <cmath>
#include "Python.h"

#include "cpgf/scriptbind/gpythonbind.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gerrorcode.h"
#include "cpgf/gstringutil.h"

#include "gbindcommon.h"
#include "gbindapiimpl.h"
#include "../gstaticuninitializerorders.h"

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

class GPythonNative : public GGlueDataWrapper
{
public:
	GPythonNative() {}
	explicit GPythonNative(const GGlueDataPointer & glueData);
	virtual ~GPythonNative();

	virtual GGlueDataPointer getData() const;
	virtual PyObject * toPythonObject() = 0;
	
	void resetData() {
		this->glueData.reset();
	}

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

class GPythonBindingContext : public GBindingContext
{
private:
	typedef GBindingContext super;

public:
	explicit GPythonBindingContext(IMetaService * service)
		: super(service)
	{
	}
};

typedef std::shared_ptr<GPythonBindingContext> GPythonContextPointer;

class GPythonScriptFunction : public GScriptFunctionBase
{
private:
	typedef GScriptFunctionBase super;

public:
	GPythonScriptFunction(const GContextPointer & context, PyObject * func);
	virtual ~GPythonScriptFunction();

	virtual GScriptValue invoke(const GVariant * params, size_t paramCount);
	virtual GScriptValue invokeIndirectly(GVariant const * const * params, size_t paramCount);
	
	PyObject * getPythonObject() const {
		return this->func;
	}

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

	PyObject * getPythonObject() const {
		return this->listObject;
	}

private:
	PyObject * listObject;
};

class GPythonScriptObject : public GScriptObjectBase
{
private:
	typedef GScriptObjectBase super;

public:
	GPythonScriptObject(IMetaService * service, PyObject * object);
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
		return std::static_pointer_cast<GPythonBindingContext>(this->getBindingContext());
	}

public:
	PyObject * getObject() const {
		return this->object;
	}

	PyObject * getPythonObject() const {
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

PyObject * variantToPython(const GContextPointer & context, const GVariant & data);

PyObject * doValueToScript(
	const GContextPointer & context,
	const GScriptValue & value,
	const ScriptValueToScriptData & data,
	const bool onlyBindAccessibleValue = true
);

GScriptValue doScriptToValue(
	const GContextPointer & context,
	PyObject * value,
	GGlueDataPointer * outputGlueData
);

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

class GPythonUserData : public GGlueUserData
{
public:
	explicit GPythonUserData(GPythonNative * pythonObject)
		: pythonObject(pythonObject)
	{}

	GPythonNative * getPythonNative() const {
		return this->pythonObject;
	}

private:
	GPythonNative * pythonObject;
};

PyObject * pyAddRef(PyObject * obj)
{
	Py_XINCREF(obj);

	return obj;
}

GPythonObject * createPythonObject(const GGlueDataPointer & glueData)
{
	GPythonUserData * userData = glueData->getUserDataAs<GPythonUserData>();
	GPythonObject * object;
	if(userData == nullptr) {
		object = new GPythonObject(glueData);
		userData = new GPythonUserData(object);
		glueData->setUserData(userData, false);
		getPythonDataWrapperPool()->dataWrapperCreated(object);
	}
	else {
		object = static_cast<GPythonObject *>(userData->getPythonNative());
		pyAddRef(object);
	}
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
	const GGlueDataPointer & glueData = object->getData();
	if(glueData->isValid()) {
		glueData->setUserData(nullptr);
	}
	getPythonDataWrapperPool()->dataWrapperDestroyed(object);
	
	object->resetData();

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

PyObject * objectToPython(
		const GContextPointer & context,
		const GClassGlueDataPointer & classData,
		const GVariant & instance,
		const bool allowGC,
		const GScriptInstanceCv cv,
		GGlueDataPointer * outputGlueData
	)
{
	void * instanceAddress = objectAddressFromVariant(instance);

	if(instanceAddress == nullptr) {
		return pyAddRef(Py_None);
	}

	GObjectGlueDataPointer objectData(context->newObjectGlueData(classData, instance, allowGC, cv));
	if(outputGlueData != nullptr) {
		*outputGlueData = objectData;
	}
	return createPythonObject(objectData);
}

PyObject * rawToPython(const GContextPointer & context, const GVariant & value)
{
	GRawGlueDataPointer rawData(context->newRawGlueData(value));
	PyObject * rawObject = createPythonObject(rawData);

	return rawObject;
}

PyObject * createClassObject(const GContextPointer & context, IMetaClass * metaClass)
{
	return createPythonObject(context->getClassData(metaClass));

//	GPythonClass * object = new GPythonClass(context->getClassData(metaClass));
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
	bindClassItems(typeObject->tp_dict, context->getClassData(metaClass));

	PyObject_INIT(typeObject, &PyType_Type);
	return ooo;
*/
}

struct GPythonMethods
{
	typedef PyObject * ResultType;

	static bool isSuccessResult(const ResultType & result)
	{
		return result != nullptr;
	}

	static ResultType defaultValue()
	{
		return ResultType();
	}

	static ResultType doScriptValueToScript(
		const GContextPointer & context,
		const GScriptValue & scriptValue,
		const ScriptValueToScriptData & data
	)
	{
		return doValueToScript(context, scriptValue, data);
	}

};

PyObject * variantToPython(const GContextPointer & /*context*/, const GVariant & data)
{
	GVariant value = getVariantRealValue(data);
	GMetaType type = getVariantRealMetaType(data);

	GVariantType vt = static_cast<GVariantType>((GVtType)value.getType() & ~(GVtType)GVariantType::maskByReference);

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

	if(variantIsAnyString(value)) {
		return PyString_FromString(stringFromVariant(value).c_str());
	}

	return nullptr;
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
		callableParam->params[i].value = doScriptToValue(context, c, &callableParam->params[i].paramGlueData);
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
		return createPythonObject(context->newObjectGlueData(classUserData, instance, true, GScriptInstanceCv::sicvNone));
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

	GScriptValue v = doScriptToValue(nativeObject->getData()->getBindingContext(), value, &valueGlueData);
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
		return variantToPython(userData->getBindingContext(), metaGetEnumValue(userData->getMetaEnum(), index));
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

	GVariant v = doScriptToValue(userData->getBindingContext(), value, nullptr).getValue();
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

	GScriptValue v = doScriptToValue(nativeObject->getData()->getBindingContext(), value, &valueGlueData);
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

GScriptValue invokePythonFunctionIndirectly(
		const GContextPointer & context,
		PyObject * object,
		PyObject * func,
		GVariant const * const * params,
		const size_t paramCount,
		const char * name
	)
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
			GPythonScopedPointer arg(doValueToScript(
				context,
				doCreateScriptValueFromVariant(context, *params[i], false),
				ScriptValueToScriptData(),
				true
			));
			if(!arg) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
			PyTuple_SetItem(args.get(), start + i, arg.release());
		}

		GPythonScopedPointer result;

		result.reset(PyObject_Call(func, args.get(), nullptr));

		return doScriptToValue(context, result.get(), nullptr);
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

PyObject * helperBindMethodList(
		const GContextPointer & context,
		const GObjectGlueDataPointer & objectData,
		const GMethodGlueDataPointer & methodData
	)
{
	GObjectAndMethodGlueDataPointer objectAndMethodData = context->newObjectAndMethodGlueData(objectData, methodData);
	return createPythonObject(objectAndMethodData);
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

PyObject * doValueToScript(
		const GContextPointer & context,
		const GScriptValue & value,
		const ScriptValueToScriptData & data,
		const bool onlyBindAccessibleValue
	)
{
	PyObject * result = nullptr;

	switch(value.getType()) {
		case GScriptValue::typeNull:
			result = Py_None;
			break;

		case GScriptValue::typePrimary:
			result = variantToPython(context, value.toPrimary());
			break;

		case GScriptValue::typeClass: {
			GScopedInterface<IMetaClass> metaClass(value.toClass());
			result = helperBindClass(context, metaClass.get());
			break;
		}

		case GScriptValue::typeObject: {
			IMetaClass * metaClass;
			bool transferOwnership;
			GScriptInstanceCv cv;
			const GVariant instance = value.toObject(&metaClass, &transferOwnership, &cv);
			GScopedInterface<IMetaClass> metaClassGuard(metaClass);

			result = objectToPython(context, context->getClassData(metaClass), instance, transferOwnership, cv, data.outputGlueData);
			break;
		}

		case GScriptValue::typeMethod: {
			const GMethodGlueDataPointer methodData = context->newMethodGlueData(value);
			result = helperBindMethodList(context, data.objectData, methodData);
			break;
		}

		case GScriptValue::typeOverloadedMethods: {
			const GMethodGlueDataPointer methodData = context->newMethodGlueData(value);
			result = helperBindMethodList(context, data.objectData, methodData);
			break;
		}

		case GScriptValue::typeEnum: {
			GScopedInterface<IMetaEnum> metaEnum(value.toEnum());
			result = helperBindEnum(context, metaEnum.get());
			break;
		}

		case GScriptValue::typeRaw:
			result = rawToPython(context, value.toRaw());
			break;

		case GScriptValue::typeAccessible: {
			void * instance;
			GScopedInterface<IMetaAccessible> accessible(value.toAccessible(&instance));

			bool instanceIsConst = false;

			if(data.objectData) {
				instance = data.objectData->getInstanceAddress();
				instanceIsConst = (data.objectData->getCV() == GScriptInstanceCv::sicvConst);
			}

			if(onlyBindAccessibleValue) {
				result = accessibleToScript<GPythonMethods>(context, accessible.get(), instance, instanceIsConst);
			}
			else {
				result = helperBindAccessible(context, instance, accessible.get());
			}
			break;
		}

		case GScriptValue::typeScriptObject: {
			GScopedInterface<IScriptObject> obj(value.toScriptObject());
			GPythonScriptObject * pythonObj = static_cast<GPythonScriptObject *>(
				static_cast<ImplScriptObject *>(obj.get())->getUnderlying()
			);
			return pyAddRef(pythonObj->getPythonObject());
		}
		
		case GScriptValue::typeScriptFunction: {
			GScopedInterface<IScriptFunction> func(value.toScriptFunction());
			GPythonScriptFunction * pythonFunc = static_cast<GPythonScriptFunction *>(
				static_cast<ImplScriptFunction *>(func.get())->getUnderlying()
			);
			return pyAddRef(pythonFunc->getPythonObject());
		}
		
		case GScriptValue::typeScriptArray: {
			GScopedInterface<IScriptArray> array(value.toScriptArray());
			GPythonScriptArray * pythonArray = static_cast<GPythonScriptArray *>(
				static_cast<ImplScriptArray *>(array.get())->getUnderlying()
			);
			return pyAddRef(pythonArray->getPythonObject());
		}
	}

	return result;
}

GScriptValue doScriptToValue(
		const GContextPointer & context,
		PyObject * value,
		GGlueDataPointer * outputGlueData
	)
{
	if(value == nullptr) {
		return GScriptValue();
	}

	if(value == Py_None) {
		return GScriptValue::fromNull();
	}

	if(PyNumber_Check(value)) {
		if(PyBool_Check(value)) {
			return GScriptValue::fromPrimary(bool(PyInt_AsLong(value) != 0));
		}

		if(PyInt_Check(value)) {
			return GScriptValue::fromPrimary(PyInt_AsLong(value));
		}

		if(PyLong_Check(value)) {
			return GScriptValue::fromPrimary(PyLong_AsLong(value));
		}

		if(PyFloat_Check(value)) {
			return GScriptValue::fromPrimary(PyFloat_AsDouble(value));
		}

	}
	else if(PyString_Check(value)) {
		return GScriptValue::fromPrimary(createStringVariant(PyString_AsString(value)));
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

		GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GPythonScriptObject(context->getService(), value), true));

		return GScriptValue::fromScriptObject(scriptObject.get());
	}

	return GScriptValue();
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

	callableParam.params[selfIndex].value = doScriptToValue(context, self, &callableParam.params[selfIndex].paramGlueData);

	callableParam.params[otherIndex].value = doScriptToValue(context, other, &callableParam.params[otherIndex].paramGlueData);

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

	callableParam.params[0].value = doScriptToValue(context, self, &callableParam.params[0].paramGlueData);

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
		return doScriptToValue(this->getBindingContext(), obj, nullptr);
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
		PyObject * valueObject = doValueToScript(this->getBindingContext(), value, ScriptValueToScriptData());
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

GPythonScriptObject::GPythonScriptObject(IMetaService * service, PyObject * object)
	: super(GContextPointer(new GPythonBindingContext(service))), object(object)
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
		return doScriptToValue(this->getBindingContext(), obj.get(), nullptr);
	}
	else {
		return GScriptValue();
	}
}

void GPythonScriptObject::doSetValue(const char * name, const GScriptValue & value)
{
	PyObject * valueObject = doValueToScript(this->getBindingContext(), value, ScriptValueToScriptData(), false);
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
			GScopedInterface<IScriptFunction> scriptFunction(new ImplScriptFunction(new GPythonScriptFunction(this->getBindingContext(), func.release()), true));
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


GScriptObject * createPythonScriptObject(IMetaService * service, PyObject * object)
{
	return new GPythonScriptObject(service, object);
}

IScriptObject * createPythonScriptInterface(IMetaService * service, PyObject * object)
{
	return new ImplScriptObject(new GPythonScriptObject(service, object), true);
}


G_GUARD_LIBRARY_LIFE


} // namespace cpgf


