#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gv8bind.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gerrorcode.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"
#include "../pinclude/gstaticuninitializerorders.h"

#include <stdexcept>


using namespace std;
using namespace cpgf::bind_internal;
using namespace v8;


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


#define ENTER_V8() \
	char local_msg[256]; bool local_error = false; { \
	try {

#define LEAVE_V8(...) \
	} \
	catch(const GException & e) { strncpy(local_msg, e.getMessage(), 256); local_error = true; } \
	catch(const exception & e) { strncpy(local_msg, e.what(), 256); local_error = true; } \
	catch(...) { strcpy(local_msg, "Unknown exception occurred."); local_error = true; } \
	} if(local_error) { local_msg[255] = 0; error(local_msg); } \
	__VA_ARGS__;


namespace cpgf {

namespace {


GGlueDataWrapperPool * getV8DataWrapperPool()
{
	static GGlueDataWrapperPool * v8DataWrapperPool = NULL;
	if(v8DataWrapperPool == NULL && isLibraryLive()) {
		v8DataWrapperPool = new GGlueDataWrapperPool();
		addOrderedStaticUninitializer(suo_ScriptDataWrapperPool, makeUninitializerDeleter(&v8DataWrapperPool));
	}

	return v8DataWrapperPool;
}

GScriptObjectCache<Persistent<Object> > * getV8ScriptObjectCache()
{
	static GScriptObjectCache<Persistent<Object> > * cache = NULL;
	if(cache == NULL && isLibraryLive()) {
		cache = new GScriptObjectCache<Persistent<Object> >;
		addOrderedStaticUninitializer(suo_ScriptObjectCache, makeUninitializerDeleter(&cache));
	}
	return cache;
}

//*********************************************
// Declarations
//*********************************************

class GV8BindingContext : public GBindingContext, public GShareFromBase
{
private:
	typedef GBindingContext super;

public:
	GV8BindingContext(IMetaService * service, const GScriptConfig & config)
		: super(service, config)
	{
	}

	virtual ~GV8BindingContext() {
		if(! this->objectTemplate.IsEmpty()) {
			this->objectTemplate.Dispose();
			this->objectTemplate.Clear();
		}
		checkedClearScriptObjectCache(getV8ScriptObjectCache());
	}

	Handle<Object > getRawObject() {
		if(this->objectTemplate.IsEmpty()) {
			this->objectTemplate  = Persistent<ObjectTemplate>::New(ObjectTemplate::New());
			this->objectTemplate->SetInternalFieldCount(1);
		}

		return this->objectTemplate->NewInstance();
	}

private:
	Persistent<ObjectTemplate> objectTemplate;
};

class GV8ScriptFunction : public GScriptFunctionBase
{
private:
	typedef GScriptFunctionBase super;

public:
	GV8ScriptFunction(const GContextPointer & context, Local<Object> receiver, Local<Value> func);
	virtual ~GV8ScriptFunction();

	virtual GVariant invoke(const GVariant * params, size_t paramCount);
	virtual GVariant invokeIndirectly(GVariant const * const * params, size_t paramCount);

private:
	Persistent<Object> receiver;
	Persistent<Function> func;
};

class GV8ScriptArray : public GScriptArrayBase
{
private:
	typedef GScriptArrayBase super;

public:
	GV8ScriptArray(const GContextPointer & context, Handle<Array> arr);
	virtual ~GV8ScriptArray();

	virtual size_t getLength();
	virtual GScriptValue getValue(size_t index);
	virtual void setValue(size_t index, const GScriptValue & value);

	virtual bool maybeIsScriptArray(size_t index);
	virtual GScriptValue getAsScriptArray(size_t index);
	virtual GScriptValue createScriptArray(size_t index);

private:
	Persistent<Array> arrayObject;
};

class GV8ScriptObject : public GScriptObjectBase
{
private:
	typedef GScriptObjectBase super;

public:
	GV8ScriptObject(IMetaService * service, Local<Object> object, const GScriptConfig & config);
	virtual ~GV8ScriptObject();

	virtual GScriptObject * doCreateScriptObject(const char * name);

	virtual GScriptValue getScriptFunction(const char * name);

	virtual GVariant invoke(const char * name, const GVariant * params, size_t paramCount);
	virtual GVariant invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);

	virtual bool maybeIsScriptArray(const char * name);
	virtual GScriptValue getAsScriptArray(const char * name);
	virtual GScriptValue createScriptArray(const char * name);

public:
	Local<Object> getObject() const {
		return Local<Object>::New(this->object);
	}

protected:
	virtual GScriptValue doGetValue(const char * name);
	virtual void doSetValue(const char * name, const GScriptValue & value);

private:
	GMethodGlueDataPointer doGetMethodData(const char * methodName);

private:
	GV8ScriptObject(const GV8ScriptObject & other, Local<Object> object);

private:
	Persistent<Object> object;
};

class GFunctionTemplateUserData : public GUserData
{
public:
	explicit GFunctionTemplateUserData(Handle<FunctionTemplate> functionTemplate)
		: functionTemplate(Persistent<FunctionTemplate>::New(functionTemplate))
	{
	}

	virtual ~GFunctionTemplateUserData() {
		this->functionTemplate.Dispose();
		this->functionTemplate.Clear();
	}

	Local<FunctionTemplate> getFunctionTemplate() const {
		return Local<FunctionTemplate>::New(this->functionTemplate);
	}

private:
	Persistent<FunctionTemplate> functionTemplate;
};


class GObjectTemplateUserData : public GUserData
{
public:
	explicit GObjectTemplateUserData(Handle<ObjectTemplate> objectTemplate)
		: objectTemplate(Persistent<ObjectTemplate>::New(objectTemplate))
	{
	}

	virtual ~GObjectTemplateUserData() {
		this->objectTemplate.Dispose();
		this->objectTemplate.Clear();
	}

	Local<ObjectTemplate> getObjectTemplate() const {
		return Local<ObjectTemplate>::New(this->objectTemplate);
	}

private:
	Persistent<ObjectTemplate> objectTemplate;
};


Handle<Value> variantToV8(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData);
Handle<FunctionTemplate> createClassTemplate(const GContextPointer & context, const GClassGlueDataPointer & classData);
Persistent<Object> helperBindEnum(const GContextPointer & context, Handle<ObjectTemplate> objectTemplate, IMetaEnum * metaEnum);
Handle<Value> helperBindMethodList(const GContextPointer & context, const char * name, IMetaList * methodList);
Handle<FunctionTemplate> createMethodTemplate(const GContextPointer & context, const GClassGlueDataPointer & classData, bool isGlobal,
	IMetaList * methodList, Handle<FunctionTemplate> classTemplate);
Handle<ObjectTemplate> createEnumTemplate(const GContextPointer & context);

void loadCallableParam(const Arguments & args, const GContextPointer & context, InvokeCallableParam * callableParam);


//*********************************************
// Global function implementations
//*********************************************


void error(const char * message)
{
	ThrowException(String::New(message));
}

void weakHandleCallback(Persistent<Value> object, void * parameter)
{
	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(parameter);

	getV8ScriptObjectCache()->freeScriptObject(dataWrapper);
	freeGlueDataWrapper(dataWrapper, getV8DataWrapperPool());

	object.Dispose();
	object.Clear();
}

const char * signatureKey = "i_sig_cpgf";
const int signatureValue = 0x168feed;
const char * userDataKey = "i_userdata_cpgf";

template <typename T>
void setObjectSignature(T * object)
{
	(*object)->SetHiddenValue(String::New(signatureKey), Int32::New(signatureValue));
}

bool isValidObject(Handle<Value> object)
{
	if(object->IsObject() || object->IsFunction()) {
		Handle<Value> value = Handle<Object>::Cast(object)->GetHiddenValue(String::New(signatureKey));

		return !value.IsEmpty() && value->IsInt32() && value->Int32Value() == signatureValue;
	}
	else {
		return false;
	}
}

GGlueDataWrapper * getNativeObject(Handle<Value> value)
{
	while(value->IsObject()) {
		Local<Object> object = value->ToObject();
		if(object->InternalFieldCount() > 0) {
			return static_cast<GGlueDataWrapper *>(object->GetPointerFromInternalField(0));
		}
		else {
			value = object->GetPrototype();
		}
	}

	return NULL;
}

GScriptValue v8UserDataToScriptValue(const GContextPointer & context, Local<Context> v8Context, Handle<Value> value, GGlueDataPointer * outputGlueData)
{
	if(value->IsFunction() || value->IsObject()) {
		Local<Object> obj = value->ToObject();
		if(isValidObject(obj)) {
			GGlueDataWrapper * dataWrapper = NULL;
			if(obj->InternalFieldCount() > 0) {
				dataWrapper = getNativeObject(obj);
			}
			if(dataWrapper == NULL) { // value maybe an IMetaClass
				Handle<Value> data = obj->GetHiddenValue(String::New(userDataKey));
				if(! data.IsEmpty() && data->IsExternal()) {
					dataWrapper = static_cast<GGlueDataWrapper *>(Handle<External>::Cast(data)->Value());
				}
			}
			if(dataWrapper != NULL) {
				GGlueDataPointer glueData = dataWrapper->getData();
				if(outputGlueData != NULL) {
					*outputGlueData = glueData;
				}
				return glueDataToScriptValue(glueData);
			}
		}
		else {
			if(value->IsFunction()) {
				GScopedInterface<IScriptFunction> func(
					new ImplScriptFunction(new GV8ScriptFunction(context, v8Context->Global(), Local<Value>::New(value)), true)
				);

				return GScriptValue::fromScriptFunction(func.get());
			}
			else {
				GScopedInterface<IScriptObject> scriptObject(
					new ImplScriptObject(new GV8ScriptObject(context->getService(), obj, context->getConfig()), true)
				);

				return GScriptValue::fromScriptObject(scriptObject.get());
			}
		}
	}

	return GScriptValue();
}

GScriptValue v8ToScriptValue(const GContextPointer & context, Local<Context> v8Context, Handle<Value> value, GGlueDataPointer * outputGlueData)
{
	if(value.IsEmpty()) {
		return GScriptValue();
	}

	if(value->IsBoolean()) {
		return GScriptValue::fromFundamental(value->BooleanValue());
	}

	if(value->IsInt32()) {
		return GScriptValue::fromFundamental(value->Int32Value());
	}

	if(value->IsNull()) {
		return GScriptValue::fromNull();
	}

	if(value->IsNumber()) {
		return GScriptValue::fromFundamental(value->NumberValue());
	}

	if(value->IsString()) {
		String::AsciiValue s(value);
		return GScriptValue::fromAndCopyString(*s);
	}

	if(value->IsUint32()) {
		return GScriptValue::fromFundamental(value->Uint32Value());
	}

	if(value->IsFunction() || value->IsObject()) {
		return v8UserDataToScriptValue(context, v8Context, value, outputGlueData);
	}

	return GScriptValue();
}

Handle<Value> objectToV8(const GContextPointer & context, const GClassGlueDataPointer & classData,
						 const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
{
	void * instanceAddress = objectAddressFromVariant(instance);
	
	if(instanceAddress == NULL) {
		return Handle<Value>();
	}

	Persistent<Object> * cachedObject = getV8ScriptObjectCache()->findScriptObject(instanceAddress, classData, cv);
	if(cachedObject != NULL) {
		return *cachedObject;
	}

	Handle<FunctionTemplate> functionTemplate = createClassTemplate(context, classData);
	Handle<Value> external = External::New(&signatureKey);
	Persistent<Object> self = Persistent<Object>::New(functionTemplate->GetFunction()->NewInstance(1, &external));

	GObjectGlueDataPointer objectData(context->newOrReuseObjectGlueData(classData, instance, flags, cv));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(objectData, getV8DataWrapperPool());
	self.MakeWeak(dataWrapper, weakHandleCallback);

	if(outputGlueData != NULL) {
		*outputGlueData = objectData;
	}

	self->SetPointerInInternalField(0, dataWrapper);
	setObjectSignature(&self);

	getV8ScriptObjectCache()->addScriptObject(instanceAddress, classData, cv, self);

	return self;
}

Handle<Value> rawToV8(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
{
	if(context->getConfig().allowAccessRawData()) {
		Persistent<Object> self = Persistent<Object>::New(sharedStaticCast<GV8BindingContext>(context)->getRawObject());

		GRawGlueDataPointer rawData(context->newRawGlueData(value));
		GGlueDataWrapper * dataWrapper = newGlueDataWrapper(rawData, getV8DataWrapperPool());
		self.MakeWeak(dataWrapper, weakHandleCallback);

		if(outputGlueData != NULL) {
			*outputGlueData = rawData;
		}

		self->SetPointerInInternalField(0, dataWrapper);
		setObjectSignature(&self);

		return self;
	}

	return Handle<Value>();
}

struct GV8Methods
{
	typedef Handle<Value> ResultType;
	
	static ResultType doObjectToScript(const GContextPointer & context, const GClassGlueDataPointer & classData,
		const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
	{
		return objectToV8(context, classData, instance, flags, cv, outputGlueData);
	}

	static ResultType doVariantToScript(const GContextPointer & context, const GVariant & value, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
	{
		return variantToV8(context, value, flags, outputGlueData);
	}
	
	static ResultType doRawToScript(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
	{
		return rawToV8(context, value, outputGlueData);
	}

	static ResultType doClassToScript(const GContextPointer & context, IMetaClass * metaClass)
	{
		Handle<FunctionTemplate> functionTemplate = createClassTemplate(context, context->getClassData(metaClass));
		return functionTemplate->GetFunction();
	}

	static ResultType doStringToScript(const GContextPointer & /*context*/, const char * s)
	{
		return String::New(s);
	}

	static ResultType doWideStringToScript(const GContextPointer & /*context*/, const wchar_t * ws)
	{
		GScopedArray<char> s(wideStringToString(ws));
		return String::New(s.get());
	}

	static bool isSuccessResult(const ResultType & result)
	{
		return ! result.IsEmpty();
	}

	static ResultType defaultValue()
	{
		return ResultType();
	}

	static ResultType doMethodsToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GFunctionTemplateUserData * userData = gdynamic_cast<GFunctionTemplateUserData *>(mapItem->getUserData());
		if(userData == NULL) {
			GContextPointer context = classData->getBindingContext();
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));
			
			GScopedInterface<IMetaList> metaList(getMethodListFromMapItem(mapItem, getGlueDataInstance(objectData)));
			Handle<FunctionTemplate> functionTemplate = createMethodTemplate(context, classData,
				! objectData, metaList.get(),
				createClassTemplate(context, context->getClassData(boundClass.get())));
			userData = new GFunctionTemplateUserData(functionTemplate);
			mapItem->setUserData(userData);
		}

		return userData->getFunctionTemplate()->GetFunction();
	}

	static ResultType doEnumToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem, const char * /*enumName*/)
	{
		GContextPointer context = classData->getBindingContext();
		GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
		GObjectTemplateUserData * userData = gdynamic_cast<GObjectTemplateUserData *>(mapItem->getUserData());
		if(userData == NULL) {
			Handle<ObjectTemplate> objectTemplate = createEnumTemplate(context);
			userData = new GObjectTemplateUserData(objectTemplate);
			mapItem->setUserData(userData);
		}
		return helperBindEnum(context, userData->getObjectTemplate(), metaEnum.get());
	}

};

Handle<Value> variantToV8(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
{
	GVariant value = getVariantRealValue(data);
	GMetaType type = getVariantRealMetaType(data);

	GVariantType vt = static_cast<GVariantType>(value.getType() & ~byReference);

	if(vtIsEmpty(vt)) {
		return Handle<Value>();
	}

	if(vtIsBoolean(vt)) {
		return Boolean::New(fromVariant<bool>(value));
	}

	if(vtIsInteger(vt)) {
		return Integer::New(fromVariant<int>(value));
	}

	if(vtIsReal(vt)) {
		return Number::New(fromVariant<double>(value));
	}

	if(!vtIsInterface(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == NULL) {
		return Null();
	}

	if(variantIsString(value)) {
		return String::New(fromVariant<char *>(value));
	}

	if(variantIsWideString(value)) {
		const wchar_t * ws = fromVariant<wchar_t *>(value);
		GScopedArray<char> s(wideStringToString(ws));
		return String::New(s.get());
	}

	return complexVariantToScript<GV8Methods>(context, value, type, flags, outputGlueData);
}

Handle<Value> accessibleGet(Local<String> /*prop*/, const AccessorInfo & info)
{
	ENTER_V8()

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Local<External>::Cast(info.Data())->Value());
	GAccessibleGlueDataPointer accessibleGlueData(dataWrapper->getAs<GAccessibleGlueData>());

	return accessibleToScript<GV8Methods>(accessibleGlueData->getBindingContext(), accessibleGlueData->getAccessible(), accessibleGlueData->getInstanceAddress(), false);

	LEAVE_V8(return Handle<Value>())
}

void accessibleSet(Local<String> /*prop*/, Local<Value> value, const AccessorInfo & info)
{
	ENTER_V8()

	HandleScope handleScope;

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Local<External>::Cast(info.Data())->Value());
	GAccessibleGlueDataPointer accessibleGlueData(dataWrapper->getAs<GAccessibleGlueData>());

	GVariant v = v8ToScriptValue(accessibleGlueData->getBindingContext(), info.Holder()->CreationContext(), value, NULL).getValue();
	metaSetValue(accessibleGlueData->getAccessible(), accessibleGlueData->getInstanceAddress(), v);

	LEAVE_V8()
}

void helperBindAccessible(const GContextPointer & context, Local<Object> container,
	const char * name, void * instance, IMetaAccessible * accessible)
{
	GAccessibleGlueDataPointer accessibleData(context->newAccessibleGlueData(instance, accessible));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(accessibleData, getV8DataWrapperPool());
	Persistent<External> data = Persistent<External>::New(External::New(dataWrapper));
	data.MakeWeak(dataWrapper, weakHandleCallback);

	container->SetAccessor(String::New(name), &accessibleGet, &accessibleSet, data);
}

Handle<Value> callbackMethodList(const Arguments & args)
{
	ENTER_V8()

	GGlueDataWrapper * dataWrapper = getNativeObject(args.Holder());

	if(dataWrapper != NULL && !isValidObject(args.Holder())) {
		raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
	}

	GObjectGlueDataPointer objectData;
	if(dataWrapper != NULL) {
		objectData = dataWrapper->getAs<GObjectGlueData>();
	}

	Local<External> data = Local<External>::Cast(args.Data());
	GGlueDataWrapper * methodDataWrapper = static_cast<GGlueDataWrapper *>(data->Value());
	GMethodGlueDataPointer methodData(methodDataWrapper->getAs<GMethodGlueData>());

	GContextPointer bindingContext(methodData->getBindingContext());
	InvokeCallableParam callableParam(args.Length(), bindingContext->borrowScriptContext());
	loadCallableParam(args, bindingContext, &callableParam);

	InvokeCallableResult result = doInvokeMethodList(bindingContext, objectData, methodData, &callableParam);
	return methodResultToScript<GV8Methods>(bindingContext, result.callable.get(), &result);

	LEAVE_V8(return Handle<Value>())
}

Handle<FunctionTemplate> createMethodTemplate(const GContextPointer & context,
	const GClassGlueDataPointer & classData, bool isGlobal, IMetaList * methodList,
	Handle<FunctionTemplate> classTemplate)
{
	GMethodGlueDataPointer glueData = context->newMethodGlueData(classData, methodList);
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(glueData, getV8DataWrapperPool());

	Persistent<External> data = Persistent<External>::New(External::New(dataWrapper));
	data.MakeWeak(dataWrapper, weakHandleCallback);

	Handle<FunctionTemplate> functionTemplate;
	if(! classData || classData->getMetaClass() == NULL || isGlobal) {
		functionTemplate = FunctionTemplate::New(callbackMethodList, data);
	}
	else {
		functionTemplate = FunctionTemplate::New(callbackMethodList, data, Signature::New(classTemplate));
	}
	functionTemplate->SetClassName(String::New(getMethodNameFromMethodList(methodList).c_str()));

	Local<Function> func = functionTemplate->GetFunction();
	setObjectSignature(&func);
	
	func->SetHiddenValue(String::New(userDataKey), data);

	return functionTemplate;
}

Handle<Value> namedEnumGetter(Local<String> prop, const AccessorInfo & info)
{
	ENTER_V8()

	GGlueDataWrapper * dataWrapper = getNativeObject(info.Holder());
	IMetaEnum * metaEnum = dataWrapper->getAs<GEnumGlueData>()->getMetaEnum();
	String::AsciiValue name(prop);
	int32_t index = metaEnum->findKey(*name);
	if(index >= 0) {
		return variantToV8(dataWrapper->getData()->getBindingContext(), metaGetEnumValue(metaEnum, index), GBindValueFlags(), NULL);
	}

	raiseCoreException(Error_ScriptBinding_CantFindEnumKey, *name);
	return Handle<Value>();

	LEAVE_V8(return Handle<Value>())
}

Handle<Value> namedEnumSetter(Local<String> /*prop*/, Local<Value> /*value*/, const AccessorInfo & /*info*/)
{
	ENTER_V8()

	raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);

	return Handle<Value>();

	LEAVE_V8(return Handle<Value>())
}

Handle<Array> namedEnumEnumerator(const AccessorInfo & info)
{
	ENTER_V8()

	GGlueDataWrapper * dataWrapper = getNativeObject(info.Holder());
	IMetaEnum * metaEnum = dataWrapper->getAs<GEnumGlueData>()->getMetaEnum();
	uint32_t keyCount = metaEnum->getCount();

	HandleScope handleScope;

	Local<Array> metaNames = Array::New(keyCount);
	for(uint32_t i = 0; i < keyCount; ++i) {
		metaNames->Set(Number::New(i), String::New(metaEnum->getKey(i)));
	}

	return handleScope.Close(metaNames);

	LEAVE_V8(return Handle<Array>())

}

Handle<ObjectTemplate> createEnumTemplate(const GContextPointer & /*context*/)
{
	Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();
	objectTemplate->SetInternalFieldCount(1);
	objectTemplate->SetNamedPropertyHandler(&namedEnumGetter, &namedEnumSetter, NULL, NULL, &namedEnumEnumerator);

	return objectTemplate;
}

Persistent<Object> helperBindEnum(const GContextPointer & context, Handle<ObjectTemplate> objectTemplate, IMetaEnum * metaEnum)
{
	Persistent<Object> obj = Persistent<Object>::New(objectTemplate->NewInstance());
	GEnumGlueDataPointer enumGlueData(context->newEnumGlueData(metaEnum));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(enumGlueData, getV8DataWrapperPool());
	obj->SetPointerInInternalField(0, dataWrapper);
	obj.MakeWeak(dataWrapper, weakHandleCallback);
	setObjectSignature(&obj);

	return obj;
}

Handle<Value> helperBindMethodList(const GContextPointer & context, IMetaList * methodList)
{
	Handle<FunctionTemplate> functionTemplate = createMethodTemplate(context, GClassGlueDataPointer(), true,
		methodList, Handle<FunctionTemplate>());

	Local<Function> func = Local<Function>::New(functionTemplate->GetFunction());
	setObjectSignature(&func);

	return func;
}

Handle<Value> helperBindClass(const GContextPointer & context, IMetaClass * metaClass)
{
	Handle<FunctionTemplate> functionTemplate = createClassTemplate(context, context->getClassData(metaClass));
	return functionTemplate->GetFunction();
}

Handle<Value> helperBindValue(const GContextPointer & context, const GScriptValue & value)
{
	Handle<Value> result;
	switch(value.getType()) {
		case GScriptValue::typeNull:
			result = Null();
			break;

		case GScriptValue::typeFundamental:
			result = variantToV8(context, value.toFundamental(), GBindValueFlags(bvfAllowRaw), NULL);
			break;

		case GScriptValue::typeString:
			result = String::New(value.toString().c_str());
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
			result = objectToV8(context, context->getClassData(metaClass), instance, flags, opcvNone, NULL);
			break;
		}

		case GScriptValue::typeMethod: {
			void * instance;
			GScopedInterface<IMetaMethod> method(value.toMethod(&instance));

			if(method->isStatic()) {
				instance = NULL;
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
			Handle<ObjectTemplate> objectTemplate = createEnumTemplate(context);
			result = helperBindEnum(context, objectTemplate, metaEnum.get());
			break;
		}

		case GScriptValue::typeRaw:
			result = rawToV8(context, value.toRaw(), NULL);
			break;

		case GScriptValue::typeAccessible:
			GASSERT(false);
			break;
	}
	
	return result;
}

Handle<Value> getNamedMember(const GGlueDataPointer & glueData, const char * name)
{
	return namedMemberToScript<GV8Methods>(glueData, name);
}

void loadCallableParam(const Arguments & args, const GContextPointer & context, InvokeCallableParam * callableParam)
{
	for(int i = 0; i < args.Length(); ++i) {
		callableParam->params[i].value = v8ToScriptValue(context, args.Holder()->CreationContext(), args[i], &callableParam->params[i].paramGlueData);
	}
}

Handle<Value> objectConstructor(const Arguments & args)
{
	ENTER_V8()

	if(! args.IsConstructCall()) {
		return ThrowException(String::New("Cannot call constructor as function"));
	}

	HandleScope scope;

	if(args.Length() == 1 && args[0]->IsExternal() && External::Unwrap(args[0]) == &signatureKey) {
		// Here means this constructor is called when wrapping an existing object, so we don't create new object.
		// See function objectToV8
		return scope.Close(args.Holder());
	}
	else {
		Local<External> data = Local<External>::Cast(args.Data());
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(data->Value());
		GClassGlueDataPointer classData = dataWrapper->getAs<GClassGlueData>();
		GContextPointer context = classData->getBindingContext();

		InvokeCallableParam callableParam(args.Length(), context->borrowScriptContext());
		loadCallableParam(args, context, &callableParam);

		void * instance = doInvokeConstructor(context, context->getService(), classData->getMetaClass(), &callableParam);

		if(instance != NULL) {
			Persistent<Object> self = Persistent<Object>::New(args.Holder());
			GObjectGlueDataPointer objectData = context->newObjectGlueData(classData, instance, GBindValueFlags(bvfAllowGC), opcvNone);
			GGlueDataWrapper * objectWrapper = newGlueDataWrapper(objectData, getV8DataWrapperPool());
			self.MakeWeak(objectWrapper, weakHandleCallback);

			self->SetPointerInInternalField(0, objectWrapper);
			setObjectSignature(&self);
			
			getV8ScriptObjectCache()->addScriptObject(instance, classData, opcvNone, self);

			return scope.Close(self);
		}
		else {
			raiseCoreException(Error_ScriptBinding_FailConstructObject);
		}
	}

	return scope.Close(args.Holder());

	LEAVE_V8(return Handle<Value>());
}

Handle<Value> staticMemberGetter(Local<String> prop, const AccessorInfo & info)
{
	ENTER_V8()

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Local<External>::Cast(info.Data())->Value());

	String::Utf8Value utf8_prop(prop);
	const char * name = *utf8_prop;

	return getNamedMember(dataWrapper->getData(), name);

	LEAVE_V8(return Handle<Value>())
}

void staticMemberSetter(Local<String> prop, Local<Value> value, const AccessorInfo & info)
{
	ENTER_V8()

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Local<External>::Cast(info.Data())->Value());

	String::Utf8Value utf8_prop(prop);
	const char * name = *utf8_prop;

	GContextPointer context = dataWrapper->getData()->getBindingContext();

	GGlueDataPointer valueGlueData;

	GScriptValue v = v8ToScriptValue(context, info.Holder()->CreationContext(), value, &valueGlueData);

	setValueOnNamedMember(dataWrapper->getData(), name, v, valueGlueData);

	LEAVE_V8()
}

Handle<Value> namedMemberGetter(Local<String> prop, const AccessorInfo & info)
{
	ENTER_V8()

	if(!isValidObject(info.Holder())) {
		raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
	}

	String::Utf8Value utf8_prop(prop);
	const char * name = *utf8_prop;

	GGlueDataWrapper * dataWrapper = getNativeObject(info.Holder());

	return getNamedMember(dataWrapper->getData(), name);

	LEAVE_V8(return Handle<Value>())
}

Handle<Value> namedMemberSetter(Local<String> prop, Local<Value> value, const AccessorInfo & info)
{
	ENTER_V8()

	String::Utf8Value utf8_prop(prop);
	const char * name = *utf8_prop;

	if(!isValidObject(info.Holder())) {
		raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
	}

	GGlueDataWrapper * dataWrapper = getNativeObject(info.Holder());

	if(getGlueDataCV(dataWrapper->getData()) == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);
	}
	else {
		GGlueDataPointer valueGlueData;

		GScriptValue v = v8ToScriptValue(dataWrapper->getData()->getBindingContext(), info.Holder()->CreationContext(), value, &valueGlueData);
		if(setValueOnNamedMember(dataWrapper->getData(), name, v, valueGlueData)) {
			return value;
		}
	}

	return Handle<Value>();

	LEAVE_V8(return Handle<Value>())
}

Handle<Array> namedMemberEnumerator(const AccessorInfo & info)
{
	ENTER_V8()

	if(!isValidObject(info.Holder())) {
		raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
	}

	GGlueDataWrapper * dataWrapper = getNativeObject(info.Holder());
	GGlueDataPointer glueData = dataWrapper->getData();

	GMetaClassTraveller traveller(getGlueDataMetaClass(glueData), getGlueDataInstance(glueData));
	GStringMap<bool, GStringMapReuseKey> nameMap;
	GScopedInterface<IMetaItem> metaItem;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(NULL, NULL));

		if(!metaClass) {
			break;
		}

		uint32_t metaCount = metaClass->getMetaCount();
		for(uint32_t i = 0; i < metaCount; ++i) {
			metaItem.reset(metaClass->getMetaAt(i));
			nameMap.set(metaItem->getName(), true);
		}
	}

	HandleScope handleScope;

	Local<Array> metaNames = Array::New(nameMap.getCount());
	int i = 0;
	for(GStringMap<bool, GStringMapReuseKey>::iterator it = nameMap.begin(); it != nameMap.end(); ++it) {
		metaNames->Set(Number::New(i), String::New(it->first));
		++i;
	}

	return handleScope.Close(metaNames);

	LEAVE_V8(return Handle<Array>())
}

void bindClassItems(Local<Object> object, IMetaClass * metaClass, Persistent<External> objectData)
{
	GScopedInterface<IMetaItem> item;
	uint32_t count = metaClass->getMetaCount();
	for(uint32_t i = 0; i < count; ++i) {
		item.reset(metaClass->getMetaAt(i));
		if(item->isStatic()) {
			object->SetAccessor(String::New(item->getName()), &staticMemberGetter, &staticMemberSetter, objectData);
			if(metaIsEnum(item->getCategory())) {
				IMetaEnum * metaEnum = gdynamic_cast<IMetaEnum *>(item.get());
				uint32_t keyCount = metaEnum->getCount();
				for(uint32_t k = 0; k < keyCount; ++k) {
					object->SetAccessor(String::New(metaEnum->getKey(k)), &staticMemberGetter, &staticMemberSetter, objectData);
				}
			}
		}
		else {
			// to allow override method with script function
			if(metaIsMethod(item->getCategory())) {
				object->SetAccessor(String::New(item->getName()), &staticMemberGetter, &staticMemberSetter, objectData);
			}
		}
	}
}

Handle<FunctionTemplate> createClassTemplate(const GContextPointer & context, const GClassGlueDataPointer & classData)
{
	GMetaMapClass * mapClass = classData->getClassMap();
	if(mapClass->getUserData() != NULL) { // && mapClass->getMetaClass() == classData->getMetaClass()) {
		return gdynamic_cast<GFunctionTemplateUserData *>(mapClass->getUserData())->getFunctionTemplate();
	}

	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(classData, getV8DataWrapperPool());

	IMetaClass * metaClass = classData->getMetaClass();

	Persistent<External> data = Persistent<External>::New(External::New(dataWrapper));
	data.MakeWeak(dataWrapper, weakHandleCallback);

	Handle<FunctionTemplate> functionTemplate = FunctionTemplate::New(objectConstructor, data);
	functionTemplate->SetClassName(String::New(metaClass->getName()));

	if(mapClass->getUserData() == NULL) {
		mapClass->setUserData(new GFunctionTemplateUserData(functionTemplate));
	}

	Local<ObjectTemplate> instanceTemplate = functionTemplate->InstanceTemplate();
	instanceTemplate->SetInternalFieldCount(1);

	instanceTemplate->SetNamedPropertyHandler(&namedMemberGetter, &namedMemberSetter, NULL, NULL, &namedMemberEnumerator);

	if(metaClass->getBaseCount() > 0) {
		GScopedInterface<IMetaClass> baseClass(metaClass->getBaseClass(0));
		if(baseClass) {
			GClassGlueDataPointer baseClassData = context->getClassData(baseClass.get());
			Handle<FunctionTemplate> baseFunctionTemplate = createClassTemplate(context, baseClassData);
			functionTemplate->Inherit(baseFunctionTemplate);
		}
	}

	Local<Function> classFunction = functionTemplate->GetFunction();
	setObjectSignature(&classFunction);
	bindClassItems(classFunction, metaClass, data);

	classFunction->SetHiddenValue(String::New(userDataKey), data);

	return functionTemplate;
}

bool valueIsCallable(Local<Value> value)
{
	return value->IsFunction() || (value->IsObject() && Local<Object>::Cast(value)->IsCallable());
}

GVariant invokeV8FunctionIndirectly(const GContextPointer & context, Local<Object> object, Local<Value> func, GVariant const * const * params, size_t paramCount, const char * name)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");
	GASSERT(! object->IsNull());

	if(! context) {
		raiseCoreException(Error_ScriptBinding_NoContext);
	}

	if(valueIsCallable(func)) {
		Handle<Value> v8Params[REF_MAX_ARITY];
		for(size_t i = 0; i < paramCount; ++i) {
			v8Params[i] = variantToV8(context, *params[i], GBindValueFlags(bvfAllowRaw), NULL);
			if(v8Params[i].IsEmpty()) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
		}

		Local<Value> result;
		if(func->IsFunction()) {
			result = Local<Function>::Cast(func)->Call(object, static_cast<int>(paramCount), v8Params);
		}
		else {
			result = Local<Object>::Cast(func)->CallAsFunction(object, static_cast<int>(paramCount), v8Params);
		}

		return v8ToScriptValue(context, object->CreationContext(), result, NULL).getValue();
	}
	else {
		raiseCoreException(Error_ScriptBinding_CantCallNonfunction);
	}

	return GVariant();
}


