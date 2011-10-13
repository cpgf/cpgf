#include "cpgf/scriptbind/gluabind.h"
#include "cpgf/gflags.h"
#include "gbindcommon.h"


#include <stdexcept>
#include <algorithm>
#include <vector>

#include <string.h>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif

using namespace std;

#define ENTER_LUA() \
	try {

#define LEAVE_LUA(L, ...) \
	} \
	catch(const GVariantException & e) { error(L, e.what()); __VA_ARGS__; } \
	catch(const GMetaException & e) { error(L, e.what()); __VA_ARGS__; } \
	catch(const GScriptException & e) { error(L, e.what()); __VA_ARGS__; } \
	catch(...) { error(L, "Unknown exception occurred."); __VA_ARGS__; }
	
	
namespace cpgf {


namespace {

	class GLuaBindingParam;

	int UserData_gc(lua_State * L);
	int UserData_call(lua_State * L);
	int UserData_index(lua_State * L);
	int UserData_newindex(lua_State * L);
	int UserData_operator(lua_State * L);
	
	void doBindAllOperators(lua_State * L, GLuaBindingParam * param, void * instance, IMetaClass * metaClass);
	void doBindMethod(lua_State * L, GLuaBindingParam * param, void * instance, IMetaMethod * method);
	void doBindClass(lua_State * L, GLuaBindingParam * param, IMetaClass * metaClass);
	void doBindEnum(lua_State * L, GLuaBindingParam * param, IMetaEnum * metaEnum);
	void doBindMethodList(lua_State * L, GLuaBindingParam * param, void * instance, IMetaList * methodList);
	
	void initObjectMetaTable(lua_State * L);
	void setMetaTableGC(lua_State * L);
	void setMetaTableCall(lua_State * L, void * userData);
	void setMetaTableSignature(lua_State * L);
	bool isValidMetaTable(lua_State * L, int index);

	const char * luaOperators[] = {
		"__add", "__sub", "__mul", "__div",
		"__mod", "__unm", "__eq", "__lt",
		"__le", "__call"
	};
	
	const GMetaOpType metaOpTypes[] = {
		mopAdd, mopSub, mopMul, mopDiv,
		mopMod, mopNeg, mopEqual, mopLessor,
		mopLessorEqual, mopFunctor
	};

	const char * metaTableSignature = "cpgf_suPer_mEta_Table";
	const lua_Integer metaTableSigValue = 0x1eabeef;
	const char * classMetaTablePrefix = "cpgf_cLaSs_mEta_Table";

	enum UserDataType {
		udtClass,
		udtMethod,
		udtMethodList,
		udtEnum,
		udtOperator
	};
	

	class GLuaBindingParam
	{
	public:
		GLuaBindingParam(IMetaService * service, const GScriptConfig & config) : service(service), config(config) {
			this->service->addReference();
		}

		IMetaService * getService() const {
			return this->service.get();
		}

		const GScriptConfig & getConfig() const {
			return this->config;
		}

	private:
		GApiScopedPointer<IMetaService> service;
		const GScriptConfig & config;
	};

	int handleError(const char * message, ...)
	{
		char buffer[4096];

		va_list args;
		va_start(args, message);

		vsprintf(buffer, message, args);

		va_end (args);

printf("Error: %s \n", buffer);
		throw GScriptException(std::string(buffer));
	}

	class GLuaUserData
	{
	public:
		GLuaUserData(UserDataType type, GLuaBindingParam * param) : type(type), param(param) {
		}

		virtual ~GLuaUserData() {
		}

		UserDataType getType() const {
			return this->type;
		}

		GLuaBindingParam * getParam() const {
			return this->param;
		}

	private:
		UserDataType type;
		GLuaBindingParam * param;
	};

	class GClassUserData : public GLuaUserData
	{
	private:
		typedef GLuaUserData super;

	public:
		GClassUserData(GLuaBindingParam * param, IMetaClass * metaClass, void * instance, bool isInstance, bool allowGC, ObjectPointerCV cv)
			: super(udtClass, param), metaClass(metaClass), instance(instance), isInstance(isInstance), allowGC(allowGC), cv(cv) {
			this->metaClass->addReference();
		}

		virtual ~GClassUserData() {
			if(this->allowGC) {
				this->metaClass->destroyInstance(instance);
			}

			this->metaClass->releaseReference();
		}

	public:
		IMetaClass * metaClass;
		void * instance;
		bool isInstance;
		bool allowGC;
		ObjectPointerCV cv;
	};
	
	class GMethodUserData : public GLuaUserData
	{
	private:
		typedef GLuaUserData super;

	public:
		GMethodUserData(GLuaBindingParam * param, void * instance, IMetaMethod * method)
			: super(udtMethod, param), instance(instance), method(method) {
			this->method->addReference();
		}

		virtual ~GMethodUserData() {
			this->method->releaseReference();
		}

	public:
		void * instance;
		IMetaMethod * method;
	};

	class GMethodListUserData : public GLuaUserData
	{
	private:
		typedef GLuaUserData super;

	public:
		GMethodListUserData(GLuaBindingParam * param, void * instance, IMetaList * methodList)
			: super(udtMethodList, param), instance(instance), methodList(methodList) {
			this->methodList->addReference();
		}

		virtual ~GMethodListUserData() {
			this->methodList->releaseReference();
		}

	public:
		void * instance;
		IMetaList * methodList;
	};

	class GOperatorUserData : public GLuaUserData
	{
	private:
		typedef GLuaUserData super;

	public:
		GOperatorUserData(GLuaBindingParam * param, void * instance, IMetaClass * metaClass, GMetaOpType op)
			: super(udtOperator, param), instance(instance), metaClass(metaClass), op(op) {
		}

		virtual ~GOperatorUserData() {
		}

	public:
		void * instance;
		IMetaClass * metaClass;
		GMetaOpType op;
	};

	class GEnumUserData : public GLuaUserData
	{
	private:
		typedef GLuaUserData super;

	public:
		GEnumUserData(GLuaBindingParam * param, IMetaEnum * metaEnum)
			: super(udtEnum, param), metaEnum(metaEnum) {
			this->metaEnum->addReference();
		}

		virtual ~GEnumUserData() {
			this->metaEnum->releaseReference();
		}

	public:
		IMetaEnum * metaEnum;
	};

	struct InvokeCallableResult
	{
		int resultCount;
		GVarData resultData;
	};


	void error(lua_State * L, const std::string & message)
	{
		lua_Debug ar;
		lua_getstack(L, 1, &ar);
		lua_getinfo(L, "nSl", &ar);

		const char * fileName = ar.source;
		char dummy = 0;
		if(fileName == NULL || *fileName != '@') {
			fileName = &dummy;
		}

		fprintf(stderr, "Error, file %s, line %d: %s \n", fileName, ar.currentline, message.c_str());

		lua_pushstring(L, message.c_str());
		lua_error(L);
	}

	void failVariantToLua()
	{
		handleError("Can't convert variant to Lua object.");
	}
	
	void objectToLua(lua_State * L, GLuaBindingParam * param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv)
	{
		if(instance == NULL) {
			lua_pushnil(L);

			return;
		}

		void * userData = lua_newuserdata(L, sizeof(GClassUserData));
		new (userData) GClassUserData(param, metaClass, instance, true, allowGC, cv);

		const char * className = metaClass->getName();
		
		GASSERT_MSG(strlen(className) < 4000, "Meta class name is too long");

		char metaTableName[4096];

		strcpy(metaTableName, classMetaTablePrefix);
		strcat(metaTableName, className);

		lua_getfield(L, LUA_REGISTRYINDEX, metaTableName);
		if(lua_isnil(L, -1)) {
			lua_pop(L, 1);

			lua_newtable(L);
			setMetaTableSignature(L);

			setMetaTableGC(L);
		
			initObjectMetaTable(L);
			doBindAllOperators(L, param, instance, metaClass);

			lua_pushvalue(L, -1); // duplicate the meta table
			lua_setfield(L, LUA_REGISTRYINDEX, metaTableName);
		}
		
		lua_setmetatable(L, -2);
	}

	void * luaToObject(lua_State * L, GLuaBindingParam * param, int index, GMetaType * outType)
	{
		(void)param;

		if(isValidMetaTable(L, index)) {
			void * userData = lua_touserdata(L, index);
			if(static_cast<GLuaUserData *>(userData)->getType() == udtClass) {
				GClassUserData * classData = static_cast<GClassUserData *>(userData);
				if(outType != NULL) {
					GMetaTypeData typeData;
					classData->metaClass->getMetaType(&typeData);
					*outType = GMetaType(typeData);
				}

				return classData->instance;
			}
		}

		return NULL;
	}

	GMetaVariant luaToVariant(lua_State * L, GLuaBindingParam * param, int index)
	{
		int type = lua_type(L, index);

		switch(type) {
			case LUA_TNIL:
				return 0;

			case LUA_TNUMBER:
				return lua_tonumber(L, index);

			case LUA_TBOOLEAN:
				return lua_toboolean(L, index);

			case LUA_TSTRING:
				return lua_tostring(L, index);

			case LUA_TUSERDATA: {
				GMetaType metaType;
				void * obj = luaToObject(L, param, index, &metaType);
				if(obj != NULL) {
					metaType.addPointer();
					return GMetaVariant(pointerToObjectVariant(obj), metaType);
				}
			}
				break;

			case LUA_TLIGHTUSERDATA:
				return lua_touserdata(L, index);
				break;

			case LUA_TTABLE:
			case LUA_TFUNCTION:
			case LUA_TTHREAD:
				break;
		}

		return GMetaVariant();
	}

	bool variantToLua(lua_State * L, GLuaBindingParam * param, const GVariant & value, const GMetaType & type, bool allowGC)
	{
		GVariantType vt = value.getType();
		
		if(vtIsEmpty(vt)) {
			lua_pushnil(L);

			return true;
		}

		if(vtIsBoolean(vt)) {
			lua_pushboolean(L, fromVariant<bool>(value));

			return true;
		}

		if(vtIsInteger(vt)) {
			lua_pushinteger(L, fromVariant<lua_Integer>(value));

			return true;
		}

		if(vtIsReal(vt)) {
			lua_pushnumber(L, fromVariant<lua_Number>(value));

			return true;
		}

		if(canFromVariant<void *>(value) && objectAddressFromVariant(value) == NULL) {
			lua_pushnil(L);

			return true;
		}

		if(vtIsVoidPointer(vt)) {
			lua_pushlightuserdata(L, fromVariant<void *>(value));

			return true;
		}

		if(value.getPointers() == 1 && value.getBaseType() == vtChar) {
			lua_pushstring(L, fromVariant<char *>(value));

			return true;
		}

		if(type.isEmpty()) {
			return false;
		}

		if(type.getPointerDimension() <= 1) {
			GApiScopedPointer<IMetaTypedItem> typedItem(param->getService()->findTypedItemByName(type.getBaseName()));
			if(typedItem) {
				if(type.getPointerDimension() == 0) {
					GASSERT_MSG(!! typedItem->isClass(), "Unknown type");
					GASSERT_MSG(type.baseIsClass(), "Unknown type");

					IMetaClass * metaClass = static_cast<IMetaClass *>(typedItem.get());
					void * instance = metaClass->cloneInstance(objectAddressFromVariant(value));
					objectToLua(L, param, instance, static_cast<IMetaClass *>(typedItem.get()), true, metaTypeToCV(type));

					return true;
				}

				if(type.getPointerDimension() == 1) {
					GASSERT_MSG(!! typedItem->isClass(), "Unknown type");

					objectToLua(L, param, fromVariant<void *>(value), static_cast<IMetaClass *>(typedItem.get()), allowGC, metaTypeToCV(type));

					return true;
				}
			}
		}

		return false;
	}
	
	bool converterToLua(lua_State * L, GLuaBindingParam * param, const GVariant & value, IMetaConverter * converter)
	{
		if(converter == NULL) {
			return false;
		}

		if(converter->canToCString()) {
			gapi_bool needFree;
			
			GApiScopedPointer<IApiAllocator> allocator(param->getService()->getAllocator());
			const char * s = converter->toCString(objectAddressFromVariant(value), &needFree, allocator.get());

			if(s != NULL) {
				lua_pushstring(L, s);

				if(needFree) {
					allocator->free((void *)s);
				}

				return true;
			}
		}

		return false;
	}
	
	GScriptDataType getLuaType(lua_State * L, int index)
	{
		switch(lua_type(L, index)) {
			case LUA_TNIL:
				return sdtNull;

			case LUA_TNUMBER:
			case LUA_TBOOLEAN:
				return sdtFundamental;

			case LUA_TSTRING:
				return sdtString;

			case LUA_TUSERDATA:
				if(isValidMetaTable(L, index)) {
					void * rawUserData = lua_touserdata(L, index);
					GLuaUserData * userData = static_cast<GLuaUserData *>(rawUserData);
					switch(userData->getType()) {
					case udtClass:
						if(static_cast<GClassUserData *>(userData)->isInstance) {
							return sdtObject;
						}
						else {
							return sdtClass;
						}

					case udtMethod:
					case udtMethodList:
						return sdtMethod;

					case udtEnum:
						return sdtEnum;

					default:
						break;
					}
				}
				break;

			case LUA_TTABLE:
				return sdtScriptObject;

			case LUA_TFUNCTION:
				return sdtScriptMethod;

		}
		
		return sdtUnknown;
	}

	void loadMethodParameters(lua_State * L, GLuaBindingParam * param, GVarData * outputParams, size_t startIndex, size_t paramCount)
	{
		for(size_t i = 0; i < paramCount; ++i) {
			outputParams[i] = luaToVariant(L, param, i + startIndex).getData().varData;
		}
	}

	void loadMethodParamTypes(lua_State * L, GScriptDataType * outputTypes, size_t startIndex, size_t paramCount)
	{
		for(size_t i = 0; i < paramCount; ++i) {
			outputTypes[i] = getLuaType(L, i + startIndex);
		}
	}

	void doInvokeCallable(void * instance, IMetaCallable * callable, GVarData * paramsData, size_t paramCount, InvokeCallableResult * result)
	{
		result->resultCount = 0;
		vtInit(result->resultData.typeData);

		callable->execute(&result->resultData, instance, paramsData, paramCount);
	}
	
	bool doPushInvokeResult(lua_State * L, GLuaBindingParam * param, IMetaCallable * callable, InvokeCallableResult * result)
	{
		if(callable->hasResult()) {
			GMetaTypeData typeData;
		
			callable->getResultType(&typeData);

			GVariant value = GVariant(result->resultData);
			bool success = variantToLua(L, param, value, GMetaType(typeData), !! callable->isResultTransferOwnership());
			if(!success) {
				GApiScopedPointer<IMetaConverter> converter(callable->createResultConverter());
				success = converterToLua(L, param, value, converter.get());
			}
			if(!success) {
				failVariantToLua();
			}

			result->resultCount = 1;
		}
		
		return true;
	}
	
	int callbackInvokeMethod(lua_State * L)
	{
		ENTER_LUA()

		GMethodUserData * userData = static_cast<GMethodUserData *>(lua_touserdata(L, lua_upvalueindex(1)));
		IMetaMethod * method = userData->method;

		InvokeCallableResult result;
		
		GVarData paramsData[REF_MAX_ARITY];
		loadMethodParameters(L, userData->getParam(), paramsData, 1, lua_gettop(L));
		if(checkCallable(method, paramsData, lua_gettop(L))) {
			doInvokeCallable(userData->instance, method, paramsData, lua_gettop(L), &result);
			doPushInvokeResult(L, userData->getParam(), method, &result);
		}
		else {
			handleError("Can't invoke method %s", method->getName());
		}
		
		return result.resultCount;

		LEAVE_LUA(L, return 0)
	}

	int callbackInvokeMethodList(lua_State * L)
	{
		ENTER_LUA()

		GMethodListUserData * userData = static_cast<GMethodListUserData *>(lua_touserdata(L, lua_upvalueindex(1)));
		IMetaList * methodList = userData->methodList;
		
		InvokeCallableResult result;

		const char * methodName = NULL;
		
		size_t methodCount = methodList->getCount();
		
		int maxRank = -1;
		size_t maxRankIndex = 0;
		
		GVarData paramsData[REF_MAX_ARITY];
		loadMethodParameters(L, userData->getParam(), paramsData, 1, lua_gettop(L));
		
		GScriptDataType paramsType[REF_MAX_ARITY];
		loadMethodParamTypes(L, paramsType, 1, lua_gettop(L));
		
		for(size_t i = 0; i < methodCount; ++i) {
			GApiScopedPointer<IMetaMethod> method(static_cast<IMetaMethod *>(methodList->getAt(i)));

			methodName = method->getName();
		
			int rank = rankCallable(method.get(), paramsData, paramsType, lua_gettop(L));
			if(rank > maxRank) {
				maxRank = rank;
				maxRankIndex = i;
			}
		}
		
		if(maxRank >= 0) {
			GApiScopedPointer<IMetaMethod> method(static_cast<IMetaMethod *>(methodList->getAt(maxRankIndex)));
			doInvokeCallable(methodList->getInstanceAt(maxRankIndex), method.get(), paramsData, lua_gettop(L), &result);
			doPushInvokeResult(L, userData->getParam(), method.get(), &result);
			return result.resultCount;
		}

		if(methodName == NULL) {
			handleError("Internal error: can't find method list name.");
		}
		else {
			handleError("Can't find matched method to invoke for %s", methodName);
		}

		return 0;		

		LEAVE_LUA(L, return 0)
	}

	int invokeConstructor(lua_State * L, GLuaBindingParam * param, IMetaClass * metaClass)
	{
		size_t paramCount = lua_gettop(L) - 1;
		void * instance = NULL;

		if(paramCount == 0 && metaClass->canCreateInstance()) {
			instance = metaClass->createInstance();
		}
		else {
			InvokeCallableResult result;
			size_t count = metaClass->getConstructorCount();
			
			int maxRank = -1;
			size_t maxRankIndex = 0;
		
			GVarData paramsData[REF_MAX_ARITY];
			loadMethodParameters(L, param, paramsData, 2, paramCount);

			GScriptDataType paramsType[REF_MAX_ARITY];
			loadMethodParamTypes(L, paramsType, 2, paramCount);
		
			for(size_t i = 0; i < count; ++i) {
				GApiScopedPointer<IMetaConstructor> constructor(metaClass->getConstructorAt(i));
				int rank = rankCallable(constructor.get(), paramsData, paramsType, paramCount);
				if(rank > maxRank) {
					maxRank = rank;
					maxRankIndex = i;
				}
			}
			if(maxRank >= 0) {
				GApiScopedPointer<IMetaConstructor> constructor(metaClass->getConstructorAt(maxRankIndex));
				doInvokeCallable(NULL, constructor.get(), paramsData, paramCount, &result);
				instance = fromVariant<void *>(GVariant(result.resultData));
			}
		}

		if(instance != NULL) {
			objectToLua(L, param, instance, metaClass, true, opcvNone);
		}
		else {
			handleError("Failed to construct an object.");
		}

		return 1;
	}

	int invokeOperator(lua_State * L, GLuaBindingParam * param, void * instance, IMetaClass * metaClass, GMetaOpType op)
	{
		size_t paramCount = lua_gettop(L);
		size_t startIndex = 1;

		InvokeCallableResult result;
		size_t count = metaClass->getOperatorCount();

		if(op == mopFunctor) { // skip the first "func" parameter
			++startIndex;
			--paramCount;
		}
			
		int maxRank = -1;
		size_t maxRankIndex = 0;
		
		GVarData paramsData[REF_MAX_ARITY];
		loadMethodParameters(L, param, paramsData, startIndex, paramCount);
		
		GScriptDataType paramsType[REF_MAX_ARITY];
		loadMethodParamTypes(L, paramsType, startIndex, paramCount);

		for(size_t i = 0; i < count; ++i) {
			GApiScopedPointer<IMetaOperator> metaOperator(metaClass->getOperatorAt(i));
			if(op == metaOperator->getOperator()) {
				int rank = rankCallable(metaOperator.get(), paramsData, paramsType, paramCount);
				if(rank > maxRank) {
					maxRank = rank;
					maxRankIndex = i;
				}
			}
		}
		if(maxRank >= 0) {
			GApiScopedPointer<IMetaOperator> metaOperator(metaClass->getOperatorAt(maxRankIndex));
			doInvokeCallable(instance, metaOperator.get(), paramsData, paramCount, &result);
			doPushInvokeResult(L, param, metaOperator.get(), &result);
			return result.resultCount;
		}

		return 0;
	}

	int UserData_gc(lua_State * L)
	{
		ENTER_LUA()

		GLuaUserData * userData = static_cast<GLuaUserData *>(lua_touserdata(L, -1));
		userData->~GLuaUserData();
		
		return 0;
		
		LEAVE_LUA(L, return 0)
	}
	
	int UserData_call(lua_State * L)
	{
		ENTER_LUA()

		GClassUserData * userData = static_cast<GClassUserData *>(lua_touserdata(L, lua_upvalueindex(1)));


		if(userData->instance == NULL) { // constructor
			return invokeConstructor(L, userData->getParam(), userData->metaClass);
		}
		else {
			handleError("Internal error: calling wrong functor.");

			return 0;
		}
		
		LEAVE_LUA(L, return 0)
	}
	
	bool indexMemberMethod(lua_State * L, GClassUserData * userData, const char * name)
	{
		GFlags<GMetaFilters> filters;

		if(userData->isInstance) {
			if(! userData->getParam()->getConfig().allowAccessStaticMethodViaInstance()) {
				filters.set(metaFilterIgnoreStatic);
			}
		}
		else {
			filters.set(metaFilterIgnoreInstance);
		}
		
		cvToFilters(userData->cv, &filters);

		GApiScopedPointer<IMetaList> methodList(userData->getParam()->getService()->createMetaList());
		
		userData->metaClass->getMethodListInHierarchy(methodList.get(), name, filters, userData->instance);
		
		size_t methodCount = methodList->getCount();

		if(methodCount == 0) {
			return false;
		}

		if(methodCount == 1) {
			GApiScopedPointer<IMetaMethod> method(static_cast<IMetaMethod *>(methodList->getAt(0)));
			doBindMethod(L, userData->getParam(), methodList->getInstanceAt(0), method.get());

			return true;
		}

		doBindMethodList(L, userData->getParam(), userData->instance, methodList.get());

		return true;
	}

	bool indexMemberData(lua_State * L, GClassUserData * userData, const char * name)
	{
		void * instance = userData->instance;

		GApiScopedPointer<IMetaAccessible> data(findAccessible(userData->metaClass, name, true, false, &instance));
		
		if(!data) {
			return false;
		}

		GVarData varData;
		GMetaTypeData typeData;
		data->get(instance, &varData);
		data->getItemType(&typeData);
		
		GVariant value = GVariant(varData);
		bool success = variantToLua(L, userData->getParam(), value, GMetaType(typeData), false);
		if(!success) {
			GApiScopedPointer<IMetaConverter> converter(data->createConverter());
			success = converterToLua(L, userData->getParam(), value, converter.get());
		}

		return success;
	}

	bool indexMemberClass(lua_State * L, GClassUserData * userData, const char * name)
	{
		GApiScopedPointer<IMetaClass> metaClass(userData->metaClass->getClassInHierarchy(name, NULL));

		if(!metaClass) {
			return false;
		}

		doBindClass(L, userData->getParam(), metaClass.get());

		return true;
	}

	bool indexMemberEnumType(lua_State * L, GClassUserData * userData, const char * name)
	{
		GApiScopedPointer<IMetaEnum> metaEnum(userData->metaClass->getEnumInHierarchy(name, NULL));

		if(!metaEnum) {
			return false;
		}

		doBindEnum(L, userData->getParam(), metaEnum.get());

		return true;
	}

	bool indexMemberEnumValue(lua_State * L, GClassUserData * userData, const char * name)
	{
		size_t count = userData->metaClass->getEnumCount();

		for(size_t i = 0; i < count; ++i) {
			GApiScopedPointer<IMetaEnum> metaEnum(userData->metaClass->getEnumAt(i));
			int index = metaEnum->findKey(name);
			if(index >= 0) {
				GVarData data;
				metaEnum->getValue(index, &data);
				lua_pushinteger(L, fromVariant<lua_Integer>(GVariant(data)));

				return true;
			}
		}


		return false;
	}

	int UserData_index(lua_State * L)
	{
		ENTER_LUA()

		GClassUserData * userData = static_cast<GClassUserData *>(lua_touserdata(L, -2));
		
		const char * name = lua_tostring(L, -1);
		
		if(indexMemberMethod(L, userData, name)) {
			return true;
		}

		if(indexMemberData(L, userData, name)) {
			return true;
		}

		if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumTypeViaInstance()) {
			if(indexMemberEnumType(L, userData, name)) {
				return true;
			}
		}

		if(! userData->isInstance || userData->getParam()->getConfig().allowAccessEnumValueViaInstance()) {
			if(indexMemberEnumValue(L, userData, name)) {
				return true;
			}
		}

		if(! userData->isInstance || userData->getParam()->getConfig().allowAccessClassViaInstance()) {
			if(indexMemberClass(L, userData, name)) {
				return true;
			}
		}

		return false;
		
		LEAVE_LUA(L, return false)
	}

	bool newindexMemberData(lua_State * L, GClassUserData * userData, const char * name, const GVariant & value)
	{
		(void)L;

		void * instance = userData->instance;

		GApiScopedPointer<IMetaAccessible> data(findAccessible(userData->metaClass, name, false, true, &instance));
		
		if(!data) {
			return false;
		}

		GVarData varData = value.getData();
		data->set(instance, &varData);

		return true;
	}

	int UserData_newindex(lua_State * L)
	{
		ENTER_LUA()

		GClassUserData * userData = static_cast<GClassUserData *>(lua_touserdata(L, -3));
		
		const char * name = lua_tostring(L, -2);

		GVariant value = luaToVariant(L, userData->getParam(), -1).getValue();
		
		if(newindexMemberData(L, userData, name, value)) {
			return true;
		}

		return 0;
		
		LEAVE_LUA(L, return 0)
	}

	int UserData_operator(lua_State * L)
	{
		ENTER_LUA()
		
		GOperatorUserData * userData = static_cast<GOperatorUserData *>(lua_touserdata(L, lua_upvalueindex(1)));

		return invokeOperator(L, userData->getParam(), userData->instance, userData->metaClass, userData->op);
		
		LEAVE_LUA(L, return 0)
	}

	void doBindOperator(lua_State * L, GLuaBindingParam * param, void * instance, IMetaClass * metaClass, GMetaOpType op)
	{
		for(size_t i = 0; i < sizeof(metaOpTypes) / sizeof(metaOpTypes[0]); ++i) {
			if(metaOpTypes[i] == op) {
				lua_pushstring(L, luaOperators[i]);
				void * userData = lua_newuserdata(L, sizeof(GOperatorUserData));
				new (userData) GOperatorUserData(param, instance, metaClass, op);
				
				lua_pushcclosure(L, &UserData_operator, 1);
				lua_rawset(L, -3);

				return ;
			}
		}

		handleError("Failed to bind an operator that's not supported by Lua.");
	}

