#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gerrorcode.h"

#include "pinclude/gbindcommon.h"
#include "pinclude/gscriptbindapiimpl.h"

#include <string>

// for test
#include "cpgf/gmetadefine.h"
#include "cpgf/gmetaapi.h"
#include "cpgf/gmetaapiutil.h"
#include "cpgf/gmetaclass.h"
#include "cpgf/gscopedinterface.h"
#include "iostream"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127 4100 4800 4512 4480 4267)
#endif

#include "jsapi.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

using namespace std;
using namespace cpgf::bind_internal;
using namespace JS;

namespace cpgf {

namespace {

//*********************************************
// Declarations
//*********************************************

#define ENTER_SPIDERMONKEY() \
	char local_msg[256]; bool local_error = false; { \
	try {

#define LEAVE_SPIDERMONKEY(...) \
	} \
	catch(const GException & e) { strncpy(local_msg, e.getMessage(), 256); local_error = true; } \
	catch(const exception & e) { strncpy(local_msg, e.what(), 256); local_error = true; } \
	catch(...) { strcpy(local_msg, "Unknown exception occurred."); local_error = true; } \
	} if(local_error) { local_msg[255] = 0; error(jsContext, local_msg); } \
	__VA_ARGS__;

typedef JS::Value JsValue;

class GSpiderBindingContext : public GBindingContext, public GShareFromBase
{
private:
	typedef GBindingContext super;

public:
	GSpiderBindingContext(IMetaService * service, const GScriptConfig & config, JSContext * jsContext, JSObject  * jsGlobalObject)
		: super(service, config), jsContext(jsContext), jsGlobalObject(jsGlobalObject)
	{
		if(this->jsGlobalObject == NULL) {
			this->jsGlobalObject = JS_NewObject(jsContext, NULL, NULL, NULL);
		}
	}

	JSContext * getJsContext() const {
		return this->jsContext;
	}

	JSObject  * getJsGlobalObject() const {
		return this->jsGlobalObject;
	}

	GGlueDataWrapperPool * getGlueDataWrapperPool() const {
		return &this->glueDataWrapperPool;
	}

private:
	JSContext * jsContext;
	JSObject  * jsGlobalObject;
	mutable GGlueDataWrapperPool glueDataWrapperPool;
};

typedef GSharedPointer<GSpiderBindingContext> GSpiderContextPointer;

class GSpiderMonkeyScriptObject : public GScriptObjectBase
{
private:
	typedef GScriptObjectBase super;

public:
	GSpiderMonkeyScriptObject(IMetaService * service, const GScriptConfig & config, JSContext *jsContext, JSObject  * jsGlobalObject);
	virtual ~GSpiderMonkeyScriptObject();

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

	virtual GScriptObject * doCreateScriptObject(const char * name);

	virtual GScriptFunction * gainScriptFunction(const char * name);

	virtual GVariant invoke(const char * name, const GVariant * params, size_t paramCount);
	virtual GVariant invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);
	virtual bool valueIsNull(const char * name);
	virtual void nullifyValue(const char * name);

	virtual void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible);

	virtual void bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader);

	GSpiderContextPointer getSpiderContext() const {
		return sharedStaticCast<GSpiderBindingContext>(this->getContext());
	}
	
private:
	JSContext *jsContext;
	JSObject  * jsGlobalObject;
};

class JsClassUserData : public GUserData
{
private:
	typedef GUserData super;

public:
	explicit JsClassUserData(const char * className);
	
	JSClass * getJsClass() {
		return &this->jsClass;
	}

private:
	GScopedArray<char> className;
	JSClass jsClass;
};

void doBindClass(const GSpiderContextPointer & context, JSObject * owner, const char * name, IMetaClass * metaClass);
void doBindMethodList(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, JSObject * owner, const char * name, IMetaList * methodList, GGlueDataMethodType methodType);
JSFunction * createJsFunction(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, const char * name, IMetaList * methodList, GGlueDataMethodType methodType);


//*********************************************
// Global function implementations
//*********************************************


void error(JSContext * jsContext, const char * message)
{
	JS_ReportError(jsContext, "%s", message);
}

JsValue variantToSpider(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
{
	GVariant value = getVariantRealValue(data);
	GMetaType type = getVariantRealMetaType(data);

	GVariantType vt = static_cast<GVariantType>(value.getType() & ~byReference);

	if(vtIsEmpty(vt)) {
		return JSVAL_NULL;
	}

	if(vtIsBoolean(vt)) {
		return BooleanValue(fromVariant<bool>(value));
	}

	if(vtIsInteger(vt)) {
		return Int32Value(fromVariant<int>(value));
	}

	if(vtIsReal(vt)) {
		return DoubleValue(fromVariant<double>(value));
	}

	if(!vtIsInterface(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == NULL) {
		return JSVAL_NULL;
	}

	if(variantIsString(value)) {
		char * s = fromVariant<char *>(value);
		JSString * jsString = JS_NewStringCopyZ(sharedStaticCast<GSpiderBindingContext>(context)->getJsContext(), s);
		return StringValue(jsString);
	}

	if(variantIsWideString(value)) {
		const wchar_t * ws = fromVariant<wchar_t *>(value);
		JSString * jsString = JS_NewUCStringCopyZ(sharedStaticCast<GSpiderBindingContext>(context)->getJsContext(), ws);
		return StringValue(jsString);
	}

return JSVAL_NULL;
//	return complexVariantToScript<GV8Methods>(jsContext, value, type, flags, outputGlueData);
}

GVariant spiderToVariant(const GSpiderContextPointer & context, JsValue value, GGlueDataPointer * outputGlueData)
{
	if(value.isUndefined()) {
		return GVariant();
	}

	if(value.isBoolean()) {
		return value.toBoolean();
	}

	if(value.isInt32()) {
		return value.toInt32();
	}

	if(value.isNull()) {
		return (void *)0;
	}

	if(value.isNumber()) {
		return value.toNumber();
	}

	if(value.isString()) {
		JSString * jsString = value.toString();
		GScopedArray<char> s(wideStringToString(JS_GetStringCharsZ(context->getJsContext(), jsString)));
		return createTypedVariant(createStringVariant(s.get()), createMetaType<char *>());
	}

	if(value.isObject()) {
//		return v8UserDataToVariant(context, v8Context, value, outputGlueData);
	}

	return GVariant();
}

GScriptDataType getSpiderType(JsValue value, IMetaTypedItem ** typeItem)
{
	if(typeItem != NULL) {
		*typeItem = NULL;
	}

	if(value.isNull()) {
		return sdtNull;
	}

	if(value.isUndefined()) {
		return sdtNull;
	}

	if(value.isBoolean()) {
		return sdtFundamental;
	}

	if(value.isInt32() || value.isNumber()) {
		return sdtFundamental;
	}

	if(value.isString()) {
		return sdtString;
	}

	if(value.isObject()) {
		//Local<Object> obj = value->ToObject();
		//if(isValidObject(obj)) {
		//	if(obj->InternalFieldCount() == 0) {
		//		Handle<Value> value = obj->GetHiddenValue(String::New(userDataKey));
		//		if(! value.IsEmpty()) {
		//			if(value->IsExternal()) {
		//				GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Handle<External>::Cast(value)->Value());
		//				switch(dataWrapper->getData()->getType()) {
		//					case gdtClass:
		//						if(typeItem != NULL) {
		//							*typeItem = dataWrapper->getAs<GClassGlueData>()->getMetaClass();
		//							(*typeItem)->addReference();
		//						}
		//						return sdtClass;

		//					case gdtMethod:
		//						return methodTypeToGlueDataType(dataWrapper->getAs<GMethodGlueData>()->getMethodType());

		//					default:
		//						break;
		//				}
		//			}
		//		}

		//	}
		//	else {
		//		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(obj->GetPointerFromInternalField(0));
		//		if(dataWrapper != NULL) {
		//			switch(dataWrapper->getData()->getType()) {
		//				case gdtObject: {
		//					if(typeItem != NULL) {
		//						*typeItem = dataWrapper->getAs<GObjectGlueData>()->getClassData()->getMetaClass();
		//						(*typeItem)->addReference();
		//					}
		//					return sdtObject;
		//				}

		//				break;

		//				case gdtEnum:
		//					if(typeItem != NULL) {
		//						*typeItem = dataWrapper->getAs<GEnumGlueData>()->getMetaEnum();
		//						(*typeItem)->addReference();
		//					}
		//					return sdtEnum;

		//			    case gdtRaw:
		//			    	return sdtRaw;

		//				default:
		//				break;
		//			}
		//		}
		//	}
		//}

		//if(value->IsFunction()) {
		//	return sdtScriptMethod;
		//}
		//else {
		//	return sdtScriptObject;
		//}
	}

	return sdtUnknown;
}

static const JsValue dummy;
struct GSpiderMethods
{
	typedef JsValue ResultType;

	static ResultType doObjectToScript(const GContextPointer & context, const GClassGlueDataPointer & classData,
		const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
	{
//		objectToSpider(context, classData, instance, flags, cv, outputGlueData);
		return dummy;
	}

	static ResultType doVariantToScript(const GContextPointer & context, const GVariant & value, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
	{
		return variantToSpider(context, value, flags, outputGlueData);
	}
	
	static ResultType doRawToScript(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
	{
//		return rawToSpider(context, value, outputGlueData);
		return dummy;
	}

	static ResultType doClassToScript(const GContextPointer & context, IMetaClass * metaClass)
	{
		GSpiderContextPointer spiderContext = sharedStaticCast<GSpiderBindingContext>(context);
		doBindClass(spiderContext, spiderContext->getJsGlobalObject(), metaClass->getName(), metaClass);
		return dummy;
	}

	static ResultType doStringToScript(const GContextPointer & context, const char * s)
	{
		JSString * jsString = JS_NewStringCopyZ(sharedStaticCast<GSpiderBindingContext>(context)->getJsContext(), s);
		return StringValue(jsString);
	}

	static ResultType doWideStringToScript(const GContextPointer & context, const wchar_t * ws)
	{
		JSString * jsString = JS_NewUCStringCopyZ(sharedStaticCast<GSpiderBindingContext>(context)->getJsContext(), ws);
		return StringValue(jsString);
	}

	static bool isSuccessResult(const ResultType & result)
	{
		return ! result.isUndefined();
	}

	static ResultType doMethodsToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem,
		const char * methodName, GMetaClassTraveller * /*traveller*/,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getUserData());
		GSpiderContextPointer context = sharedStaticCast<GSpiderBindingContext>(classData->getContext());
		if(data == NULL) {
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));
			GMethodGlueDataPointer glueData = context->newMethodGlueData(context->getClassData(boundClass.get()), NULL, methodName, gdmtInternal);
			data = new GMapItemMethodData(glueData);
			mapItem->setUserData(data);
		}
		JSFunction * jsFunction = createJsFunction(context, classData, methodName, data->getMethodData()->getMethodList(), gdmtMethodList);
		JSObject * functionObject = JS_GetFunctionObject(jsFunction);
		return ObjectValue(*functionObject);
	}
	
	static ResultType doEnumToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem, const char * /*enumName*/)
	{
		GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
//		doBindEnum(classData->getContext(), metaEnum.get());
		return dummy;
	}

};

void loadCallableParam(jsval * valuePointer, const GSpiderContextPointer & context, InvokeCallableParam * callableParam)
{
	for(int i = 0; i < callableParam->paramCount; ++i) {
		JsValue value = JS_ARGV(context->getJsContext(), valuePointer)[i];
		callableParam->params[i].value = spiderToVariant(context, value , &callableParam->params[i].glueData);
		IMetaTypedItem * typeItem;
		callableParam->params[i].dataType = getSpiderType(value , &typeItem);
		callableParam->params[i].typeItem.reset(typeItem);
	}
}

typedef map<void *, void *> ObjectPrivateDataMap;
void setPrivateData(ObjectPrivateDataMap * dataMap, void * object, void * data)
{
	(*dataMap)[object] = data;
}

void * getPrivateData(ObjectPrivateDataMap * dataMap, void * object)
{
	ObjectPrivateDataMap::iterator it = dataMap->find(object);
	if(it == dataMap->end()) {
		GASSERT(false);
		return NULL;
	}
	else {
		return it->second;
	}
}

ObjectPrivateDataMap objectPrivateDataMap;
ObjectPrivateDataMap functionPrivateDataMap;

void setClassPrivateData(JSContext * jsContext, JSObject * object, void * data)
{
	JSObject * ctor = JS_GetConstructor(jsContext, object);
	JSObject * proto = NULL;
	if(JS_GetPrototype(jsContext, ctor, &proto)) {
		JS_SetPrivate(proto, data);
	}
	else {
		GASSERT(false);
	}
}

void * getClassPrivateData(JSContext * jsContext, JSObject * object)
{
	JSObject * ctor = JS_GetConstructor(jsContext, object);
	JSObject * proto = NULL;
	if(! JS_GetPrototype(jsContext, ctor, &proto)) {
		return NULL;
	}
	return JS_GetPrivate(proto);
}

void setFunctionPrivateData(JSContext * jsContext, JSObject * object, void * data)
{
	setPrivateData(&functionPrivateDataMap, object, data);
}

void * getFunctionPrivateData(JSContext * jsContext, JSObject * object)
{
	return getPrivateData(&functionPrivateDataMap, object);
}

JSBool callbackMethodList(JSContext * jsContext, unsigned int argc, jsval * valuePointer)
{
	ENTER_SPIDERMONKEY()

	JSObject * fobject = JS_THIS_OBJECT(jsContext, valuePointer);
	JsValue callee = JS_CALLEE(jsContext, valuePointer);
	if(callee.isObject()) {
		JSObject * functionObject = &callee.toObject();
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getFunctionPrivateData(jsContext, functionObject));

		GMethodGlueDataPointer methodData(dataWrapper->getAs<GMethodGlueData>());

		GObjectGlueDataPointer objectData;
		
		InvokeCallableParam callableParam(argc);
		loadCallableParam(valuePointer, sharedStaticCast<GSpiderBindingContext>(methodData->getContext()), &callableParam);

		InvokeCallableResult result = doInvokeMethodList(methodData->getContext(), objectData, methodData, &callableParam);
		JsValue resultValue;
		if(result.resultCount > 0) {
			resultValue = methodResultToScript<GSpiderMethods>(methodData->getContext(), result.callable.get(), &result);
		}
		else {
			resultValue = JSVAL_VOID;
		}
		JS_SET_RVAL(jsContext, valuePointer, resultValue);
		return JS_TRUE;
	}
	
	return JS_FALSE;

	LEAVE_SPIDERMONKEY(return JS_FALSE)
}

void doBindMethodList(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, JSObject * owner, const char * name, IMetaList * methodList, GGlueDataMethodType methodType)
{
	//GMethodGlueDataPointer glueData = context->newMethodGlueData(GClassGlueDataPointer(), methodList, name, methodType);
	//GGlueDataWrapper * dataWrapper = newGlueDataWrapper(glueData, context->getGlueDataWrapperPool());

	//JSContext * jsContext = context->getJsContext();
	//JSFunction * jsFunction = JS_DefineFunction(jsContext, owner, name, &callbackMethodList, 0, 0);
	//JSObject * functionObject = JS_GetFunctionObject(jsFunction);

	//setFunctionPrivateData(context->getJsContext(), functionObject, dataWrapper);

	JSFunction * jsFunction = createJsFunction(context, classData, name, methodList, methodType);
	JSObject * functionObject = JS_GetFunctionObject(jsFunction);
	JsValue value = ObjectValue(*functionObject);
	JS_DefineProperty(context->getJsContext(), owner, name, value, NULL, NULL, 0);
}

JSFunction * createJsFunction(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, const char * name, IMetaList * methodList, GGlueDataMethodType methodType)
{
	GMethodGlueDataPointer glueData = context->newMethodGlueData(GClassGlueDataPointer(), methodList, name, methodType);
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(glueData, context->getGlueDataWrapperPool());

	JSContext * jsContext = context->getJsContext();
	JSFunction * jsFunction = JS_NewFunction(jsContext, &callbackMethodList, 0, 0, NULL, name);

	JSObject * functionObject = JS_GetFunctionObject(jsFunction);
	setFunctionPrivateData(context->getJsContext(), functionObject, dataWrapper);

	return jsFunction;
}

JsValue objectToSpider(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData,
				 const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
{
	if(objectAddressFromVariant(instance) == NULL) {
		return JSVAL_NULL;
	}

	GObjectGlueDataPointer objectData = context->newObjectGlueData(classData, instance, flags, cv);
	GGlueDataWrapper * objectWrapper = newGlueDataWrapper(objectData, sharedStaticCast<GSpiderBindingContext>(context)->getGlueDataWrapperPool());

	if(outputGlueData != NULL) {
		*outputGlueData = objectData;
	}

	IMetaClass * metaClass = classData->getMetaClass();
	GMetaMapClass * mapClass = classData->getClassMap();
	JsClassUserData * classUserData = static_cast<JsClassUserData *>(mapClass->getUserData());
	JSObject * object = JS_NewObject(context->getJsContext(), classUserData->getJsClass(), NULL, NULL);

	return ObjectValue(*object);
}

JSBool objectConstructor(JSContext * jsContext, unsigned int argc, jsval * valuePointer)
{
	ENTER_SPIDERMONKEY()

	JsValue callee = JS_CALLEE(jsContext, valuePointer);
	if(callee.isObject()) {
		JSObject * sObject = &callee.toObject();
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getClassPrivateData(jsContext, sObject));
		if(dataWrapper == NULL) {
		}

		GClassGlueDataPointer classData = dataWrapper->getAs<GClassGlueData>();
		GSpiderContextPointer context = sharedStaticCast<GSpiderBindingContext>(classData->getContext());
		
		InvokeCallableParam callableParam(argc);
		loadCallableParam(valuePointer, context, &callableParam);

		void * instance = doInvokeConstructor(context, context->getService(), classData->getMetaClass(), &callableParam);

		if(instance != NULL) {
			JsValue object = objectToSpider(context, classData, instance, GBindValueFlags(bvfAllowGC), opcvNone, NULL);
			JS_SET_RVAL(jsContext, valuePointer, object);
		}
		else {
			raiseCoreException(Error_ScriptBinding_FailConstructObject);
		}

		return JS_TRUE;
	}

	return JS_FALSE;

	LEAVE_SPIDERMONKEY(return JS_FALSE)
}

void doBindClass(const GSpiderContextPointer & context, JSObject * owner, const char * name, IMetaClass * metaClass)
{
	GClassGlueDataPointer classData(context->newClassData(metaClass));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(classData, context->getGlueDataWrapperPool());
	GMetaMapClass * mapClass = classData->getClassMap();
	JsClassUserData * classUserData = new JsClassUserData(name);
	mapClass->setUserData(classUserData);

	JSObject * classObject = JS_InitClass(context->getJsContext(), owner, owner,
		classUserData->getJsClass(), &objectConstructor, 0,
		NULL, NULL, NULL, NULL);
	setClassPrivateData(context->getJsContext(), classObject, dataWrapper);
}

//*********************************************
// Classes implementations
//*********************************************

static JSClass jsClassTemplate = {
    NULL,  /* name */
    0,  /* flags */
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, NULL,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

JsClassUserData::JsClassUserData(const char * className) : super(), className(new char[strlen(className) + 1])
{
	strcpy(this->className.get(), className);

	this->jsClass = jsClassTemplate;

	this->jsClass.name = this->className.get();
	this->jsClass.flags = JSCLASS_HAS_PRIVATE | JSCLASS_NEW_RESOLVE;
	this->jsClass.addProperty = JS_PropertyStub;
	this->jsClass.delProperty = JS_PropertyStub;
	this->jsClass.getProperty = JS_PropertyStub;
	this->jsClass.setProperty = JS_StrictPropertyStub;
	this->jsClass.enumerate = JS_EnumerateStub;
	this->jsClass.resolve = JS_ResolveStub;
	this->jsClass.convert = JS_ConvertStub;
	this->jsClass.finalize = NULL;
	this->jsClass.checkAccess = NULL;
	this->jsClass.call = NULL;
	this->jsClass.construct = NULL;
	this->jsClass.hasInstance = NULL;
	this->jsClass.trace = NULL;
}


GSpiderMonkeyScriptObject::GSpiderMonkeyScriptObject(IMetaService * service, const GScriptConfig & config, JSContext *jsContext, JSObject  * jsGlobalObject)
	: super(GContextPointer(new GSpiderBindingContext(service, config, jsContext, jsGlobalObject)), config), jsContext(jsContext)
{
	this->jsGlobalObject = sharedStaticCast<GSpiderBindingContext>(this->getContext())->getJsGlobalObject();
}

GSpiderMonkeyScriptObject::~GSpiderMonkeyScriptObject()
{
}

GScriptDataType GSpiderMonkeyScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	ENTER_SPIDERMONKEY()

	return sdtUnknown;

	LEAVE_SPIDERMONKEY(return sdtUnknown)
}

void GSpiderMonkeyScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	ENTER_SPIDERMONKEY()

	doBindClass(this->getSpiderContext(), this->jsGlobalObject, name, metaClass);

	LEAVE_SPIDERMONKEY()
}

void GSpiderMonkeyScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	ENTER_SPIDERMONKEY()

	LEAVE_SPIDERMONKEY()
}

GScriptObject * GSpiderMonkeyScriptObject::doCreateScriptObject(const char * name)
{
	ENTER_SPIDERMONKEY()

	return NULL;

	LEAVE_SPIDERMONKEY(return NULL)
}

GScriptFunction * GSpiderMonkeyScriptObject::gainScriptFunction(const char * name)
{
	ENTER_SPIDERMONKEY()

	return NULL;

	LEAVE_SPIDERMONKEY(return NULL)
}

GVariant GSpiderMonkeyScriptObject::invoke(const char * name, const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GVariant GSpiderMonkeyScriptObject::invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount)
{
	ENTER_SPIDERMONKEY()

	return 0;

	LEAVE_SPIDERMONKEY(return GVariant())
}

void GSpiderMonkeyScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.refData().typeData)), "Only fundamental value can be bound via bindFundamental");

	ENTER_SPIDERMONKEY()

	JS_DefineProperty(this->jsContext, this->jsGlobalObject, name, variantToSpider(this->getContext(), value, GBindValueFlags(bvfAllowRaw), NULL), NULL, NULL, 0);
	
	LEAVE_SPIDERMONKEY()
}

void GSpiderMonkeyScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	ENTER_SPIDERMONKEY()

	LEAVE_SPIDERMONKEY()
}

void GSpiderMonkeyScriptObject::bindString(const char * stringName, const char * s)
{
	ENTER_SPIDERMONKEY()

	JSString * jsString = JS_NewStringCopyZ(this->jsContext, s);
	JsValue value = StringValue(jsString);
	JS_DefineProperty(this->jsContext, this->jsGlobalObject, stringName, value, NULL, NULL, 0);

	LEAVE_SPIDERMONKEY()
}

void GSpiderMonkeyScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	ENTER_SPIDERMONKEY()

	LEAVE_SPIDERMONKEY()
}

void GSpiderMonkeyScriptObject::bindRaw(const char * name, const GVariant & value)
{
	ENTER_SPIDERMONKEY()

	LEAVE_SPIDERMONKEY()
}

void GSpiderMonkeyScriptObject::bindMethod(const char * name, void * instance, IMetaMethod * method)
{
	ENTER_SPIDERMONKEY()

	if(method->isStatic()) {
		instance = NULL;
	}

	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method, instance);

	doBindMethodList(this->getSpiderContext(), GClassGlueDataPointer(), this->jsGlobalObject, name, methodList.get(), gdmtMethod);

	LEAVE_SPIDERMONKEY()
}

void GSpiderMonkeyScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	ENTER_SPIDERMONKEY()

	doBindMethodList(this->getSpiderContext(), GClassGlueDataPointer(), this->jsGlobalObject, name, methodList, gdmtMethodList);

	LEAVE_SPIDERMONKEY()
}

IMetaClass * GSpiderMonkeyScriptObject::getClass(const char * className)
{
	ENTER_SPIDERMONKEY()

	return NULL;

	LEAVE_SPIDERMONKEY(return NULL)
}

IMetaEnum * GSpiderMonkeyScriptObject::getEnum(const char * enumName)
{
	ENTER_SPIDERMONKEY()

	return NULL;

	LEAVE_SPIDERMONKEY(return NULL)
}

GVariant GSpiderMonkeyScriptObject::getFundamental(const char * name)
{
	ENTER_SPIDERMONKEY()

	return GVariant();

	LEAVE_SPIDERMONKEY(return GVariant())
}

std::string GSpiderMonkeyScriptObject::getString(const char * stringName)
{
	ENTER_SPIDERMONKEY()

	return "";

	LEAVE_SPIDERMONKEY(return "")
}

void * GSpiderMonkeyScriptObject::getObject(const char * objectName)
{
	ENTER_SPIDERMONKEY()

	return NULL;

	LEAVE_SPIDERMONKEY(return NULL)
}

GVariant GSpiderMonkeyScriptObject::getRaw(const char * name)
{
	ENTER_SPIDERMONKEY()

	return GVariant();

	LEAVE_SPIDERMONKEY(return GVariant())
}

IMetaMethod * GSpiderMonkeyScriptObject::getMethod(const char * methodName, void ** outInstance)
{
	ENTER_SPIDERMONKEY()

	return NULL;

	LEAVE_SPIDERMONKEY(return NULL)
}

IMetaList * GSpiderMonkeyScriptObject::getMethodList(const char * methodName)
{
	ENTER_SPIDERMONKEY()

	return NULL;

	LEAVE_SPIDERMONKEY(return NULL)
}

void GSpiderMonkeyScriptObject::assignValue(const char * fromName, const char * toName)
{
	ENTER_SPIDERMONKEY()

	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsGlobalObject, fromName, &value)) {
		JS_SetProperty(this->jsContext, this->jsGlobalObject, toName, &value);
	}

	LEAVE_SPIDERMONKEY()
}

bool GSpiderMonkeyScriptObject::valueIsNull(const char * name)
{
	ENTER_SPIDERMONKEY()

	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsGlobalObject, name, &value)) {
		return value.isNullOrUndefined();
	}
	else {
		return false;
	}
	
	LEAVE_SPIDERMONKEY(return false)
}

void GSpiderMonkeyScriptObject::nullifyValue(const char * name)
{
	ENTER_SPIDERMONKEY()

	JsValue value = JSVAL_NULL;
	JS_SetProperty(this->jsContext, this->jsGlobalObject, name, &value);

	LEAVE_SPIDERMONKEY()
}

void GSpiderMonkeyScriptObject::bindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	ENTER_SPIDERMONKEY()

	this->getContext()->bindScriptCoreService(this, name, libraryLoader);

	LEAVE_SPIDERMONKEY()
}


}


} // namespace cpgf

using namespace cpgf;

const char * greeting(int message)
{
	cout << "Hello, " << message << endl;
	return "that's fine";
}

class Point
{
public:
	Point() : x(0), y(0) {
	}

	Point(int x, int y) : x(x), y(y) {
	}

	void extend(int scale) {
		x *= scale;
		y *= scale;
	}

	int getArea() const {
		return x * y;
	}

public:
	int x;
	int y;
};

template <typename Define>
void reflectPoint(Define define)
{
	define._method("extend", &Point::extend);
	define._method("getArea", &Define::ClassType::getArea);
	define.template _constructor<void * ()>();
	define.template _constructor<void * (int, int)>();
	define._field("x", &Point::x);
	define._field("y", &Point::y);
}

void buildMetaData()
{
	GDefineMetaClass<Point> defineForPoint = GDefineMetaClass<Point>::declare("Point");
	reflectPoint(defineForPoint);
	GDefineMetaGlobal defineForGlobal;
	defineForGlobal._method("greeting", &greeting);
	defineForGlobal._class(defineForPoint);
}

void testSpidermonkey()
{
	buildMetaData();

	GScopedInterface<IMetaService> service(createDefaultMetaService());

	GScopedInterface<IMetaMethod> method;
	GScopedInterface<IMetaField> field;
	GScopedInterface<IMetaModule> globalModule(service->getModuleAt(0));
	GScopedInterface<IMetaClass> globalMetaClass(globalModule->getGlobalMetaClass());
	GScopedInterface<IMetaClass> pointMetaClass(service->findClassByName("Point"));

	method.reset(globalMetaClass->getMethod("greeting"));
	
	JSRuntime *runtime=NULL;
	JSContext *jsContext=NULL;
	JSObject  *global=NULL;

	runtime = JS_NewRuntime(1024L*1024L, JS_NO_HELPER_THREADS);
	jsContext = JS_NewContext(runtime, 8192);
	global  = JS_NewObject(jsContext, NULL, NULL, NULL);
	
	if (!JS_InitStandardClasses(jsContext, global))
		return;

	GSpiderMonkeyScriptObject scriptObject(NULL, GScriptConfig(), jsContext, global);
	scriptObject.bindFundamental("vI", 38);
	scriptObject.bindString("vS", "What");
	scriptObject.bindMethod("hello", NULL, method.get());
	scriptObject.bindClass("Point", pointMetaClass.get());

	char *script=""
		"var vNull = null;"
		"var pt1 = new Point(1, 2);"
		"var pt2 = new Point(21, 22);"
		"var h = hello(2);"
		"\"\" + h + \"  \" + pt1.x + \"  \" + pt2.x + \" -- \" + vS;"
	;
	jsval rval;

	JS_EvaluateScript(jsContext, global,script, (unsigned int)strlen(script), "script", 1, &rval);

	if(JSVAL_IS_STRING(rval)) {
		JSString * jsString = rval.toString();
		const jschar * ws = JS_GetStringCharsZ(jsContext, jsString);
		GScopedArray<char> s(wideStringToString(ws));
		cout << "Result: " << s.get() << endl;
	}
	else {
		cout << "Result is wrong!" << endl;
	}
	cout << scriptObject.valueIsNull("vNull") << endl;
	scriptObject.nullifyValue("vI");
	cout << scriptObject.valueIsNull("vI") << endl;

	JS_DestroyContext(jsContext);
	JS_DestroyRuntime(runtime);
	JS_ShutDown();
return;
	int s;
	cin >> s;
}
