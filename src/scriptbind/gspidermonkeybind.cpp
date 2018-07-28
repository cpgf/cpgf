#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gerrorcode.h"
#include "cpgf/gstringutil.h"

#include "gbindcommon.h"
#include "gbindapiimpl.h"
#include "../gstaticuninitializerorders.h"

#include <string>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127 4100 4800 4512 4480 4267)
#endif

#ifndef UINT32_MAX
#	define UINT32_MAX  ((uint32_t)-1)
#endif

#include "jsapi.h"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996) // warning C4996: 'mbstowcs': This function or variable may be unsafe. Consider using mbstowcs_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
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

class GScopedJsObject : public GNoncopyable
{
public:
	GScopedJsObject() : jsContext(nullptr), jsObject(nullptr) {
	}

	GScopedJsObject(JSContext * jsContext, JSObject  * jsObject) : jsContext(nullptr), jsObject(nullptr) {
		this->reset(jsContext, jsObject);
	}

	~GScopedJsObject() {
		this->reset(nullptr, nullptr);
	}

	JSObject  * getJsObject() const {
		return this->jsObject;
	}

	void reset(JSContext * jsContext, JSObject  * jsObject) {
		if(this->jsObject != nullptr) {
			JS_RemoveObjectRoot(this->jsContext, &this->jsObject);
		}

		this->jsContext = jsContext;
		this->jsObject = jsObject;

		if(this->jsObject != nullptr) {
			JS_AddObjectRoot(this->jsContext, &this->jsObject);
		}
	}

private:
	JSContext * jsContext;
	JSObject  * jsObject;
};

class GSharedJsObject
{
public:
	GSharedJsObject() : jsContext(nullptr), jsObject(nullptr) {
	}

	GSharedJsObject(JSContext * jsContext, JSObject  * jsObject) : jsContext(jsContext), jsObject(jsObject) {
		this->retain();
	}

	GSharedJsObject(const GSharedJsObject & other) : jsContext(other.jsContext), jsObject(other.jsObject) {
		this->retain();
	}

	GSharedJsObject & operator = (GSharedJsObject other) {
		this->swap(other);
		return *this;
	}

	~GSharedJsObject() {
		this->release();
	}

	JSObject  * getJsObject() const {
		return this->jsObject;
	}

	void swap(GSharedJsObject & other) {
		std::swap(this->jsContext, other.jsContext);
		std::swap(this->jsObject, other.jsObject);
	}

private:
	void retain() {
		if(this->jsObject != nullptr) {
			JS_AddObjectRoot(this->jsContext, &this->jsObject);
		}
	}

	void release() {
		if(this->jsObject != nullptr) {
			JS_RemoveObjectRoot(this->jsContext, &this->jsObject);
		}
	}

private:
	JSContext * jsContext;
	JSObject  * jsObject;
};

class JsClassUserData : public GUserData
{
private:
	typedef GUserData super;
	typedef GStringMap<GAccessibleGlueDataPointer> AccessibleMapType;

public:
	explicit JsClassUserData(const char * className);

	JSClass * getJsClass() {
		return &this->jsClass;
	}

	void setClassObject(JSContext * jsContext, JSObject * classObject) {
		this->classObject.reset(jsContext, classObject);
	}

	JSObject * getClassObject() const {
		return this->classObject.getJsObject();
	}

	bool hasAccessible() const {
		return this->accessibleMap;
	}

	void addAccessibleGlueData(const char * name, const GAccessibleGlueDataPointer & accessibleGlueData) {
		if(! this->accessibleMap) {
			this->accessibleMap.reset(new AccessibleMapType);
		}
		this->accessibleMap->set(name, accessibleGlueData);
	}

	GAccessibleGlueDataPointer getAccessibleGlueData(const char * name) {
		if(this->hasAccessible()) {
			AccessibleMapType::iterator it = this->accessibleMap->find(name);
			if(it != this->accessibleMap->end()) {
				return it->second;
			}
		}

		return GAccessibleGlueDataPointer();
	}

private:
	string className;
	JSClass jsClass;
	GScopedJsObject classObject;
	std::unique_ptr<AccessibleMapType> accessibleMap;
};

class GSpiderBindingContext : public GBindingContext, public GShareFromBase
{
private:
	typedef GBindingContext super;

public:
	GSpiderBindingContext(IMetaService * service, JSContext * jsContext, JSObject  * jsObject)
		: super(service), jsContext(jsContext), jsObject(jsObject)
	{
	}

	JSContext * getJsContext() const {
		return this->jsContext;
	}

	JSObject  * getJsGlobalObject() const {
		return this->jsObject;
	}

	GGlueDataWrapperPool * getGlueDataWrapperPool() const {
		return &this->glueDataWrapperPool;
	}

private:
	JSContext * jsContext;
	JSObject  * jsObject;
	mutable GGlueDataWrapperPool glueDataWrapperPool;
};

typedef std::shared_ptr<GSpiderBindingContext> GSpiderContextPointer;

class GSpiderScriptFunction : public GScriptFunctionBase
{
private:
	typedef GScriptFunctionBase super;

public:
	GSpiderScriptFunction(const GSpiderContextPointer & context, JSObject * self, JSObject * function);
	virtual ~GSpiderScriptFunction();

	virtual GScriptValue invoke(const GVariant * params, size_t paramCount);
	virtual GScriptValue invokeIndirectly(GVariant const * const * params, size_t paramCount);

private:
	JSContext * jsContext;
	GScopedJsObject self;
	GScopedJsObject function;
};

class GSpiderScriptArray : public GScriptArrayBase
{
private:
	typedef GScriptArrayBase super;

public:
	GSpiderScriptArray(const GSpiderContextPointer & context, JSObject * arrayObject);
	virtual ~GSpiderScriptArray();

	virtual size_t getLength();
	virtual GScriptValue getValue(size_t index);
	virtual void setValue(size_t index, const GScriptValue & value);

	virtual bool maybeIsScriptArray(size_t index);
	virtual GScriptValue getAsScriptArray(size_t index);
	virtual GScriptValue createScriptArray(size_t index);

private:
	GScopedJsObject arrayObject;
};

class GSpiderMonkeyScriptObject : public GScriptObjectBase
{
private:
	typedef GScriptObjectBase super;

public:
	GSpiderMonkeyScriptObject(IMetaService * service, JSContext *jsContext, JSObject  * jsObject);
	GSpiderMonkeyScriptObject(const GSpiderMonkeyScriptObject & other, JSContext *jsContext, JSObject  * jsObject);
	virtual ~GSpiderMonkeyScriptObject();

	virtual GScriptObject * doCreateScriptObject(const char * name);

	virtual GScriptValue getScriptFunction(const char * name);

