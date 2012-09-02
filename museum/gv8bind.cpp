#include "cpgf/scriptbind/gv8bind.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gstringmap.h"
#include "cpgf/gcallback.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"

#include <vector>
#include <set>
#include <map>


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


class GV8ScriptObject;
class GV8ScriptBindingParam;
class GUserDataPool;

class GV8ScriptObjectImplement
{
public:
	GV8ScriptObjectImplement(const GBindingParamPointer & param, Local<Object> object);
	~GV8ScriptObjectImplement();

	void doBindMethodList(const char * name, IMetaList * methodList, GUserDataMethodType methodType);
	GMethodUserData * doGetMethodUserData(const char * methodName);

public:
	GBindingParamPointer param;
	Persistent<Object> object;
};


GMAKE_FINAL(GV8ScriptObject)

class GV8ScriptObject : public GScriptObject, GFINAL_BASE(GV8ScriptObject)
{
private:
	typedef GScriptObject super;

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

	virtual GScriptObject * createScriptObject(const char * name);
	virtual GScriptObject * gainScriptObject(const char * name);

	virtual GScriptFunction * gainScriptFunction(const char * name);

	virtual GMetaVariant invoke(const char * name, const GMetaVariant * params, size_t paramCount);
	virtual GMetaVariant invokeIndirectly(const char * name, GMetaVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);
	virtual bool valueIsNull(const char * name);
	virtual void nullifyValue(const char * name);

	virtual void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible);

	virtual void bindCoreService(const char * name);

public:
	const GBindingParamPointer & getParam() const {
		return this->implement->param;
	}

	Local<Object> getObject() const {
		return Local<Object>::New(this->implement->object);
	}

private:
	GV8ScriptObject(const GV8ScriptObject & other, Local<Object> object);

private:
	GScopedPointer<GV8ScriptObjectImplement> implement;
	GSharedPointer<GUserDataPool> userDataPool;

private:
};


class GUserDataMap
{
private:
	typedef void * KeyType;
	typedef void * ValueType;

	typedef map<KeyType, ValueType> KeyValueMap;
	typedef map<ValueType, KeyType> ValueKeyMap;

public:
	GUserDataMap() : nextKey((KeyType)0) {
	}

	KeyType getNextKey() const {
		KeyType oldKey = this->nextKey;
		for(;;) {
			this->nextKey = this->incKey(this->nextKey);
			if(this->nextKey == (KeyType)0) {
				continue;
			}

			if(this->nextKey == oldKey) {
				break;
			}

			if(this->keyMap.find(this->nextKey) == this->keyMap.end()) {
				return this->nextKey;
			}
		}

		throw GException(-1, "V8 key space is full!");
	}

	KeyType addValue(ValueType value) {
		KeyType key = this->getNextKey();
		this->keyMap.insert(make_pair(key, value));
		this->valueMap.insert(make_pair(value, key));
		return key;
	}

	void removeValue(ValueType value) {
		ValueKeyMap::iterator it = this->valueMap.find(value);
		if(it != this->valueMap.end()) {
			this->keyMap.erase(this->keyMap.find(it->second));
			this->valueMap.erase(it);
		}
	}

	KeyType getKey(ValueType value) const {
		ValueKeyMap::iterator it = this->valueMap.find(value);
		if(it != this->valueMap.end()) {
			return it->second;
		}
		else {
			return (KeyType)0;
		}
	}

	ValueType getValue(KeyType key) const {
		KeyValueMap::const_iterator it = this->keyMap.find(key);
		if(it == this->keyMap.end()) {
			return (ValueType)0;
		}
		else {
			return it->second;
		}
	}

private:
	template <typename T>
	KeyType incKey(T * key) const {
		return (KeyType)(((char *)key) + 1);
	}

	template <typename T>
	KeyType incKey(T key) const {
		return (KeyType)(key + 1);
	}

private:
	mutable KeyType nextKey;
	mutable KeyValueMap keyMap;
	mutable ValueKeyMap valueMap;
};

GUserDataMap userDataMap;


class GUserDataPool
{
private:
	typedef std::set<GScriptUserData *> ListType;

public:
	~GUserDataPool() {
		for(ListType::iterator it = this->userDataList.begin(); it != this->userDataList.end(); ++it) {
			userDataMap.removeValue(*it);
			delete *it;
		}
	}

	void * addUserData(GScriptUserData * userData) {
		if(this->userDataList.find(userData) == this->userDataList.end()) {
			this->userDataList.insert(userData);
			return userDataMap.addValue(userData);
		}
		else {
			return userDataMap.getKey(userData);
		}
	}

	void removeUserData(GScriptUserData * userData) {
		ListType::iterator it = this->userDataList.find(userData);
		if(it != this->userDataList.end()) {
			userDataMap.removeValue(userData);
			GScriptUserData * p = *it;
			this->userDataList.erase(it);
			delete p;
		}
	}


private:
	ListType userDataList;
};

class GMapItemClassData : public GMetaMapItemData
{
public:
	virtual ~GMapItemClassData() {
		this->functionTemplate.Dispose();
		this->functionTemplate.Clear();
	}

	void setTemplate(Handle<FunctionTemplate> newTemplate) {
		this->functionTemplate = Persistent<FunctionTemplate>::New(newTemplate);
	}

	const Persistent<FunctionTemplate> & getFunctionTemplate() const {
		return this->functionTemplate;
	}

private:
	Persistent<FunctionTemplate> functionTemplate;
};

