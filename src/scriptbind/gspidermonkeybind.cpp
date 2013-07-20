#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gerrorcode.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"

#include <string>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4127 4100 4800 4512 4480 4267)
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
	GScopedJsObject() : jsContext(NULL), jsObject(NULL) {
	}

	GScopedJsObject(JSContext * jsContext, JSObject  * jsObject) : jsContext(NULL), jsObject(NULL) {
		this->reset(jsContext, jsObject);
	}

	~GScopedJsObject() {
		this->reset(NULL, NULL);
	}

	JSObject  * getJsObject() const {
		return this->jsObject;
	}
	
	void reset(JSContext * jsContext, JSObject  * jsObject) {
		if(this->jsObject != NULL) {
			JS_RemoveObjectRoot(this->jsContext, &this->jsObject);
		}

		this->jsContext = jsContext;
		this->jsObject = jsObject;

		if(this->jsObject != NULL) {
			JS_AddObjectRoot(this->jsContext, &this->jsObject);
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
	GScopedArray<char> className;
	JSClass jsClass;
	GScopedJsObject classObject;
	GScopedPointer<AccessibleMapType> accessibleMap;
};

class GSpiderBindingContext : public GBindingContext, public GShareFromBase
{
private:
	typedef GBindingContext super;

public:
	GSpiderBindingContext(IMetaService * service, const GScriptConfig & config, JSContext * jsContext, JSObject  * jsObject)
		: super(service, config), jsContext(jsContext), jsObject(jsObject)
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

typedef GSharedPointer<GSpiderBindingContext> GSpiderContextPointer;

class GSpiderScriptFunction : public GScriptFunctionBase
{
private:
	typedef GScriptFunctionBase super;

public:
	GSpiderScriptFunction(const GSpiderContextPointer & context, JSObject * self, JSObject * function);
	virtual ~GSpiderScriptFunction();

	virtual GVariant invoke(const GVariant * params, size_t paramCount);
	virtual GVariant invokeIndirectly(GVariant const * const * params, size_t paramCount);

private:
	JSContext * jsContext;
	GScopedJsObject self;
	GScopedJsObject function;
};

class GSpiderMonkeyScriptObject : public GScriptObjectBase
{
private:
	typedef GScriptObjectBase super;

public:
	GSpiderMonkeyScriptObject(IMetaService * service, const GScriptConfig & config, JSContext *jsContext, JSObject  * jsObject);
	GSpiderMonkeyScriptObject(const GSpiderMonkeyScriptObject & other, JSContext *jsContext, JSObject  * jsObject);
	virtual ~GSpiderMonkeyScriptObject();

	virtual GScriptObject * doCreateScriptObject(const char * name);

	virtual GScriptValue getScriptFunction(const char * name);

	virtual GVariant invoke(const char * name, const GVariant * params, size_t paramCount);
	virtual GVariant invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);

	GSpiderContextPointer getSpiderContext() const {
		return sharedStaticCast<GSpiderBindingContext>(this->getContext());
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
	GMethodGlueDataPointer doGetMethodData(const char * methodName);
	
private:
	JSContext *jsContext;
	GScopedJsObject jsObject;
};

JSObject * createClassBinding(const GSpiderContextPointer & context, JSObject * owner, const char * name, const GClassGlueDataPointer & classData);
JSObject * helperBindClass(const GSpiderContextPointer & context, JSObject * owner, const char * name, IMetaClass * metaClass);
void helperBindMethodList(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, JSObject * owner, const char * name, IMetaList * methodList);
JSFunction * createJsFunction(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, const char * name, IMetaList * methodList);
JsValue objectToSpider(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData,
				 const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData);
JsValue variantToSpider(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData);
JsValue rawToSpider(const GSpiderContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData);
JSObject * createEnumBinding(const GSpiderContextPointer & context, IMetaEnum * metaEnum);
void helperBindEnum(const GSpiderContextPointer & context, JSObject * owner, const char * name, IMetaEnum * metaEnum);
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
		return NULL;
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
	if(getClassPrivateData(jsContext, object) != NULL) {
		getClassPrivateData(jsContext, object);
	}
	JSObject * ctor = JS_GetConstructor(jsContext, object);
	JSObject * proto = ctor;
	if(proto != NULL) {
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
	if(data == NULL) {
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

struct GSpiderMethods
{
	typedef JsValue ResultType;

	static ResultType doObjectToScript(const GContextPointer & context, const GClassGlueDataPointer & classData,
		const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
	{
		return objectToSpider(sharedStaticCast<GSpiderBindingContext>(context), classData, instance, flags, cv, outputGlueData);
	}

	static ResultType doVariantToScript(const GContextPointer & context, const GVariant & value, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
	{
		return variantToSpider(context, value, flags, outputGlueData);
	}
	
	static ResultType doRawToScript(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
	{
		return rawToSpider(sharedStaticCast<GSpiderBindingContext>(context), value, outputGlueData);
	}

	static ResultType doClassToScript(const GContextPointer & context, IMetaClass * metaClass)
	{
		GSpiderContextPointer spiderContext = sharedStaticCast<GSpiderBindingContext>(context);
		JSObject * classObject = helperBindClass(spiderContext, spiderContext->getJsGlobalObject(), metaClass->getName(), metaClass);
		JSObject * ctor = JS_GetConstructor(spiderContext->getJsContext(), classObject);
		return ObjectValue(*ctor);
	}

	static ResultType doStringToScript(const GContextPointer & context, const char * s)
	{
		JSString * jsString = JS_NewStringCopyZ(sharedStaticCast<GSpiderBindingContext>(context)->getJsContext(), s);
		return StringValue(jsString);
	}

	static ResultType doWideStringToScript(const GContextPointer & context, const wchar_t * ws)
	{
		GScopedArray<char> s(wideStringToString(ws));
		JSString * jsString = JS_NewStringCopyZ(sharedStaticCast<GSpiderBindingContext>(context)->getJsContext(), s.get());
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
		const char * methodName, GMetaClassTraveller * /*traveller*/,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getUserData());
		GSpiderContextPointer context = sharedStaticCast<GSpiderBindingContext>(classData->getContext());
		if(data == NULL) {
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));
			
			GScopedInterface<IMetaList> metaList(getMethodListFromMapItem(mapItem, getGlueDataInstance(objectData)));
			GMethodGlueDataPointer glueData = context->newMethodGlueData(context->getClassData(boundClass.get()), metaList.get(), methodName);
			data = new GMapItemMethodData(glueData);
			mapItem->setUserData(data);
		}
		JSFunction * jsFunction = createJsFunction(context, classData, methodName, data->getMethodData()->getMethodList());
		JSObject * functionObject = JS_GetFunctionObject(jsFunction);
		return ObjectValue(*functionObject);
	}
	
	static ResultType doEnumToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem, const char * /*enumName*/)
	{
		GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
		JSObject * enumObject = createEnumBinding(sharedStaticCast<GSpiderBindingContext>(classData->getContext()), metaEnum.get());
		return ObjectValue(*enumObject );
	}

};

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
		GScopedArray<char> s(wideStringToString(ws));
		JSString * jsString = JS_NewStringCopyZ(sharedStaticCast<GSpiderBindingContext>(context)->getJsContext(), s.get());
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
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectOrClassPrivateData(context->getJsContext(), object));
		if(dataWrapper == NULL) {
			dataWrapper = static_cast<GGlueDataWrapper *>(getFunctionPrivateData(object));
		}
		if(dataWrapper != NULL) {
			GGlueDataPointer glueData = dataWrapper->getData();
			if(outputGlueData != NULL) {
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
				GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GSpiderMonkeyScriptObject(context->getService(), context->getConfig(), context->getJsContext(), object), true));

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
		return GScriptValue::fromFundamental(bool(value.toBoolean()));
	}

	if(value.isInt32()) {
		return GScriptValue::fromFundamental(value.toInt32());
	}

	if(value.isNull()) {
		return GScriptValue::fromNull();
	}

	if(value.isNumber()) {
		return GScriptValue::fromFundamental(value.toNumber());
	}

	if(value.isDouble()) {
		return GScriptValue::fromFundamental(value.toDouble());
	}

	if(value.isString()) {
		JSString * jsString = value.toString();
		GScopedArray<char> s(jsStringToString(context->getJsContext(), jsString));
		return GScriptValue::fromAndCopyString(s.get());
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

	JSObject * object = JS_NewObject(context->getJsContext(), NULL, NULL, NULL);
	setObjectPrivateData(object, dataWrapper);

	if(outputGlueData != NULL) {
		*outputGlueData = rawData;
	}

	return ObjectValue(*object);
}

void loadCallableParam(jsval * valuePointer, const GSpiderContextPointer & context, InvokeCallableParam * callableParam)
{
	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		JsValue value = JS_ARGV(context->getJsContext(), valuePointer)[i];
		callableParam->params[i].value = spiderToScriptValue(context, value , &callableParam->params[i].glueData);
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
		GGlueDataWrapper * objectDataWrapper = static_cast<GGlueDataWrapper *>(getObjectOrClassPrivateData(jsContext, selfObject));
		if(objectDataWrapper != NULL && objectDataWrapper->getData()->getType() == gdtObject) {
			objectData = objectDataWrapper->getAs<GObjectGlueData>();
		}
		
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
	
	return failedResult();

	LEAVE_SPIDERMONKEY(return failedResult())
}

void helperBindMethodList(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, JSObject * owner, const char * name, IMetaList * methodList)
{
	JSFunction * jsFunction = createJsFunction(context, classData, name, methodList);
	JSObject * functionObject = JS_GetFunctionObject(jsFunction);
	JsValue value = ObjectValue(*functionObject);
	JS_DefineProperty(context->getJsContext(), owner, name, value, NULL, NULL, JSPROP_PERMANENT | JSPROP_READONLY);
}

JSFunction * createJsFunction(const GSpiderContextPointer & context, const GClassGlueDataPointer & classData, const char * name, IMetaList * methodList)
{
	GMethodGlueDataPointer glueData = context->newMethodGlueData(classData, methodList, name);
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(glueData, context->getGlueDataWrapperPool());

	JSContext * jsContext = context->getJsContext();
	JSFunction * jsFunction = JS_NewFunction(jsContext, &callbackMethodList, 0, 0, NULL, name);

	JSObject * functionObject = JS_GetFunctionObject(jsFunction);
	setFunctionPrivateData(functionObject, dataWrapper);

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

	createClassBinding(context, context->getJsGlobalObject(), classData->getMetaClass()->getName(), classData);
	GMetaMapClass * mapClass = classData->getClassMap();
	JsClassUserData * classUserData = static_cast<JsClassUserData *>(mapClass->getUserData());

	JSObject * object = JS_NewObject(context->getJsContext(), classUserData->getJsClass(), NULL, NULL);
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
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectOrClassPrivateData(jsContext, obj));

		if(dataWrapper == NULL) {
			return JS_PropertyStub(jsContext, obj, id, vp);
		}

		if(dataWrapper->getData()->getType() == gdtEnum) {
			return enumGetter(jsContext, obj, id, vp);
		}

		JSString * jsString = idValue.toString();
		GScopedArray<char> name(jsStringToString(jsContext, jsString));

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
				vp.set(accessibleToScript<GSpiderMethods>(accessibleGlueData->getContext(), accessibleGlueData->getAccessible(), accessibleGlueData->getInstanceAddress(), false));
	
				return JS_TRUE;
			}
		}
		if(! dataWrapper->getData()->getContext()) {
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
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectOrClassPrivateData(jsContext, obj));
		if(dataWrapper == NULL) {
			return JS_StrictPropertyStub(jsContext, obj, id, strict, vp);
		}

		if(dataWrapper->getData()->getType() == gdtEnum) {
			return enumSetter(jsContext, obj, id, strict, vp);
		}
	
		JSString * jsString = idValue.toString();
		GScopedArray<char> name(jsStringToString(jsContext, jsString));

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
				GScriptValue v = spiderToScriptValue(sharedStaticCast<GSpiderBindingContext>(accessibleGlueData->getContext()), vp, NULL);
				metaSetValue(accessibleGlueData->getAccessible(), accessibleGlueData->getInstanceAddress(), v.getValue());

				return JS_TRUE;
			}
		}
		if(! dataWrapper->getData()->getContext()) {
			return JS_StrictPropertyStub(jsContext, obj, id, strict, vp);
		}
	
		if(getGlueDataCV(dataWrapper->getData()) == opcvConst) {
			raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);
		}
		else {
			GScriptValue v;
			GGlueDataPointer valueGlueData;

			v = spiderToScriptValue(sharedStaticCast<GSpiderBindingContext>(dataWrapper->getData()->getContext()), vp, &valueGlueData);
			if(setValueOnNamedMember(dataWrapper->getData(), name.get(), v.getValue(), valueGlueData)) {
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
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectOrClassPrivateData(jsContext, obj));
		if(dataWrapper->getData()->getType() == gdtEnum) {
			JSString * jsString = idValue.toString();
			GScopedArray<char> name(jsStringToString(jsContext, jsString));

			IMetaEnum * metaEnum = dataWrapper->getAs<GEnumGlueData>()->getMetaEnum();
			int32_t index = metaEnum->findKey(name.get());
			if(index >= 0) {
				vp.set(variantToSpider(dataWrapper->getData()->getContext(), metaGetEnumValue(metaEnum, index), GBindValueFlags(), NULL));
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

JSObject * enumClassObject = NULL;

JSObject * createEnumBinding(const GSpiderContextPointer & context, IMetaEnum * metaEnum)
{
	if(enumClassObject == NULL) {
		JSObject * owner = context->getJsGlobalObject();
		JSObject * obj = JS_InitClass(context->getJsContext(), owner, owner, &jsEnumClass, NULL, 0, NULL, NULL, NULL, NULL);
		enumClassObject = obj;
	}
	GEnumGlueDataPointer enumGlueData(context->newEnumGlueData(metaEnum));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(enumGlueData, context->getGlueDataWrapperPool());
	JSObject * enumObject = JS_NewObject(context->getJsContext(), &jsEnumClass, NULL, NULL);
	setObjectPrivateData(enumObject, dataWrapper);
	return enumObject;
}

void helperBindEnum(const GSpiderContextPointer & context, JSObject * owner, const char * name, IMetaEnum * metaEnum)
{
	JSObject * enumObject = createEnumBinding(context, metaEnum);
	JS_DefineProperty(context->getJsContext(), owner, name, ObjectValue(*enumObject), NULL, NULL, JSPROP_PERMANENT); // Don't readonly, let cpgf raise exception. | JSPROP_READONLY);
}

void helperBindAccessible(const GSpiderContextPointer & context, JSObject * owner, const char * name, void * instance, IMetaAccessible * accessible)
{
	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectOrClassPrivateData(context->getJsContext(), owner));
	JsClassUserData * userData = NULL;
	if(dataWrapper != NULL) {
		if(dataWrapper->getData()->getType() == gdtClass) {
			userData = static_cast<JsClassUserData *>(dataWrapper->getAs<GClassGlueData>()->getClassMap()->getUserData());
		}
		else {
			if(dataWrapper->getData()->getType() == gdtObject) {
				userData = static_cast<JsClassUserData *>(dataWrapper->getAs<GObjectGlueData>()->getClassData()->getClassMap()->getUserData());
			}
		}
	}
	if(userData == NULL) {
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

JSObject * createClassBinding(const GSpiderContextPointer & context, JSObject * owner, const char * name, const GClassGlueDataPointer & classData)
{
	GMetaMapClass * mapClass = classData->getClassMap();
	JsClassUserData * classUserData = static_cast<JsClassUserData *>(mapClass->getUserData());
	if(classUserData == NULL) {
		GGlueDataWrapper * dataWrapper = newGlueDataWrapper(classData, context->getGlueDataWrapperPool());
		classUserData = new JsClassUserData(name);
		mapClass->setUserData(classUserData);

		IMetaClass * metaClass = classData->getMetaClass();
		JSObject * parent = owner;

		if(metaClass->getBaseCount() > 0) {
			GScopedInterface<IMetaClass> baseClass(metaClass->getBaseClass(0));
			if(baseClass) {
				GClassGlueDataPointer baseClassData = context->getClassData(baseClass.get());
				parent = createClassBinding(context, owner, metaClass->getName(), baseClassData);
			}
		}

		JSObject * classObject = JS_InitClass(context->getJsContext(), owner, parent, classUserData->getJsClass(), &objectConstructor, 0, NULL, NULL, NULL, NULL);

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

JSObject * helperBindClass(const GSpiderContextPointer & context, JSObject * owner, const char * name, IMetaClass * metaClass)
{
	GClassGlueDataPointer classData(context->getClassData(metaClass));
	return createClassBinding(context, owner, name, classData);
}

void objectFinalizer(JSFreeOp * /*jsop*/, JSObject * object)
{
	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getObjectPrivateData(object));
	if(dataWrapper != NULL) {
		freeGlueDataWrapper(dataWrapper, sharedStaticCast<GSpiderBindingContext>(dataWrapper->getData()->getContext())->getGlueDataWrapperPool());
	}
}

JSBool jsHasInstance(JSContext * /*jsContext*/, JSHandleObject /*obj*/, JSMutableHandleValue /*vp*/, JSBool * bp)
{
	*bp = JS_FALSE;

	return JS_TRUE;
}


static JSClass jsClassTemplate = {
    NULL,  // name
    JSCLASS_HAS_PRIVATE,  // flags
    JS_PropertyStub, // addProperty
    JS_PropertyStub, // delProperty
    &propertyGetter, // getProperty
    &propertySetter, // setProperty
    JS_EnumerateStub, // enumerate
    JS_ResolveStub, // resolve
    JS_ConvertStub, // convert
    &objectFinalizer, // finalize
    NULL, // checkAccess
    NULL, // call
    NULL, // &jsHasInstance, // hasInstance
    NULL, // construct
    NULL, // trace
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
    NULL, // finalize
    NULL, // checkAccess
    NULL, // call
    NULL, // &jsHasInstance, // hasInstance
    NULL, // construct
    NULL, // trace
	JSCLASS_NO_INTERNAL_MEMBERS
};

static GScopedJsObject globalObject;
static GClassGlueDataPointer globalClassData;

class GJsClassGlueData : public GClassGlueData
{
public:
	GJsClassGlueData() : GClassGlueData(GContextPointer(), NULL) {
	}
};

JSObject * getOrCreateGlobalJsObject(JSContext * jsContext, JSObject * jsObject)
{
	if(jsObject == NULL) {
		if(globalObject.getJsObject() == NULL) {
			JSObject * g = JS_NewGlobalObject(jsContext, &globalJsClass, NULL);
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
		JSObject * classObject = JS_InitClass(jsContext, global, NULL, classUserData->getJsClass(), NULL, 0, NULL, NULL, NULL, NULL);
		classUserData->setClassObject(jsContext, classObject);

		jsObject = JS_NewObject(jsContext, classUserData->getJsClass(), global, global);
		setObjectPrivateData(jsObject, dataWrapper);
	}
	return jsObject;
}

bool isValidObject(JSContext * jsContext, JSObject * jsObject)
{
	return (getObjectOrClassPrivateData(jsContext, jsObject) != NULL
		|| getFunctionPrivateData(jsObject) != NULL)
		&& JS_GetClass(jsObject) != static_cast<JsClassUserData *>(globalClassData->getClassMap()->getUserData())->getJsClass();
	;
}

//*********************************************
// Classes implementations
//*********************************************

JsClassUserData::JsClassUserData(const char * className)
	: super(), className(new char[strlen(className) + 1]), classObject()
{
	strcpy(this->className.get(), className);

	this->jsClass = jsClassTemplate;

	this->jsClass.name = this->className.get();
}


GSpiderScriptFunction::GSpiderScriptFunction(const GSpiderContextPointer & context, JSObject * self, JSObject * function)
	: super(context), jsContext(context->getJsContext()), self(context->getJsContext(), self), function(context->getJsContext(), function)
{
}

GSpiderScriptFunction::~GSpiderScriptFunction()
{
}

GVariant GSpiderScriptFunction::invoke(const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GVariant invokeSpiderFunctionIndirectly(const GSpiderContextPointer & context, GVariant const * const * params, size_t paramCount, JSObject * function, JSObject * self)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	JSContext * jsContext = context->getJsContext();

	JsValue parameters[REF_MAX_ARITY];
	for(size_t i = 0; i < paramCount; ++i) {
		parameters[i] = variantToSpider(context, *params[i], GBindValueFlags(bvfAllowRaw), NULL);
		if(parameters[i].isUndefined()) {
			raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, "unknown name");
		}
	}
	JsValue result;
	JsValue value = ObjectValue(*function);
	JSBool success = JS_CallFunctionValue(jsContext, self, value, (unsigned int)paramCount, parameters, &result);
	if(success) {
		return spiderToScriptValue(context, result, NULL).getValue();
	}
	else {
		return GVariant();
	}
}

GVariant GSpiderScriptFunction::invokeIndirectly(GVariant const * const * params, size_t paramCount)
{
	return invokeSpiderFunctionIndirectly(sharedStaticCast<GSpiderBindingContext>(this->getContext()), params, paramCount, this->function.getJsObject(), this->self.getJsObject());
}

	
GSpiderMonkeyScriptObject::GSpiderMonkeyScriptObject(IMetaService * service, const GScriptConfig & config, JSContext * jsContext, JSObject  * jsObject)
	: super(GContextPointer(new GSpiderBindingContext(service, config, jsContext, getOrCreateGlobalJsObject(jsContext, jsObject))), config), jsContext(jsContext)
{
	this->jsObject.reset(jsContext, sharedStaticCast<GSpiderBindingContext>(this->getContext())->getJsGlobalObject());
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
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value)) {
		return spiderToScriptValue(this->getSpiderContext(), value, NULL);
	}
	else {
		return GScriptValue();
	}
}

void GSpiderMonkeyScriptObject::doBindClass(const char * name, IMetaClass * metaClass)
{
	helperBindClass(this->getSpiderContext(), this->jsObject.getJsObject(), name, metaClass);
}

void GSpiderMonkeyScriptObject::doBindEnum(const char * name, IMetaEnum * metaEnum)
{
	helperBindEnum(this->getSpiderContext(), this->jsObject.getJsObject(), name, metaEnum);
}

GScriptObject * GSpiderMonkeyScriptObject::doCreateScriptObject(const char * name)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value)) {
		if(value.isObject()) {
			JSObject * object = &value.toObject();
			if(isValidObject(this->jsContext, object)) {
				return NULL;
			}
			GSpiderMonkeyScriptObject * newScriptObject = new GSpiderMonkeyScriptObject(*this, this->jsContext, object);
			newScriptObject->setOwner(this);
			newScriptObject->setName(name);
			return newScriptObject;
		}
		if(! value.isNullOrUndefined()) {
			return NULL;
		}
	}

	JSObject * object = JS_NewObject(jsContext, NULL, NULL, NULL); // this->getSpiderContext()->getJsGlobalObject(); // getOrCreateGlobalJsObject(this->jsContext, NULL);
	GSpiderMonkeyScriptObject * newScriptObject = new GSpiderMonkeyScriptObject(*this, this->jsContext, object);
	newScriptObject->setOwner(this);
	newScriptObject->setName(name);
	JS_DefineProperty(this->jsContext, this->jsObject.getJsObject(), name, ObjectValue(*object), NULL, NULL, 0);
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
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value)) {
		if(value.isObject()) {
			JSObject * object = &value.toObject();
			if(JS_ObjectIsFunction(this->jsContext, object)) {
				return invokeSpiderFunctionIndirectly(this->getSpiderContext(), params, paramCount, object, this->jsObject.getJsObject());
			}
		}
	}

	return GVariant();
}