	virtual GScriptValue invoke(const char * name, const GVariant * params, size_t paramCount);
	virtual GScriptValue invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);

	virtual bool maybeIsScriptArray(const char * name);
	virtual GScriptValue getAsScriptArray(const char * name);
	virtual GScriptValue createScriptArray(const char * name);

	GSpiderContextPointer getSpiderContext() const {
		return std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext());
	}

protected:
	virtual GScriptValue doGetValue(const char * name);
	virtual void doSetValue(const char * name, const GScriptValue & value);

private:
	GMethodGlueDataPointer doGetMethodData(const char * methodName);

private:
	JSContext *jsContext;
	GScopedJsObject jsObject;
};

JSObject * createClassBinding(const GSpiderContextPointer & context, JSObject * owner, const GClassGlueDataPointer & classData);
JsValue helperBindClass(const GSpiderContextPointer & context, JSObject * owner, IMetaClass * metaClass);
JsValue helperBindMethodList(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, IMetaList * methodList);
JSFunction * createJsFunction(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, IMetaList * methodList);
JsValue objectToSpider(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData,
				 const GVariant & instance, const GBindValueFlags & flags, const GScriptInstanceCv cv, GGlueDataPointer * outputGlueData);
JsValue variantToSpider(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData);
JsValue rawToSpider(const GSpiderContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData);
JSObject * createEnumBinding(const GSpiderContextPointer & context, IMetaEnum * metaEnum);
JsValue helperBindEnum(const GSpiderContextPointer & context, IMetaEnum * metaEnum);
void objectFinalizer(JSFreeOp * jsop, JSObject * object);
JSBool enumGetter(JSContext *jsContext, JSHandleObject obj, JSHandleId id, JSMutableHandleValue vp);
JSBool enumSetter(JSContext * jsContext, JSHandleObject obj, JSHandleId id, JSBool strict, JSMutableHandleValue vp);


//*********************************************
// Global function implementations
//*********************************************


void error(JSContext * jsContext, const char * message)
{
	JS_ReportError(jsContext, "%s", message);
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
		return nullptr;
	}
	else {
		return it->second;
	}
}

ObjectPrivateDataMap classPrivateDataMap;
ObjectPrivateDataMap objectPrivateDataMap;
ObjectPrivateDataMap functionPrivateDataMap;

void * getClassPrivateData(JSContext * jsContext, JSObject * object);
void setClassPrivateData(JSContext * jsContext, JSObject * object, void * data)
{
	JSObject * ctor = JS_GetConstructor(jsContext, object);
	JSObject * proto = ctor;
	if(proto != nullptr) {
		setPrivateData(&classPrivateDataMap, proto, data);
	}
	else {
		GASSERT(false);
	}
}

void * getClassPrivateData(JSContext * /*jsContext*/, JSObject * object)
{
	return getPrivateData(&classPrivateDataMap, object);
}

void setObjectPrivateData(JSObject * object, void * data)
{
	setPrivateData(&objectPrivateDataMap, object, data);
}

void * getObjectPrivateData(JSObject * object)
{
	return getPrivateData(&objectPrivateDataMap, object);
}

void * getObjectOrClassPrivateData(JSContext * jsContext, JSObject * object)
{
	void * data = getObjectPrivateData(object);
	if(data == nullptr) {
		data = getClassPrivateData(jsContext, object);
	}
	return data;
}

void setFunctionPrivateData(JSObject * object, void * data)
{
	setPrivateData(&functionPrivateDataMap, object, data);
}

void * getFunctionPrivateData(JSObject * object)
{
	return getPrivateData(&functionPrivateDataMap, object);
}

GGlueDataWrapper * getNativeObjectOrClass(JSContext * jsContext, JSObject * object)
{
	while(object != nullptr) {
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectOrClassPrivateData(jsContext, object));
		if(dataWrapper != nullptr) {
			return dataWrapper;
		}
		JSObject * temp = object;
		if(JS_GetPrototype(jsContext, temp, &object) != JS_TRUE) {
			break;
		}
	}

	return nullptr;
}

GGlueDataWrapper * getNativeObject(JSContext * jsContext, JSObject * object)
{
	while(object != nullptr) {
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectPrivateData(object));
		if(dataWrapper != nullptr) {
			return dataWrapper;
		}
		JSObject * temp = object;
		if(JS_GetPrototype(jsContext, temp, &object) != JS_TRUE) {
			break;
		}
	}

	return nullptr;
}

GGlueDataWrapper * getNativeClass(JSContext * jsContext, JSObject * object)
{
	while(object != nullptr) {
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getClassPrivateData(jsContext, object));
		if(dataWrapper != nullptr) {
			return dataWrapper;
		}
		JSObject * temp = object;
		if(JS_GetPrototype(jsContext, temp, &object) != JS_TRUE) {
			break;
		}
	}

	return nullptr;
}

struct GSpiderMethods
{
	typedef JsValue ResultType;

	static ResultType doObjectToScript(const GContextPointer & context, const GClassGlueDataPointer & classData,
		const GVariant & instance, const GBindValueFlags & flags, const GScriptInstanceCv cv, GGlueDataPointer * outputGlueData)
	{
		return objectToSpider(std::static_pointer_cast<GSpiderBindingContext>(context), classData, instance, flags, cv, outputGlueData);
	}

	static ResultType doVariantToScript(const GContextPointer & context, const GVariant & value, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
	{
		return variantToSpider(context, value, flags, outputGlueData);
	}

	static ResultType doRawToScript(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
	{
		return rawToSpider(std::static_pointer_cast<GSpiderBindingContext>(context), value, outputGlueData);
	}

	static ResultType doClassToScript(const GContextPointer & context, IMetaClass * metaClass)
	{
		GSpiderContextPointer spiderContext = std::static_pointer_cast<GSpiderBindingContext>(context);
		return helperBindClass(spiderContext, spiderContext->getJsGlobalObject(), metaClass);
	}

	static ResultType doStringToScript(const GContextPointer & context, const char * s)
	{
		JSString * jsString = JS_NewStringCopyZ(std::static_pointer_cast<GSpiderBindingContext>(context)->getJsContext(), s);
		return StringValue(jsString);
	}

	static ResultType doWideStringToScript(const GContextPointer & context, const wchar_t * ws)
	{
		std::string s(wideStringToString(ws));
		JSString * jsString = JS_NewStringCopyZ(std::static_pointer_cast<GSpiderBindingContext>(context)->getJsContext(), s.c_str());
		return StringValue(jsString);
	}

	static bool isSuccessResult(const ResultType & result)
	{
		return ! result.isUndefined();
	}

	static ResultType defaultValue()
	{
		return JSVAL_VOID;
	}

	static ResultType doMethodsToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getUserData());
		GSpiderContextPointer context = std::static_pointer_cast<GSpiderBindingContext>(classData->getBindingContext());
		if(data == nullptr) {
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));