class GMapItemV8MethodData : public GMetaMapItemData
{
public:
	GMapItemV8MethodData() : userData(NULL) {
	}

	virtual ~GMapItemV8MethodData() {
		this->functionTemplate.Dispose();
		this->functionTemplate.Clear();
	}

	void setTemplate(Handle<FunctionTemplate> newTemplate) {
		this->functionTemplate = Persistent<FunctionTemplate>::New(newTemplate);
	}

	void setUserData(GMethodUserData * userData) {
		this->userData = userData;
	}

public:
	Persistent<FunctionTemplate> functionTemplate;
	GMethodUserData * userData;
};

class GMapItemEnumData : public GMetaMapItemData
{
public:
	GMapItemEnumData() : userData(NULL) {
	}

	virtual ~GMapItemEnumData() {
		this->objectTemplate.Dispose();
		this->objectTemplate.Clear();
	}

	void setTemplate(Handle<ObjectTemplate> newTemplate) {
		this->objectTemplate = Persistent<ObjectTemplate>::New(newTemplate);
	}

	void setUserData(GEnumUserData * userData) {
		this->userData = userData;
	}

	GEnumUserData * getUserData() const {
		return this->userData;
	}

public:
	Persistent<ObjectTemplate> objectTemplate;
	GEnumUserData * userData;
};


class GV8ScriptBindingParam : public GScriptBindingParam
{
private:
	typedef GScriptBindingParam super;

public:
	GV8ScriptBindingParam(IMetaService * service, const GScriptConfig & config, GUserDataPool * userDataPool)
		: super(service, config), userDataPool(userDataPool)
	{
	}

	virtual ~GV8ScriptBindingParam() {
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

	GUserDataPool * getUserDataPool() {
		return userDataPool;
	}

private:
	Persistent<ObjectTemplate> objectTemplate;
	GUserDataPool * userDataPool;
};


class GV8ScriptFunction : public GScriptFunction
{
public:
	GV8ScriptFunction(const GBindingParamPointer & bindingParam, Local<Object> receiver, Local<Value> func);
	virtual ~GV8ScriptFunction();

