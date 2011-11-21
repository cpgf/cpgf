#include "cpgf/scriptbind/gluabind.h"
#include "cpgf/gflags.h"
#include "cpgf/gexception.h"
#include "cpgf/gmetaclasstraveller.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"


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
	catch(const GException & e) { error(L, e.getMessage()); __VA_ARGS__; } \
	catch(...) { error(L, "Unknown exception occurred."); __VA_ARGS__; }
	
	
namespace cpgf {


namespace {

	int UserData_gc(lua_State * L);
	int UserData_call(lua_State * L);
	int UserData_index(lua_State * L);
	int UserData_newindex(lua_State * L);
	int UserData_operator(lua_State * L);
	
	void doBindAllOperators(lua_State * L, GScriptBindingParam * param, void * instance, IMetaClass * metaClass);
	void doBindMethod(lua_State * L, GScriptBindingParam * param, void * instance, IMetaMethod * method);
	void doBindClass(lua_State * L, GScriptBindingParam * param, IMetaClass * metaClass);
	void doBindEnum(lua_State * L, GScriptBindingParam * param, IMetaEnum * metaEnum);
	void doBindMethodList(lua_State * L, GScriptBindingParam * param, IMetaList * methodList);
	
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

	struct InvokeCallableResult
	{
		int resultCount;
		GVariantData resultData;
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

		GScopedArray<char> s(new char[message.length() + strlen(fileName) + 100]);
		sprintf(s.get(), "Error, file %s, line %d: %s", fileName, ar.currentline, message.c_str());

		lua_pushstring(L, message.c_str());
		lua_error(L);
	}

	void objectToLua(lua_State * L, GScriptBindingParam * param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv)
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

			lua_pushvalue(L, -1); // duplicate the meta table
			lua_setfield(L, LUA_REGISTRYINDEX, metaTableName);
		}
		doBindAllOperators(L, param, instance, metaClass);
		