			GScopedInterface<IMetaList> metaList(getMethodListFromMapItem(mapItem, getGlueDataInstanceAddress(objectData)));
			GMethodGlueDataPointer glueData = context->newMethodGlueData(metaList.get());
			data = new GMapItemMethodData(glueData);
			mapItem->setUserData(data);
		}
		JSFunction * jsFunction = createJsFunction(context, classData, data->getMethodData()->getMethodList());
		JSObject * functionObject = JS_GetFunctionObject(jsFunction);
		return ObjectValue(*functionObject);
	}

	static ResultType doEnumToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem, const char * /*enumName*/)
	{
		GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
		JSObject * enumObject = createEnumBinding(std::static_pointer_cast<GSpiderBindingContext>(classData->getBindingContext()), metaEnum.get());
		return ObjectValue(*enumObject );
	}

};

JsValue variantToSpider(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
{
	GVariant value = getVariantRealValue(data);
	GMetaType type = getVariantRealMetaType(data);

	GVariantType vt = static_cast<GVariantType>((GVtType)value.getType() & ~(GVtType)GVariantType::maskByReference);

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

	if(!vtIsInterface(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == nullptr) {
		return JSVAL_NULL;
	}

	if(variantIsAnyString(value)) {
		JSString * jsString = JS_NewStringCopyZ(std::static_pointer_cast<GSpiderBindingContext>(context)->getJsContext(), stringFromVariant(value).c_str());
		return StringValue(jsString);
	}

	return complexVariantToScript<GSpiderMethods>(context, value, type, flags, outputGlueData);
}

char * jsStringToString(JSContext * jsContext, JSString * jsString)
{
	char * s = JS_EncodeString(jsContext, jsString);
	size_t len = strlen(s);
	char * ns = new char[len + 1];
	strcpy(ns, s);
	JS_free(jsContext, s);
	return ns;
}

GScriptValue spiderUserDataToScriptValue(const GSpiderContextPointer & context, JsValue value, GGlueDataPointer * outputGlueData)
{
	if(value.isObject()) {
		JSObject * object = &value.toObject();
		GGlueDataWrapper * dataWrapper = getNativeObjectOrClass(context->getJsContext(), object);
		if(dataWrapper == nullptr) {
			dataWrapper = static_cast<GGlueDataWrapper *>(getFunctionPrivateData(object));
		}
		if(dataWrapper != nullptr) {
			GGlueDataPointer glueData = dataWrapper->getData();
			if(outputGlueData != nullptr) {
				*outputGlueData = glueData;
			}
			return glueDataToScriptValue(glueData);
		}
		else {
			if(JS_ObjectIsFunction(context->getJsContext(), object)) {
				GScopedInterface<IScriptFunction> func(new ImplScriptFunction(new GSpiderScriptFunction(context, context->getJsGlobalObject(), object), true));

				return GScriptValue::fromScriptFunction(func.get());
			}
			else {
				GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GSpiderMonkeyScriptObject(context->getService(), context->getJsContext(), object), true));

				return GScriptValue::fromScriptObject(scriptObject.get());
			}
		}
	}

	return GScriptValue();
}

GScriptValue spiderToScriptValue(const GSpiderContextPointer & context, JsValue value, GGlueDataPointer * outputGlueData)
{
	if(value.isUndefined()) {
		return GScriptValue();
	}

	if(value.isBoolean()) {
		return GScriptValue::fromPrimary(bool(value.toBoolean()));
	}

	if(value.isInt32()) {
		return GScriptValue::fromPrimary(value.toInt32());
	}

	if(value.isNull()) {
		return GScriptValue::fromNull();
	}

	if(value.isNumber()) {
		return GScriptValue::fromPrimary(value.toNumber());
	}

	if(value.isDouble()) {
		return GScriptValue::fromPrimary(value.toDouble());
	}

	if(value.isString()) {
		JSString * jsString = value.toString();
		std::unique_ptr<char[]> s(jsStringToString(context->getJsContext(), jsString));
		return GScriptValue::fromPrimary(createStringVariant(s.get()));
	}

	if(value.isObject()) {
		return spiderUserDataToScriptValue(context, value, outputGlueData);
	}

	return GScriptValue();
}

JsValue rawToSpider(const GSpiderContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
{
	GRawGlueDataPointer rawData(context->newRawGlueData(value));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(rawData, context->getGlueDataWrapperPool());

	JSObject * object = JS_NewObject(context->getJsContext(), nullptr, nullptr, nullptr);
	setObjectPrivateData(object, dataWrapper);

	if(outputGlueData != nullptr) {
		*outputGlueData = rawData;
	}

	return ObjectValue(*object);
}

void loadCallableParam(jsval * valuePointer, const GSpiderContextPointer & context, InvokeCallableParam * callableParam)
{
	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		JsValue value = JS_ARGV(context->getJsContext(), valuePointer)[i];
		callableParam->params[i].value = spiderToScriptValue(context, value , &callableParam->params[i].paramGlueData);
	}
}
JSBool failedResult()
{
	return JS_FALSE;
}

JSBool callbackMethodList(JSContext * jsContext, unsigned int argc, jsval * valuePointer)
{
	ENTER_SPIDERMONKEY()

	JSObject * selfObject = JS_THIS_OBJECT(jsContext, valuePointer);
	JsValue callee = JS_CALLEE(jsContext, valuePointer);
	if(callee.isObject()) {
		JSObject * functionObject = &callee.toObject();
		GGlueDataWrapper * methodDataWrapper = static_cast<GGlueDataWrapper *>(getFunctionPrivateData(functionObject));
		GMethodGlueDataPointer methodData(methodDataWrapper->getAs<GMethodGlueData>());

		GObjectGlueDataPointer objectData;
		GGlueDataWrapper * objectDataWrapper = getNativeObjectOrClass(jsContext, selfObject);
		if(objectDataWrapper != nullptr && objectDataWrapper->getData()->getType() == gdtObject) {
			objectData = objectDataWrapper->getAs<GObjectGlueData>();
		}

		GContextPointer bindingContext(methodData->getBindingContext());
		InvokeCallableParam callableParam(argc, bindingContext->borrowScriptContext());
		loadCallableParam(valuePointer, std::static_pointer_cast<GSpiderBindingContext>(bindingContext), &callableParam);

		InvokeCallableResult result = doInvokeMethodList(bindingContext, objectData, methodData, &callableParam);
		JsValue resultValue;
		if(result.resultCount > 0) {
			resultValue = methodResultToScript<GSpiderMethods>(bindingContext, result.callable.get(), &result);
		}
		else {
			resultValue = JSVAL_VOID;
		}
		JS_SET_RVAL(jsContext, valuePointer, resultValue);
		return JS_TRUE;
	}

	return failedResult();

	LEAVE_SPIDERMONKEY(return failedResult())
}