	virtual GMetaVariant invoke(const GMetaVariant * params, size_t paramCount);
	virtual GMetaVariant invokeIndirectly(GMetaVariant const * const * params, size_t paramCount);

private:
	GWeakBindingParamPointer bindingParam;
	Persistent<Object> receiver;
	Persistent<Function> func;
};


void weakHandleCallback(Persistent<Value> object, void * parameter);
Handle<FunctionTemplate> doCreateClassTemplate(const GBindingParamPointer & param, IMetaClass * metaClass);
Handle<FunctionTemplate> createClassTemplate(const GBindingParamPointer & param, IMetaClass * metaClass);

Handle<Value> variantToV8(const GBindingParamPointer & param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw);

GMetaMapClass * getMetaClassMap(const GBindingParamPointer & param, IMetaClass * metaClass);

const char * signatureKey = "i_sig_cpgf";
const int signatureValue = 0x168feed;
const char * userDataKey = "i_userdata_cpgf";

template <typename T>
void setObjectSignature(T * object)
{
	(*object)->SetHiddenValue(String::New(signatureKey), Int32::New(signatureValue));
}

void error(const char * message)
{
	ThrowException(String::New(message));
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

void * addUserDataToPool(const GBindingParamPointer & param, GScriptUserData * userData)
{
	return gdynamic_cast<GV8ScriptBindingParam *>(param.get())->getUserDataPool()->addUserData(userData);
}

void removeUserDataFromPool(const GBindingParamPointer & param, GScriptUserData * userData)
{
	gdynamic_cast<GV8ScriptBindingParam *>(param.get())->getUserDataPool()->removeUserData(userData);
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
						GScriptUserData * userData = static_cast<GScriptUserData *>(Handle<External>::Cast(value)->Value());
						switch(userData->getType()) {
							case udtObject:
								if(typeItem != NULL) {
									*typeItem = gdynamic_cast<GObjectUserData *>(userData)->getObjectData()->getMetaClass();
									(*typeItem)->addReference();
								}
								return sdtClass;

							case udtExtendMethod:
								return methodTypeToUserDataType(gdynamic_cast<GMethodUserData *>(userData)->getMethodData().getMethodType());

							default:
								break;
						}
					}
				}

			}
			else {
				GScriptUserData * userData = static_cast<GScriptUserData *>(obj->GetPointerFromInternalField(0));
				if(userData != NULL) {
					switch(userData->getType()) {
						case udtObject: {
							GObjectUserData * classData = gdynamic_cast<GObjectUserData *>(userData);
							if(typeItem != NULL) {
								*typeItem = classData->getObjectData()->getMetaClass();
								(*typeItem)->addReference();
							}
							if(classData->getObjectData()->getInstance() == NULL) {
								return sdtClass;
							}
							else {
								return sdtObject;
							}
						}

						break;

						case udtEnum:
							if(typeItem != NULL) {
								*typeItem = gdynamic_cast<GEnumUserData *>(userData)->getMetaEnum();
								(*typeItem)->addReference();
							}
							return sdtEnum;

					    case udtRaw:
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

Handle<Value> objectToV8(const GBindingParamPointer & param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv, ObjectUserDataType dataType)
{
	if(instance == NULL) {
		return Handle<Value>();
	}

	GMetaMapClass * map = getMetaClassMap(param, metaClass);
	GMapItemClassData * mapData = gdynamic_cast<GMapItemClassData *>(map->getData());
	Handle<FunctionTemplate> functionTemplate = mapData->getFunctionTemplate();
	Handle<Value> external = External::New(&signatureKey);
	Persistent<Object> self = Persistent<Object>::New(functionTemplate->GetFunction()->NewInstance(1, &external));

	GObjectUserData * instanceUserData = new GObjectUserData(param, metaClass, instance, allowGC, cv, dataType);
	void * key = addUserDataToPool(param, instanceUserData);
	self.MakeWeak(key, weakHandleCallback);

	self->SetPointerInInternalField(0, instanceUserData);
	setObjectSignature(&self);

	return self;
}

Handle<Value> rawToV8(const GBindingParamPointer & param, const GVariant & value)
{
	if(param->getConfig().allowAccessRawData()) {
		Persistent<Object> self = Persistent<Object>::New(gdynamic_cast<GV8ScriptBindingParam *>(param.get())->getRawObject());

		GRawUserData * instanceUserData = new GRawUserData(param, value);
		void * key = addUserDataToPool(param, instanceUserData);
		self.MakeWeak(key, weakHandleCallback);

		self->SetPointerInInternalField(0, instanceUserData);
		setObjectSignature(&self);

		return self;
	}

	return Handle<Value>();
}

struct GV8Methods
{
	typedef Handle<Value> ResultType;
	
	static ResultType doObjectToScript(const GBindingParamPointer & param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv, ObjectUserDataType dataType)
	{
		return objectToV8(param, instance, metaClass, allowGC, cv, dataType);
	}

	static ResultType doVariantToScript(const GBindingParamPointer & param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
	{
		return variantToV8(param, value, type, allowGC, allowRaw);
	}
	
	static ResultType doRawToScript(const GBindingParamPointer & param, const GVariant & value)
	{
		return rawToV8(param, value);
	}

	static ResultType doConverterToScript(const GBindingParamPointer & param, const GVariant & value, IMetaConverter * converter)
	{
		ResultType result;
		if(converterToScript<GV8Methods>(&result, param, value, converter)) {
			return result;
		}
		return Handle<Value>();
	}

	static ResultType doClassToScript(const GBindingParamPointer & param, IMetaClass * metaClass)
	{
		Handle<FunctionTemplate> functionTemplate = createClassTemplate(param, metaClass);
		return functionTemplate->GetFunction();
	}

	static ResultType doStringToScript(const GBindingParamPointer & /*param*/, const char * s)
	{
		return String::New(s);
	}

	static ResultType doWideStringToScript(const GBindingParamPointer & /*param*/, const wchar_t * ws)
	{
		GScopedArray<char> s(wideStringToString(ws));
		return String::New(s.get());
	}

	static bool isSuccessResult(const ResultType & result)
	{
		return ! result.IsEmpty();
	}
};

Handle<Value> variantToV8(const GBindingParamPointer & param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
{
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

	Handle<Value> result;
	if(variantToScript<GV8Methods>(&result, param, value, type, allowGC, allowRaw)) {
		return result;
	}

	return Handle<Value>();
}

void * v8ToObject(Handle<Value> value, GMetaType * outType)
{
	if(isValidObject(value)) {
		GScriptUserData * userData = static_cast<GScriptUserData *>(Handle<Object>::Cast(value)->GetPointerFromInternalField(0));
		if(userData != NULL && userData->getType() == udtObject) {
			GObjectUserData * classData = gdynamic_cast<GObjectUserData *>(userData);
			if(outType != NULL) {
				GMetaTypeData typeData;
				classData->getObjectData()->getMetaClass()->getMetaType(&typeData);
				metaCheckError(classData->getObjectData()->getMetaClass());
				*outType = GMetaType(typeData);
			}

			return classData->getObjectData()->getInstance();
		}
	}

	return NULL;
}

GMetaVariant v8UserDataToVariant(const GBindingParamPointer & param, Local<Context> context, Handle<Value> value)
{
	if(value->IsFunction() || value->IsObject()) {
		Local<Object> obj = value->ToObject();
		if(isValidObject(obj)) {
			GScriptUserData * userData = static_cast<GScriptUserData *>(obj->GetPointerFromInternalField(0));
			if(userData == NULL) { // value maybe an IMetaClass
				Handle<Value> data = obj->GetHiddenValue(String::New(userDataKey));
				if(! data.IsEmpty() && data->IsExternal()) {
					userData = static_cast<GScriptUserData *>(Handle<External>::Cast(data)->Value());
				}
			}
			if(userData != NULL) {
				return userDataToVariant(userData);
			}
		}
		else {
			if(value->IsFunction()) {
				GScopedInterface<IScriptFunction> func(new ImplScriptFunction(new GV8ScriptFunction(param, context->Global(), Local<Value>::New(value)), true));

				return GMetaVariant(func.get(), GMetaType());
			}
			else {
				GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GV8ScriptObject(param->getService(), obj, param->getConfig()), true));

				return GMetaVariant(scriptObject.get(), GMetaType());
			}
		}
	}

	return GMetaVariant();
}

GMetaVariant v8ToVariant(const GBindingParamPointer & param, Local<Context> context, Handle<Value> value)
{
	if(value.IsEmpty()) {
		return GMetaVariant();
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
		return GMetaVariant(createStringVariant(*s), createMetaType<char *>());
	}

	if(value->IsUint32()) {
		return value->Uint32Value();
	}

	if(value->IsFunction() || value->IsObject()) {
		return v8UserDataToVariant(param, context, value);
	}

	return GMetaVariant();
}

void weakHandleCallback(Persistent<Value> object, void * parameter)
{
	GScriptUserData * userData = static_cast<GScriptUserData *>(userDataMap.getValue(parameter));

	if(userData != NULL) {
		removeUserDataFromPool(userData->getParam(), userData);
	}

	object.Dispose();
	object.Clear();
}

void loadMethodParameters(const Arguments & args, const GBindingParamPointer & param, GVariant * outputParams)
{
	for(int i = 0; i < args.Length(); ++i) {
		outputParams[i] = v8ToVariant(param, args.Holder()->CreationContext(), args[i]).getValue();
	}
}

void loadMethodParamTypes(const Arguments & args, CallableParamDataType * outputTypes)
{
	for(int i = 0; i < args.Length(); ++i) {
		IMetaTypedItem * typeItem;
		outputTypes[i].dataType = getV8Type(args[i], &typeItem);
		outputTypes[i].typeItem.reset(typeItem);
	}
}

void loadCallableParam(const Arguments & args, const GBindingParamPointer & param, InvokeCallableParam * callableParam)
{
	loadMethodParameters(args, param, callableParam->paramsData);
	loadMethodParamTypes(args, callableParam->paramsType);
}

Handle<Value> accessibleGet(Local<String> /*prop*/, const AccessorInfo & info)
{
	ENTER_V8()

	GAccessibleUserData * userData = static_cast<GAccessibleUserData *>(Local<External>::Cast(info.Data())->Value());

	Handle<Value> r;
	if(accessibleToScript<GV8Methods>(&r, userData->getParam(), userData->getAccessible(), userData->getInstance(), false)) {
		return r;
	}
	return Handle<Value>();

	LEAVE_V8(return Handle<Value>())
}

void accessibleSet(Local<String> /*prop*/, Local<Value> value, const AccessorInfo & info)
{
	ENTER_V8()

	HandleScope handleScope;

	GAccessibleUserData * userData = static_cast<GAccessibleUserData *>(Local<External>::Cast(info.Data())->Value());

	GMetaVariant v = v8ToVariant(userData->getParam(), info.Holder()->CreationContext(), value);
	metaSetValue(userData->getAccessible(), userData->getInstance(), v.getValue());

	LEAVE_V8()
}

void doBindAccessible(const GBindingParamPointer & param, Local<Object> container,
	const char * name, void * instance, IMetaAccessible * accessible)
{
	GAccessibleUserData * userData = new GAccessibleUserData(param, instance, accessible);
	void * key = addUserDataToPool(param, userData);
	Persistent<External> data = Persistent<External>::New(External::New(userData));
	data.MakeWeak(key, weakHandleCallback);

	container->SetAccessor(String::New(name), &accessibleGet, &accessibleSet, data);
}

Handle<Value> methodResultToV8(const GBindingParamPointer & param, IMetaCallable * callable, InvokeCallableResult * result)
{
	Handle<Value> r;
	if(methodResultToScript<GV8Methods>(&r, param, callable, result)) {
		return r;
	}

	return Handle<Value>();
}

Handle<Value> callbackMethodList(const Arguments & args)
{
	ENTER_V8()

	bool isGlobal = isGlobalObject(args.Holder());

	if(!isGlobal && !isValidObject(args.Holder())) {
		raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
	}

	GObjectUserData * objectUserData = NULL;

	if(!isGlobal) {
		objectUserData = static_cast<GObjectUserData *>(args.Holder()->GetPointerFromInternalField(0));
	}

	Local<External> data = Local<External>::Cast(args.Data());
	GMethodUserData * methodUserData = static_cast<GMethodUserData *>(data->Value());

	InvokeCallableParam callableParam(args.Length());
	loadCallableParam(args, methodUserData->getParam(), &callableParam);

	InvokeCallableResult result = doInvokeMethodList(methodUserData->getParam(), objectUserData, methodUserData->getMethodData(), &callableParam);
	return methodResultToV8(methodUserData->getParam(), result.callable.get(), &result);

	LEAVE_V8(return Handle<Value>())
}

Handle<FunctionTemplate> createMethodTemplate(const GBindingParamPointer & param, IMetaClass * metaClass, bool isGlobal, IMetaList * methodList,
	const char * name, Handle<FunctionTemplate> classTemplate, GUserDataMethodType methodType, GMethodUserData ** outUserData)
{
	GMethodUserData * userData = new GMethodUserData(param, metaClass, methodList, name, methodType);
	if(outUserData != NULL) {
		*outUserData = userData;
	}

	Persistent<External> data = Persistent<External>::New(External::New(userData));
	addUserDataToPool(param, userData);
	data.MakeWeak(NULL, weakHandleCallback);

	Handle<FunctionTemplate> functionTemplate;
	if(metaClass == NULL || isGlobal) {
		functionTemplate = FunctionTemplate::New(callbackMethodList, data);
	}
	else {
		functionTemplate = FunctionTemplate::New(callbackMethodList, data, Signature::New(classTemplate));
	}
	functionTemplate->SetClassName(String::New(name));

	Local<Function> func = functionTemplate->GetFunction();
	setObjectSignature(&func);
	GMethodUserData * funcUserData = new GMethodUserData(param, metaClass, methodList, name, methodType);
	Persistent<External> funcData = Persistent<External>::New(External::New(funcUserData));
	void * key = addUserDataToPool(param, funcUserData);
	funcData.MakeWeak(key, weakHandleCallback);
	func->SetHiddenValue(String::New(userDataKey), funcData);

	return functionTemplate;
}

Handle<Value> namedEnumGetter(Local<String> prop, const AccessorInfo & info)
{
	ENTER_V8()

	GEnumUserData * userData = static_cast<GEnumUserData *>(info.Holder()->GetPointerFromInternalField(0));
	IMetaEnum * metaEnum = userData->getMetaEnum();
	String::AsciiValue name(prop);
	int32_t index = metaEnum->findKey(*name);
	if(index >= 0) {
		return variantToV8(userData->getParam(), metaGetEnumValue(metaEnum, index), GMetaType(), true, false);
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

	GEnumUserData * userData = static_cast<GEnumUserData *>(info.Holder()->GetPointerFromInternalField(0));
	IMetaEnum * metaEnum = userData->getMetaEnum();
	uint32_t keyCount = metaEnum->getCount();

	HandleScope handleScope;

	Local<Array> metaNames = Array::New(keyCount);
	for(uint32_t i = 0; i < keyCount; ++i) {
		metaNames->Set(Number::New(i), String::New(metaEnum->getKey(i)));
	}

	return handleScope.Close(metaNames);

	LEAVE_V8(return Handle<Array>())
}

Handle<ObjectTemplate> createEnumTemplate(const GBindingParamPointer & param, IMetaEnum * metaEnum,
	const char * /*name*/, GEnumUserData ** outUserData)
{
	GEnumUserData * userData = new GEnumUserData(param, metaEnum);
	addUserDataToPool(param, userData);
	if(outUserData != NULL) {
		*outUserData = userData;
	}

	Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();
	objectTemplate->SetInternalFieldCount(1);
	objectTemplate->SetNamedPropertyHandler(&namedEnumGetter, &namedEnumSetter, NULL, NULL, &namedEnumEnumerator);

	return objectTemplate;
}

Handle<Value> getNamedMember(GObjectUserData * userData, const char * name)
{
	GMetaClassTraveller traveller(userData->getObjectData()->getMetaClass(), userData->getObjectData()->getInstance());

	void * instance = NULL;
	IMetaClass * outDerived;

	for(;;) {
		GScopedInterface<IMetaClass> metaClass(traveller.next(&instance, &outDerived));
		GScopedInterface<IMetaClass> derived(outDerived);

		if(!metaClass) {
			break;
		}

		GMetaMapClass * mapClass = getMetaClassMap(userData->getParam(), metaClass.get());
		if(mapClass == NULL) {
			continue;
		}
		GMetaMapItem * mapItem = mapClass->findItem(name);
		if(mapItem == NULL) {
			continue;
		}

		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {
				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(userData->getParam()->getConfig(), getObjectData(userData), data.get())) {
					Handle<Value> r;
					if(accessibleToScript<GV8Methods>(&r, userData->getParam(), data.get(), instance, userData->getObjectData()->getCV() == opcvConst)) {
						return r;
					}
					return Handle<Value>();
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList: {
				GMapItemV8MethodData * data = gdynamic_cast<GMapItemV8MethodData *>(mapItem->getData());
				if(data == NULL) {
					GScopedInterface<IMetaList> methodList(createMetaList());
					loadMethodList(&traveller, methodList.get(), userData->getParam()->getMetaMap(), mapItem, instance, name);

					data = new GMapItemV8MethodData;
					mapItem->setData(data);
					GMethodUserData * newUserData;

					GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass.get(), derived.get()));

					GMetaMapClass * baseMapClass = getMetaClassMap(userData->getParam(), boundClass.get());
					data->setTemplate(createMethodTemplate(userData->getParam(), userData->getObjectData()->getMetaClass(),
						userData->getObjectData()->getInstance() == NULL, methodList.get(), name,
						gdynamic_cast<GMapItemClassData *>(baseMapClass->getData())->getFunctionTemplate(), udmtInternal, &newUserData));
					data->setUserData(newUserData);
				}

				return data->functionTemplate->GetFunction();
			}

			case mmitEnum:
				if(! userData->getObjectData()->isInstance() || userData->getParam()->getConfig().allowAccessEnumTypeViaInstance()) {
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
				if(! userData->getObjectData()->isInstance() || userData->getParam()->getConfig().allowAccessEnumValueViaInstance()) {
					GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
					return variantToV8(userData->getParam(), metaGetEnumValue(metaEnum, static_cast<uint32_t>(mapItem->getEnumIndex())), GMetaType(), false, true);
				}
				break;

			case mmitClass:
				if(! userData->getObjectData()->isInstance() || userData->getParam()->getConfig().allowAccessClassViaInstance()) {
					GScopedInterface<IMetaClass> innerMetaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
					Handle<FunctionTemplate> functionTemplate = createClassTemplate(userData->getParam(), innerMetaClass.get());
					return functionTemplate->GetFunction();
				}
				break;

			default:
				break;
		}
	}

	return Handle<Value>();
}

Handle<Value> staticMemberGetter(Local<String> prop, const AccessorInfo & info)
{
	ENTER_V8()

	GObjectUserData * userData = static_cast<GObjectUserData *>(Local<External>::Cast(info.Data())->Value());

	String::Utf8Value utf8_prop(prop);
	const char * name = *utf8_prop;

	return getNamedMember(userData, name);

	LEAVE_V8(return Handle<Value>())
}

void staticMemberSetter(Local<String> prop, Local<Value> value, const AccessorInfo & info)
{
	ENTER_V8()

	GObjectUserData * userData = static_cast<GObjectUserData *>(Local<External>::Cast(info.Data())->Value());

	String::Utf8Value utf8_prop(prop);
	const char * name = *utf8_prop;

	doSetFieldValue(userData, name, v8ToVariant(userData->getParam(), info.Holder()->CreationContext(), value).getValue());

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

	GObjectUserData * userData = static_cast<GObjectUserData *>(info.Holder()->GetPointerFromInternalField(0));

	return getNamedMember(userData, name);

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

	GObjectUserData * userData = static_cast<GObjectUserData *>(info.Holder()->GetPointerFromInternalField(0));

	if(userData->getObjectData()->getCV() == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);

		return Handle<Value>();
	}

	if(userData == NULL) {
		return Handle<Value>();
	}


	if(doSetFieldValue(userData, name, v8ToVariant(userData->getParam(), info.Holder()->CreationContext(), value).getValue())) {
		return value;
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

	GObjectUserData * userData = static_cast<GObjectUserData *>(info.Holder()->GetPointerFromInternalField(0));

	GMetaClassTraveller traveller(userData->getObjectData()->getMetaClass(), userData->getObjectData()->getInstance());
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

void accessorNamedMemberSetter(Local<String> prop, Local<Value> value, const AccessorInfo & info)
{
	namedEnumSetter(prop, value, info);
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

void * invokeConstructor(const Arguments & args, const GBindingParamPointer & param, IMetaClass * metaClass)
{
	InvokeCallableParam callableParam(args.Length());
	loadCallableParam(args, param, &callableParam);

	void * instance = doInvokeConstructor(param->getService(), metaClass, &callableParam);

	if(instance != NULL) {
		return instance;
	}
	else {
		raiseCoreException(Error_ScriptBinding_InternalError_WrongFunctor);
	}

	return NULL;
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
		GObjectUserData * classUserData = static_cast<GObjectUserData *>(data->Value());

		void * instance = invokeConstructor(args, classUserData->getParam(), classUserData->getObjectData()->getMetaClass());

		GObjectUserData * instanceUserData = new GObjectUserData(classUserData->getParam(), classUserData->getObjectData()->getMetaClass(), instance, true, opcvNone, cudtObject);
		instanceCreated(instanceUserData, classUserData);
		void * key = addUserDataToPool(classUserData->getParam(), instanceUserData);
		self.MakeWeak(key, weakHandleCallback);

		self->SetPointerInInternalField(0, instanceUserData);
		setObjectSignature(&self);
	}

	return self;

	LEAVE_V8(return Handle<Value>());
}

GMetaMapClass * getMetaClassMap(const GBindingParamPointer & param, IMetaClass * metaClass)
{
	GMetaMapClass * map = param->getMetaMap()->findClassMap(metaClass);

	if(map->getData() == NULL) {
		createClassTemplate(param, metaClass);
	}

	return map;
}

Handle<FunctionTemplate> doCreateClassTemplate(const GBindingParamPointer & param, IMetaClass * metaClass)
{
	GObjectUserData * userData = new GObjectUserData(param, metaClass, NULL, false, opcvNone, cudtClass);
	void * key = addUserDataToPool(param, userData);
	Persistent<External> data = Persistent<External>::New(External::New(userData));
	data.MakeWeak(key, weakHandleCallback);

	Handle<FunctionTemplate> functionTemplate = FunctionTemplate::New(objectConstructor, data);
	functionTemplate->SetClassName(String::New(metaClass->getName()));

	Local<ObjectTemplate> instanceTemplate = functionTemplate->InstanceTemplate();
	instanceTemplate->SetInternalFieldCount(1);

	instanceTemplate->SetNamedPropertyHandler(&namedMemberGetter, &namedMemberSetter, NULL, NULL, &namedMemberEnumerator);

	if(metaClass->getBaseCount() > 0) {
		GScopedInterface<IMetaClass> baseClass(metaClass->getBaseClass(0));
		if(baseClass) {
			Handle<FunctionTemplate> baseFunctionTemplate = createClassTemplate(param, baseClass.get());
			functionTemplate->Inherit(baseFunctionTemplate);
		}
	}

	Local<Function> classFunction = functionTemplate->GetFunction();
	setObjectSignature(&classFunction);
	bindClassItems(classFunction, metaClass, data);

	classFunction->SetHiddenValue(String::New(userDataKey), data);

	return functionTemplate;
}

Handle<FunctionTemplate> createClassTemplate(const GBindingParamPointer & param, IMetaClass * metaClass)
{
	GMetaMapClass * map = param->getMetaMap()->findClassMap(metaClass);

	if(map->getData() != NULL) {
		return gdynamic_cast<GMapItemClassData *>(map->getData())->getFunctionTemplate();
	}

	Handle<FunctionTemplate> functionTemplate = doCreateClassTemplate(param, metaClass);

	GMapItemClassData * mapData = new GMapItemClassData;
	mapData->setTemplate(functionTemplate);
	map->setData(mapData);

	return functionTemplate;
}

void doBindClass(const GBindingParamPointer & param, Local<Object> container, const char * name, IMetaClass * metaClass)
{
	Handle<FunctionTemplate> functionTemplate = createClassTemplate(param, metaClass);
	container->Set(String::New(name), functionTemplate->GetFunction());
}


GV8ScriptObjectImplement::GV8ScriptObjectImplement(const GBindingParamPointer & param, Local<Object> object)
	: param(param), object(Persistent<Object>::New(object))
{
}

GV8ScriptObjectImplement::~GV8ScriptObjectImplement()
{
	this->object.Dispose();
}

void GV8ScriptObjectImplement::doBindMethodList(const char * name, IMetaList * methodList, GUserDataMethodType methodType)
{
	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->object));

	GMethodUserData * newUserData;
	Handle<FunctionTemplate> functionTemplate = createMethodTemplate(this->param, NULL, true, methodList, name,
		Handle<FunctionTemplate>(), methodType, &newUserData);

	Persistent<Function> func = Persistent<Function>::New(functionTemplate->GetFunction());
	setObjectSignature(&func);
	func.MakeWeak(NULL, weakHandleCallback);

	localObject->Set(String::New(name), func);
}

GMethodUserData * GV8ScriptObjectImplement::doGetMethodUserData(const char * methodName)
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
					GScriptUserData * userData = static_cast<GScriptUserData *>(Handle<External>::Cast(data)->Value());
					if(userData->getType() == udtExtendMethod) {
						GMethodUserData * methodListData = gdynamic_cast<GMethodUserData *>(userData);
						if(methodListData->getMethodData().getMethodList()) {
							return methodListData;
						}
					}
				}
			}

		}
	}

	return NULL;
}


bool valueIsCallable(Local<Value> value)
{
	return value->IsFunction() || (value->IsObject() && Local<Object>::Cast(value)->IsCallable());
}

GMetaVariant invokeV8FunctionIndirectly(const GBindingParamPointer & bindingParam, Local<Object> object, Local<Value> func, GMetaVariant const * const * params, size_t paramCount, const char * name)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(! bindingParam) {
		raiseCoreException(Error_ScriptBinding_NoContext);
	}

	if(valueIsCallable(func)) {
		Handle<Value> v8Params[REF_MAX_ARITY];
		for(size_t i = 0; i < paramCount; ++i) {
			v8Params[i] = variantToV8(bindingParam, params[i]->getValue(), params[i]->getType(), false, true);
			if(v8Params[i].IsEmpty()) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
		}

		Local<Value> result;
		Handle<Object> receiver = object;

		if(func->IsFunction()) {
			result = Local<Function>::Cast(func)->Call(receiver, static_cast<int>(paramCount), v8Params);
		}
		else {
			result = Local<Object>::Cast(func)->CallAsFunction(receiver, static_cast<int>(paramCount), v8Params);
		}

		return v8ToVariant(bindingParam, receiver->CreationContext(), result);
	}
	else {
		raiseCoreException(Error_ScriptBinding_CantCallNonfunction);
	}

	return GMetaVariant();
}


GV8ScriptFunction::GV8ScriptFunction(const GBindingParamPointer & bindingParam, Local<Object> receiver, Local<Value> func)
	: bindingParam(bindingParam),
		receiver(Persistent<Object>::New(Local<Object>::Cast(receiver))),
		func(Persistent<Function>::New(Local<Function>::Cast(func)))
{
}

GV8ScriptFunction::~GV8ScriptFunction()
{
	this->receiver.Dispose();
	this->receiver.Clear();
	this->func.Dispose();
	this->func.Clear();
}

GMetaVariant GV8ScriptFunction::invoke(const GMetaVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GMetaVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GMetaVariant GV8ScriptFunction::invokeIndirectly(GMetaVariant const * const * params, size_t paramCount)
{
	ENTER_V8()

	HandleScope handleScope;

	Local<Object> receiver = Local<Object>::New(this->receiver);
	return invokeV8FunctionIndirectly(this->bindingParam.get(), receiver, Local<Value>::New(this->func), params, paramCount, "");

	LEAVE_V8(return GMetaVariant())
}



GV8ScriptObject::GV8ScriptObject(IMetaService * service, Local<Object> object, const GScriptConfig & config)
	: super(config), userDataPool(new GUserDataPool())
{
	this->implement.reset(new GV8ScriptObjectImplement(GBindingParamPointer(new GV8ScriptBindingParam(service, config, userDataPool.get())), object));
}

GV8ScriptObject::GV8ScriptObject(const GV8ScriptObject & other, Local<Object> object)
	: super(other.implement->param->getConfig()), userDataPool(other.userDataPool)
{
	this->implement.reset(new GV8ScriptObjectImplement(other.implement->param, object));
}

GV8ScriptObject::~GV8ScriptObject()
{
}

GScriptDataType GV8ScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> obj = localObject->Get(String::New(name));
	return getV8Type(obj, outMetaTypeItem);

	LEAVE_V8(return sdtUnknown)
}

void GV8ScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	doBindClass(this->implement->param, localObject, name, metaClass);

	LEAVE_V8()
}

void GV8ScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	GEnumUserData * newUserData;
	Handle<ObjectTemplate> objectTemplate = createEnumTemplate(this->implement->param, metaEnum, name, &newUserData);
	objectTemplate->SetInternalFieldCount(1);
	Persistent<Object> obj = Persistent<Object>::New(objectTemplate->NewInstance());
	obj->SetPointerInInternalField(0, newUserData);
	setObjectSignature(&obj);
	void * key = addUserDataToPool(this->implement->param, newUserData);
	obj.MakeWeak(key, weakHandleCallback);

	localObject->Set(String::New(name), obj);

	LEAVE_V8()
}

GScriptObject * GV8ScriptObject::createScriptObject(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));
	if(isValidObject(value)) {
		return NULL;
	}

	Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();
	Local<Object> obj = objectTemplate->NewInstance();
	localObject->Set(String::New(name), obj);

	GV8ScriptObject * binding = new GV8ScriptObject(*this, obj);
	binding->owner = this;
	binding->name = name;

	return binding;

	LEAVE_V8(return NULL)
}

GScriptObject * GV8ScriptObject::gainScriptObject(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));
	if((value->IsObject() || value->IsFunction())) { // { && !isValidObject(value)) {
		GV8ScriptObject * binding = new GV8ScriptObject(*this, Local<Object>::Cast(value));
		binding->owner = this;
		binding->name = name;

		return binding;
	}
	else {
		return NULL;
	}

	LEAVE_V8(return NULL)
}

GScriptFunction * GV8ScriptObject::gainScriptFunction(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));

	if(valueIsCallable(value)) {
		return new GV8ScriptFunction(this->getParam(), localObject, value);
	}
	else {
		return NULL;
	}

	LEAVE_V8(return NULL)
}

GMetaVariant GV8ScriptObject::invoke(const char * name, const GMetaVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GMetaVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GMetaVariant GV8ScriptObject::invokeIndirectly(const char * name, GMetaVariant const * const * params, size_t paramCount)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> func = localObject->Get(String::New(name));

	return invokeV8FunctionIndirectly(this->getParam(), this->getObject(), func, params, paramCount, name);

	LEAVE_V8(return GMetaVariant())
}

void GV8ScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.data.typeData)), "Only fundamental value can be bound via bindFundamental");

	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	localObject->Set(String::New(name), variantToV8(this->implement->param, value, GMetaType(), false, true));

	LEAVE_V8()
}

void GV8ScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	doBindAccessible(this->implement->param, localObject, name, instance, accessible);

	LEAVE_V8()
}

void GV8ScriptObject::bindString(const char * stringName, const char * s)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	localObject->Set(String::New(stringName), String::New(s));

	LEAVE_V8()
}

void GV8ScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Handle<Value> obj = objectToV8(this->implement->param, instance, type, transferOwnership, opcvNone, cudtObject);
	localObject->Set(String::New(objectName), obj);

	LEAVE_V8()
}