		lua_setmetatable(L, -2);
	}

	void * luaToObject(lua_State * L, GScriptBindingParam * param, int index, GMetaType * outType)
	{
		(void)param;

		if(isValidMetaTable(L, index)) {
			void * userData = lua_touserdata(L, index);
			if(static_cast<GScriptUserData *>(userData)->getType() == udtClass) {
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

	GMetaVariant luaToVariant(lua_State * L, GScriptBindingParam * param, int index)
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

	bool variantToLua(lua_State * L, GScriptBindingParam * param, const GVariant & value, const GMetaType & type, bool allowGC)
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
			GScopedInterface<IMetaTypedItem> typedItem(param->getService()->findTypedItemByName(type.getBaseName()));
			if(typedItem) {
				if(type.getPointerDimension() == 0) {
					GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");
					GASSERT_MSG(type.baseIsClass(), "Unknown type");

					IMetaClass * metaClass = static_cast<IMetaClass *>(typedItem.get());
					void * instance = metaClass->cloneInstance(objectAddressFromVariant(value));
					objectToLua(L, param, instance, static_cast<IMetaClass *>(typedItem.get()), true, metaTypeToCV(type));

					return true;
				}

				if(type.getPointerDimension() == 1) {
					GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

					objectToLua(L, param, fromVariant<void *>(value), static_cast<IMetaClass *>(typedItem.get()), allowGC, metaTypeToCV(type));

					return true;
				}
			}
		}

		return false;
	}
	
	bool converterToLua(lua_State * L, GScriptBindingParam * param, const GVariant & value, IMetaConverter * converter)
	{
		if(converter == NULL) {
			return false;
		}

		if(converter->canToCString()) {
			gapi_bool needFree;
			
			GScopedInterface<IMemoryAllocator> allocator(param->getService()->getAllocator());
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
	
	GScriptDataType getLuaType(lua_State * L, int index, IMetaTypedItem ** typeItem)
	{
		if(typeItem != NULL) {
			*typeItem = NULL;
		}
		
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
					GScriptUserData * userData = static_cast<GScriptUserData *>(rawUserData);
					switch(userData->getType()) {
					case udtClass:
						if(typeItem != NULL) {
							*typeItem = static_cast<GClassUserData *>(userData)->metaClass;
							(*typeItem)->addReference();
						}

						if(static_cast<GClassUserData *>(userData)->isInstance) {
							return sdtObject;
						}
						else {
							return sdtClass;
						}

					case udtMethod:
						return sdtMethod;
						
					case udtMethodList:
						return sdtMethodList;

					case udtEnum:
						if(typeItem != NULL) {
							*typeItem = static_cast<GEnumUserData *>(userData)->metaEnum;
							(*typeItem)->addReference();
						}
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

	void loadMethodParameters(lua_State * L, GScriptBindingParam * param, GVariantData * outputParams, int startIndex, int paramCount)
	{
		for(int i = 0; i < paramCount; ++i) {
			outputParams[i] = luaToVariant(L, param, i + startIndex).getData().varData;
		}
	}

	void loadMethodParamTypes(lua_State * L, GBindDataType * outputTypes, int startIndex, int paramCount)
	{
		for(int i = 0; i < paramCount; ++i) {
			IMetaTypedItem * typeItem;
			outputTypes[i].dataType = getLuaType(L, i + startIndex, &typeItem);
			outputTypes[i].typeItem.reset(typeItem);
		}
	}

	void doInvokeCallable(void * instance, IMetaCallable * callable, GVariantData * paramsData, int paramCount, InvokeCallableResult * result)
	{
		result->resultCount = 0;
		vtInit(result->resultData.typeData);

		callable->execute(&result->resultData, instance, paramsData, paramCount);
	}
	
	bool doPushInvokeResult(lua_State * L, GScriptBindingParam * param, IMetaCallable * callable, InvokeCallableResult * result)
	{
		if(callable->hasResult()) {
			GMetaTypeData typeData;
		
			callable->getResultType(&typeData);

			GVariant value = GVariant(result->resultData);
			bool success = variantToLua(L, param, value, GMetaType(typeData), !! callable->isResultTransferOwnership());
			if(!success) {
				GScopedInterface<IMetaConverter> converter(callable->createResultConverter());
				success = converterToLua(L, param, value, converter.get());
			}
			if(!success) {
				raiseException(Error_ScriptBinding_FailVariantToScript, "Can't convert variant to Lua object.");
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

		GVariantData paramsData[REF_MAX_ARITY];
		loadMethodParameters(L, userData->getParam(), paramsData, 1, lua_gettop(L));
		if(checkCallable(method, paramsData, lua_gettop(L))) {
			doInvokeCallable(userData->instance, method, paramsData, lua_gettop(L), &result);
			doPushInvokeResult(L, userData->getParam(), method, &result);
		}
		else {
			raiseFormatException(Error_ScriptBinding_CantFindMatchedMethod, "Can't invoke meta method %s", method->getName());
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

		GVariantData paramsData[REF_MAX_ARITY];
		loadMethodParameters(L, userData->getParam(), paramsData, 1, lua_gettop(L));
		
		GBindDataType paramsType[REF_MAX_ARITY];
		loadMethodParamTypes(L, paramsType, 1, lua_gettop(L));
		
		for(size_t i = 0; i < methodCount; ++i) {
			GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(methodList->getAt(static_cast<uint32_t>(i))));

			methodName = method->getName();
		
			int rank = rankCallable(userData->getParam()->getService(), method.get(), paramsData, paramsType, lua_gettop(L));
			if(rank > maxRank) {
				maxRank = rank;
				maxRankIndex = i;
			}
		}
		
		if(maxRank >= 0) {
			GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(methodList->getAt(static_cast<uint32_t>(maxRankIndex))));
			doInvokeCallable(methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex)), method.get(), paramsData, lua_gettop(L), &result);
			doPushInvokeResult(L, userData->getParam(), method.get(), &result);
			return result.resultCount;
		}

		if(methodName == NULL) {
			raiseException(Error_ScriptBinding_InternalError_CantFindMethodListName, "Internal error: can't find method list name.");
		}
		else {
			raiseFormatException(Error_ScriptBinding_CantFindMatchedMethod, "Can't find matched method to invoke for %s", methodName);
		}

		return 0;		

		LEAVE_LUA(L, return 0)
	}

	int invokeConstructor(lua_State * L, GScriptBindingParam * param, IMetaClass * metaClass)
	{
		int paramCount = lua_gettop(L) - 1;
		void * instance = NULL;

		if(paramCount == 0 && metaClass->canCreateInstance()) {
			instance = metaClass->createInstance();
		}
		else {
			InvokeCallableResult result;
			int count = metaClass->getConstructorCount();
			
			int maxRank = -1;
			size_t maxRankIndex = 0;
		
			GVariantData paramsData[REF_MAX_ARITY];
			loadMethodParameters(L, param, paramsData, 2, paramCount);

			GBindDataType paramsType[REF_MAX_ARITY];
			loadMethodParamTypes(L, paramsType, 2, paramCount);
		
			for(int i = 0; i < count; ++i) {
				GScopedInterface<IMetaConstructor> constructor(metaClass->getConstructorAt(i));
				int rank = rankCallable(param->getService(), constructor.get(), paramsData, paramsType, paramCount);
				if(rank > maxRank) {
					maxRank = rank;
					maxRankIndex = i;
				}
			}
			if(maxRank >= 0) {
				GScopedInterface<IMetaConstructor> constructor(metaClass->getConstructorAt(static_cast<uint32_t>(maxRankIndex)));
				doInvokeCallable(NULL, constructor.get(), paramsData, paramCount, &result);
				instance = fromVariant<void *>(GVariant(result.resultData));
			}
		}

		if(instance != NULL) {
			objectToLua(L, param, instance, metaClass, true, opcvNone);
		}
		else {
			raiseException(Error_ScriptBinding_FailConstructObject, "Failed to construct an object.");
		}

		return 1;
	}

	int invokeOperator(lua_State * L, GScriptBindingParam * param, void * instance, IMetaClass * metaClass, GMetaOpType op)
	{
		int paramCount = lua_gettop(L);
		int startIndex = 1;

		InvokeCallableResult result;
		size_t count = metaClass->getOperatorCount();

		if(op == mopFunctor) { // skip the first "func" parameter
			++startIndex;
			--paramCount;
		}
		
		if(op == mopNeg) {
			paramCount = 1; // Lua pass two parameters to __unm...
		}
			
		int maxRank = -1;
		uint32_t maxRankIndex = 0;
		
		GVariantData paramsData[REF_MAX_ARITY];
		loadMethodParameters(L, param, paramsData, startIndex, paramCount);
		
		GBindDataType paramsType[REF_MAX_ARITY];
		loadMethodParamTypes(L, paramsType, startIndex, paramCount);

		for(uint32_t i = 0; i < count; ++i) {
			GScopedInterface<IMetaOperator> metaOperator(metaClass->getOperatorAt(i));
			if(op == metaOperator->getOperator()) {
				int rank = rankCallable(param->getService(), metaOperator.get(), paramsData, paramsType, paramCount);
				if(rank > maxRank) {
					maxRank = rank;
					maxRankIndex = i;
				}
			}
		}
		if(maxRank >= 0) {
			GScopedInterface<IMetaOperator> metaOperator(metaClass->getOperatorAt(maxRankIndex));
			doInvokeCallable(instance, metaOperator.get(), paramsData, paramCount, &result);
			doPushInvokeResult(L, param, metaOperator.get(), &result);
			return result.resultCount;
		}

		return 0;
	}

	int UserData_gc(lua_State * L)
	{
		ENTER_LUA()

		GScriptUserData * userData = static_cast<GScriptUserData *>(lua_touserdata(L, -1));
		userData->~GScriptUserData();
		
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
			raiseException(Error_ScriptBinding_InternalError_WrongFunctor, "Internal error: calling wrong functor.");

			return 0;
		}
		
		LEAVE_LUA(L, return 0)
	}
	
	bool indexMemberData(lua_State * L, GClassUserData * userData, IMetaAccessible * data, void * instance)
	{
		GVariantData varData;
		GMetaTypeData typeData;
		data->get(&varData, instance);
		data->getItemType(&typeData);
		
		GVariant value = GVariant(varData);
		bool success = variantToLua(L, userData->getParam(), value, GMetaType(typeData), false);
		if(!success) {
			GScopedInterface<IMetaConverter> converter(data->createConverter());
			success = converterToLua(L, userData->getParam(), value, converter.get());
		}

		return success;
	}
	
	bool indexMemberEnumType(lua_State * L, GClassUserData * userData, GMetaMapItem * mapItem)
	{
		GScopedInterface<IMetaEnum> metaEnum(static_cast<IMetaEnum *>(mapItem->getItem()));

		doBindEnum(L, userData->getParam(), metaEnum.get());

		return true;
	}

	bool indexMemberEnumValue(lua_State * L, GClassUserData * userData, GMetaMapItem * mapItem)
	{
		(void)userData;

		GScopedInterface<IMetaEnum> metaEnum(static_cast<IMetaEnum *>(mapItem->getItem()));

		GVariantData data;
		metaEnum->getValue(&data, static_cast<uint32_t>(mapItem->getEnumIndex()));
		lua_pushinteger(L, fromVariant<lua_Integer>(GVariant(data)));

		return true;
	}

	bool indexMemberClass(lua_State * L, GClassUserData * userData, GMetaMapItem * mapItem)
	{
		GScopedInterface<IMetaClass> metaClass(static_cast<IMetaClass *>(mapItem->getItem()));

		doBindClass(L, userData->getParam(), metaClass.get());

		return true;
	}

	bool indexMemberMethod(lua_State * L, GClassUserData * userData, GMetaMapItem * mapItem, void * instance)
	{
		GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(mapItem->getItem()));
		
		doBindMethod(L, userData->getParam(), instance, method.get());
		
		return true;
	}
	
	void loadMethodList(GMetaClassTraveller * traveller, IMetaList * metaList, GMetaMap * metaMap, GMetaMapItem * mapItem, void * instance, GClassUserData * userData, const char * methodName)
	{
		while(mapItem != NULL) {
			if(mapItem->getType() == mmitMethod) {
				GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(mapItem->getItem()));
				if(allowInvokeMethod(userData, method.get())) {
					metaList->add(method.get(), instance);
				}
			}
			else {
				GScopedInterface<IMetaList> methodList(static_cast<IMetaList *>(mapItem->getItem()));
				for(uint32_t i = 0; i < methodList->getCount(); ++i) {
					GScopedInterface<IMetaItem> item(methodList->getAt(i));
					if(allowInvokeMethod(userData, static_cast<IMetaMethod *>(item.get()))) {
						metaList->add(item.get(), instance);
					}
				}
			}
			
			GScopedInterface<IMetaClass> metaClass(traveller->next(&instance));
			if(!metaClass) {
				break;
			}
			mapItem = findMetaMapItem(metaMap, metaClass.get(), methodName);
		}
	}
	
	int UserData_index(lua_State * L)
	{
		ENTER_LUA()

		GClassUserData * userData = static_cast<GClassUserData *>(lua_touserdata(L, -2));
		
		const char * name = lua_tostring(L, -1);
		
		GMetaClassTraveller traveller(userData->metaClass, userData->instance);
		
		void * instance = NULL;
		
		for(;;) {
			GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
			if(!metaClass) {
				return false;
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
						return indexMemberData(L, userData, data.get(), instance);
					}
				}
				   break;

				case mmitMethod:
				case mmitMethodList: {
					GScopedInterface<IMetaList> metaList(createMetaList());
					loadMethodList(&traveller, metaList.get(), userData->getParam()->getMetaMap(), mapItem, instance, userData, name);
					if(metaList->getCount() == 1) {
						GScopedInterface<IMetaMethod> method(static_cast<IMetaMethod *>(metaList->getAt(0)));
						doBindMethod(L, userData->getParam(), metaList->getInstanceAt(0), method.get());
					}
					else {
						doBindMethodList(L, userData->getParam(), metaList.get());
					}
					return true;
				}

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

				default:
					break;
			}
		}

		LEAVE_LUA(L, return false)
	}

	bool newindexMemberData(lua_State * L, GClassUserData * userData, const char * name, const GVariant & value)
	{
		(void)L;

		if(userData->cv == opcvConst) {
			raiseException(Error_ScriptBinding_CantWriteToConstObject, "Can't write to constant object.");

			return false;
		}

		GMetaClassTraveller traveller(userData->metaClass, userData->instance);
		
		void * instance = NULL;

		for(;;) {
			GScopedInterface<IMetaClass> metaClass(traveller.next(&instance));
			if(!metaClass) {
				return false;
			}
			
			GMetaMapItem * mapItem = findMetaMapItem(userData->getParam()->getMetaMap(), metaClass.get(), name);
			if(mapItem == NULL) {
				continue;
			}
			
			if(!metaMapItemIsAccessible(mapItem->getType())) {
				continue;
			}
			
			GScopedInterface<IMetaAccessible> data(static_cast<IMetaAccessible *>(mapItem->getItem()));

			if(allowAccessData(userData, data.get())) {
				GVariantData varData = value.getData();
				data->set(instance, &varData);

				return true;
			}
		}

	}

	int UserData_newindex(lua_State * L)
	{
		ENTER_LUA()

		GClassUserData * userData = static_cast<GClassUserData *>(lua_touserdata(L, -3));
		
		const char * name = lua_tostring(L, -2);

		GVariant value = luaToVariant(L, userData->getParam(), -1).getValue();
		
		if(newindexMemberData(L, userData, name, value)) {
			return 1;
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

	void doBindOperator(lua_State * L, GScriptBindingParam * param, void * instance, IMetaClass * metaClass, GMetaOpType op)
	{
		for(size_t i = 0; i < sizeof(metaOpTypes) / sizeof(metaOpTypes[0]); ++i) {
			if(metaOpTypes[i] == op) {
				lua_pushstring(L, luaOperators[i]);
				void * userData = lua_newuserdata(L, sizeof(GOperatorUserData));
				new (userData) GOperatorUserData(param, instance, metaClass, op);
				
				lua_newtable(L);
				
				setMetaTableSignature(L);
				setMetaTableGC(L);
				lua_setmetatable(L, -2);

				lua_pushcclosure(L, &UserData_operator, 1);
				lua_rawset(L, -3);

				return ;
			}
		}

		raiseException(Error_ScriptBinding_NotSupportedOperator, "Failed to bind an operator that's not supported by Lua.");
	}

	void doBindAllOperators(lua_State * L, GScriptBindingParam * param, void * instance, IMetaClass * metaClass)
	{
		std::vector<uint32_t> boundOperators;

		int count = metaClass->getOperatorCount();
		for(int i = 0; i < count; ++i) {
			GScopedInterface<IMetaOperator> item(metaClass->getOperatorAt(i));
			uint32_t op = item->getOperator();
			if(std::find(boundOperators.begin(), boundOperators.end(), op) == boundOperators.end()) {
				doBindOperator(L, param, instance, metaClass, static_cast<GMetaOpType>(op));
			}
		}
	}

	void doBindMethod(lua_State * L, GScriptBindingParam * param, void * instance, IMetaMethod * method)
	{
		void * userData = lua_newuserdata(L, sizeof(GMethodUserData));
		new (userData) GMethodUserData(param, instance, method);

		lua_newtable(L);
		
		setMetaTableSignature(L);
		setMetaTableGC(L);
		lua_setmetatable(L, -2);

		lua_pushcclosure(L, &callbackInvokeMethod, 1);
	}
	
	void doBindClass(lua_State * L, GScriptBindingParam * param, IMetaClass * metaClass)
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

	void doBindMethodList(lua_State * L, GScriptBindingParam * param, IMetaList * methodList)
	{
		void * userData = lua_newuserdata(L, sizeof(GMethodListUserData));
		new (userData) GMethodListUserData(param, methodList);
		
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
			raiseFormatException(Error_ScriptBinding_CantFindEnumKey, "Can't find enumerator key -- %s.", name);
		}
		else {
			GVariantData data;
			userData->metaEnum->getValue(&data, index);
			lua_pushinteger(L, fromVariant<lua_Integer>(GVariant(data)));
		}
		
		return true;
		
		LEAVE_LUA(L, return false)
	}

	int Enum_newindex(lua_State * L)
	{
		ENTER_LUA()

		raiseException(Error_ScriptBinding_CantAssignToEnum, "Can't assign value to enumerator.");

		return 0;
		
		LEAVE_LUA(L, return 0)
	}

	void doBindEnum(lua_State * L, GScriptBindingParam * param, IMetaEnum * metaEnum)
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
	GLuaScriptObjectImplement(GLuaScriptObject * binding, IMetaService * service, lua_State * L, const GScriptConfig & config, GMetaMap * metaMap, bool freeMap)
		: binding(binding), param(service, config, metaMap), luaState(L), freeMap(freeMap)
	{
	}
	
	~GLuaScriptObjectImplement() {
		if(this->freeMap) {
			delete this->param.getMetaMap();
		}
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
	GScriptBindingParam param;
	lua_State * luaState;
	bool freeMap;
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
	this->implement.reset(new GLuaScriptObjectImplement(this, service, L, super::getConfig(), new GMetaMap, true));
}

GLuaScriptObject::GLuaScriptObject(const GLuaScriptObject & other)
	: super(other.implement->param.getConfig())
{
	this->implement.reset(new GLuaScriptObjectImplement(this, other.implement->param.getService(), other.implement->luaState, super::getConfig(), other.implement->param.getMetaMap(), false));
}

GLuaScriptObject::~GLuaScriptObject()
{
}

bool GLuaScriptObject::cacheName(GScriptName * name)
{
	GLuaScriptNameData * data = new GLuaScriptNameData(this, name->getName());
	*name = GScriptName(name->getName(), data);
	data->release();

	return name->hasData();
}

GScriptDataType GLuaScriptObject::getType(const GScriptName & name, IMetaTypedItem ** outMetaTypeItem)
{
	ENTER_LUA()
	
	GLuaScopeGuard scopeGuard(this);
	
	scopeGuard.get(name);

	return getLuaType(this->implement->luaState, -1, outMetaTypeItem);
	
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

	GLuaScriptObject * binding = new GLuaScriptObject(*this);
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

				raiseFormatException(Error_ScriptBinding_ScriptMethodParamMismatch, "Can't pass parameter at index %d in function %s", i, name.getName());
			}
		}

		int error = lua_pcall(this->implement->luaState, static_cast<int>(paramCount), LUA_MULTRET, 0);
		if(error) {
			raiseFormatException(Error_ScriptBinding_ScriptFunctionReturnError, "Error when calling function %s, message: %s", name.getName(), lua_tostring(this->implement->luaState, -1));
		}
		else {
			int resultCount = lua_gettop(this->implement->luaState) - top;
			if(resultCount > 1) {
				raiseFormatException(Error_ScriptBinding_CantReturnMultipleValue, "Can't return multiple value when calling function %s", name.getName());
			}
			else {
				if(resultCount > 0) {
					return luaToVariant(this->implement->luaState, &this->implement->param, -1);
				}
			}
		}
	}
	else {
		raiseException(Error_ScriptBinding_CantCallNonfunction, "The script function being invoked is not a function.");
	}
	
	return GMetaVariant();
	
	LEAVE_LUA(this->implement->luaState, return GMetaVariant())
}

void GLuaScriptObject::bindFundamental(const GScriptName & name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.data.typeData)), "Only fundamental value can be bound via bindFundamental");

	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	if(! variantToLua(this->implement->luaState, &this->implement->param, value, GMetaType(), false)) {
		raiseException(Error_ScriptBinding_CantBindFundamental, "Failed to bind fundamental variable");
	}
	
	scopeGuard.set(name);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindString(const GScriptName & stringName, const char * s)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	lua_pushstring(this->implement->luaState, s);

	scopeGuard.set(stringName);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindObject(const GScriptName & objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	objectToLua(this->implement->luaState, &this->implement->param, instance, static_cast<IMetaClass *>(type), transferOwnership, opcvNone);

	scopeGuard.set(objectName);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindMethod(const GScriptName & name, void * instance, IMetaMethod * method)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);
	
	doBindMethod(this->implement->luaState, &this->implement->param, instance, method);
	
	scopeGuard.set(name);
	
	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindMethodList(const GScriptName & name, IMetaList * methodList)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);
	
	doBindMethodList(this->implement->luaState, &this->implement->param, methodList);
	
	scopeGuard.set(name);
	
	LEAVE_LUA(this->implement->luaState)
}

IMetaClass * GLuaScriptObject::getClass(const GScriptName & className)
{
	IMetaTypedItem * typedItem = NULL;

	GScriptDataType sdt = this->getType(className, &typedItem);
	GScopedInterface<IMetaTypedItem> item(typedItem);
	if(sdt == sdtClass) {
		return static_cast<IMetaClass *>(item.take());
	}

	return NULL;
}

IMetaEnum * GLuaScriptObject::getEnum(const GScriptName & enumName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(enumName);

	if(isValidMetaTable(this->implement->luaState, -1)) {
		void * userData = lua_touserdata(this->implement->luaState, -1);
		if(static_cast<GScriptUserData *>(userData)->getType() == udtEnum) {
			GEnumUserData * enumData = static_cast<GEnumUserData *>(userData);

			IMetaEnum * metaEnum = enumData->metaEnum;
			metaEnum->addReference();
			return metaEnum;
		}
	}

	return NULL;
	
	LEAVE_LUA(this->implement->luaState, return NULL)
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

IMetaMethod * GLuaScriptObject::getMethod(const GScriptName & methodName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(methodName);

	if(isValidMetaTable(this->implement->luaState, -1)) {
		void * userData = lua_touserdata(this->implement->luaState, -1);
		if(static_cast<GScriptUserData *>(userData)->getType() == udtMethod) {
			GMethodUserData * methodData = static_cast<GMethodUserData *>(userData);

			IMetaMethod * metaMethod = methodData->method;
			metaMethod->addReference();
			return metaMethod;
		}
	}

	return NULL;
	
	LEAVE_LUA(this->implement->luaState, return NULL)
}

IMetaList * GLuaScriptObject::getMethodList(const GScriptName & methodName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(methodName);

	if(isValidMetaTable(this->implement->luaState, -1)) {
		void * userData = lua_touserdata(this->implement->luaState, -1);
		if(static_cast<GScriptUserData *>(userData)->getType() == udtMethodList) {
			GMethodListUserData * methodListData = static_cast<GMethodListUserData *>(userData);

			IMetaList * methodList = methodListData->methodList;
			methodList->addReference();
			return methodList;
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



IScriptObject * createLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config)
{
	return new ImplScriptObject(new GLuaScriptObject(service, L, config));
}



} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif


