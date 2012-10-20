#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gstringmap.h"

#include "v8.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"
#include "../pinclude/gstaticuninitializerorders.h"


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

class GV8ScriptObject : public GScriptObjectBase
{
private:
	typedef GScriptObjectBase super;

public:
	GV8ScriptObject(IMetaService * service, Local<Object> object, const GScriptConfig & config);
	virtual ~GV8ScriptObject();

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

	virtual void bindCoreService(const char * name);
	
public:
	Local<Object> getObject() const {
		return Local<Object>::New(this->object);
	}

private:
	void doBindMethodList(const char * name, IMetaList * methodList, GGlueDataMethodType methodType);
	GMethodGlueDataPointer doGetMethodData(const char * methodName);

private:
	GV8ScriptObject(const GV8ScriptObject & other, Local<Object> object);

private:
	Persistent<Object> object;
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
Persistent<Object> doBindEnum(const GContextPointer & context, Handle<ObjectTemplate> objectTemplate, IMetaEnum * metaEnum);
Handle<FunctionTemplate> createMethodTemplate(const GContextPointer & context, const GClassGlueDataPointer & classData, bool isGlobal, IMetaList * methodList,
	const char * name, Handle<FunctionTemplate> classTemplate, GGlueDataMethodType methodType);
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

bool isGlobalObject(Handle<Value> object)
{
	if(object->IsObject() || object->IsFunction()) {
		return Handle<Object>::Cast(object)->InternalFieldCount () == 0
			|| Handle<Object>::Cast(object)->GetPointerFromInternalField(0) == NULL;
	}
	else {
		return false;
	}
}

GScriptDataType getV8Type(Local<Value> value, IMetaTypedItem ** typeItem)
{
	if(typeItem != NULL) {
		*typeItem = NULL;
	}

	if(value->IsNull()) {
		return sdtNull;
	}

	if(value->IsUndefined()) {
		return sdtNull;
	}

	if(value->IsBoolean()) {
		return sdtFundamental;
	}

	if(value->IsInt32() || value->IsNumber()) {
		return sdtFundamental;
	}

	if(value->IsString()) {
		return sdtString;
	}

	if(value->IsFunction() || value->IsObject()) {
		Local<Object> obj = value->ToObject();
		if(isValidObject(obj)) {
			if(obj->InternalFieldCount() == 0) {
				Handle<Value> value = obj->GetHiddenValue(String::New(userDataKey));
				if(! value.IsEmpty()) {
					if(value->IsExternal()) {
						GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Handle<External>::Cast(value)->Value());
						switch(dataWrapper->getData()->getType()) {
							case gdtClass:
								if(typeItem != NULL) {
									*typeItem = dataWrapper->getAs<GClassGlueData>()->getMetaClass();
									(*typeItem)->addReference();
								}
								return sdtClass;

							case gdtMethod:
								return methodTypeToGlueDataType(dataWrapper->getAs<GMethodGlueData>()->getMethodType());

							default:
								break;
						}
					}
				}

			}
			else {
				GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(obj->GetPointerFromInternalField(0));
				if(dataWrapper != NULL) {
					switch(dataWrapper->getData()->getType()) {
						case gdtObject: {
							if(typeItem != NULL) {
								*typeItem = dataWrapper->getAs<GObjectGlueData>()->getClassData()->getMetaClass();
								(*typeItem)->addReference();
							}
							return sdtObject;
						}

						break;

						case gdtEnum:
							if(typeItem != NULL) {
								*typeItem = dataWrapper->getAs<GEnumGlueData>()->getMetaEnum();
								(*typeItem)->addReference();
							}
							return sdtEnum;

					    case gdtRaw:
					    	return sdtRaw;

						default:
						break;
					}
				}
			}
		}

		if(value->IsFunction()) {
			return sdtScriptMethod;
		}
		else {
			return sdtScriptObject;
		}
	}

	return sdtUnknown;
}

void * v8ToObject(Handle<Value> value, GMetaType * outType)
{
	if(isValidObject(value)) {
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Handle<Object>::Cast(value)->GetPointerFromInternalField(0));
		if(dataWrapper != NULL && dataWrapper->getData()->getType() == gdtObject) {
			GObjectGlueDataPointer objectData(dataWrapper->getAs<GObjectGlueData>());
			if(outType != NULL) {
				GMetaTypeData typeData;
				objectData->getClassData()->getMetaClass()->getMetaType(&typeData);
				metaCheckError(objectData->getClassData()->getMetaClass());
				*outType = GMetaType(typeData);
			}

			return objectData->getInstanceAddress();
		}
	}

	return NULL;
}

GVariant v8UserDataToVariant(const GContextPointer & context, Local<Context> v8Context, Handle<Value> value, GGlueDataPointer * outputGlueData)
{
	if(value->IsFunction() || value->IsObject()) {
		Local<Object> obj = value->ToObject();
		if(isValidObject(obj)) {
			GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(obj->GetPointerFromInternalField(0));
			if(dataWrapper == NULL) { // value maybe an IMetaClass
				Handle<Value> data = obj->GetHiddenValue(String::New(userDataKey));
				if(! data.IsEmpty() && data->IsExternal()) {
					dataWrapper = static_cast<GGlueDataWrapper *>(Handle<External>::Cast(data)->Value());
				}
			}
			GGlueDataPointer glueData = dataWrapper->getData();
			if(outputGlueData != NULL) {
				*outputGlueData = glueData;
			}
			return glueDataToVariant(glueData);
		}
		else {
			if(value->IsFunction()) {
				GScopedInterface<IScriptFunction> func(new ImplScriptFunction(new GV8ScriptFunction(context, v8Context->Global(), Local<Value>::New(value)), true));

				return GVariant(func.get());
			}
			else {
				GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GV8ScriptObject(context->getService(), obj, context->getConfig()), true));

				return GVariant(scriptObject.get());
			}
		}
	}

	return GVariant();
}

GVariant v8ToVariant(const GContextPointer & context, Local<Context> v8Context, Handle<Value> value, GGlueDataPointer * outputGlueData)
{
	if(value.IsEmpty()) {
		return GVariant();
	}

	if(value->IsBoolean()) {
		return value->BooleanValue();
	}

	if(value->IsInt32()) {
		return value->Int32Value();
	}

	if(value->IsNull()) {
		return (void *)0;
	}

	if(value->IsNumber()) {
		return value->NumberValue();
	}

	if(value->IsString()) {
		String::AsciiValue s(value);
		return createTypedVariant(createStringVariant(*s), createMetaType<char *>());
	}

	if(value->IsUint32()) {
		return value->Uint32Value();
	}

	if(value->IsFunction() || value->IsObject()) {
		return v8UserDataToVariant(context, v8Context, value, outputGlueData);
	}

	return GVariant();
}

Handle<Value> objectToV8(const GContextPointer & context, const GClassGlueDataPointer & classData,
						 const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
{
	if(objectAddressFromVariant(instance) == NULL) {
		return Handle<Value>();
	}

	Handle<FunctionTemplate> functionTemplate = createClassTemplate(context, classData);
	Handle<Value> external = External::New(&signatureKey);
	Persistent<Object> self = Persistent<Object>::New(functionTemplate->GetFunction()->NewInstance(1, &external));

	GObjectGlueDataPointer objectData(context->newObjectGlueData(classData, instance, flags, cv));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(objectData, getV8DataWrapperPool());
	self.MakeWeak(dataWrapper, weakHandleCallback);

	if(outputGlueData != NULL) {
		*outputGlueData = objectData;
	}

	self->SetPointerInInternalField(0, dataWrapper);
	setObjectSignature(&self);

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

	static ResultType doMethodsToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem,
		const char * methodName, GMetaClassTraveller * /*traveller*/,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GFunctionTemplateUserData * userData = gdynamic_cast<GFunctionTemplateUserData *>(mapItem->getUserData());
		if(userData == NULL) {
			GContextPointer context = classData->getContext();
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));
			Handle<FunctionTemplate> functionTemplate = createMethodTemplate(context, classData,
				! objectData, NULL, methodName,
				createClassTemplate(context, context->getClassData(boundClass.get())), gdmtInternal);
			userData = new GFunctionTemplateUserData(functionTemplate);
			mapItem->setUserData(userData);
		}

		return userData->getFunctionTemplate()->GetFunction();
	}

	static ResultType doEnumToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem, const char * /*enumName*/)
	{
		GContextPointer context = classData->getContext();
		GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
		GObjectTemplateUserData * userData = gdynamic_cast<GObjectTemplateUserData *>(mapItem->getUserData());
		if(userData == NULL) {
			Handle<ObjectTemplate> objectTemplate = createEnumTemplate(context);
			userData = new GObjectTemplateUserData(objectTemplate);
			mapItem->setUserData(userData);
		}
		return doBindEnum(context, userData->getObjectTemplate(), metaEnum.get());
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

	return accessibleToScript<GV8Methods>(accessibleGlueData->getContext(), accessibleGlueData->getAccessible(), accessibleGlueData->getInstanceAddress(), false);

	LEAVE_V8(return Handle<Value>())
}

void accessibleSet(Local<String> /*prop*/, Local<Value> value, const AccessorInfo & info)
{
	ENTER_V8()

	HandleScope handleScope;

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(Local<External>::Cast(info.Data())->Value());
	GAccessibleGlueDataPointer accessibleGlueData(dataWrapper->getAs<GAccessibleGlueData>());

	GVariant v = v8ToVariant(accessibleGlueData->getContext(), info.Holder()->CreationContext(), value, NULL);
	metaSetValue(accessibleGlueData->getAccessible(), accessibleGlueData->getInstanceAddress(), v);

	LEAVE_V8()
}

void doBindAccessible(const GContextPointer & context, Local<Object> container,
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

	bool isGlobal = isGlobalObject(args.Holder());

	if(!isGlobal && !isValidObject(args.Holder())) {
		raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
	}

	GGlueDataWrapper * dataWrapper = NULL;

	if(!isGlobal) {
		dataWrapper = static_cast<GGlueDataWrapper *>(args.Holder()->GetPointerFromInternalField(0));
	}
	GObjectGlueDataPointer objectData;
	if(dataWrapper != NULL) {
		objectData = dataWrapper->getAs<GObjectGlueData>();
	}

	Local<External> data = Local<External>::Cast(args.Data());
	GGlueDataWrapper * methodDataWrapper = static_cast<GGlueDataWrapper *>(data->Value());
	GMethodGlueDataPointer methodData(methodDataWrapper->getAs<GMethodGlueData>());

	InvokeCallableParam callableParam(args.Length());
	loadCallableParam(args, methodData->getContext(), &callableParam);

	InvokeCallableResult result = doInvokeMethodList(methodData->getContext(), objectData, methodData, &callableParam);
	return methodResultToScript<GV8Methods>(methodData->getContext(), result.callable.get(), &result);

	LEAVE_V8(return Handle<Value>())
}

Handle<FunctionTemplate> createMethodTemplate(const GContextPointer & context, const GClassGlueDataPointer & classData, bool isGlobal, IMetaList * methodList,
	const char * name, Handle<FunctionTemplate> classTemplate, GGlueDataMethodType methodType)
{
	GMethodGlueDataPointer glueData = context->newMethodGlueData(classData, methodList, name, methodType);
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
	functionTemplate->SetClassName(String::New(name));

	Local<Function> func = functionTemplate->GetFunction();
	setObjectSignature(&func);
	
	func->SetHiddenValue(String::New(userDataKey), data);

	return functionTemplate;
}

Handle<Value> namedEnumGetter(Local<String> prop, const AccessorInfo & info)
{
	ENTER_V8()

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(info.Holder()->GetPointerFromInternalField(0));
	IMetaEnum * metaEnum = dataWrapper->getAs<GEnumGlueData>()->getMetaEnum();
	String::AsciiValue name(prop);
	int32_t index = metaEnum->findKey(*name);
	if(index >= 0) {
		return variantToV8(dataWrapper->getData()->getContext(), metaGetEnumValue(metaEnum, index), GBindValueFlags(), NULL);
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

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(info.Holder()->GetPointerFromInternalField(0));
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

Persistent<Object> doBindEnum(const GContextPointer & context, Handle<ObjectTemplate> objectTemplate, IMetaEnum * metaEnum)
{
	Persistent<Object> obj = Persistent<Object>::New(objectTemplate->NewInstance());
	GEnumGlueDataPointer enumGlueData(context->newEnumGlueData(metaEnum));
	GGlueDataWrapper * dataWrapper = newGlueDataWrapper(enumGlueData, getV8DataWrapperPool());
	obj->SetPointerInInternalField(0, dataWrapper);
	obj.MakeWeak(dataWrapper, weakHandleCallback);
	setObjectSignature(&obj);

	return obj;
}

Handle<Value> getNamedMember(const GGlueDataPointer & glueData, const char * name)
{
	return namedMemberToScript<GV8Methods>(glueData, name);
}

void loadCallableParam(const Arguments & args, const GContextPointer & context, InvokeCallableParam * callableParam)
{
	for(int i = 0; i < args.Length(); ++i) {
		callableParam->params[i].value = getVariantRealValue(v8ToVariant(context, args.Holder()->CreationContext(), args[i], &callableParam->params[i].glueData));
		IMetaTypedItem * typeItem;
		callableParam->params[i].dataType = getV8Type(args[i], &typeItem);
		callableParam->params[i].typeItem.reset(typeItem);
	}
}

void * invokeConstructor(const Arguments & args, const GContextPointer & context, IMetaClass * metaClass)
{
	InvokeCallableParam callableParam(args.Length());
	loadCallableParam(args, context, &callableParam);

	void * instance = doInvokeConstructor(context->getService(), metaClass, &callableParam);
	return instance;
}

Handle<Value> objectConstructor(const Arguments & args)
{
	ENTER_V8()

	if(! args.IsConstructCall()) {
		return ThrowException(String::New("Cannot call constructor as function"));
	}

	Persistent<Object> self = Persistent<Object>::New(args.Holder());

	if(args.Length() == 1 && args[0]->IsExternal() && External::Unwrap(args[0]) == &signatureKey) {
		// Here means this constructor is called when wrapping an existing object, so we don't create new object.
		// See function objectToV8
	}
	else {
		Local<External> data = Local<External>::Cast(args.Data());
		GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(data->Value());
		GClassGlueDataPointer classData = dataWrapper->getAs<GClassGlueData>();

		void * instance = invokeConstructor(args, classData->getContext(), classData->getMetaClass());

		GObjectGlueDataPointer objectData = classData->getContext()->newObjectGlueData(classData, instance, GBindValueFlags(bvfAllowGC), opcvNone);
		GGlueDataWrapper * objectWrapper = newGlueDataWrapper(objectData, getV8DataWrapperPool());
		self.MakeWeak(objectWrapper, weakHandleCallback);

		self->SetPointerInInternalField(0, objectWrapper);
		setObjectSignature(&self);
	}

	return self;

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

	GContextPointer context = dataWrapper->getData()->getContext();

	GVariant v;
	GGlueDataPointer valueGlueData;

	v = v8ToVariant(context, info.Holder()->CreationContext(), value, &valueGlueData);

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

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(info.Holder()->GetPointerFromInternalField(0));

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

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(info.Holder()->GetPointerFromInternalField(0));

	if(getGlueDataCV(dataWrapper->getData()) == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);
	}
	else {
		GVariant v;
		GGlueDataPointer valueGlueData;

		v = v8ToVariant(dataWrapper->getData()->getContext(), info.Holder()->CreationContext(), value, &valueGlueData);
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

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(info.Holder()->GetPointerFromInternalField(0));
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

void doBindClass(const GContextPointer & context, Local<Object> container, const char * name, IMetaClass * metaClass)
{
	Handle<FunctionTemplate> functionTemplate = createClassTemplate(context, context->getClassData(metaClass));
	container->Set(String::New(name), functionTemplate->GetFunction());
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

		return v8ToVariant(context, object->CreationContext(), result, NULL);
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
	ENTER_V8()

	HandleScope handleScope;

	Local<Object> receiver = Local<Object>::New(this->receiver);
	return invokeV8FunctionIndirectly(this->getContext(), receiver, Local<Value>::New(this->func), params, paramCount, "");

	LEAVE_V8(return GVariant())
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

GScriptDataType GV8ScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> obj = localObject->Get(String::New(name));
	return getV8Type(obj, outMetaTypeItem);

	LEAVE_V8(return sdtUnknown)
}

void GV8ScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	doBindClass(this->getContext(), localObject, name, metaClass);

	LEAVE_V8()
}

void GV8ScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Handle<ObjectTemplate> objectTemplate = createEnumTemplate(this->getContext());

	Persistent<Object> obj = doBindEnum(this->getContext(), objectTemplate, metaEnum);

	localObject->Set(String::New(name), obj);

	LEAVE_V8()
}

GScriptObject * GV8ScriptObject::doCreateScriptObject(const char * name)
{
	ENTER_V8()

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

	LEAVE_V8(return NULL)
}

GScriptFunction * GV8ScriptObject::gainScriptFunction(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(name));

	if(valueIsCallable(value)) {
		return new GV8ScriptFunction(this->getContext(), localObject, value);
	}
	else {
		return NULL;
	}

	LEAVE_V8(return NULL)
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
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> func = localObject->Get(String::New(name));

	return invokeV8FunctionIndirectly(this->getContext(), this->getObject(), func, params, paramCount, name);

	LEAVE_V8(return GVariant())
}

void GV8ScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.refData().typeData)), "Only fundamental value can be bound via bindFundamental");

	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	localObject->Set(String::New(name), variantToV8(this->getContext(), value, GBindValueFlags(bvfAllowRaw), NULL));

	LEAVE_V8()
}

void GV8ScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	doBindAccessible(this->getContext(), localObject, name, instance, accessible);

	LEAVE_V8()
}

void GV8ScriptObject::bindString(const char * stringName, const char * s)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	localObject->Set(String::New(stringName), String::New(s));

	LEAVE_V8()
}

void GV8ScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	GBindValueFlags flags;
	flags.setByBool(bvfAllowGC, transferOwnership);
	Handle<Value> obj = objectToV8(this->getContext(), this->getContext()->getClassData(type), instance, flags, opcvNone, NULL);
	localObject->Set(String::New(objectName), obj);

	LEAVE_V8()
}

void GV8ScriptObject::bindRaw(const char * name, const GVariant & value)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	localObject->Set(String::New(name), rawToV8(this->getContext(), value, NULL));

	LEAVE_V8()
}

void GV8ScriptObject::bindMethod(const char * name, void * instance, IMetaMethod * method)
{
	ENTER_V8()

	if(method->isStatic()) {
		instance = NULL;
	}

	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method, instance);

	this->doBindMethodList(name, methodList.get(), gdmtMethod);

	LEAVE_V8()
}

void GV8ScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	ENTER_V8()

	this->doBindMethodList(name, methodList, gdmtMethodList);

	LEAVE_V8()
}

IMetaClass * GV8ScriptObject::getClass(const char * className)
{
	ENTER_V8()

	IMetaTypedItem * typedItem = NULL;

	GScriptDataType sdt = this->getType(className, &typedItem);
	GScopedInterface<IMetaTypedItem> item(typedItem);
	if(sdt == sdtClass) {
		return gdynamic_cast<IMetaClass *>(item.take());
	}

	return NULL;

	LEAVE_V8(return NULL)
}

IMetaEnum * GV8ScriptObject::getEnum(const char * enumName)
{
	ENTER_V8()

	IMetaTypedItem * typedItem = NULL;

	GScriptDataType sdt = this->getType(enumName, &typedItem);
	GScopedInterface<IMetaTypedItem> item(typedItem);
	if(sdt == sdtEnum) {
		return gdynamic_cast<IMetaEnum *>(item.take());
	}

	return NULL;

	LEAVE_V8(return NULL)
}

GVariant GV8ScriptObject::getFundamental(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(name));
	if(getV8Type(value, NULL) == sdtFundamental) {
		return v8ToVariant(this->getContext(), this->object->CreationContext(), value, NULL);
	}
	else {
		return GVariant();
	}

	LEAVE_V8(return GVariant())
}

std::string GV8ScriptObject::getString(const char * stringName)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(stringName));
	if(value->IsString()) {
		String::AsciiValue s(value);
		return *s;
	}
	else {
		return "";
	}

	LEAVE_V8(return "")
}

void * GV8ScriptObject::getObject(const char * objectName)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(objectName));
	return v8ToObject(value, NULL);

	LEAVE_V8(return NULL)
}

GVariant GV8ScriptObject::getRaw(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(name));
	if(getV8Type(value, NULL) == sdtRaw) {
		return v8ToVariant(this->getContext(), this->object->CreationContext(), value, NULL);
	}
	else {
		return GVariant();
	}

	LEAVE_V8(return GVariant())
}

IMetaMethod * GV8ScriptObject::getMethod(const char * methodName, void ** outInstance)
{
	ENTER_V8()

	if(outInstance != NULL) {
		*outInstance = NULL;
	}

	GMethodGlueDataPointer methodData = this->doGetMethodData(methodName);
	if(methodData && methodData->getMethodType() == gdmtMethod) {
		if(outInstance != NULL) {
			*outInstance = methodData->getMethodList()->getInstanceAt(0);
		}

		return gdynamic_cast<IMetaMethod *>(methodData->getMethodList()->getAt(0));
	}
	else {
		return NULL;
	}

	LEAVE_V8(return NULL)
}

IMetaList * GV8ScriptObject::getMethodList(const char * methodName)
{
	ENTER_V8()

	GMethodGlueDataPointer methodData = this->doGetMethodData(methodName);
	if(methodData && methodData->getMethodType() == gdmtMethodList) {
		methodData->getMethodList()->addReference();

		return methodData->getMethodList();
	}
	else {
		return NULL;
	}

	LEAVE_V8(return NULL)
}

void GV8ScriptObject::assignValue(const char * fromName, const char * toName)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(fromName));
	localObject->Set(String::New(toName), value);

	LEAVE_V8()
}

bool GV8ScriptObject::valueIsNull(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Local<Value> value = localObject->Get(String::New(name));
	return value.IsEmpty() || value->IsUndefined() || value->IsNull();

	LEAVE_V8(return false)
}

void GV8ScriptObject::nullifyValue(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	localObject->Set(String::New(name), Null());

	LEAVE_V8()
}

void GV8ScriptObject::bindCoreService(const char * name)
{
	ENTER_V8()

	this->getContext()->bindScriptCoreService(this, name);

	LEAVE_V8()
}

void GV8ScriptObject::doBindMethodList(const char * name, IMetaList * methodList, GGlueDataMethodType methodType)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	Handle<FunctionTemplate> functionTemplate = createMethodTemplate(this->getContext(), GClassGlueDataPointer(), true, methodList, name,
		Handle<FunctionTemplate>(), methodType);

	Local<Function> func = Local<Function>::New(functionTemplate->GetFunction());
	setObjectSignature(&func);

	localObject->Set(String::New(name), func);
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