void GV8ScriptObject::bindRaw(const char * name, const GVariant & value)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	localObject->Set(String::New(name), rawToV8(this->implement->param, value));

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

	this->implement->doBindMethodList(name, methodList.get(), udmtMethod);

	LEAVE_V8()
}

void GV8ScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	ENTER_V8()

	this->implement->doBindMethodList(name, methodList, udmtMethodList);

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
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));
	if(getV8Type(value, NULL) == sdtFundamental) {
		return v8ToVariant(this->getParam(), this->implement->object->CreationContext(), value).getValue();
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
	Local<Object> localObject(Local<Object>::New(this->implement->object));

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
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(objectName));
	return v8ToObject(value, NULL);

	LEAVE_V8(return NULL)
}

GVariant GV8ScriptObject::getRaw(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));
	if(getV8Type(value, NULL) == sdtRaw) {
		return v8ToVariant(this->getParam(), this->implement->object->CreationContext(), value).getValue();
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

	GMethodUserData * userData = this->implement->doGetMethodUserData(methodName);
	if(userData != NULL && userData->getMethodData().getMethodType() == udmtMethod) {
		if(outInstance != NULL) {
			*outInstance = userData->getMethodData().getMethodList()->getInstanceAt(0);
		}

		return gdynamic_cast<IMetaMethod *>(userData->getMethodData().getMethodList()->getAt(0));
	}
	else {
		return NULL;
	}

	LEAVE_V8(return NULL)
}

IMetaList * GV8ScriptObject::getMethodList(const char * methodName)
{
	ENTER_V8()

	GMethodUserData * userData = this->implement->doGetMethodUserData(methodName);
	if(userData != NULL && userData->getMethodData().getMethodType() == udmtMethodList) {
		userData->getMethodData().getMethodList()->addReference();

		return userData->getMethodData().getMethodList();
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
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(fromName));
	localObject->Set(String::New(toName), value);

	LEAVE_V8()
}

bool GV8ScriptObject::valueIsNull(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));
	return value.IsEmpty() || value->IsUndefined() || value->IsNull();

	LEAVE_V8(return false)
}

void GV8ScriptObject::nullifyValue(const char * name)
{
	ENTER_V8()

	HandleScope handleScope;
	Local<Object> localObject(Local<Object>::New(this->implement->object));

	localObject->Set(String::New(name), Null());

	LEAVE_V8()
}

void GV8ScriptObject::bindCoreService(const char * name)
{
	ENTER_V8()

	this->implement->param->bindScriptCoreService(this, name);

	LEAVE_V8()
}


} // unnamed namespace


GScriptObject * old_createV8ScriptObject(IMetaService * service, Local<Object> object, const GScriptConfig & config)
{
	return new GV8ScriptObject(service, object, config);
}

IScriptObject * old_createV8ScriptInterface(IMetaService * service, Local<Object> object, const GScriptConfig & config)
{
	return new ImplScriptObject(new GV8ScriptObject(service, object, config), true);
}



} // namespace cpgf




#if defined(_MSC_VER)
#pragma warning(pop)
#endif


