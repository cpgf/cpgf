#include "gv8bind.h"
#include "cpgf/gmetaclasstraveller.h"

#include "gbindcommon.h"
#include "gscriptbindapiimpl.h"

// for test
#include "testscriptbindmetadata.h"
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
			this->userData = userData;
		}

	public:
		v8::Persistent<v8::FunctionTemplate> functionTemplate;
		GMethodListUserData * userData;
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
			this->userData = userData;
		}

	public:
		v8::Persistent<v8::ObjectTemplate> objectTemplate;
		GEnumUserData * userData;
	};

	
	void weakHandleCallback(v8::Persistent<v8::Value> object, void * parameter);
	v8::Handle<v8::FunctionTemplate> createClassTemplate(GScriptBindingParam * param, const GScriptName & name, IMetaClass * metaClass);

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
		self.MakeWeak(instanceUserData, weakHandleCallback);

		self->SetPointerInInternalField(0, instanceUserData);

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

		if(value.getPointers() == 1 && value.getBaseType() == vtChar) {
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

	GMetaVariant v8ToVariant(GScriptBindingParam * param, v8::Handle<v8::Value> value)
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
			return *(value->ToString());
		}

		if(value->IsUint32()) {
			return value->Uint32Value();
		}

		if(value->IsFunction() || value->IsObject()) {
			Local<Object> obj = value->ToObject();
			GScriptUserData * userData = static_cast<GScriptUserData *>(obj->GetPointerFromInternalField(0));
			if(userData != NULL) {
				switch(userData->getType()) {
					case udtClass: {
						GClassUserData * classData = static_cast<GClassUserData *>(userData);
						if(classData->instance != NULL) {
							GMetaTypeData typeData;
							classData->metaClass->getMetaType(&typeData);
							metaCheckError(classData->metaClass);
							return GMetaVariant(pointerToObjectVariant(classData->instance), GMetaType(typeData));
						}
					}
					break;
				}
			}
		}

		return GMetaVariant();
	}

	void weakHandleCallback(v8::Persistent<v8::Value> object, void * parameter)
	{
		GScriptUserData * userData = static_cast<GScriptUserData *>(parameter);

		delete userData;

		object.Dispose();
		object.Clear();
	}

	void loadMethodParameters(const v8::Arguments & args, GScriptBindingParam * param, GVariantData * outputParams)
	{
		for(int i = 0; i < args.Length(); ++i) {
			outputParams[i] = v8ToVariant(param, args[i]).getData().varData;
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

	void doBindFundamental(GScriptBindingParam * param, v8::Local<v8::Object> container, const GScriptName & name, const GVariant & value)
	{
		container->Set(v8::String::New(name.getName()), variantToV8(param, value, GMetaType(), false));
	}

	v8::Handle<v8::Value> accessibleGet(v8::Local<v8::String> prop, const v8::AccessorInfo & info)
	{
		using namespace v8;

		GAccessibleUserData * userData = static_cast<GAccessibleUserData *>(Local<External>::Cast(info.Data())->Value());

		GVariant result = metaGetValue(userData->accessible, userData->instance);
		
		return variantToV8(userData->getParam(), result, metaGetItemType(userData->accessible), false);
	}

	void accessibleSet(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo & info)
	{
		using namespace v8;

		GAccessibleUserData * userData = static_cast<GAccessibleUserData *>(Local<External>::Cast(info.Data())->Value());

		GMetaVariant v = v8ToVariant(userData->getParam(), value);
		metaSetValue(userData->accessible, userData->instance, v.getValue());
	}

	void doBindAccessible(GScriptBindingParam * param, v8::Local<v8::Object> container,
		const GScriptName & name, void * instance, IMetaAccessible * accessible)
	{
		using namespace v8;

		GAccessibleUserData * userData = new GAccessibleUserData(param, instance, accessible);
		Persistent<External> data = Persistent<External>::New(External::New(userData));
		data.MakeWeak(userData, weakHandleCallback);

		container->SetAccessor(String::New(name.getName()), accessibleGet, accessibleSet, data);
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
		return static_cast<char *>(a) - static_cast<char *>(b);
	}

	v8::Handle<v8::Value> callbackMethodList(const v8::Arguments & args)
	{
		using namespace v8;

		ENTER_V8()

		GClassUserData * userData = static_cast<GClassUserData *>(args.This()->GetPointerFromInternalField(0));

		Local<External> data = Local<External>::Cast(args.Data());
		GMethodListUserData * namedUserData = static_cast<GMethodListUserData *>(data->Value());

		IMetaList * methodList = namedUserData->methodList;
		
		InvokeCallableParam callableParam;
		loadCallableParam(args, userData->getParam(), &callableParam);

		int maxRankIndex = findAppropriateCallable(userData->getParam()->getService(),
			makeCallback(methodList, &IMetaList::getAt), methodList->getCount(),
			&callableParam, FindCallablePredict());

		if(maxRankIndex >= 0) {
			InvokeCallableResult result;
			GScopedInterface<IMetaCallable> callable(gdynamic_cast<IMetaCallable *>(methodList->getAt(maxRankIndex)));
			void * instance = addPointer(userData->instance,
				subPointer(methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex)), methodList->getInstanceAt(0)));
			doInvokeCallable(instance, callable.get(), callableParam.paramsData, callableParam.paramCount, &result);
			return methodResultToV8(userData->getParam(), callable.get(), &result);
		}

		raiseCoreException(Error_ScriptBinding_CantFindMatchedMethod);
		return v8::Handle<v8::Value>();

		LEAVE_V8(return Handle<Value>())
	}

	v8::Handle<v8::FunctionTemplate> createMethodTemplate(GScriptBindingParam * param, IMetaList * methodList,
		const char * name, GMethodListUserData ** outUserData)
	{
		using namespace v8;

		GMethodListUserData * userData = new GMethodListUserData(param, methodList);
		*outUserData = userData;
		Persistent<External> data = Persistent<External>::New(External::New(userData));
		data.MakeWeak(userData, weakHandleCallback);

		Handle<FunctionTemplate> functionTemplate = FunctionTemplate::New(callbackMethodList, data);
		functionTemplate->SetClassName(String::New(name));

		return functionTemplate;
	}

	v8::Handle<v8::Value> namedEnumGetter(v8::Local<v8::String> prop, const v8::AccessorInfo & info)
	{
		using namespace v8;

		ENTER_V8()

		GEnumUserData * userData = static_cast<GEnumUserData *>(info.This()->GetPointerFromInternalField(0));
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

	v8::Handle<v8::ObjectTemplate> createEnumTemplate(GScriptBindingParam * param, IMetaEnum * metaEnum,
		const char * name, GEnumUserData ** outUserData)
	{
		using namespace v8;

		GEnumUserData * userData = new GEnumUserData(param, metaEnum);
		*outUserData = userData;
//		Persistent<External> data = Persistent<External>::New(External::New(userData));
//		data.MakeWeak(userData, weakHandleCallback);

		Handle<ObjectTemplate> objectTemplate = ObjectTemplate::New();
		objectTemplate->SetNamedPropertyHandler(&namedEnumGetter);

		return objectTemplate;
	}

	v8::Handle<v8::Value> namedMemberGetter(v8::Local<v8::String> prop, const v8::AccessorInfo & info)
	{
		using namespace v8;

		ENTER_V8()

		String::Utf8Value utf8_prop(prop);
		const char * name = *utf8_prop;

		GClassUserData * userData = static_cast<GClassUserData *>(info.This()->GetPointerFromInternalField(0));

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
						data->setTemplate(createMethodTemplate(userData->getParam(), methodList.get(), name, &newUserData));
						data->setUserData(newUserData);
					}
					Local<Function> func = data->functionTemplate->GetFunction();
					func->SetPointerInInternalField(0, data->userData);
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
						obj->SetPointerInInternalField(0, data->userData);
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
						GScopedInterface<IMetaClass> metaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));
						Handle<FunctionTemplate> functionTemplate = createClassTemplate(userData->getParam(), name, metaClass.get());
						return functionTemplate->GetFunction();
					}
					break;

				default:
					break;
			}
		}

		return Handle<Value>();

		LEAVE_V8(return Handle<Value>())
	}

	v8::Handle<v8::Value> namedMemberSetter(v8::Local<v8::String> prop, v8::Local<v8::Value> value, const v8::AccessorInfo & info)
	{
		using namespace v8;

		ENTER_V8()

		String::Utf8Value utf8_prop(prop);
		const char * name = *utf8_prop;

		GClassUserData * userData = static_cast<GClassUserData *>(info.This()->GetPointerFromInternalField(0));
		if(userData == NULL) {
			return Handle<Value>();
		}

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
			
			switch(mapItem->getType()) {
				case mmitField:
				case mmitProperty: {
					GScopedInterface<IMetaAccessible> data(static_cast<IMetaAccessible *>(mapItem->getItem()));
					if(allowAccessData(userData, data.get())) {
						GVariant v = v8ToVariant(userData->getParam(), value).getValue();
						metaSetValue(data.get(), userData->instance, v);
						return value;
					}
				}
				   break;

				case mmitMethod:
				case mmitMethodList:
					break;

#if 0
				case mmitEnum:
					if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumTypeViaInstance()) {
						if(indexMemberEnumType(L, userData, mapItem)) {
							return true;
						}
					}
					break;

				case mmitEnumValue:
					if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumValueViaInstance()) {
						if(indexMemberEnumValue(L, userData, mapItem)) {
							return true;
						}
					}
					break;

				case mmitClass:
					if(! userData->isInstance || userData->getParam()->getConfig().allowAccessClassViaInstance()) {
						if(indexMemberClass(L, userData, mapItem)) {
							return true;
						}
					}
					break;
#endif
				default:
					break;
			}
		}

		return Handle<Value>();

		LEAVE_V8(return Handle<Value>())
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

		if(! args.IsConstructCall()) {
			return ThrowException(String::New("Cannot call constructor as function"));
		}

		Local<External> data = Local<External>::Cast(args.Data());
		GClassUserData * userData = static_cast<GClassUserData *>(data->Value());

		void * instance = invokeConstructor(args, userData->getParam(), userData->metaClass);
		Persistent<Object> self = Persistent<Object>::New(args.Holder());

		GClassUserData * instanceUserData = new GClassUserData(userData->getParam(), userData->metaClass, instance, true, true, opcvNone);
		self.MakeWeak(instanceUserData, weakHandleCallback);

		self->SetPointerInInternalField(0, instanceUserData);

		return self;
	}

	v8::Handle<v8::FunctionTemplate> createClassTemplate(GScriptBindingParam * param, const GScriptName & name, IMetaClass * metaClass)
	{
		using namespace v8;

		GMetaMapClass * map = param->getMetaMap()->findClassMap(metaClass);
		
		if(map->getData() != NULL) {
			return static_cast<GMapItemClassData *>(map->getData())->functionTemplate;
		}

		GClassUserData * userData = new GClassUserData(param, metaClass, NULL, false, false, opcvNone);
		Persistent<External> data = Persistent<External>::New(External::New(userData));
		data.MakeWeak(userData, weakHandleCallback);

		Handle<FunctionTemplate> functionTemplate = FunctionTemplate::New(objectConstructor, data);
		functionTemplate->SetClassName(String::New(name.getName()));

		Local<ObjectTemplate> instanceTemplate = functionTemplate->InstanceTemplate();
		instanceTemplate->SetInternalFieldCount(1);

//		Local<ObjectTemplate> prototypeTemplate = functionTemplate->PrototypeTemplate();
//		prototypeTemplate->SetNamedPropertyHandler(&namedMemberGetter, &namedMemberSetter);

		instanceTemplate->SetNamedPropertyHandler(&namedMemberGetter, &namedMemberSetter);

		GMapItemClassData * mapData = new GMapItemClassData;
		mapData->setTemplate(functionTemplate);
		map->setData(mapData);

		return functionTemplate;
	}

	void doBindClass(GScriptBindingParam * param, v8::Local<v8::Object> container, const GScriptName & name, IMetaClass * metaClass)
	{
		using namespace v8;
		
		Handle<FunctionTemplate> functionTemplate = createClassTemplate(param, name, metaClass);
		container->Set(v8::String::New(name.getName()), functionTemplate->GetFunction());
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

GV8ScriptObject::~GV8ScriptObject()
{
}

bool GV8ScriptObject::cacheName(GScriptName * name)
{
	return false;
}

GScriptDataType GV8ScriptObject::getType(const GScriptName & name, IMetaTypedItem ** outMetaTypeItem)
{
	using namespace v8;

	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	Local<Value> obj = localObject->Get(String::New(name.getName()));
	return getV8Type(obj, outMetaTypeItem);

	LEAVE_V8(return sdtUnknown)
}

void GV8ScriptObject::bindClass(const GScriptName & name, IMetaClass * metaClass)
{
	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	doBindClass(&this->implement->param, localObject, name, metaClass);

	LEAVE_V8()
}

void GV8ScriptObject::bindEnum(const GScriptName & name, IMetaEnum * metaEnum)
{
}

GScriptObject * GV8ScriptObject::createScriptObject(const GScriptName & name)
{
	return NULL;
}

GMetaVariant GV8ScriptObject::invoke(const GScriptName & name, const GMetaVariant * params, size_t paramCount)
{
	return GMetaVariant();
}

GMetaVariant GV8ScriptObject::invokeIndirectly(const GScriptName & name, GMetaVariant const * const * params, size_t paramCount)
{
	return GMetaVariant();
}

void GV8ScriptObject::bindFundamental(const GScriptName & name, const GVariant & value)
{
	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	doBindFundamental(&this->implement->param, localObject, name, value);

	LEAVE_V8()
}

void GV8ScriptObject::bindAccessible(const GScriptName & name, void * instance, IMetaAccessible * accessible)
{
	ENTER_V8()

	v8::HandleScope handleScope;
	v8::Local<v8::Object> localObject(v8::Local<v8::Object>::New(this->implement->object));

	doBindAccessible(&this->implement->param, localObject, name, instance, accessible);

	LEAVE_V8()
}

void GV8ScriptObject::bindString(const GScriptName & stringName, const char * s)
{
}

void GV8ScriptObject::bindObject(const GScriptName & objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
}

void GV8ScriptObject::bindMethod(const GScriptName & name, void * instance, IMetaMethod * method)
{
}

void GV8ScriptObject::bindMethodList(const GScriptName & name, IMetaList * methodList)
{
}

IMetaClass * GV8ScriptObject::getClass(const GScriptName & className)
{
	return NULL;
}

IMetaEnum * GV8ScriptObject::getEnum(const GScriptName & enumName)
{
	return NULL;
}

GVariant GV8ScriptObject::getFundamental(const GScriptName & name)
{
	return GVariant();
}

std::string GV8ScriptObject::getString(const GScriptName & stringName)
{
	return "";
}

void * GV8ScriptObject::getObject(const GScriptName & objectName)
{
	return NULL;
}

IMetaMethod * GV8ScriptObject::getMethod(const GScriptName & methodName)
{
	return NULL;
}

IMetaList * GV8ScriptObject::getMethodList(const GScriptName & methodName)
{
	return NULL;
}

void GV8ScriptObject::assignValue(const GScriptName & fromName, const GScriptName & toName)
{
}

bool GV8ScriptObject::valueIsNull(const GScriptName & name)
{
	return false;
}

void GV8ScriptObject::nullifyValue(const GScriptName & name)
{
}



} // namespace cpgf



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
	
	GScopedInterface<IScriptObject> bindingApi(new ImplScriptObject(new GV8ScriptObject(service.get(), context->Global(), GScriptConfig())));
	bindBasicData(bindingApi.get(), service.get());
	GScopedInterface<IMetaClass> metaClass(service->findClassByName("testscript::TestObject"));
	GScopedInterface<IMetaField> metaField(metaClass->getField("value"));

	TestObject obj;
	
	obj.value = 3838;
	script->bindAccessible("v", &obj, metaField.get());
	executeString("v");
	executeString("v = 5");
	cout << obj.value << endl;

	script->bindClass("TestObject", metaClass.get());
	executeString("a = new TestObject()");
	executeString("b = new TestObject()");
	executeString("a.value = 67");
	executeString("b.value = 18");
	executeString("a.value");
	executeString("b.value");
	executeString("a.add(8)");
	executeString("b.add(9)");
	executeString("b = a.add");
executeString("b(9)");
	executeString("a instanceof TestObject");
	executeString("b = a.self()");
	executeString("b.value");
	executeString("b.add(8)");
	executeString("b instanceof TestObject");
	
	metaClass.reset(service->findClassByName("testscript::BasicA"));
	script->bindClass("BasicA", metaClass.get());

	executeString("a = new BasicA()");
	executeString("a.BasicEnum.a");
	executeString("a.b");
	executeString("b = new a.Inner()");
	executeString("b.x");
	executeString("b.add()");
	executeString("b.x");

	context.Dispose();
}




#if defined(_MSC_VER)
#pragma warning(pop)
#endif