//*********************************************
// Classes implementations
//*********************************************

GV8ScriptFunction::GV8ScriptFunction(const GContextPointer & context, Local<Object> receiver, Local<Value> func)
	: super(context),
		receiver(Persistent<Object>::New(Local<Object>::Cast(receiver))),
		func(Persistent<Function>::New(Local<Function>::Cast(func)))
{
	GASSERT(! receiver->IsNull());
}

GV8ScriptFunction::~GV8ScriptFunction()
{
	this->receiver.Dispose();
	this->receiver.Clear();
	this->func.Dispose();
	this->func.Clear();
}

GVariant GV8ScriptFunction::invoke(const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GVariant GV8ScriptFunction::invokeIndirectly(GVariant const * const * params, size_t paramCount)
{
	HandleScope handleScope;

	Local<Object> receiver = Local<Object>::New(this->receiver);
	return invokeV8FunctionIndirectly(this->getBindingContext(), receiver, Local<Value>::New(this->func), params, paramCount, "");
}


GV8ScriptArray::GV8ScriptArray(const GContextPointer & context, Handle<Array> arr)
	: super(context), arrayObject(Persistent<Array>::New(arr))
{
}

GV8ScriptArray::~GV8ScriptArray()
{
	this->arrayObject.Dispose();
	this->arrayObject.Clear();
}

size_t GV8ScriptArray::getLength()
{
	return this->arrayObject->Length();
}

GScriptValue GV8ScriptArray::getValue(size_t index)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->arrayObject));

	Local<Value> value = localObject->Get((uint32_t)index);
	return v8ToScriptValue(this->getBindingContext(), this->arrayObject->CreationContext(), value, NULL);
}

void GV8ScriptArray::setValue(size_t index, const GScriptValue & value)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->arrayObject));

	if(value.isAccessible()) {
		raiseCoreException(Error_ScriptBinding_NotSupportedFeature, "Set Accessible Into Array", "Google V8");
	}
	else {
		Handle<Value> valueObject = helperBindValue(this->getBindingContext(), value);
		localObject->Set((uint32_t)index, valueObject);
	}
}