	void doBindAllOperators(lua_State * L, GLuaBindingParam * param, void * instance, IMetaClass * metaClass)
	{
		std::vector<uint32_t> boundOperators;

		size_t count = metaClass->getOperatorCount();
		for(size_t i = 0; i < count; ++i) {
			GApiScopedPointer<IMetaOperator> item(metaClass->getOperatorAt(i));
			uint32_t op = item->getOperator();
			if(std::find(boundOperators.begin(), boundOperators.end(), op) == boundOperators.end()) {
				doBindOperator(L, param, instance, metaClass, static_cast<GMetaOpType>(op));
			}
		}
	}

	void doBindMethod(lua_State * L, GLuaBindingParam * param, void * instance, IMetaMethod * method)
	{
		void * userData = lua_newuserdata(L, sizeof(GMethodUserData));
		new (userData) GMethodUserData(param, instance, method);

		lua_newtable(L);
		
		setMetaTableSignature(L);
		setMetaTableGC(L);
		lua_setmetatable(L, -2);

		lua_pushcclosure(L, &callbackInvokeMethod, 1);
	}
	
	void doBindClass(lua_State * L, GLuaBindingParam * param, IMetaClass * metaClass)
	{
		void * userData = lua_newuserdata(L, sizeof(GClassUserData));
		new (userData) GClassUserData(param, metaClass, NULL, false, false, opcvNone);

		lua_newtable(L);

		setMetaTableSignature(L);
		setMetaTableGC(L);
		setMetaTableCall(L, userData);

		initObjectMetaTable(L);

		lua_setmetatable(L, -2);
	}

	void doBindMethodList(lua_State * L, GLuaBindingParam * param, void * instance, IMetaList * methodList)
	{
		void * userData = lua_newuserdata(L, sizeof(GMethodListUserData));
		new (userData) GMethodListUserData(param, instance, methodList);
		
		lua_newtable(L);
		
		setMetaTableSignature(L);
		setMetaTableGC(L);
		lua_setmetatable(L, -2);
		
		lua_pushcclosure(L, &callbackInvokeMethodList, 1);
	}
	
	void setMetaTableGC(lua_State * L)
	{
		lua_pushstring(L, "__gc");	
		lua_pushcclosure(L, &UserData_gc, 0);
		lua_rawset(L, -3);
	}

	void setMetaTableCall(lua_State * L, void * userData)
	{
		lua_pushstring(L, "__call");
		lua_pushlightuserdata(L, userData);
		lua_pushcclosure(L, &UserData_call, 1);
		lua_rawset(L, -3);
	}

	void setMetaTableSignature(lua_State * L)
	{
		lua_pushstring(L, metaTableSignature);
		lua_pushinteger(L, metaTableSigValue);
		lua_rawset(L, -3);
	}

	bool isValidMetaTable(lua_State * L, int index)
	{
		if(lua_getmetatable(L, index) == 0) {
			return false;
		}

		lua_pushstring(L, metaTableSignature);
		lua_gettable(L, -2);
		bool valid = (lua_tointeger(L, -1) == metaTableSigValue);
		lua_pop(L, 2);
		return valid;
	}

	void initObjectMetaTable(lua_State * L)
	{
		lua_pushstring(L, "__index");
		lua_pushcclosure(L, &UserData_index, 0);
		lua_rawset(L, -3);
	
		lua_pushstring(L, "__newindex");
		lua_pushcclosure(L, &UserData_newindex, 0);
		lua_rawset(L, -3);
	}


	int Enum_index(lua_State * L)
	{
		ENTER_LUA()

		GEnumUserData * userData = static_cast<GEnumUserData *>(lua_touserdata(L, -2));
		
		const char * name = lua_tostring(L, -1);

		int index = userData->metaEnum->findKey(name);
		if(index < 0) {
			handleError("Can't find enumerator key -- %s.", name);
		}
		else {
			GVarData data;
			userData->metaEnum->getValue(index, &data);
			lua_pushinteger(L, fromVariant<lua_Integer>(GVariant(data)));
		}
		
		return true;
		
		LEAVE_LUA(L, return false)
	}

	int Enum_newindex(lua_State * L)
	{
		ENTER_LUA()

		handleError("Can't assign value to enumerator.");

		return 0;
		
		LEAVE_LUA(L, return 0)
	}

	void doBindEnum(lua_State * L, GLuaBindingParam * param, IMetaEnum * metaEnum)
	{
		void * userData = lua_newuserdata(L, sizeof(GEnumUserData));
		new (userData) GEnumUserData(param, metaEnum);

		lua_newtable(L);

		setMetaTableSignature(L);
		setMetaTableGC(L);
		
		lua_pushstring(L, "__index");
		lua_pushcclosure(L, &Enum_index, 0);
		lua_rawset(L, -3);
	
		lua_pushstring(L, "__newindex");
		lua_pushcclosure(L, &Enum_newindex, 0);
		lua_rawset(L, -3);

		lua_setmetatable(L, -2);
	}

} // unnamed namespace


class GLuaScriptObjectImplement
{
public:
	GLuaScriptObjectImplement(GLuaScriptObject * binding, IMetaService * service, lua_State * L, const GScriptConfig & config)
		: binding(binding), param(service, config), luaState(L)
	{
	}
	
	void getTable(lua_State * L) const {
		if(! this->binding->getOwner()->isGlobal()) {
			static_cast<GLuaScriptObject *>(this->binding->getOwner())->implement->doGetTable(L);
		}
		this->doGetTable(L);
	}

protected:
	void doGetTable(lua_State * L) const {
		if(this->binding->getOwner()->isGlobal()) {
			lua_getglobal(L, this->binding->getName());
		}
		else {
			lua_getfield(L, -1, this->binding->getName());
		}
	}

public:
	GLuaScriptObject * binding;
	GLuaBindingParam param;
	lua_State * luaState;
};