void GSpiderMonkeyScriptObject::doBindNull(const char * name)
{
	JsValue value = JSVAL_NULL;
	JS_SetProperty(this->jsContext, this->jsObject.getJsObject(), name, &value);
}

void GSpiderMonkeyScriptObject::doBindFundamental(const char * name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.refData().typeData)), "Only fundamental value can be bound via bindFundamental");

	JS_DefineProperty(this->jsContext, this->jsObject.getJsObject(), name, variantToSpider(this->getContext(), value, GBindValueFlags(bvfAllowRaw), NULL), NULL, NULL, 0);
}

void GSpiderMonkeyScriptObject::doBindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	helperBindAccessible(this->getSpiderContext(), this->jsObject.getJsObject(), name, instance, accessible);
}

void GSpiderMonkeyScriptObject::doBindString(const char * stringName, const char * s)
{
	JSString * jsString = JS_NewStringCopyZ(this->jsContext, s);
	JsValue value = StringValue(jsString);
	JS_DefineProperty(this->jsContext, this->jsObject.getJsObject(), stringName, value, NULL, NULL, 0);
}

void GSpiderMonkeyScriptObject::doBindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	GBindValueFlags flags;
	flags.setByBool(bvfAllowGC, transferOwnership);
	JsValue value = objectToSpider(this->getSpiderContext(), this->getContext()->getClassData(type), instance, flags, opcvNone, NULL);
	JS_DefineProperty(this->jsContext, this->jsObject.getJsObject(), objectName, value, NULL, NULL, 0);
}

void GSpiderMonkeyScriptObject::doBindRaw(const char * name, const GVariant & value)
{
	JsValue jsValue = rawToSpider(this->getSpiderContext(), value, NULL);
	JS_DefineProperty(this->jsContext, this->jsObject.getJsObject(), name, jsValue, NULL, NULL, 0);
}

void GSpiderMonkeyScriptObject::doBindMethod(const char * name, void * instance, IMetaMethod * method)
{
	if(method->isStatic()) {
		instance = NULL;
	}

	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method, instance);

	helperBindMethodList(this->getSpiderContext(), GClassGlueDataPointer(), this->jsObject.getJsObject(), name, methodList.get());
}

void GSpiderMonkeyScriptObject::doBindMethodList(const char * name, IMetaList * methodList)
{
	helperBindMethodList(this->getSpiderContext(), GClassGlueDataPointer(), this->jsObject.getJsObject(), name, methodList);
}

void GSpiderMonkeyScriptObject::assignValue(const char * fromName, const char * toName)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), fromName, &value)) {
		JS_SetProperty(this->jsContext, this->jsObject.getJsObject(), toName, &value);
	}
}

void GSpiderMonkeyScriptObject::doBindCoreService(const char * name, IScriptLibraryLoader * libraryLoader)
{
	this->getContext()->bindScriptCoreService(this, name, libraryLoader);
}

GMethodGlueDataPointer GSpiderMonkeyScriptObject::doGetMethodData(const char * methodName)
{
	JsValue value;
	if(JS_GetProperty(this->jsContext, this->jsObject.getJsObject(), methodName, &value)) {
		if(value.isObject()) {
			JSObject * object = &value.toObject();
			GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(getFunctionPrivateData(object));
			if(dataWrapper != NULL) {
				return dataWrapper->getAs<GMethodGlueData>();
			}
		}
	}

	return GMethodGlueDataPointer();
}


}

JSObject * createSpiderMonkeyGlobaObject(JSContext * jsContext)
{
	return getOrCreateGlobalJsObject(jsContext, NULL);
}

GScriptObject * createSpiderMonkeyScriptObject(IMetaService * service, JSContext * jsContext, JSObject  * jsObject, const GScriptConfig & config)
{
	return new GSpiderMonkeyScriptObject(service, config, jsContext, jsObject);
}

IScriptObject * createSpiderMonkeyScriptInterface(IMetaService * service, JSContext *jsContext, JSObject  * jsObject, const GScriptConfig & config)
{
	return new ImplScriptObject(new GSpiderMonkeyScriptObject(service, config, jsContext, jsObject), true);
}


} // namespace cpgf


