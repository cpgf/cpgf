#include "cpgf/scriptbind/gv8bind.h"
#include "cpgf/gmetaclasstraveller.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"

#include <vector>

// for test
#include <iostream>
using namespace std;


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
	} if(local_error) { local_msg[255] = 0; v8::ThrowException(v8::String::New(local_msg)); } \
	__VA_ARGS__;

namespace cpgf {

namespace {

	class GMapItemClassData : public GMetaMapItemData
	{
	public:
		virtual ~GMapItemClassData() {
			this->functionTemplate.Dispose();
			this->functionTemplate.Clear();
		}

		void setTemplate(v8::Handle<v8::FunctionTemplate> newTemplate) {
			this->functionTemplate = v8::Persistent<v8::FunctionTemplate>::New(newTemplate);
		}

	public:
		v8::Persistent<v8::FunctionTemplate> functionTemplate;
	};

	class GMapItemMethodData : public GMetaMapItemData
	{
	public:
		virtual ~GMapItemMethodData() {
			this->functionTemplate.Dispose();
			this->functionTemplate.Clear();
		}

		void setTemplate(v8::Handle<v8::FunctionTemplate> newTemplate) {
			this->functionTemplate = v8::Persistent<v8::FunctionTemplate>::New(newTemplate);
		}

		void setUserData(GMethodListUserData * userData) {
			this->userData.reset(userData);
		}

		GMethodListUserData * getUserData() const {
		    return this->userData.get();
		}

	public:
		v8::Persistent<v8::FunctionTemplate> functionTemplate;
		GScopedPointer<GMethodListUserData> userData;
	};

	class GMapItemEnumData : public GMetaMapItemData
	{
	public:
		virtual ~GMapItemEnumData() {
			this->objectTemplate.Dispose();
			this->objectTemplate.Clear();
		}

		void setTemplate(v8::Handle<v8::ObjectTemplate> newTemplate) {
			this->objectTemplate = v8::Persistent<v8::ObjectTemplate>::New(newTemplate);
		}

		void setUserData(GEnumUserData * userData) {
			this->userData.reset(userData);
		}

		GEnumUserData * getUserData() const {
		    return this->userData.get();
		}

	public:
		v8::Persistent<v8::ObjectTemplate> objectTemplate;
		GScopedPointer<GEnumUserData> userData;
	};


	void weakHandleCallback(v8::Persistent<v8::Value> object, void * parameter);
	v8::Handle<v8::FunctionTemplate> createClassTemplate(GScriptBindingParam * param, const char * name, IMetaClass * metaClass);

	const char * signatureKey = "i_sig_cpgf";
	const int signatureValue = 0x168feed;

	template <typename T>
	void setObjectSignature(T * object)
	{
		(*object)->SetHiddenValue(v8::String::New(signatureKey), v8::Int32::New(signatureValue));
	}

	bool isValidObject(v8::Handle<v8::Value> object)
	{
		using namespace v8;

		if(object->IsObject() || object->IsFunction()) {
			v8::Handle<v8::Value> value = Handle<Object>::Cast(object)->GetHiddenValue(v8::String::New(signatureKey));

			return !value.IsEmpty() && value->IsInt32() && value->Int32Value() == signatureValue;
		}
		else {
			return false;
		}
	}
	
	bool isGlobalObject(v8::Handle<v8::Value> object)
	{
		using namespace v8;

		if(object->IsObject() || object->IsFunction()) {
			return Handle<Object>::Cast(object)->GetPointerFromInternalField(0) == NULL;
		}
		else {
			return false;
		}
	}

	GScriptDataType getV8Type(v8::Local<v8::Value> value, IMetaTypedItem ** typeItem)
	{
		using namespace v8;

		if(typeItem != NULL) {
			*typeItem = NULL;
		}

		if(value->IsNull()) {
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
				GScriptUserData * userData = static_cast<GScriptUserData *>(obj->GetPointerFromInternalField(0));
				if(userData != NULL) {
					switch(userData->getType()) {
						case udtClass: {
							GClassUserData * classData = static_cast<GClassUserData *>(userData);
							if(typeItem != NULL) {
								*typeItem = classData->metaClass;
								(*typeItem)->addReference();
							}
							if(classData->instance == NULL) {
								return sdtClass;
							}
							else {
								return sdtObject;
							}
						}

						break;

						case udtMethod:
							return sdtMethod;
						break;

						case udtMethodList:
							return sdtMethodList;
						break;

						default:
						break;
					}
				}
			}
		}

		return sdtUnknown;
	}

	v8::Handle<v8::Value> objectToV8(GScriptBindingParam * param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv)
	{
		using namespace v8;

		if(instance == NULL) {
			return Handle<Value>();
		}

		GMetaMapClass * map = param->getMetaMap()->findClassMap(metaClass);
		GMapItemClassData * mapData = gdynamic_cast<GMapItemClassData *>(map->getData());
		Handle<FunctionTemplate> functionTemplate = mapData->functionTemplate;
		Persistent<Object> self = Persistent<Object>::New(functionTemplate->GetFunction()->NewInstance());

		GClassUserData * instanceUserData = new GClassUserData(param, metaClass, instance, true, allowGC, cv);
		param->addUserData(instanceUserData);
		self.MakeWeak(instanceUserData, weakHandleCallback);

		self->SetPointerInInternalField(0, instanceUserData);
		setObjectSignature(&self);

		return self;
	}

	v8::Handle<v8::Value> variantToV8(GScriptBindingParam * param, const GVariant & value, const GMetaType & type, bool allowGC)
	{
		GVariantType vt = value.getType();

		if(vtIsEmpty(vt)) {
			return v8::Handle<v8::Value>();
		}

		if(vtIsBoolean(vt)) {
			return v8::Boolean::New(fromVariant<bool>(value));
		}

		if(vtIsInteger(vt)) {
			return v8::Integer::New(fromVariant<int>(value));
		}

		if(vtIsReal(vt)) {
			return v8::Number::New(fromVariant<double>(value));
		}

		if(canFromVariant<void *>(value) && objectAddressFromVariant(value) == NULL) {
			return v8::Null();
		}

		if(vtIsVoidPointer(vt)) {
			return v8::External::New(fromVariant<void *>(value));
		}

		if(variantIsString(value)) {
			return v8::String::New(fromVariant<char *>(value));
		}

		if(type.isEmpty()) {
			return v8::Undefined();
		}

		if(type.getPointerDimension() <= 1) {
			GScopedInterface<IMetaTypedItem> typedItem(param->getService()->findTypedItemByName(type.getBaseName()));
			if(typedItem) {
				if(type.getPointerDimension() == 0) {
					GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");
					GASSERT_MSG(type.baseIsClass(), "Unknown type");

					IMetaClass * metaClass = static_cast<IMetaClass *>(typedItem.get());
					void * instance = metaClass->cloneInstance(objectAddressFromVariant(value));
					return objectToV8(param, instance, static_cast<IMetaClass *>(typedItem.get()), true, metaTypeToCV(type));
				}

				if(type.getPointerDimension() == 1) {
					GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

					return objectToV8(param, fromVariant<void *>(value), static_cast<IMetaClass *>(typedItem.get()), allowGC, metaTypeToCV(type));
				}
			}
		}

		return v8::Undefined();
	}

	void * v8ToObject(v8::Handle<v8::Value> value, GMetaType * outType)
	{
		using namespace v8;

		if(isValidObject(value)) {
			GScriptUserData * userData = static_cast<GScriptUserData *>(Handle<Object>::Cast(value)->GetPointerFromInternalField(0));
			if(userData != NULL && userData->getType() == udtClass) {
				GClassUserData * classData = static_cast<GClassUserData *>(userData);
				if(outType != NULL) {
					GMetaTypeData typeData;
					classData->metaClass->getMetaType(&typeData);
					metaCheckError(classData->metaClass);
					*outType = GMetaType(typeData);
				}

				return classData->instance;
			}
		}

		return NULL;
	}

	GMetaVariant v8ToVariant(v8::Handle<v8::Value> value)
	{
		using namespace v8;

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
			v8::String::AsciiValue s(value);
			return GMetaVariant(createStringVariant(*s), createMetaType<char *>());
		}

		if(value->IsUint32()) {
			return value->Uint32Value();
		}

		if(value->IsFunction() || value->IsObject()) {
			Local<Object> obj = value->ToObject();
			if(isValidObject(obj)) {
				GScriptUserData * userData = static_cast<GScriptUserData *>(obj->GetPointerFromInternalField(0));
				if(userData != NULL) {
					switch(userData->getType()) {
						case udtClass: {
							GMetaType metaType;
							void * obj = v8ToObject(value, &metaType);
							if(obj != NULL) {
								metaType.addPointer();
								return GMetaVariant(pointerToObjectVariant(obj), metaType);
							}
						}
						break;

						default:
						break;
					}
				}
			}
		}

		return GMetaVariant();
	}

	void weakHandleCallback(v8::Persistent<v8::Value> object, void * parameter)
	{
		GScriptUserData * userData = static_cast<GScriptUserData *>(parameter);

		if(userData != NULL) {
			userData->getParam()->removeUserData(userData);
		}

		object.Dispose();
		object.Clear();
	}

	void loadMethodParameters(const v8::Arguments & args, GScriptBindingParam * param, GVariantData * outputParams)
	{
		(void)param;

		for(int i = 0; i < args.Length(); ++i) {
			outputParams[i] = v8ToVariant(args[i]).takeData().varData;
		}
	}

	void loadMethodParamTypes(const v8::Arguments & args, GBindDataType * outputTypes)
	{
		for(int i = 0; i < args.Length(); ++i) {
			IMetaTypedItem * typeItem;
			outputTypes[i].dataType = getV8Type(args[i], &typeItem);
			outputTypes[i].typeItem.reset(typeItem);
		}
	}

	void loadCallableParam(const v8::Arguments & args, GScriptBindingParam * param, InvokeCallableParam * callableParam)
	{
		if(args.Length() > REF_MAX_ARITY) {
			raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
		}

		callableParam->paramCount = args.Length();
		loadMethodParameters(args, param, callableParam->paramsData);
		loadMethodParamTypes(args, callableParam->paramsType);
	}

	void doBindFundamental(GScriptBindingParam * param, v8::Local<v8::Object> container, const char * name, const GVariant & value)
	{
		container->Set(v8::String::New(name), variantToV8(param, value, GMetaType(), false));
	}

	v8::Handle<v8::Value> accessibleGet(v8::Local<v8::String> prop, const v8::AccessorInfo & info)
	{
		using namespace v8;

		(void)prop;

		ENTER_V8()

		GAccessibleUserData * userData = static_cast<GAccessibleUserData *>(Local<External>::Cast(info.Data())->Value());

		GVariant result = metaGetValue(userData->accessible, userData->instance);

		return variantToV8(userData->getParam(), result, metaGetItemType(userData->accessible), false);
		
		LEAVE_V8(return Handle<Value>())
	}

	void accessibleSet(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo & info)
	{
		using namespace v8;

		(void)prop;

		ENTER_V8()

		GAccessibleUserData * userData = static_cast<GAccessibleUserData *>(Local<External>::Cast(info.Data())->Value());

		GMetaVariant v = v8ToVariant(value);
		metaSetValue(userData->accessible, userData->instance, v.getValue());
		
		LEAVE_V8()
	}

	void doBindAccessible(GScriptBindingParam * param, v8::Local<v8::Object> container,
		const char * name, void * instance, IMetaAccessible * accessible)
	{
		using namespace v8;

		GAccessibleUserData * userData = new GAccessibleUserData(param, instance, accessible);
		param->addUserData(userData);
		Persistent<External> data = Persistent<External>::New(External::New(userData));
		data.MakeWeak(userData, weakHandleCallback);

		container->SetAccessor(String::New(name), &accessibleGet, &accessibleSet, data);
	}

	v8::Handle<v8::Value> converterToV8(GScriptBindingParam * param, const GVariant & value, IMetaConverter * converter)
	{
		using namespace v8;

		if(converter != NULL) {
			if(converter->canToCString()) {
				gapi_bool needFree;

				GScopedInterface<IMemoryAllocator> allocator(param->getService()->getAllocator());
				const char * s = converter->toCString(objectAddressFromVariant(value), &needFree, allocator.get());

				if(s != NULL) {
					Handle<Value> value = String::New(s);

					if(needFree) {
						allocator->free((void *)s);
					}

					return value;
				}
			}
		}

		return Handle<Value>();
	}

	v8::Handle<v8::Value> methodResultToV8(GScriptBindingParam * param, IMetaCallable * callable, InvokeCallableResult * result)
	{
		if(result->resultCount > 0) {
			GMetaTypeData typeData;

			callable->getResultType(&typeData);
			metaCheckError(callable);

			GVariant value = GVariant(result->resultData);
			v8::Handle<v8::Value> v;
			v = variantToV8(param, value, GMetaType(typeData), !! callable->isResultTransferOwnership());
			if(v.IsEmpty()) {
				GScopedInterface<IMetaConverter> converter(callable->createResultConverter());
				v = converterToV8(param, value, converter.get());
			}
			if(v.IsEmpty()) {
				raiseCoreException(Error_ScriptBinding_FailVariantToScript);
			}
			return v;

		}

		return v8::Handle<v8::Value>();
	}

	void * addPointer(void * a, int delta)
	{
		return static_cast<char *>(a) + delta;
	}

	int subPointer(void * a, void * b)
	{
		return static_cast<int>(static_cast<char *>(a) - static_cast<char *>(b));
	}

	v8::Handle<v8::Value> callbackMethodList(const v8::Arguments & args)
	{
		using namespace v8;

		ENTER_V8()

		if(!isGlobalObject(args.Holder()) && !isValidObject(args.Holder())) {
			raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
		}

		GClassUserData * userData = static_cast<GClassUserData *>(args.Holder()->GetPointerFromInternalField(0));

		Local<External> data = Local<External>::Cast(args.Data());
		GMethodListUserData * namedUserData = static_cast<GMethodListUserData *>(data->Value());

		IMetaList * methodList = namedUserData->methodList;

		InvokeCallableParam callableParam;
		loadCallableParam(args, namedUserData->getParam(), &callableParam);

		int maxRankIndex = findAppropriateCallable(namedUserData->getParam()->getService(),
			makeCallback(methodList, &IMetaList::getAt), methodList->getCount(),
			&callableParam, FindCallablePredict());

		if(maxRankIndex >= 0) {
			InvokeCallableResult result;
			GScopedInterface<IMetaCallable> callable(gdynamic_cast<IMetaCallable *>(methodList->getAt(maxRankIndex)));
			void * instance = NULL;
			if(userData != NULL) {
				instance = addPointer(userData->instance,
					subPointer(methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex)), methodList->getInstanceAt(0)));
			}
			doInvokeCallable(instance, callable.get(), callableParam.paramsData, callableParam.paramCount, &result);
			return methodResultToV8(namedUserData->getParam(), callable.get(), &result);
		}

		raiseCoreException(Error_ScriptBinding_CantFindMatchedMethod);
		return v8::Handle<v8::Value>();

		LEAVE_V8(return Handle<Value>())
	}

	v8::Handle<v8::FunctionTemplate> createMethodTemplate(GScriptBindingParam * param, IMetaList * methodList,
		const char * name, v8::Handle<v8::FunctionTemplate> classTemplate, GMethodListUserData ** outUserData)
	{
		using namespace v8;

		GMethodListUserData * userData = new GMethodListUserData(param, methodList);
		if(outUserData != NULL) {
			*outUserData = userData;
		}

		Persistent<External> data = Persistent<External>::New(External::New(userData));
		data.MakeWeak(NULL, weakHandleCallback);

		Handle<FunctionTemplate> functionTemplate = FunctionTemplate::New(callbackMethodList, data, Signature::New(classTemplate));
		functionTemplate->SetClassName(String::New(name));

		return functionTemplate;
	}

	v8::Handle<v8::Value> namedEnumGetter(v8::Local<v8::String> prop, const v8::AccessorInfo & info)
	{
		using namespace v8;

		ENTER_V8()

		GEnumUserData * userData = static_cast<GEnumUserData *>(info.Holder()->GetPointerFromInternalField(0));
		IMetaEnum * metaEnum = userData->metaEnum;
		v8::String::AsciiValue name(prop);
		int32_t index = metaEnum->findKey(*name);
		if(index >= 0) {
			return variantToV8(userData->getParam(), metaGetEnumValue(metaEnum, index), GMetaType(), true);
		}

		raiseCoreException(Error_ScriptBinding_CantFindEnumKey, *name);
		return v8::Handle<v8::Value>();

		LEAVE_V8(return Handle<Value>())
	}

	v8::Handle<v8::Value> namedEnumSetter(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo & info)
	{
		(void)prop;
		(void)value;
		(void)info;

		ENTER_V8()

		raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);
		
		return v8::Handle<v8::Value>();

		LEAVE_V8(return v8::Handle<v8::Value>())
	}

	v8::Handle<v8::ObjectTemplate> createEnumTemplate(GScriptBindingParam * param, IMetaEnum * metaEnum,
		const char * name, GEnumUserData ** outUserData)
	{
		using namespace v8;
		
		(void)name;

		GEnumUserData * userData = new GEnumUserData(param, metaEnum);
		if(outUserData != NULL) {
			*outUserData = userData;
		}

		Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();
		objectTemplate->SetInternalFieldCount(1);
		objectTemplate->SetNamedPropertyHandler(&namedEnumGetter, &namedEnumSetter);

		return objectTemplate;
	}

	v8::Handle<v8::Value> getNamedMember(GClassUserData * userData, const char * name)
	{
		using namespace v8;

		GMetaClassTraveller traveller(userData->metaClass, userData->instance);

		void * instance = NULL;

		for(;;) {
			GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
			if(!metaClass) {
				break;
			}

			GMetaMapClass * mapClass = userData->getParam()->getMetaMap()->findClassMap(metaClass.get());
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
					GScopedInterface<IMetaAccessible> data(static_cast<IMetaAccessible *>(mapItem->getItem()));
					if(allowAccessData(userData, data.get())) {
						GVariant result = metaGetValue(data.get(), userData->instance);
						return variantToV8(userData->getParam(), result, metaGetItemType(data.get()), false);
					}
				}
				   break;

				case mmitMethod:
				case mmitMethodList: {
					GScopedInterface<IMetaList> methodList(createMetaList());
					loadMethodList(&traveller, methodList.get(), userData->getParam()->getMetaMap(), mapItem, instance, userData, name);

					GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getData());
					if(data == NULL) {
						data = new GMapItemMethodData;
						mapItem->setData(data);
						GMethodListUserData * newUserData;
						data->setTemplate(createMethodTemplate(userData->getParam(), methodList.get(), name,
							static_cast<GMapItemClassData *>(mapClass->getData())->functionTemplate, &newUserData));
						data->setUserData(newUserData);
					}
					Local<Function> func = data->functionTemplate->GetFunction();
					func->SetPointerInInternalField(0, data->getUserData());
					setObjectSignature(&func);

					return func;
				}

				case mmitEnum:
					if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumTypeViaInstance()) {
						GMapItemEnumData * data = gdynamic_cast<GMapItemEnumData *>(mapItem->getData());
						if(data == NULL) {
							data = new GMapItemEnumData;
							mapItem->setData(data);
							GEnumUserData * newUserData;
							GScopedInterface<IMetaEnum> metaEnum(static_cast<IMetaEnum *>(mapItem->getItem()));
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
						return variantToV8(userData->getParam(), metaGetEnumValue(metaEnum, static_cast<uint32_t>(mapItem->getEnumIndex())), GMetaType(), true);
					}
					break;

				case mmitClass:
					if(! userData->isInstance || userData->getParam()->getConfig().allowAccessClassViaInstance()) {
						GScopedInterface<IMetaClass> innerMetaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
						Handle<FunctionTemplate> functionTemplate = createClassTemplate(userData->getParam(), name, innerMetaClass.get());
						return functionTemplate->GetFunction();
					}
					break;

				default:
					break;
			}
		}

		return Handle<Value>();
	}

	v8::Handle<v8::Value> setNamedMember(GClassUserData * userData, const char * name, v8::Local<v8::Value> value)
	{
		using namespace v8;

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
					GScopedInterface<IMetaAccessible> data(static_cast<IMetaAccessible *>(mapItem->getItem()));
					if(allowAccessData(userData, data.get())) {
						GVariant v = v8ToVariant(value).getValue();
						metaSetValue(data.get(), userData->instance, v);
						return value;
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

		return Handle<Value>();
	}

	v8::Handle<v8::Value> staticMemberGetter(v8::Local<v8::String> prop, const v8::AccessorInfo & info)
	{
		using namespace v8;

		ENTER_V8()

		GClassUserData * userData = static_cast<GClassUserData *>(Local<External>::Cast(info.Data())->Value());

		String::Utf8Value utf8_prop(prop);
		const char * name = *utf8_prop;

		return getNamedMember(userData, name);

		LEAVE_V8(return Handle<Value>())
	}

	void staticMemberSetter(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo & info)
	{
		using namespace v8;

		ENTER_V8()

		GClassUserData * userData = static_cast<GClassUserData *>(Local<External>::Cast(info.Data())->Value());

		String::Utf8Value utf8_prop(prop);
		const char * name = *utf8_prop;

		setNamedMember(userData, name, value);

		LEAVE_V8()
	}

	v8::Handle<v8::Value> namedMemberGetter(v8::Local<v8::String> prop, const v8::AccessorInfo & info)
	{
		using namespace v8;

		ENTER_V8()

		if(!isValidObject(info.Holder())) {
			raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
		}

		String::Utf8Value utf8_prop(prop);
		const char * name = *utf8_prop;

		GClassUserData * userData = static_cast<GClassUserData *>(info.Holder()->GetPointerFromInternalField(0));

		return getNamedMember(userData, name);

		LEAVE_V8(return Handle<Value>())
	}

	v8::Handle<v8::Value> namedMemberSetter(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo & info)
	{
		using namespace v8;

		ENTER_V8()

		String::Utf8Value utf8_prop(prop);
		const char * name = *utf8_prop;

		if(!isValidObject(info.Holder())) {
			raiseCoreException(Error_ScriptBinding_AccessMemberWithWrongObject);
		}

		GClassUserData * userData = static_cast<GClassUserData *>(info.Holder()->GetPointerFromInternalField(0));
		if(userData == NULL) {
			return Handle<Value>();
		}

		return setNamedMember(userData, name, value);

		LEAVE_V8(return Handle<Value>())
	}

	void accessorNamedMemberSetter(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo & info)
	{
		namedEnumSetter(prop, value, info);
	}

	void bindClassItems(v8::Local<v8::Object> object, GScriptBindingParam * param, IMetaClass * metaClass, bool allowStatic, bool allowMember)
	{
		using namespace v8;

		GClassUserData * userData = new GClassUserData(param, metaClass, NULL, false, false, opcvNone);
		param->addUserData(userData);
		Persistent<External> data = Persistent<External>::New(External::New(userData));
		data.MakeWeak(userData, weakHandleCallback);

		GScopedInterface<IMetaItem> item;
		uint32_t count = metaClass->getMetaCount();
		for(uint32_t i = 0; i < count; ++i) {
			item.reset(metaClass->getMetaAt(i));
			if(allowStatic && item->isStatic()) {
				object->SetAccessor(String::New(item->getName()), &staticMemberGetter, &staticMemberSetter, data);
			}
			else {
				if(allowMember && !item->isStatic()) {
					object->SetAccessor(String::New(item->getName()), &namedMemberGetter, &accessorNamedMemberSetter, data);
				}
			}
		}
	}

	void * invokeConstructor(const v8::Arguments & args, GScriptBindingParam * param, IMetaClass * metaClass)
	{
		int paramCount = args.Length();
		void * instance = NULL;

		if(paramCount == 0 && metaClass->canCreateInstance()) {
			instance = metaClass->createInstance();
		}
		else {
			InvokeCallableParam callableParam;
			loadCallableParam(args, param, &callableParam);

			int maxRankIndex = findAppropriateCallable(param->getService(),
				makeCallback(metaClass, &IMetaClass::getConstructorAt), metaClass->getConstructorCount(),
				&callableParam, FindCallablePredict());

			if(maxRankIndex >= 0) {
				InvokeCallableResult result;

				GScopedInterface<IMetaConstructor> constructor(metaClass->getConstructorAt(static_cast<uint32_t>(maxRankIndex)));
				doInvokeCallable(NULL, constructor.get(), callableParam.paramsData, paramCount, &result);
				instance = fromVariant<void *>(GVariant(result.resultData));
			}
		}

		if(instance != NULL) {
			return instance;
		}
		else {
			raiseCoreException(Error_ScriptBinding_FailConstructObject);
		}

		return NULL;
	}

	v8::Handle<v8::Value> objectConstructor(const v8::Arguments & args)
	{
		using namespace v8;

		ENTER_V8()

		if(! args.IsConstructCall()) {
			return ThrowException(String::New("Cannot call constructor as function"));
		}

		Local<External> data = Local<External>::Cast(args.Data());
		GClassUserData * userData = static_cast<GClassUserData *>(data->Value());

		void * instance = invokeConstructor(args, userData->getParam(), userData->metaClass);
		Persistent<Object> self = Persistent<Object>::New(args.Holder());

		GClassUserData * instanceUserData = new GClassUserData(userData->getParam(), userData->metaClass, instance, true, true, opcvNone);
		userData->getParam()->addUserData(instanceUserData);
		self.MakeWeak(instanceUserData, weakHandleCallback);

		self->SetPointerInInternalField(0, instanceUserData);
		setObjectSignature(&self);

		return self;

		LEAVE_V8(return Handle<Value>());
	}

	v8::Handle<v8::FunctionTemplate> createClassTemplate(GScriptBindingParam * param, const char * name, IMetaClass * metaClass)
	{
		using namespace v8;

		GMetaMapClass * map = param->getMetaMap()->findClassMap(metaClass);

		if(map->getData() != NULL) {
			return static_cast<GMapItemClassData *>(map->getData())->functionTemplate;
		}

		GClassUserData * userData = new GClassUserData(param, metaClass, NULL, false, false, opcvNone);
		param->addUserData(userData);
		Persistent<External> data = Persistent<External>::New(External::New(userData));
		data.MakeWeak(userData, weakHandleCallback);

		Handle<FunctionTemplate> functionTemplate = FunctionTemplate::New(objectConstructor, data);
		functionTemplate->SetClassName(String::New(name));

		Local<ObjectTemplate> instanceTemplate = functionTemplate->InstanceTemplate();
		instanceTemplate->SetInternalFieldCount(1);

		instanceTemplate->SetNamedPropertyHandler(&namedMemberGetter, &namedMemberSetter);

		GMapItemClassData * mapData = new GMapItemClassData;
		mapData->setTemplate(functionTemplate);
		map->setData(mapData);

		return functionTemplate;
	}

	void doBindClass(GScriptBindingParam * param, v8::Local<v8::Object> container, const char * name, IMetaClass * metaClass)
	{
		using namespace v8;

		Handle<FunctionTemplate> functionTemplate = createClassTemplate(param, name, metaClass);
		container->Set(v8::String::New(name), functionTemplate->GetFunction());
		bindClassItems(functionTemplate->GetFunction(), param, metaClass, true, false);
	}

} // unnamed namespace



class GV8ScriptObjectImplement
{
public:
	GV8ScriptObjectImplement(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config, GMetaMap * metaMap, bool freeMap)
		: param(service, config, metaMap), object(v8::Persistent<v8::Object>::New(object)), freeMap(freeMap) {
	}

	~GV8ScriptObjectImplement() {
		if(this->freeMap) {
			delete this->param.getMetaMap();
		}

		this->object.Dispose();
	}

public:
	GScriptBindingParam param;
	v8::Persistent<v8::Object> object;
	bool freeMap;
};



GV8ScriptObject::GV8ScriptObject(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config)
	: super(config)
{
	this->implement.reset(new GV8ScriptObjectImplement(service, object, config, new GMetaMap, true));
}

GV8ScriptObject::GV8ScriptObject(const GV8ScriptObject & other, v8::Local<v8::Object> object)
	: super(other.implement->param.getConfig())
{
	this->implement.reset(new GV8ScriptObjectImplement(other.implement->param.getService(), object, super::getConfig(), other.implement->param.getMetaMap(), false));
}

GV8ScriptObject::~GV8ScriptObject()
{
}

GScriptDataType GV8ScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> obj = localObject->Get(String::New(name));
	return getV8Type(obj, outMetaTypeItem);

	LEAVE_V8(return sdtUnknown)
}

void GV8ScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	doBindClass(&this->implement->param, localObject, name, metaClass);

	LEAVE_V8()
}

void GV8ScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	GEnumUserData * newUserData;
	Handle<ObjectTemplate> objectTemplate = createEnumTemplate(&this->implement->param, metaEnum, name, &newUserData);
	objectTemplate->SetInternalFieldCount(1);
	Persistent<Object> obj = Persistent<Object>::New(objectTemplate->NewInstance());
	obj->SetPointerInInternalField(0, newUserData);
	setObjectSignature(&obj);
	this->implement->param.addUserData(newUserData);
	obj.MakeWeak(newUserData, weakHandleCallback);

	localObject->Set(String::New(name), obj);

	LEAVE_V8()
}

GScriptObject * GV8ScriptObject::createScriptObject(const char * name)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();
	Local<Object> obj = objectTemplate->NewInstance();
	localObject->Set(String::New(name), obj);

	GV8ScriptObject * binding = new GV8ScriptObject(*this, obj);
	binding->owner = this;
	binding->name = name;

	return binding;

	LEAVE_V8(return NULL)
}

GScriptObject * GV8ScriptObject::getScriptObject(const char * name)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));
	if((value->IsObject() || value->IsFunction()) && !isValidObject(value)) {
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
	using namespace v8;

	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> func = localObject->Get(String::New(name));

	if(func->IsFunction() || (func->IsObject() && Local<Object>::Cast(func)->IsCallable())) {
		Handle<Value> v8Params[REF_MAX_ARITY];
		for(size_t i = 0; i < paramCount; ++i) {
			v8Params[i] = variantToV8(&this->implement->param, params[i]->getValue(), params[i]->getType(), false);
			if(v8Params[i].IsEmpty()) {
				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
		}

		Local<Value> result;
		Handle<Object> receiver = localObject;

		if(func->IsFunction()) {
			result = Local<Function>::Cast(func)->Call(receiver, static_cast<int>(paramCount), v8Params);
		}
		else {
			result = Local<Object>::Cast(func)->CallAsFunction(receiver, static_cast<int>(paramCount), v8Params);
		}

		return v8ToVariant(result);
	}
	else {
		raiseCoreException(Error_ScriptBinding_CantCallNonfunction);
	}

	return GMetaVariant();

	LEAVE_V8(return GMetaVariant())
}

void GV8ScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	doBindFundamental(&this->implement->param, localObject, name, value);

	LEAVE_V8()
}

void GV8ScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	doBindAccessible(&this->implement->param, localObject, name, instance, accessible);

	LEAVE_V8()
}

void GV8ScriptObject::bindString(const char * stringName, const char * s)
{
	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	localObject->Set(v8::String::New(stringName), v8::String::New(s));

	LEAVE_V8()
}

void GV8ScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	v8::Handle<v8::Value> obj = objectToV8(&this->implement->param, instance, type, transferOwnership, opcvNone);
	localObject->Set(v8::String::New(objectName), obj);

	LEAVE_V8()
}

void GV8ScriptObject::bindMethod(const char * name, void * instance, IMetaMethod * method)
{
	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method, instance);

	this->bindMethodList(name, methodList.get());
}

void GV8ScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	GMethodListUserData * newUserData;
	Handle<FunctionTemplate> functionTemplate = createMethodTemplate(&this->implement->param, methodList, name,
		Handle<FunctionTemplate>(), &newUserData);

	Persistent<Function> func = Persistent<Function>::New(functionTemplate->GetFunction());
	func->SetPointerInInternalField(0, newUserData);
	setObjectSignature(&func);
	this->implement->param.addUserData(newUserData);
	func.MakeWeak(newUserData, weakHandleCallback);

	localObject->Set(v8::String::New(name), func);

	LEAVE_V8()
}

IMetaClass * GV8ScriptObject::getClass(const char * className)
{
	IMetaTypedItem * typedItem = NULL;

	GScriptDataType sdt = this->getType(className, &typedItem);
	GScopedInterface<IMetaTypedItem> item(typedItem);
	if(sdt == sdtClass) {
		return gdynamic_cast<IMetaClass *>(item.take());
	}

	return NULL;
}

IMetaEnum * GV8ScriptObject::getEnum(const char * enumName)
{
	IMetaTypedItem * typedItem = NULL;

	GScriptDataType sdt = this->getType(enumName, &typedItem);
	GScopedInterface<IMetaTypedItem> item(typedItem);
	if(sdt == sdtEnum) {
		return gdynamic_cast<IMetaEnum *>(item.take());
	}

	return NULL;
}

GVariant GV8ScriptObject::getFundamental(const char * name)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));
	return v8ToVariant(value).getValue();

	LEAVE_V8(return GVariant())
}

std::string GV8ScriptObject::getString(const char * stringName)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(stringName));
	if(value->IsString()) {
		v8::String::AsciiValue s(value);
		return *s;
	}
	else {
		return "";
	}

	LEAVE_V8(return "")
}

void * GV8ScriptObject::getObject(const char * objectName)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(objectName));
	return v8ToObject(value, NULL);

	LEAVE_V8(return NULL)
}

IMetaMethod * GV8ScriptObject::getMethod(const char * methodName, void ** outInstance)
{
	(void)methodName;

	if(outInstance != NULL) {
		*outInstance = NULL;
	}

	return NULL;
}

IMetaList * GV8ScriptObject::getMethodList(const char * methodName)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(methodName));
	if(isValidObject(value)) {
		GScriptUserData * userData = static_cast<GScriptUserData *>(Handle<Object>::Cast(value)->GetPointerFromInternalField(0));
		if(userData != NULL && userData->getType() == udtMethodList) {
			GMethodListUserData * methodListData = static_cast<GMethodListUserData *>(userData);
			methodListData->methodList->addReference();

			return methodListData->methodList;
		}
	}
	return NULL;

	LEAVE_V8(return NULL)
}

void GV8ScriptObject::assignValue(const char * fromName, const char * toName)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(fromName));
	localObject->Set(String::New(toName), value);

	LEAVE_V8()
}

bool GV8ScriptObject::valueIsNull(const char * name)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> value = localObject->Get(String::New(name));
	return value.IsEmpty() || value->IsUndefined() || value->IsNull();

	LEAVE_V8(return false)
}

void GV8ScriptObject::nullifyValue(const char * name)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	localObject->Delete(String::New(name));

	LEAVE_V8()
}


IScriptObject * createV8ScriptObject(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config)
{
	return new ImplScriptObject(new GV8ScriptObject(service, object, config));
}



} // namespace cpgf


#if 0

bool executeString(const char * source, bool print_result = true)
{
	using namespace v8;

	v8::HandleScope handle_scope;
	v8::TryCatch try_catch;
	v8::Handle<v8::Script> script = v8::Script::Compile(String::New(source), String::New("sample"));
	if(script.IsEmpty()) {
		v8::String::AsciiValue error(try_catch.Exception());
		printf("%s\n", *error);
		return false;
	}
	else {
		v8::Handle<v8::Value> result = script->Run();
		if(result.IsEmpty()) {
			v8::String::AsciiValue error(try_catch.Exception());
			printf("%s\n", *error);
			return false;
		}
		else {
			if (print_result && !result->IsUndefined()) {
				v8::String::AsciiValue str(result);
				printf("%s\n", *str);
			}
			return true;
		}
	}
}

void testBindV8()
{
	using namespace v8;
	using namespace cpgf;
	using namespace testscript;

	cpgf::GScopedInterface<cpgf::IMetaService> service(cpgf::createDefaultMetaService());

	HandleScope handle_scope;

	Persistent<Context> context = Context::New();
	Context::Scope contextScope(context);

	GScopedPointer<GV8ScriptObject> script(new GV8ScriptObject(service.get(), context->Global(), GScriptConfig()));

	script->bindFundamental("x", 1999); executeString("x");
	script->bindFundamental("x", true); executeString("x");
	script->bindFundamental("x", "This is a string"); executeString("x");

	GScopedInterface<IMetaClass> metaClass(service->findClassByName("testscript::TestObject"));
	GScopedInterface<IMetaField> metaField(metaClass->getField("value"));

	TestObject obj;

	obj.value = 3838;
	script->bindAccessible("v", &obj, metaField.get());
	executeString("v");
	executeString("v = 5");
	cout << obj.value << endl;

	script->bindClass("TestObject", metaClass.get());
	metaClass.reset(service->findClassByName("testscript::BasicA"));
	script->bindClass("BasicA", metaClass.get());

	GScopedInterface<IMetaModule> module(service->getModuleAt(0));
	GScopedInterface<IMetaClass> metaGlobal(module->getGlobalMetaClass());
	GScopedInterface<IMetaEnum> metaEnum(metaGlobal->getEnum("testscript::TestEnum"));
	script->bindEnum("TestEnum", metaEnum.get());

	GScopedPointer<GScriptObject> myscope(script->createScriptObject("myscope"));
	myscope->bindEnum("ScopeTestEnum", metaEnum.get());
	myscope->bindString("BindString", "This is a bound string."); executeString("myscope.BindString");

	executeString("a = new TestObject()");
	executeString("b = new TestObject()");
	executeString("a.value = 67");
	executeString("b.value = 18");
	executeString("a.value");
	executeString("b.value");
	executeString("a.add(8)");
	executeString("b = {}");
	executeString("b.__proto__ = a");
	executeString("b.add(9)");
	executeString("a instanceof TestObject");
	executeString("b = a.self()");
	executeString("b.value");
	executeString("b.add(8)");
	executeString("b instanceof TestObject");

	executeString("a = new BasicA()");
	executeString("a.BasicEnum.a");
	executeString("a.b");
	executeString("b = new a.Inner()");
	executeString("b.x");
	executeString("b.add()");
	executeString("b.x");

	executeString("TestEnum.teCpp");

	executeString("myscope.ScopeTestEnum.teLua");

	executeString("value = 58");
	cout << fromVariant<int>(script->getFundamental("value")) << endl;

	executeString("function myFunc(n) { return n * 2; } ");
	GMetaVariant params[10];
	params[0] = 5;
	cout << fromVariant<int>(script->invoke("myFunc", params, 1).getValue()) << endl;

	context.Dispose();
	context.Clear();
}

#endif


#if defined(_MSC_VER)
#pragma warning(pop)
#endif