template <typename T>
bool v8MaybeIsScriptArray(T key, Handle<Object> object)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(object));

	Local<Value> value = localObject->Get(key);
	return value->IsArray();
}
template <typename T>
GScriptValue v8GetAsScriptArray(const GContextPointer & context, T key, Handle<Object> object)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(object));

	Local<Value> value = localObject->Get(key);
	if(value->IsArray()) {
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GV8ScriptArray(context, Handle<Array>::Cast(value)), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
	else {
		return GScriptValue();
	}
}
template <typename T>
GScriptValue v8CreateScriptArray(const GContextPointer & context, T key, Handle<Object> object)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(object));

	Local<Value> value = localObject->Get(key);
	if(value->IsArray()) { // already exists
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GV8ScriptArray(context, Handle<Array>::Cast(value)), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
	else {
		Local<Array> arrayObject = Array::New();
		localObject->Set(key, arrayObject);
		GScopedInterface<IScriptArray> scriptArray(
			new ImplScriptArray(new GV8ScriptArray(context, arrayObject), true)
		);
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
}

bool GV8ScriptArray::maybeIsScriptArray(size_t index)
{
	return v8MaybeIsScriptArray((uint32_t)index, this->arrayObject);
}
GScriptValue GV8ScriptArray::getAsScriptArray(size_t index)
{
	return v8GetAsScriptArray(this->getBindingContext(), (uint32_t)index, this->arrayObject);
}
GScriptValue GV8ScriptArray::createScriptArray(size_t index)
{
	return v8CreateScriptArray(this->getBindingContext(), (uint32_t)index, this->arrayObject);
}

GV8ScriptObject::GV8ScriptObject(IMetaService * service, Local<Object> object, const GScriptConfig & config)
	: super(GContextPointer(new GV8BindingContext(service, config)), config), object(Persistent<Object>::New(object))
{
}

GV8ScriptObject::GV8ScriptObject(const GV8ScriptObject & other, Local<Object> object)
	: super(other), object(Persistent<Object>::New(object))
{
}

GV8ScriptObject::~GV8ScriptObject()
{
}

GScriptValue GV8ScriptObject::doGetValue(const char * name)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(name));
	return v8ToScriptValue(this->getBindingContext(), this->object->CreationContext(), value, NULL);
}

void GV8ScriptObject::doSetValue(const char * name, const GScriptValue & value)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	if(value.isAccessible()) {
		void * instance;
		GScopedInterface<IMetaAccessible> accessible(value.toAccessible(&instance));
		helperBindAccessible(this->getBindingContext(), localObject, name, instance, accessible.get());
	}
	else {
		Handle<Value> valueObject = helperBindValue(this->getBindingContext(), value);
		localObject->Set(String::New(name), valueObject);
	}
}

GScriptObject * GV8ScriptObject::doCreateScriptObject(const char * name)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(name));
	if(isValidObject(value)) {
		return NULL;
	}

	if((value->IsObject() || value->IsFunction())) { // already exists
		GV8ScriptObject * binding = new GV8ScriptObject(*this, Local<Object>::Cast(value));
		binding->setOwner(this);
		binding->setName(name);

		return binding;
	}
	else {
		Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();
		Local<Object> obj = objectTemplate->NewInstance();
		localObject->Set(String::New(name), obj);

		GV8ScriptObject * binding = new GV8ScriptObject(*this, obj);
		binding->setOwner(this);
		binding->setName(name);

		return binding;
	}
}

GScriptValue GV8ScriptObject::getScriptFunction(const char * name)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(name));

	if(valueIsCallable(value)) {
		GScopedInterface<IScriptFunction> scriptFunction(
			new ImplScriptFunction(new GV8ScriptFunction(this->getBindingContext(), localObject, value), true)
		);
		return GScriptValue::fromScriptFunction(scriptFunction.get());
	}
	else {
		return GScriptValue();
	}
}

GVariant GV8ScriptObject::invoke(const char * name, const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GVariant GV8ScriptObject::invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> func = localObject->Get(String::New(name));

	return invokeV8FunctionIndirectly(this->getBindingContext(), this->getObject(), func, params, paramCount, name);
}

void GV8ScriptObject::assignValue(const char * fromName, const char * toName)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(fromName));
	localObject->Set(String::New(toName), value);
}

bool GV8ScriptObject::maybeIsScriptArray(const char * name)
{
	return v8MaybeIsScriptArray(String::New(name), this->object);
}

GScriptValue GV8ScriptObject::getAsScriptArray(const char * name)
{
	return v8GetAsScriptArray(this->getBindingContext(), String::New(name), this->object);
}

GScriptValue GV8ScriptObject::createScriptArray(const char * name)
{
	return v8CreateScriptArray(this->getBindingContext(), String::New(name), this->object);
}

GMethodGlueDataPointer GV8ScriptObject::doGetMethodData(const char * methodName)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(methodName));
	if(isValidObject(value)) {
		Local<Object> obj = Local<Object>::Cast(value);
		if(obj->InternalFieldCount() == 0) {
			Handle<Value> data = obj->GetHiddenValue(String::New(userDataKey));
			if(! data.IsEmpty()) {
				if(data->IsExternal()) {
					GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Handle<External>::Cast(data)->Value());
					if(dataWrapper->getData()->getType() == gdtMethod) {
						GMethodGlueDataPointer methodData(dataWrapper->getAs<GMethodGlueData>());
						if(methodData->getMethodList()) {
							return methodData;
						}
					}
				}
			}

		}
	}

	return GMethodGlueDataPointer();
}



} // unnamed namespace


GScriptObject * createV8ScriptObject(IMetaService * service, Local<Object> object, const GScriptConfig & config)
{
	return new GV8ScriptObject(service, object, config);
}

IScriptObject * createV8ScriptInterface(IMetaService * service, Local<Object> object, const GScriptConfig & config)
{
	return new ImplScriptObject(new GV8ScriptObject(service, object, config), true);
}


G_GUARD_LIBRARY_LIFE


} // namespace cpgf