class GLuaScriptNameData : public GScriptNameData
{
public:
	GLuaScriptNameData(GLuaScriptObject * table, const char * name);
	virtual ~GLuaScriptNameData();

	void set();
	void get();

	virtual bool isAvailable() const;

private:
	int luaReference;
	GLuaScriptObject * table;
};


class GLuaScopeGuard
{
public:
	GLuaScopeGuard(GScriptObject * scope);
	~GLuaScopeGuard();
	
	void set(const GScriptName & name);
	void get(const GScriptName & name);

private:
	GLuaScriptObject * scope;
	int top;
};


GLuaScopeGuard::GLuaScopeGuard(GScriptObject * scope)
	: scope(static_cast<GLuaScriptObject *>(scope))
{
	this->top = lua_gettop(this->scope->implement->luaState);
	if(! this->scope->isGlobal()) {
		this->scope->implement->getTable(this->scope->implement->luaState);
	}
}
	
GLuaScopeGuard::~GLuaScopeGuard()
{
	int currentTop = lua_gettop(this->scope->implement->luaState);
	if(currentTop > this->top) {
		lua_pop(this->scope->implement->luaState, currentTop - this->top);
	}
}

void GLuaScopeGuard::set(const GScriptName & name)
{
	if(scope->isGlobal()) {
		if(name.hasData()) {
			static_cast<GLuaScriptNameData *>(name.getData())->set();
		}
		else {
			lua_setglobal(this->scope->implement->luaState, name.getName());
		}
	}
	else {
		if(name.hasData()) {
			static_cast<GLuaScriptNameData *>(name.getData())->set();
		}
		else {
			lua_setfield(this->scope->implement->luaState, -2, name.getName());
		}
	}
}
	
void GLuaScopeGuard::get(const GScriptName & name)
{
	if(scope->isGlobal()) {
		if(name.hasData()) {
			static_cast<GLuaScriptNameData *>(name.getData())->get();
		}
		else {
			lua_getglobal(this->scope->implement->luaState, name.getName());
		}
	}
	else {
		if(name.hasData()) {
			static_cast<GLuaScriptNameData *>(name.getData())->get();
		}
		else {
			lua_getfield(this->scope->implement->luaState, -1, name.getName());
		}
	}
}


GLuaScriptNameData::GLuaScriptNameData(GLuaScriptObject * table, const char * name) : table(table)
{
	GLuaScopeGuard scopeGuard(table);
	scopeGuard.get(name);
	this->luaReference = luaL_ref(table->implement->luaState, LUA_REGISTRYINDEX);
}

GLuaScriptNameData::~GLuaScriptNameData()
{
	GLuaScopeGuard scopeGuard(this->table);
	luaL_unref(table->implement->luaState, LUA_REGISTRYINDEX, this->luaReference);
}

void GLuaScriptNameData::set()
{
	lua_rawseti(table->implement->luaState, LUA_REGISTRYINDEX, this->luaReference);
}

void GLuaScriptNameData::get()
{
	lua_rawgeti(table->implement->luaState, LUA_REGISTRYINDEX, this->luaReference);
}

bool GLuaScriptNameData::isAvailable() const
{
	return this->luaReference != LUA_REFNIL;
}


GLuaScriptObject::GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config)
	: super(config)
{
	this->implement.reset(new GLuaScriptObjectImplement(this, service, L, super::getConfig()));
}

GLuaScriptObject::~GLuaScriptObject()
{
}

bool GLuaScriptObject::cacheName(GScriptName * name)
{
	*name = GScriptName(name->getName(), new GLuaScriptNameData(this, name->getName()));

	return name->hasData();
}

GScriptDataType GLuaScriptObject::getType(const GScriptName & name)
{
	ENTER_LUA()
	
	GLuaScopeGuard scopeGuard(this);
	
	scopeGuard.get(name);

	return getLuaType(this->implement->luaState, -1);
	
	LEAVE_LUA(this->implement->luaState, return sdtUnknown)
}

void GLuaScriptObject::bindClass(const GScriptName & name, IMetaClass * metaClass)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	doBindClass(this->implement->luaState, &this->implement->param, metaClass);
	
	scopeGuard.set(name);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindEnum(const GScriptName & name, IMetaEnum * metaEnum)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	doBindEnum(this->implement->luaState, &this->implement->param, metaEnum);
	
	scopeGuard.set(name);

	LEAVE_LUA(this->implement->luaState)
}

GScriptObject * GLuaScriptObject::createScriptObject(const GScriptName & name)
{
	ENTER_LUA()
	
	GLuaScopeGuard scopeGuard(this);

	lua_newtable(this->implement->luaState);

	scopeGuard.get(name);

	if(lua_isnil(this->implement->luaState, -1)) {
		lua_pop(this->implement->luaState, 1);
		scopeGuard.set(name);
	}

	GLuaScriptObject * binding = new GLuaScriptObject(this->implement->param.getService(), this->implement->luaState, this->implement->param.getConfig());
	binding->owner = this;
	binding->name = name.getName();
	
	return binding;

	LEAVE_LUA(this->implement->luaState, return NULL)
}