JsValue helperBindMethodList(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData,
	IMetaList * methodList)
{
	JSFunction * jsFunction = createJsFunction(context, classData, methodList);
	JSObject * functionObject = JS_GetFunctionObject(jsFunction);
	return ObjectValue(*functionObject);
}

JSFunction * createJsFunction(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, IMetaList * methodList)
{
	GMethodGlueDataPointer glueData = context->newMethodGlueData(methodList);
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(glueData, context->getGlueDataWrapperPool());

	JSContext * jsContext = context->getJsContext();
	JSFunction * jsFunction = JS_NewFunction(jsContext, &callbackMethodList, 0, 0, nullptr, nullptr);

	JSObject * functionObject = JS_GetFunctionObject(jsFunction);
	setFunctionPrivateData(functionObject, dataWrapper);

	return jsFunction;
}

JsValue objectToSpider(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData,
				 const GVariant & instance, const GBindValueFlags & flags, const GScriptInstanceCv cv, GGlueDataPointer * outputGlueData)
{
	void * instanceAddress = objectAddressFromVariant(instance);

	if(instanceAddress == nullptr) {
		return JSVAL_NULL;
	}

	GObjectGlueDataPointer objectData = context->newObjectGlueData(classData, instance, flags, cv);
	GGlueDataWrapper * objectWrapper = newGlueDataWrapper(objectData, std::static_pointer_cast<GSpiderBindingContext>(context)->getGlueDataWrapperPool());

	if(outputGlueData != nullptr) {
		*outputGlueData = objectData;
	}

	createClassBinding(context, context->getJsGlobalObject(), classData);
	GMetaMapClass * mapClass = classData->getClassMap();
	JsClassUserData * classUserData = static_cast<JsClassUserData *>(mapClass->getUserData());

	JSObject * object = JS_NewObject(context->getJsContext(), classUserData->getJsClass(), nullptr, nullptr);
	setObjectPrivateData(object, objectWrapper);

	return ObjectValue(*object);
}

JSBool propertyGetter(JSContext *jsContext, JSHandleObject obj, JSHandleId id, JSMutableHandleValue vp)
{
	ENTER_SPIDERMONKEY()

	JsValue idValue;
	if(! JS_IdToValue(jsContext, id, &idValue)) {
		return failedResult();
	}
	if(idValue.isString()) {
		GGlueDataWrapper * dataWrapper = getNativeObjectOrClass(jsContext, obj);

		if(dataWrapper == nullptr) {
			return JS_PropertyStub(jsContext, obj, id, vp);
		}

		if(dataWrapper->getData()->getType() == gdtEnum) {
			return enumGetter(jsContext, obj, id, vp);
		}

		JSString * jsString = idValue.toString();
		std::unique_ptr<char[]> name(jsStringToString(jsContext, jsString));

		GClassGlueDataPointer classData;
		if(dataWrapper->getData()->getType() == gdtObject) {
			classData = dataWrapper->getAs<GObjectGlueData>()->getClassData();
		}
		else {
			classData = dataWrapper->getAs<GClassGlueData>();
		}
		JsClassUserData * userData = static_cast<JsClassUserData *>(classData->getClassMap()->getUserData());
		if(userData->hasAccessible()) {
			GAccessibleGlueDataPointer accessibleGlueData = userData->getAccessibleGlueData(name.get());
			if(accessibleGlueData) {
				vp.set(accessibleToScript<GSpiderMethods>(accessibleGlueData->getBindingContext(), accessibleGlueData->getAccessible(), accessibleGlueData->getInstanceAddress(), false));

				return JS_TRUE;
			}
		}
		if(! dataWrapper->getData()->getBindingContext()) {
			return JS_PropertyStub(jsContext, obj, id, vp);
		}


		JsValue value = namedMemberToScript<GSpiderMethods>(dataWrapper->getData(), name.get());

		if(value.isUndefined()) {
			return failedResult();
		}
		else {
			vp.set(value);
			return JS_TRUE;
		}
	}

	return failedResult();

	LEAVE_SPIDERMONKEY(return failedResult())
}

JSBool propertySetter(JSContext * jsContext, JSHandleObject obj, JSHandleId id, JSBool strict, JSMutableHandleValue vp)
{
	ENTER_SPIDERMONKEY()

	JsValue idValue;
	if(! JS_IdToValue(jsContext, id, &idValue)) {
		return failedResult();
	}
	if(idValue.isString()) {
		GGlueDataWrapper * dataWrapper = getNativeObjectOrClass(jsContext, obj);
		if(dataWrapper == nullptr) {
			return JS_StrictPropertyStub(jsContext, obj, id, strict, vp);
		}

		if(dataWrapper->getData()->getType() == gdtEnum) {
			return enumSetter(jsContext, obj, id, strict, vp);
		}

		JSString * jsString = idValue.toString();
		std::unique_ptr<char[]> name(jsStringToString(jsContext, jsString));

		GClassGlueDataPointer classData;
		if(dataWrapper->getData()->getType() == gdtObject) {
			classData = dataWrapper->getAs<GObjectGlueData>()->getClassData();
		}
		else {
			classData = dataWrapper->getAs<GClassGlueData>();
		}
		JsClassUserData * userData = static_cast<JsClassUserData *>(classData->getClassMap()->getUserData());
		if(userData->hasAccessible()) {
			GAccessibleGlueDataPointer accessibleGlueData = userData->getAccessibleGlueData(name.get());
			if(accessibleGlueData) {
				GScriptValue v = spiderToScriptValue(std::static_pointer_cast<GSpiderBindingContext>(accessibleGlueData->getBindingContext()),
					vp, nullptr);
				metaSetValue(accessibleGlueData->getAccessible(), accessibleGlueData->getInstanceAddress(), v.getValue());

				return JS_TRUE;
			}
		}
		if(! dataWrapper->getData()->getBindingContext()) {
			return JS_StrictPropertyStub(jsContext, obj, id, strict, vp);
		}

		if(getGlueDataCV(dataWrapper->getData()) == opcvConst) {
			raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);
		}
		else {
			GGlueDataPointer valueGlueData;

			GScriptValue v = spiderToScriptValue(std::static_pointer_cast<GSpiderBindingContext>(dataWrapper->getData()->getBindingContext()), vp, &valueGlueData);
			if(setValueOnNamedMember(dataWrapper->getData(), name.get(), v, valueGlueData)) {
				return JS_TRUE;
			}
		}
	}

	return failedResult();

	LEAVE_SPIDERMONKEY(return failedResult())
}

JSBool enumGetter(JSContext *jsContext, JSHandleObject obj, JSHandleId id, JSMutableHandleValue vp)
{
	ENTER_SPIDERMONKEY()

	JsValue idValue;
	if(! JS_IdToValue(jsContext, id, &idValue)) {
		return failedResult();
	}
	if(idValue.isString()) {
		GGlueDataWrapper * dataWrapper = getNativeObjectOrClass(jsContext, obj);
		if(dataWrapper->getData()->getType() == gdtEnum) {
			JSString * jsString = idValue.toString();
			std::unique_ptr<char[]> name(jsStringToString(jsContext, jsString));

			IMetaEnum * metaEnum = dataWrapper->getAs<GEnumGlueData>()->getMetaEnum();
			int32_t index = metaEnum->findKey(name.get());
			if(index >= 0) {
				vp.set(variantToSpider(dataWrapper->getData()->getBindingContext(), metaGetEnumValue(metaEnum, index), GBindValueFlags(), nullptr));
			}
			else {
				raiseCoreException(Error_ScriptBinding_CantFindEnumKey, *name);
			}
			return JS_TRUE;
		}
	}

	return failedResult();

	LEAVE_SPIDERMONKEY(return failedResult())
}

JSBool enumSetter(JSContext * jsContext, JSHandleObject /*obj*/, JSHandleId /*id*/, JSBool /*strict*/, JSMutableHandleValue /*vp*/)
{
	ENTER_SPIDERMONKEY()

	raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);

	return failedResult();

	LEAVE_SPIDERMONKEY(return failedResult())
}

static JSClass jsEnumClass = {
	"__internal__enum__",  // name
	JSCLASS_HAS_PRIVATE,  // flags
	JS_PropertyStub, // addProperty
	JS_PropertyStub, // delProperty
	&enumGetter, // getProperty
	&enumSetter, // setProperty
	JS_EnumerateStub, // enumerate
	JS_ResolveStub, // resolve
	JS_ConvertStub, // convert
	&objectFinalizer, // finalize
	JSCLASS_NO_OPTIONAL_MEMBERS
};

JSObject * enumClassObject = nullptr;

JSObject * createEnumBinding(const GSpiderContextPointer & context, IMetaEnum * metaEnum)
{
	if(enumClassObject == nullptr) {
		JSObject * owner = context->getJsGlobalObject();
		JSObject * obj = JS_InitClass(context->getJsContext(), owner, owner, &jsEnumClass, nullptr, 0, nullptr, nullptr, nullptr, nullptr);
		enumClassObject = obj;
	}
	GEnumGlueDataPointer enumGlueData(context->newEnumGlueData(metaEnum));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(enumGlueData, context->getGlueDataWrapperPool());
	JSObject * enumObject = JS_NewObject(context->getJsContext(), &jsEnumClass, nullptr, nullptr);
	setObjectPrivateData(enumObject, dataWrapper);
	return enumObject;
}

JsValue helperBindEnum(const GSpiderContextPointer & context, IMetaEnum * metaEnum)
{
	JSObject * enumObject = createEnumBinding(context, metaEnum);
	return ObjectValue(*enumObject);
}

void helperBindAccessible(const GSpiderContextPointer & context, JSObject * owner,
	const char * name, void * instance, IMetaAccessible * accessible)
{
	GGlueDataWrapper * dataWrapper = getNativeObjectOrClass(context->getJsContext(), owner);
	JsClassUserData * userData = nullptr;
	if(dataWrapper != nullptr) {
		if(dataWrapper->getData()->getType() == gdtClass) {
			userData = static_cast<JsClassUserData *>(dataWrapper->getAs<GClassGlueData>()->getClassMap()->getUserData());
		}
		else {
			if(dataWrapper->getData()->getType() == gdtObject) {
				userData = static_cast<JsClassUserData *>(dataWrapper->getAs<GObjectGlueData>()->getClassData()->getClassMap()->getUserData());
			}
		}
	}
	if(userData == nullptr) {
		raiseCoreException(Error_ScriptBinding_NotSupportedFeature, "bindAccessible", "Mozilla SpiderMonkey");
		return;
	}
	else {
		userData->addAccessibleGlueData(name, context->newAccessibleGlueData(instance, accessible));
	}
	JS_DefineProperty(context->getJsContext(), owner, name, JsValue(), &propertyGetter, &propertySetter, JSPROP_PERMANENT);
}

JSBool objectConstructor(JSContext * jsContext, unsigned int argc, jsval * valuePointer)
{
	ENTER_SPIDERMONKEY()

	JsValue callee = JS_CALLEE(jsContext, valuePointer);
	if(callee.isObject()) {
		JSObject * sObject = &callee.toObject();
		GGlueDataWrapper * dataWrapper = getNativeClass(jsContext, sObject);
		if(dataWrapper == nullptr) {
		}

		GClassGlueDataPointer classData = dataWrapper->getAs<GClassGlueData>();
		GSpiderContextPointer context = std::static_pointer_cast<GSpiderBindingContext>(classData->getBindingContext());

		InvokeCallableParam callableParam(argc, context->borrowScriptContext());
		loadCallableParam(valuePointer, context, &callableParam);

		void * instance = doInvokeConstructor(context, context->getService(), classData->getMetaClass(), &callableParam);

		if(instance != nullptr) {
			GGlueDataPointer objectData;
			JsValue object = objectToSpider(context, classData, instance, GBindValueFlags(bvfAllowGC), opcvNone, &objectData);
			JS_SET_RVAL(jsContext, valuePointer, object);
		}
		else {
			raiseCoreException(Error_ScriptBinding_FailConstructObject);
		}

		return JS_TRUE;
	}

	return failedResult();

	LEAVE_SPIDERMONKEY(return failedResult())
}

void bindClassItems(const GSpiderContextPointer & context, JSObject * owner, IMetaClass * metaClass)
{
	GScopedInterface<IMetaItem> item;
	uint32_t count = metaClass->getMetaCount();
	for(uint32_t i = 0; i < count; ++i) {
		item.reset(metaClass->getMetaAt(i));
		if(item->isStatic()) {
			JS_DefineProperty(context->getJsContext(), owner, item->getName(), JSVAL_NULL, &propertyGetter, &propertySetter, JSPROP_PERMANENT | JSPROP_SHARED);
			if(metaIsEnum(item->getCategory())) {
				IMetaEnum * metaEnum = gdynamic_cast<IMetaEnum *>(item.get());
				uint32_t keyCount = metaEnum->getCount();
				for(uint32_t k = 0; k < keyCount; ++k) {
					JS_DefineProperty(context->getJsContext(), owner, metaEnum->getKey(k), JSVAL_NULL, &propertyGetter, &propertySetter, JSPROP_PERMANENT | JSPROP_SHARED); // Don't readonly, let cpgf raise exception.  | JSPROP_READONLY);
				}
			}
		}
		else {
			// to allow override method with script function
			if(metaIsMethod(item->getCategory())) {
				JS_DefineProperty(context->getJsContext(), owner, item->getName(), JSVAL_NULL, &propertyGetter, &propertySetter, JSPROP_PERMANENT | JSPROP_SHARED);
			}
		}
	}
}

JSObject * createClassBinding(const GSpiderContextPointer & context, JSObject * owner, const GClassGlueDataPointer & classData)
{
	GMetaMapClass * mapClass = classData->getClassMap();
	JsClassUserData * classUserData = static_cast<JsClassUserData *>(mapClass->getUserData());
	if(classUserData == nullptr) {
		GGlueDataWrapper * dataWrapper = newGlueDataWrapper(classData, context->getGlueDataWrapperPool());
		classUserData = new JsClassUserData("");
		mapClass->setUserData(classUserData);

		IMetaClass * metaClass = classData->getMetaClass();
		JSObject * parent = owner;

		if(metaClass->getBaseCount() > 0) {
			GScopedInterface<IMetaClass> baseClass(metaClass->getBaseClass(0));
			if(baseClass) {
				GClassGlueDataPointer baseClassData = context->getClassData(baseClass.get());
				parent = createClassBinding(context, owner, baseClassData);
			}
		}

		JSObject * classObject = JS_InitClass(context->getJsContext(), owner, parent,
			classUserData->getJsClass(), &objectConstructor, 0, nullptr, nullptr, nullptr, nullptr);

		classUserData->setClassObject(context->getJsContext(), classObject);
		setClassPrivateData(context->getJsContext(), classObject, dataWrapper);

		JSObject * obj = JS_GetObjectPrototype(context->getJsContext(), classObject);
		bindClassItems(context, obj, metaClass);

		return classObject;
	}
	else {
		return classUserData->getClassObject();
	}
}

JsValue helperBindClass(const GSpiderContextPointer & context, JSObject * owner, IMetaClass * metaClass)
{
	GClassGlueDataPointer classData(context->getClassData(metaClass));
	JSObject * classObject = createClassBinding(context, owner, classData);
	JSObject * ctor = JS_GetConstructor(context->getJsContext(), classObject);
	return ObjectValue(*ctor);
}

JsValue helperBindValue(const GSpiderContextPointer & context, JSObject * jsObject,
	const GScriptValue & value, unsigned int * outFlags)
{
	*outFlags = 0;

	JsValue result = JsValue();
	switch(value.getType()) {
		case GScriptValue::typeNull:
			result = JSVAL_NULL;
			break;

		case GScriptValue::typePrimary:
			result = variantToSpider(context, value.toPrimary(), GBindValueFlags(bvfAllowRaw), nullptr);
			break;

		case GScriptValue::typeString: {
			JSString * jsString = JS_NewStringCopyZ(context->getJsContext(), value.toString().c_str());
			result = StringValue(jsString);
			break;
		}

		case GScriptValue::typeClass: {
			GScopedInterface<IMetaClass> metaClass(value.toClass());
			result = helperBindClass(context, jsObject, metaClass.get());
			*outFlags = JSPROP_PERMANENT;
			break;
		}

		case GScriptValue::typeObject: {
			IMetaClass * metaClass;
			bool transferOwnership;
			GScriptInstanceCv cv;
			const GVariant instance = objectAddressFromVariant(value.toObject(&metaClass, &transferOwnership, &cv));
			GScopedInterface<IMetaClass> metaClassGuard(metaClass);

			GBindValueFlags flags;
			flags.setByBool(bvfAllowGC, transferOwnership);
			result = objectToSpider(context, context->getClassData(metaClass), instance, flags, cv, nullptr);
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

			result = helperBindMethodList(context, GClassGlueDataPointer(), methodList.get());
			*outFlags = JSPROP_PERMANENT | JSPROP_READONLY;
			break;
		}

		case GScriptValue::typeOverloadedMethods: {
			GScopedInterface<IMetaList> methodList(value.toOverloadedMethods());
			result = helperBindMethodList(context, GClassGlueDataPointer(), methodList.get());
			*outFlags = JSPROP_PERMANENT | JSPROP_READONLY;
			break;
		}

		case GScriptValue::typeEnum: {
			GScopedInterface<IMetaEnum> metaEnum(value.toEnum());
			result = helperBindEnum(context, metaEnum.get());
			*outFlags = JSPROP_PERMANENT;
			break;
		}

		case GScriptValue::typeRaw:
			result = rawToSpider(context, value.toRaw(), nullptr);
			break;

		case GScriptValue::typeAccessible:
			GASSERT(false);
			break;
	}

	return result;
}

void objectFinalizer(JSFreeOp * /*jsop*/, JSObject * object)
{
	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectPrivateData(object));
//	GGlueDataWrapper * dataWrapper = getNativeObject(object);
	if(dataWrapper != nullptr) {
		if (dataWrapper->getData()->isValid()) {
			dataWrapper->getData()->getBindingContext()->getScriptObjectCache()->freeScriptObject(dataWrapper);
		}
		freeGlueDataWrapper(dataWrapper, std::static_pointer_cast<GSpiderBindingContext>(dataWrapper->getData()->getBindingContext())->getGlueDataWrapperPool());
	}
}

JSBool jsHasInstance(JSContext * /*jsContext*/, JSHandleObject /*obj*/, JSMutableHandleValue /*vp*/, JSBool * bp)
{
	*bp = JS_FALSE;

	return JS_TRUE;
}


static JSClass jsClassTemplate = {
	nullptr,  // name
	JSCLASS_HAS_PRIVATE,  // flags
	JS_PropertyStub, // addProperty
	JS_PropertyStub, // delProperty
	&propertyGetter, // getProperty
	&propertySetter, // setProperty
	JS_EnumerateStub, // enumerate
	JS_ResolveStub, // resolve
	JS_ConvertStub, // convert
	&objectFinalizer, // finalize
	nullptr, // checkAccess
	nullptr, // call
	nullptr, // &jsHasInstance, // hasInstance
	nullptr, // construct
	nullptr, // trace
	JSCLASS_NO_INTERNAL_MEMBERS
};

static JSClass globalJsClass = {
	"cpgf_spidermonkey_global",  // name
	JSCLASS_GLOBAL_FLAGS | JSCLASS_HAS_PRIVATE,  // flags
	JS_PropertyStub, // addProperty
	JS_PropertyStub, // delProperty
	JS_PropertyStub, // getProperty
	JS_StrictPropertyStub, // setProperty
	JS_EnumerateStub, // enumerate
	JS_ResolveStub, // resolve
	JS_ConvertStub, // convert
	nullptr, // finalize
	nullptr, // checkAccess
	nullptr, // call
	nullptr, // &jsHasInstance, // hasInstance
	nullptr, // construct
	nullptr, // trace
	JSCLASS_NO_INTERNAL_MEMBERS
};

static GScopedJsObject globalObject;
static GClassGlueDataPointer globalClassData;

class GJsClassGlueData : public GClassGlueData
{
public:
	GJsClassGlueData() : GClassGlueData(GContextPointer(), nullptr) {
	}
};

JSObject * getOrCreateGlobalJsObject(JSContext * jsContext, JSObject * jsObject)
{
	if(jsObject == nullptr) {
		if(globalObject.getJsObject() == nullptr) {
			JSObject * g = JS_NewGlobalObject(jsContext, &globalJsClass, nullptr);
			globalObject.reset(jsContext, g);
		}
		if(! globalClassData) {
			globalClassData.reset(new GJsClassGlueData);
		}

		JSObject * global = globalObject.getJsObject();

		GMetaMapClass * mapClass = globalClassData->getClassMap();
		GGlueDataWrapper * dataWrapper = newGlueDataWrapper(globalClassData);
		JsClassUserData * classUserData = new JsClassUserData("cpgf_spidermonkey_global_object");
		mapClass->setUserData(classUserData);
		JSObject * classObject = JS_InitClass(jsContext, global, nullptr, classUserData->getJsClass(), nullptr, 0, nullptr, nullptr, nullptr, nullptr);
		classUserData->setClassObject(jsContext, classObject);

		jsObject = JS_NewObject(jsContext, classUserData->getJsClass(), global, global);
		setObjectPrivateData(jsObject, dataWrapper);
	}
	return jsObject;
}

bool isValidObject(JSContext * jsContext, JSObject * jsObject)
{
	return (getNativeObjectOrClass(jsContext, jsObject) != nullptr
		|| getFunctionPrivateData(jsObject) != nullptr)
		&& JS_GetClass(jsObject) != static_cast<JsClassUserData *>(globalClassData->getClassMap()->getUserData())->getJsClass();
	;
}

//*********************************************
// Classes implementations
//*********************************************

JsClassUserData::JsClassUserData(const char * className)
	: super(), className(className), classObject()
{
	this->jsClass = jsClassTemplate;

	this->jsClass.name = this->className.c_str();
}


GSpiderScriptFunction::GSpiderScriptFunction(const GSpiderContextPointer & context, JSObject * self, JSObject * function)
	: super(context), jsContext(context->getJsContext()), self(context->getJsContext(), self), function(context->getJsContext(), function)
{
}

GSpiderScriptFunction::~GSpiderScriptFunction()
{
}

GScriptValue GSpiderScriptFunction::invoke(const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GScriptValue invokeSpiderFunctionIndirectly(const GSpiderContextPointer & context, GVariant const * const * params, size_t paramCount, JSObject * function, JSObject * self)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	JSContext * jsContext = context->getJsContext();

	JsValue parameters[REF_MAX_ARITY];
	for(size_t i = 0; i < paramCount; ++i) {
		parameters[i] = variantToSpider(context, *params[i], GBindValueFlags(bvfAllowRaw), nullptr);
		if(parameters[i].isUndefined()) {
			raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, "unknown name");
		}
	}
	JsValue result;
	JsValue value = ObjectValue(*function);
	JSBool success = JS_CallFunctionValue(jsContext, self, value, (unsigned int)paramCount, parameters, &result);
	if(success) {
		return spiderToScriptValue(context, result, nullptr);
	}
	else {
		return GScriptValue::fromNull();
	}
}

GScriptValue GSpiderScriptFunction::invokeIndirectly(GVariant const * const * params, size_t paramCount)
{
	return invokeSpiderFunctionIndirectly(std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext()), params, paramCount, this->function.getJsObject(), this->self.getJsObject());
}


GSpiderScriptArray::GSpiderScriptArray(const GSpiderContextPointer & context, JSObject * arrayObject)
	: super(context), arrayObject(context->getJsContext(), arrayObject)
{
}

GSpiderScriptArray::~GSpiderScriptArray()
{
}

size_t GSpiderScriptArray::getLength()
{
	GSpiderContextPointer spiderContext = std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext());
	uint32_t length;
	if(JS_GetArrayLength(spiderContext->getJsContext(), this->arrayObject.getJsObject(), &length) == JS_TRUE) {
		return length;
	}
	else {
		return 0;
	}
}

GScriptValue GSpiderScriptArray::getValue(size_t index)
{
	JsValue value;
	GSpiderContextPointer spiderContext = std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext());
	if(JS_GetElement(spiderContext->getJsContext(), this->arrayObject.getJsObject(), (uint32_t)index, &value) == JS_TRUE) {
		return spiderToScriptValue(spiderContext, value, nullptr);
	}
	else {
		return GScriptValue();
	}
}

void GSpiderScriptArray::setValue(size_t index, const GScriptValue & value)
{
	if(value.isAccessible()) {
		raiseCoreException(Error_ScriptBinding_NotSupportedFeature, "Set Accessible Into Array", "Mozilla SpiderMonkey");
	}
	else {
		GSpiderContextPointer spiderContext = std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext());
		unsigned int flags = 0;
		JsValue jsValue = helperBindValue(spiderContext, this->arrayObject.getJsObject(),
			value, &flags);
		JS_SetElement(spiderContext->getJsContext(), this->arrayObject.getJsObject(), (uint32_t)index, &jsValue);
	}
}


bool GSpiderScriptArray::maybeIsScriptArray(size_t index)
{
	JsValue value;
	GSpiderContextPointer spiderContext = std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext());
	if(JS_GetElement(spiderContext->getJsContext(), this->arrayObject.getJsObject(), (uint32_t)index, &value) == JS_TRUE) {
		if(value.isObject()) {
			return JS_IsArrayObject(spiderContext->getJsContext(), &value.toObject()) == JS_TRUE;
		}
	}
	return false;
}

GScriptValue GSpiderScriptArray::getAsScriptArray(size_t index)
{
	JsValue value;
	GSpiderContextPointer spiderContext = std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext());
	if(JS_GetElement(spiderContext->getJsContext(), this->arrayObject.getJsObject(), (uint32_t)index, &value) == JS_TRUE) {
		if(value.isObject()
			&& JS_IsArrayObject(spiderContext->getJsContext(), &value.toObject()) == JS_TRUE) {
			GScopedInterface<IScriptArray> scriptArray(
				new ImplScriptArray(new GSpiderScriptArray(spiderContext, &value.toObject()), true)
			);
			return GScriptValue::fromScriptArray(scriptArray.get());
		}
	}
	return GScriptValue();
}

GScriptValue GSpiderScriptArray::createScriptArray(size_t index)
{
	GScriptValue value = this->getAsScriptArray(index);
	if(value.isNull()) {
		GSpiderContextPointer spiderContext = std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext());
		JSObject * arrayObject = JS_NewArrayObject(spiderContext->getJsContext(), 0, nullptr);
		JsValue jsValue = ObjectValue(*arrayObject);
		JS_SetElement(spiderContext->getJsContext(), this->arrayObject.getJsObject(), (uint32_t)index, &jsValue);
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GSpiderScriptArray(spiderContext, arrayObject), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
	return value;
}

GSpiderMonkeyScriptObject::GSpiderMonkeyScriptObject(IMetaService * service, JSContext * jsContext, JSObject  * jsObject)
	: super(GContextPointer(new GSpiderBindingContext(service, jsContext, getOrCreateGlobalJsObject(jsContext, jsObject)))), jsContext(jsContext)
{
	this->jsObject.reset(jsContext, std::static_pointer_cast<GSpiderBindingContext>(this->getBindingContext())->getJsGlobalObject());
}

GSpiderMonkeyScriptObject::GSpiderMonkeyScriptObject(const GSpiderMonkeyScriptObject & other, JSContext *jsContext, JSObject  * jsObject)
	: super(other), jsContext(jsContext), jsObject(jsContext, jsObject)
{
}

GSpiderMonkeyScriptObject::~GSpiderMonkeyScriptObject()
{
}

GScriptValue GSpiderMonkeyScriptObject::doGetValue(const char * name)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value) == JS_TRUE) {
		return spiderToScriptValue(this->getSpiderContext(), value, nullptr);
	}
	else {
		return GScriptValue();
	}
}

void GSpiderMonkeyScriptObject::doSetValue(const char * name, const GScriptValue & value)
{
	if(value.isAccessible()) {
		void * instance;
		GScopedInterface<IMetaAccessible> accessible(value.toAccessible(&instance));
		helperBindAccessible(this->getSpiderContext(), this->jsObject.getJsObject(), name, instance, accessible.get());
	}
	else {
		unsigned int flags = 0;
		JsValue jsValue = helperBindValue(this->getSpiderContext(), this->jsObject.getJsObject(),
			value, &flags);
		JS_DefineProperty(this->getSpiderContext()->getJsContext(), this->jsObject.getJsObject(),
			name, jsValue, nullptr, nullptr, flags);
	}
}

GScriptObject * GSpiderMonkeyScriptObject::doCreateScriptObject(const char * name)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value)) {
		if(value.isObject()) {
			JSObject * object = &value.toObject();
			if(isValidObject(this->jsContext, object)) {
				return nullptr;
			}
			GSpiderMonkeyScriptObject * newScriptObject = new GSpiderMonkeyScriptObject(*this, this->jsContext, object);
			newScriptObject->setOwner(this);
			newScriptObject->setName(name);
			return newScriptObject;
		}
		if(! value.isNullOrUndefined()) {
			return nullptr;
		}
	}

	JSObject * object = JS_NewObject(jsContext, nullptr, nullptr, nullptr); // this->getSpiderContext()->getJsGlobalObject(); // getOrCreateGlobalJsObject(this->jsContext, nullptr);
	GSpiderMonkeyScriptObject * newScriptObject = new GSpiderMonkeyScriptObject(*this, this->jsContext, object);
	newScriptObject->setOwner(this);
	newScriptObject->setName(name);
	JS_DefineProperty(this->jsContext, this->jsObject.getJsObject(), name, ObjectValue(*object), nullptr, nullptr, 0);
	return newScriptObject;

}

GScriptValue GSpiderMonkeyScriptObject::getScriptFunction(const char * name)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value)) {
		if(value.isObject()) {
			JSObject * object = &value.toObject();
			if(JS_ObjectIsFunction(this->jsContext, object)) {
				GScopedInterface<IScriptFunction> scriptFunction(new ImplScriptFunction(new GSpiderScriptFunction(this->getSpiderContext(), this->jsObject.getJsObject(), object), true));
				return GScriptValue::fromScriptFunction(scriptFunction.get());
			}
		}
	}
	return GScriptValue();
}

GScriptValue GSpiderMonkeyScriptObject::invoke(const char * name, const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GScriptValue GSpiderMonkeyScriptObject::invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value)) {
		if(value.isObject()) {
			JSObject * object = &value.toObject();
			if(JS_ObjectIsFunction(this->jsContext, object)) {
				return invokeSpiderFunctionIndirectly(this->getSpiderContext(), params, paramCount, object, this->jsObject.getJsObject());
			}
		}
	}

	return GScriptValue::fromNull();
}

void GSpiderMonkeyScriptObject::assignValue(const char * fromName, const char * toName)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), fromName, &value)) {
		JS_SetProperty(this->jsContext, this->jsObject.getJsObject(), toName, &value);
	}
}

bool GSpiderMonkeyScriptObject::maybeIsScriptArray(const char * name)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value) == JS_TRUE) {
		if(value.isObject()) {
			return JS_IsArrayObject(this->jsContext, &value.toObject()) == JS_TRUE;
		}
	}
	return false;
}

GScriptValue GSpiderMonkeyScriptObject::getAsScriptArray(const char * name)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value) == JS_TRUE) {
		if(value.isObject()
			&& JS_IsArrayObject(this->jsContext, &value.toObject()) == JS_TRUE) {
			GScopedInterface<IScriptArray> scriptArray(
				new ImplScriptArray(new GSpiderScriptArray(this->getSpiderContext(), &value.toObject()), true)
			);
			return GScriptValue::fromScriptArray(scriptArray.get());
		}
	}
	return GScriptValue();
}

GScriptValue GSpiderMonkeyScriptObject::createScriptArray(const char * name)
{
	GScriptValue value = this->getAsScriptArray(name);
	if(value.isNull()) {
		JSObject * arrayObject = JS_NewArrayObject(this->jsContext, 0, nullptr);
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GSpiderScriptArray(this->getSpiderContext(), arrayObject), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
	return value;
}

GMethodGlueDataPointer GSpiderMonkeyScriptObject::doGetMethodData(const char * methodName)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), methodName, &value)) {
		if(value.isObject()) {
			JSObject * object = &value.toObject();
			GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getFunctionPrivateData(object));
			if(dataWrapper != nullptr) {
				return dataWrapper->getAs<GMethodGlueData>();
			}
		}
	}

	return GMethodGlueDataPointer();
}


}

JSObject * createSpiderMonkeyGlobaObject(JSContext * jsContext)
{
	return getOrCreateGlobalJsObject(jsContext, nullptr);
}

GScriptObject * createSpiderMonkeyScriptObject(IMetaService * service, JSContext * jsContext, JSObject  * jsObject)
{
	return new GSpiderMonkeyScriptObject(service, jsContext, jsObject);
}

IScriptObject * createSpiderMonkeyScriptInterface(IMetaService * service, JSContext *jsContext, JSObject  * jsObject)
{
	return new ImplScriptObject(new GSpiderMonkeyScriptObject(service, jsContext, jsObject), true);
}


} // namespace cpgf