GMetaVariant GLuaScriptObject::invoke(const GScriptName & name, const GMetaVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GMetaVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GMetaVariant GLuaScriptObject::invokeIndirectly(const GScriptName & name, GMetaVariant const * const * params, size_t paramCount)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	int top = lua_gettop(this->implement->luaState);

	scopeGuard.get(name);
	
	if(lua_isfunction(this->implement->luaState, -1)) {
		for(size_t i = 0; i < paramCount; ++i) {
			if(!variantToLua(this->implement->luaState, &this->implement->param, params[i]->getValue(), params[i]->getType(), false)) {
				if(i > 0) {
					lua_pop(this->implement->luaState, static_cast<int>(i) - 1);
				}

				handleError("Can't pass parameter at index %d in function %s", i, name.getName());
			}
		}

		int error = lua_pcall(this->implement->luaState, paramCount, LUA_MULTRET, 0);
		if(error) {
			handleError("Error when calling function %s, message: %s", name.getName(), lua_tostring(this->implement->luaState, -1));
		}
		else {
			int resultCount = lua_gettop(this->implement->luaState) - top;
			if(resultCount > 1) {
				handleError("Can't return multiple value when calling function %s", name.getName());
			}
			else {
				if(resultCount > 0) {
					return luaToVariant(this->implement->luaState, &this->implement->param, -1);
				}
			}
		}
	}
	else {
		handleError("Can't call non-function.");
	}
	
	return GMetaVariant();
	
	LEAVE_LUA(this->implement->luaState, return GMetaVariant())
}

void GLuaScriptObject::setFundamental(const GScriptName & name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.data.typeData)), "Only fundamental value can be bound via setFundamental");

	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	if(! variantToLua(this->implement->luaState, &this->implement->param, value, GMetaType(), false)) {
		handleError("Failed to bind fundamental variable");
	}
	
	scopeGuard.set(name);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::setString(const GScriptName & stringName, const char * s)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	lua_pushstring(this->implement->luaState, s);
	
	scopeGuard.set(stringName);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::setObject(const GScriptName & objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	objectToLua(this->implement->luaState, &this->implement->param, instance, static_cast<IMetaClass *>(type), transferOwnership, opcvNone);
	
	scopeGuard.set(objectName);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::setMethod(const GScriptName & name, void * instance, IMetaMethod * method)
{
	ENTER_LUA()
	
	GLuaScopeGuard scopeGuard(this);
	
	doBindMethod(this->implement->luaState, &this->implement->param, instance, method);
	
	scopeGuard.set(name);
	
	LEAVE_LUA(this->implement->luaState)
}

GVariant GLuaScriptObject::getFundamental(const GScriptName & name)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	return luaToVariant(this->implement->luaState, &this->implement->param, -1).getValue();
	
	LEAVE_LUA(this->implement->luaState, return GVariant())
}

std::string GLuaScriptObject::getString(const GScriptName & stringName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(stringName);

	return lua_tostring(this->implement->luaState, -1);
	
	LEAVE_LUA(this->implement->luaState, return "")
}

void * GLuaScriptObject::getObject(const GScriptName & objectName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(objectName);

	return luaToObject(this->implement->luaState, &this->implement->param, -1, NULL);
	
	LEAVE_LUA(this->implement->luaState, return NULL)
}

IMetaClass * GLuaScriptObject::getObjectType(const GScriptName & objectName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(objectName);

	if(isValidMetaTable(this->implement->luaState, -1)) {
		void * userData = lua_touserdata(this->implement->luaState, -1);
		if(static_cast<GLuaUserData *>(userData)->getType() == udtClass) {
			GClassUserData * classData = static_cast<GClassUserData *>(userData);

			IMetaClass * metaClass = classData->metaClass;
			metaClass->addReference();
			return metaClass;
		}
	}

	return NULL;
	
	LEAVE_LUA(this->implement->luaState, return NULL)
}

IMetaClass * GLuaScriptObject::getClass(const GScriptName & className)
{
	return this->getObjectType(className);
}

IMetaMethod * GLuaScriptObject::getMethod(const GScriptName & methodName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(methodName);

	if(isValidMetaTable(this->implement->luaState, -1)) {
		void * userData = lua_touserdata(this->implement->luaState, -1);
		if(static_cast<GLuaUserData *>(userData)->getType() == udtMethod
			|| static_cast<GLuaUserData *>(userData)->getType() == udtMethodList) {
			GMethodUserData * methodData = static_cast<GMethodUserData *>(userData);

			IMetaMethod * metaMethod = methodData->method;
			metaMethod->addReference();
			return metaMethod;
		}
	}

	return NULL;
	
	LEAVE_LUA(this->implement->luaState, return NULL)
}

IMetaEnum * GLuaScriptObject::getEnum(const GScriptName & enumName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(enumName);

	if(isValidMetaTable(this->implement->luaState, -1)) {
		void * userData = lua_touserdata(this->implement->luaState, -1);
		if(static_cast<GLuaUserData *>(userData)->getType() == udtEnum) {
			GEnumUserData * enumData = static_cast<GEnumUserData *>(userData);

			IMetaEnum * metaEnum = enumData->metaEnum;
			metaEnum->addReference();
			return metaEnum;
		}
	}

	return NULL;
	
	LEAVE_LUA(this->implement->luaState, return NULL)
}

void GLuaScriptObject::assignValue(const GScriptName & fromName, const GScriptName & toName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(fromName);
	scopeGuard.set(toName);

	LEAVE_LUA(this->implement->luaState)
}

bool GLuaScriptObject::valueIsNull(const GScriptName & name)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	return ! lua_isnil(this->implement->luaState, -1);

	LEAVE_LUA(this->implement->luaState, return false)
}

void GLuaScriptObject::nullifyValue(const GScriptName & name)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);
	if(! lua_isnil(this->implement->luaState, -1)) {
		lua_pop(this->implement->luaState, 1);
		lua_pushnil(this->implement->luaState);
		scopeGuard.set(name);
	}

	LEAVE_LUA(this->implement->luaState)
}


} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif



