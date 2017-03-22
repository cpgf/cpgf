#include "cpgf/scriptbind/gluabind.h"
#include "cpgf/gflags.h"
#include "cpgf/gexception.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gcallback.h"
#include "cpgf/gerrorcode.h"

#include "../pinclude/gbindcommon.h"
#include "../pinclude/gscriptbindapiimpl.h"


#include <stdexcept>
#include <algorithm>
#include <vector>
#include <set>

#include <string.h>


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4996)
#endif


#if LUA_VERSION_NUM >= 502
	#define HAS_LUA_GLOBALSINDEX 0
#else
	#define HAS_LUA_GLOBALSINDEX 1
#endif


using namespace std;
using namespace cpgf::bind_internal;

#define ENTER_LUA() \
	char local_msg[256]; bool local_error = false; { \
	try {

#define LEAVE_LUA(L, ...) \
	} \
	catch(const GException & e) { strncpy(local_msg, e.getMessage(), 256); local_error = true; } \
	catch(const exception & e) { strncpy(local_msg, e.what(), 256); local_error = true; } \
	catch(...) { strcpy(local_msg, "Unknown exception occurred."); local_error = true; } \
	} if(local_error) { local_msg[255] = 0; error(L, local_msg); } \
	__VA_ARGS__;
	

namespace cpgf {


namespace {

class GLuaScriptObject;
class GLuaGlobalAccessor;

class GLuaScriptFunction : public GScriptFunctionBase
{
private:
	typedef GScriptFunctionBase super;

public:
	GLuaScriptFunction(const GContextPointer & context, int objectIndex);
	virtual ~GLuaScriptFunction();
	
	virtual GScriptValue invoke(const GVariant * params, size_t paramCount);
	virtual GScriptValue invokeIndirectly(GVariant const * const * params, size_t paramCount);

private:
	int ref;
};

class GLuaScriptArray : public GScriptArrayBase
{
private:
	typedef GScriptArrayBase super;

public:
	GLuaScriptArray(GLuaScriptObject * scriptObject, int objectIndex);
	virtual ~GLuaScriptArray();

	virtual size_t getLength();
	virtual GScriptValue getValue(size_t index);
	virtual void setValue(size_t index, const GScriptValue & value);

	virtual bool maybeIsScriptArray(size_t index);
	virtual GScriptValue getAsScriptArray(size_t index);
	virtual GScriptValue createScriptArray(size_t index);
	
private:
	int ref;
	GLuaScriptObject * scriptObject;
};

class GLuaScriptObject : public GScriptObjectBase
{
private:
	typedef GScriptObjectBase super;

public:
	GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config);
	GLuaScriptObject(const GLuaScriptObject & other);
	GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config, int objectIndex);
	virtual ~GLuaScriptObject();

	virtual bool isGlobal() const;
	
	virtual GScriptObject * doCreateScriptObject(const char * name);
	
	virtual GScriptValue getScriptFunction(const char * name);
	
	virtual GScriptValue invoke(const char * name, const GVariant * params, size_t paramCount);
	virtual GScriptValue invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount);

	virtual void assignValue(const char * fromName, const char * toName);

	virtual bool maybeIsScriptArray(const char * name);
	virtual GScriptValue getAsScriptArray(const char * name);
	virtual GScriptValue createScriptArray(const char * name);

public:
	lua_State * getLuaState() const {
		return this->luaState;
	}
	
protected:
	virtual GScriptValue doGetValue(const char * name);
	virtual void doSetValue(const char * name, const GScriptValue & value);

private:
	void getTable() const;
	GMethodGlueDataPointer doGetMethodUserData();
	GLuaGlobalAccessor * getGlobalAccessor();


private:
	lua_State * luaState;
	int ref;
	GScopedPointer<GLuaGlobalAccessor> globalAccessor;

private:
	friend class GLuaScopeGuard;
	friend class GLuaScriptObjectImplement;
	friend class GLuaScriptNameData;
	friend class GLuaGlobalAccessor;
	friend class GLuaScriptArray;
};


class GLuaBindingContext : public GBindingContext, public GShareFromBase
{
private:
	typedef GBindingContext super;

public:
	GLuaBindingContext(IMetaService * service, const GScriptConfig & config, lua_State * luaState)
		: super(service, config), luaState(luaState)
	{
	}

	lua_State * getLuaState() const {
		return this->luaState;
	}

private:
	lua_State * luaState;
};


class GLuaScopeGuard
{
public:
	GLuaScopeGuard(GScriptObject * scope);
	~GLuaScopeGuard();
	
	void keepStack();

	void set(const char * name);
	void get(const char * name);
	void rawGet(const char * name);

private:
	GLuaScriptObject * scope;
	int top;
};


class GLuaGlobalAccessorItem
{
public:
	GLuaGlobalAccessorItem(void * instance, IMetaAccessible * accessible) : instance(instance), accessible(accessible) {
	}
	
public:
	void * instance;
	GSharedInterface<IMetaAccessible> accessible;
};

class GLuaGlobalAccessor
{
private:
	typedef map<std::string, GLuaGlobalAccessorItem> MapType;

public:
	explicit GLuaGlobalAccessor(GLuaScriptObject * scriptObject);
	void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible);

	int doIndex();
	int doNewIndex();

private:
	void initialize();

	int doPreviousIndex(const char * name);
	int doPreviousNewIndex(const char * name);

private:
	MapType itemMap;
	GLuaScriptObject * scriptObject;
};

int UserData_gc(lua_State * L);
int UserData_call(lua_State * L);
int UserData_index(lua_State * L);
int UserData_newindex(lua_State * L);
int UserData_operator(lua_State * L);

void helperBindAllOperators(const GContextPointer & context, const GObjectGlueDataPointer & objectData,
	IMetaClass * metaClass, bool bindingToObject);
void helperBindClass(const GContextPointer & context, IMetaClass * metaClass);
void helperBindEnum(const GContextPointer & context, IMetaEnum * metaEnum);
void helperBindMethodList(const GContextPointer & context, const GObjectGlueDataPointer & objectData, const GMethodGlueDataPointer & methodData);
void helperBindMethodList(const GContextPointer & context, IMetaList * methodList);

typedef GCallback<GLuaGlobalAccessor * ()> GlobalAccessorGetter;
bool helperBindValue(const GContextPointer & context, const char * name,
	const GScriptValue & value, const GlobalAccessorGetter & globalAccessorGetter);

void initObjectMetaTable(lua_State * L);
void setMetaTableGC(lua_State * L);
void setMetaTableCall(lua_State * L, void * userData);
void setMetaTableSignature(lua_State * L);
bool isValidMetaTable(lua_State * L, int index);

bool variantToLua(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData);
GScriptValue luaToScriptValue(const GContextPointer & context, int index, GGlueDataPointer * outputGlueData);

bool doIndexMemberData(const GContextPointer & context, IMetaAccessible * data, void * instance, bool instanceIsConst);

void error(lua_State * L, const char * message);

const int RefTable = LUA_REGISTRYINDEX;

int refLua(lua_State * L, int objectIndex)
{
	lua_pushvalue(L, objectIndex);
	return luaL_ref(L, RefTable);
}

void unrefLua(lua_State * L, int ref)
{
	if(ref == LUA_NOREF || L == nullptr) {
		return;
	}

	luaL_unref(L, RefTable, ref);
}

void getRefObject(lua_State * L, int ref)
{
	if(ref == LUA_NOREF) {
		return;
	}

	lua_rawgeti(L, RefTable, ref);
}

lua_State * getLuaState(const GContextPointer & context)
{
	if(! context) {
		return nullptr;
	}
	else {
		return gdynamic_cast<GLuaBindingContext *>(context.get())->getLuaState();
	}
}


int GlobalAccessor_index(lua_State * L)
{
	ENTER_LUA()

	GLuaGlobalAccessor * accessor = static_cast<GLuaGlobalAccessor *>(lua_touserdata(L, lua_upvalueindex(1)));

	return accessor->doIndex();

	LEAVE_LUA(L, lua_pushnil(L); return 0)
}

int GlobalAccessor_newindex(lua_State * L)
{
	ENTER_LUA()

	GLuaGlobalAccessor * accessor = static_cast<GLuaGlobalAccessor *>(lua_touserdata(L, lua_upvalueindex(1)));
	
	return accessor->doNewIndex();
	
	LEAVE_LUA(L, return 0)
}

GLuaGlobalAccessor::GLuaGlobalAccessor(GLuaScriptObject * scriptObject)
	: scriptObject(scriptObject)
{
	this->initialize();
}

void GLuaGlobalAccessor::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	string sname(name);

	this->itemMap.insert(make_pair(sname, GLuaGlobalAccessorItem(instance, accessible)));
}

int GLuaGlobalAccessor::doIndex()
{
	lua_State * L = this->scriptObject->getLuaState();

	const char * name = lua_tostring(L, -1);

	string sname(name);
	MapType::iterator it = this->itemMap.find(sname);
	if(it != this->itemMap.end()) {
		doIndexMemberData(this->scriptObject->getBindingContext(), it->second.accessible.get(), it->second.instance, false);
		return 1;
	}

	return this->doPreviousIndex(name);
}

int GLuaGlobalAccessor::doNewIndex()
{
	lua_State * L = this->scriptObject->getLuaState();

	const char * name = lua_tostring(L, -2);
	
	string sname(name);
	MapType::iterator it = this->itemMap.find(sname);
	if(it != this->itemMap.end()) {
		GVariant value = luaToScriptValue(this->scriptObject->getBindingContext(), -1, nullptr).getValue();
		GVariantData varData = value.refData();
		it->second.accessible->set(it->second.instance, &varData);
		metaCheckError(it->second.accessible.get());
		return 1;
	}

	return this->doPreviousNewIndex(name);
}

int GLuaGlobalAccessor::doPreviousIndex(const char * name)
{
	lua_State * L = this->scriptObject->getLuaState();
	
	this->scriptObject->getTable();

	if(lua_isfunction(L, -1)) {
		lua_pushstring(L, name);
		lua_call(L, 1, 1);
		return 1;
	}
	if(lua_istable(L, -1)) {
		lua_pushstring(L, name);
		lua_rawget(L, -2);

		return 1;
	}

	return 0;
}

int GLuaGlobalAccessor::doPreviousNewIndex(const char * name)
{
	lua_State * L = this->scriptObject->getLuaState();
	
	this->scriptObject->getTable();

	if(lua_isfunction(L, -1)) {
		lua_pushstring(L, name);
		lua_pushvalue(L, -3);
		lua_call(L, 2, 0);
		return 1;
	}
	if(lua_istable(L, -1)) {
		lua_pushstring(L, name);
		lua_pushvalue(L, -3);
		lua_rawset(L, -3);

		return 1;
	}

	return 0;
}

void GLuaGlobalAccessor::initialize()
{
	lua_State * L = this->scriptObject->getLuaState();

	if(! this->scriptObject->isGlobal()) {
		this->scriptObject->getTable();
	}

	lua_newtable(L);
	
	lua_pushstring(L, "__index");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &GlobalAccessor_index, 1);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushlightuserdata(L, this);
	lua_pushcclosure(L, &GlobalAccessor_newindex, 1);
	lua_rawset(L, -3);

	if(this->scriptObject->isGlobal()) {
#if HAS_LUA_GLOBALSINDEX
		lua_setmetatable(L, LUA_GLOBALSINDEX);
#else
		lua_pushglobaltable(L);
		lua_pushvalue(L, -2);
		lua_setmetatable(L, -2);
#endif
	}
	else {
		lua_setmetatable(L, -2);
	}
}


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


void error(lua_State * L, const char * message)
{
	lua_Debug ar;
	lua_getstack(L, 1, &ar);
	lua_getinfo(L, "nSl", &ar);

	const char * fileName = ar.source;
	char dummy = 0;
	if(fileName == nullptr || *fileName != '@') {
		fileName = &dummy;
	}

	char s[1024];
	sprintf(s, "Error: file %.256s, line %d: %.700s", fileName, ar.currentline, message);

	lua_pushstring(L, s);
	lua_error(L);
}

void objectToLua(const GContextPointer & context, const GClassGlueDataPointer & classData,
				 const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
{
	lua_State * L = getLuaState(context);

	if(objectAddressFromVariant(instance) == nullptr) {
		lua_pushnil(L);

		return;
	}

	void * userData = lua_newuserdata(L, getGlueDataWrapperSize<GObjectGlueData>());
	GObjectGlueDataPointer objectData(context->newOrReuseObjectGlueData(classData, instance, flags, cv));
	newGlueDataWrapper(userData, objectData);

	if(outputGlueData != nullptr) {
		*outputGlueData = objectData;
	}

	IMetaClass * metaClass = classData->getMetaClass();

	const char * className = metaClass->getName();
	
	GASSERT_MSG(strlen(className) < 1000, "Meta class name is too long");

	char metaTableName[1100];

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
	
		helperBindAllOperators(context, objectData, metaClass, false);
	}
	helperBindAllOperators(context, objectData, metaClass, true);
	
	lua_setmetatable(L, -2);
}

GScriptValue luaUserDataToScriptValue(const GContextPointer & context, int index, GGlueDataPointer * outputGlueData)
{
	lua_State * L = getLuaState(context);

	if(isValidMetaTable(L, index)) {
		void * userData = lua_touserdata(L, index);
		GGlueDataPointer glueData = static_cast<GGlueDataWrapper *>(userData)->getData();
		if(outputGlueData != nullptr) {
			*outputGlueData = glueData;
		}
		return glueDataToScriptValue(glueData);
	}

	return GScriptValue();
}

GScriptValue luaToScriptValue(const GContextPointer & context, int index, GGlueDataPointer * outputGlueData)
{
	lua_State * L = getLuaState(context);

	int type = lua_type(L, index);

	switch(type) {
		case LUA_TNIL:
			return GScriptValue::fromNull();

		case LUA_TNUMBER:
			return GScriptValue::fromFundamental(lua_tonumber(L, index));

		case LUA_TBOOLEAN:
			return GScriptValue::fromFundamental(bool(lua_toboolean(L, index) != 0));

		case LUA_TSTRING:
			return GScriptValue::fromAndCopyString(lua_tostring(L, index));

		case LUA_TUSERDATA:
			return luaUserDataToScriptValue(context, index, outputGlueData);

		case LUA_TFUNCTION: {
			const bool ok = (lua_getupvalue(L, index, 1) != NULL);
			if(ok && ! lua_isnil(L, -1)) {
				void * rawUserData = lua_touserdata(L, -1);
				GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(rawUserData);

				if(dataWrapper != nullptr) {
					GScriptValue scriptValue = glueDataToScriptValue(dataWrapper->getData());
					if(! scriptValue.isNull()) {
						return scriptValue;
					}
				}
			}

			if(ok) {
				// Remove the up value from stack
				lua_pop(L, 1);
			}

			GScopedInterface<IScriptFunction> func(new ImplScriptFunction(new GLuaScriptFunction(context, index), true));
			return GScriptValue::fromScriptFunction(func.get());
		}
		break;

		case LUA_TLIGHTUSERDATA:
			break;

		case LUA_TTABLE: {
			GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GLuaScriptObject(context->getService(), L, context->getConfig(), index), true));
			return GScriptValue::fromScriptObject(scriptObject.get());
		}
			
		case LUA_TTHREAD:
			break;
	}

	return GScriptValue();
}

bool rawToLua(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
{
	if(context->getConfig().allowAccessRawData()) {
		lua_State * L = getLuaState(context);

		void * userData = lua_newuserdata(L, getGlueDataWrapperSize<GRawGlueData>());
		GRawGlueDataPointer rawData(context->newRawGlueData(value));
		newGlueDataWrapper(userData, rawData);

		if(outputGlueData != nullptr) {
			*outputGlueData = rawData;
		}

		lua_newtable(L);

		setMetaTableSignature(L);
		setMetaTableGC(L);

		lua_setmetatable(L, -2);

		return true;
	}

	return false;
}

struct GLuaMethods
{
	typedef bool ResultType;

	static ResultType doObjectToScript(const GContextPointer & context, const GClassGlueDataPointer & classData,
		const GVariant & instance, const GBindValueFlags & flags, ObjectPointerCV cv, GGlueDataPointer * outputGlueData)
	{
		objectToLua(context, classData, instance, flags, cv, outputGlueData);
		return true;
	}

	static ResultType doVariantToScript(const GContextPointer & context, const GVariant & value, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
	{
		return variantToLua(context, value, flags, outputGlueData);
	}
	
	static ResultType doRawToScript(const GContextPointer & context, const GVariant & value, GGlueDataPointer * outputGlueData)
	{
		return rawToLua(context, value, outputGlueData);
	}

	static ResultType doClassToScript(const GContextPointer & context, IMetaClass * metaClass)
	{
		helperBindClass(context, metaClass);
		return true;
	}

	static ResultType doStringToScript(const GContextPointer & context, const char * s)
	{
		lua_pushstring(getLuaState(context), s);

		return true;
	}

	static ResultType doWideStringToScript(const GContextPointer & context, const wchar_t * ws)
	{
		GScopedArray<char> s(wideStringToString(ws));
		lua_pushstring(getLuaState(context), s.get());

		return true;
	}

	static bool isSuccessResult(const ResultType & result)
	{
		return result;
	}

	static ResultType defaultValue()
	{
		return ResultType();
	}

	static ResultType doMethodsToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem,
		IMetaClass * metaClass, IMetaClass * derived, const GObjectGlueDataPointer & objectData)
	{
		GMapItemMethodData * data = gdynamic_cast<GMapItemMethodData *>(mapItem->getUserData());
		GContextPointer context = classData->getBindingContext();
		if(data == nullptr) {
			GScopedInterface<IMetaClass> boundClass(selectBoundClass(metaClass, derived));

			GScopedInterface<IMetaList> metaList(getMethodListFromMapItem(mapItem, getGlueDataInstanceAddress(objectData)));
			GMethodGlueDataPointer glueData = context->newMethodGlueData(context->getClassData(boundClass.get()), metaList.get());
			data = new GMapItemMethodData(glueData);
			mapItem->setUserData(data);
		}
		helperBindMethodList(context, objectData, data->getMethodData());
		return true;
	}
	
	static ResultType doEnumToScript(const GClassGlueDataPointer & classData, GMetaMapItem * mapItem, const char * /*enumName*/)
	{
		GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));
		helperBindEnum(classData->getBindingContext(), metaEnum.get());
		return true;
	}

};

bool variantToLua(const GContextPointer & context, const GVariant & data, const GBindValueFlags & flags, GGlueDataPointer * outputGlueData)
{
	lua_State * L = getLuaState(context);

	GVariant value = getVariantRealValue(data);
	GMetaType type = getVariantRealMetaType(data);

	GVariantType vt = static_cast<GVariantType>((uint16_t)value.getType() & ~(uint16_t)GVariantType::maskByReference);
	
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

	if(!vtIsInterface(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == nullptr) {
		lua_pushnil(L);

		return true;
	}

	if(variantIsString(value)) {
		lua_pushstring(L, fromVariant<char *>(value));

		return true;
	}

	if(variantIsWideString(value)) {
		const wchar_t * ws = fromVariant<wchar_t *>(value);
		GScopedArray<char> s(wideStringToString(ws));
		lua_pushstring(L, s.get());
		return true;
	}

	return complexVariantToScript<GLuaMethods >(context, value, type, flags, outputGlueData);
}

void loadCallableParam(const GContextPointer & context, InvokeCallableParam * callableParam, int startIndex)
{
	for(size_t i = 0; i < callableParam->paramCount; ++i) {
		callableParam->params[i].value = luaToScriptValue(context, static_cast<int>(i) + startIndex, &callableParam->params[i].paramGlueData);
	}
}

bool methodResultToLua(const GContextPointer & context, IMetaCallable * callable, InvokeCallableResult * result)
{
	return methodResultToScript<GLuaMethods>(context, callable, result);
}

int callbackInvokeMethodList(lua_State * L)
{
	ENTER_LUA()

	GObjectAndMethodGlueDataPointer userData = static_cast<GGlueDataWrapper *>(lua_touserdata(L, lua_upvalueindex(1)))->getAs<GObjectAndMethodGlueData>();

	GContextPointer bindingContext(userData->getBindingContext());
	InvokeCallableParam callableParam(lua_gettop(L), bindingContext->borrowScriptContext());
	loadCallableParam(bindingContext, &callableParam, 1);
	
	InvokeCallableResult result = doInvokeMethodList(bindingContext, userData->getObjectData(), userData->getMethodData(), &callableParam);
	
	methodResultToLua(bindingContext, result.callable.get(), &result);
	return result.resultCount;

	LEAVE_LUA(L, return 0)
}

int invokeConstructor(const GClassGlueDataPointer & classUserData)
{
	lua_State * L = getLuaState(classUserData->getBindingContext());

	int paramCount = lua_gettop(L) - 1;

	const GContextPointer & context(classUserData->getBindingContext());

	InvokeCallableParam callableParam(paramCount, context->borrowScriptContext());
	loadCallableParam(context, &callableParam, 2);
	
	void * instance = doInvokeConstructor(context, context->getService(), classUserData->getMetaClass(), &callableParam);

	if(instance != nullptr) {
		objectToLua(context, classUserData, instance, GBindValueFlags(bvfAllowGC), opcvNone, nullptr);
	}
	else {
		raiseCoreException(Error_ScriptBinding_FailConstructObject);
	}

	return 1;
}

int invokeOperator(const GContextPointer & context, const GObjectGlueDataPointer & objectData, IMetaClass * metaClass, GMetaOpType op)
{
	lua_State * L = getLuaState(context);

	int paramCount = lua_gettop(L);
	int startIndex = 1;

	if(op == mopFunctor) { // skip the first "func" parameter
		++startIndex;
		--paramCount;
	}
	
	if(op == mopNeg) {
		paramCount = 1; // Lua pass two parameters to __unm...
	}

	InvokeCallableParam callableParam(paramCount, context->borrowScriptContext());
	loadCallableParam(context, &callableParam, startIndex);
	
	InvokeCallableResult result = doInvokeOperator(context, objectData, metaClass, op, &callableParam);
	methodResultToLua(context, result.callable.get(), &result);
	return result.resultCount;
}

int UserData_gc(lua_State * L)
{
	ENTER_LUA()

	GGlueDataWrapper * dataWrapper = static_cast<GGlueDataWrapper *>(lua_touserdata(L, -1));
	destroyGlueDataWrapper(dataWrapper);
	
	return 0;
	
	LEAVE_LUA(L, return 0)
}

int UserData_call(lua_State * L)
{
	ENTER_LUA()
		
	GClassGlueDataPointer classData = static_cast<GGlueDataWrapper *>(lua_touserdata(L, lua_upvalueindex(1)))->getAs<GClassGlueData>();

	if(classData) {
		return invokeConstructor(classData);
	}
	else {
		raiseCoreException(Error_ScriptBinding_InternalError_WrongFunctor);

		return 0;
	}
	
	LEAVE_LUA(L, return 0)
}

bool doIndexMemberData(const GContextPointer & context, IMetaAccessible * data, void * instance, bool instanceIsConst)
{
	return accessibleToScript<GLuaMethods>(context, data, instance, instanceIsConst);
}

int UserData_index(lua_State * L)
{
	ENTER_LUA()

	GGlueDataPointer glueData = static_cast<GGlueDataWrapper *>(lua_touserdata(L, -2))->getData();
	const char * name = lua_tostring(L, -1);

	if(namedMemberToScript<GLuaMethods>(glueData, name)) {
		return true;
	}
	else {
		lua_pushnil(L);
		return false;
	}
	

	LEAVE_LUA(L, lua_pushnil(L); return false)
}

int UserData_newindex(lua_State * L)
{
	ENTER_LUA()

	GGlueDataPointer instanceGlueData = static_cast<GGlueDataWrapper *>(lua_touserdata(L, -3))->getData();
	
	const char * name = lua_tostring(L, -2);

	GGlueDataPointer valueGlueData;

	GScriptValue value = luaToScriptValue(instanceGlueData->getBindingContext(), -1, &valueGlueData);
	if(setValueOnNamedMember(instanceGlueData, name, value, valueGlueData)) {
		return 1;
	}

	return 0;
	
	LEAVE_LUA(L, return 0)
}

int UserData_operator(lua_State * L)
{
	ENTER_LUA()
	
	GOperatorGlueDataPointer glueData = static_cast<GGlueDataWrapper *>(lua_touserdata(L, lua_upvalueindex(1)))->getAs<GOperatorGlueData>();

	return invokeOperator(glueData->getBindingContext(), glueData->getObjectData(), glueData->getMetaClass(), glueData->getOp());
	
	LEAVE_LUA(L, return 0)
}

void helperBindOperator(const GContextPointer & context, const GObjectGlueDataPointer & objectData, IMetaClass * metaClass, GMetaOpType op)
{
	lua_State * L = getLuaState(context);

	for(size_t i = 0; i < sizeof(metaOpTypes) / sizeof(metaOpTypes[0]); ++i) {
		if(metaOpTypes[i] == op) {
			lua_pushstring(L, luaOperators[i]);
			void * userData = lua_newuserdata(L, getGlueDataWrapperSize<GOperatorGlueData>());
			GOperatorGlueDataPointer operatorData(context->newOperatorGlueData(objectData, metaClass, op));
			newGlueDataWrapper(userData, operatorData);

			lua_newtable(L);
			setMetaTableSignature(L);
			setMetaTableGC(L);
			lua_setmetatable(L, -2);

			lua_pushcclosure(L, &UserData_operator, 1);
			lua_rawset(L, -3);

			return;
		}
	}
}

void helperBindAllOperators(const GContextPointer & context, const GObjectGlueDataPointer & objectData,
	IMetaClass * metaClass, bool bindingToObject)
{
	std::set<uint32_t> boundOperators;

	int count = metaClass->getOperatorCount();
	for(int i = 0; i < count; ++i) {
		GScopedInterface<IMetaOperator> item(metaClass->getOperatorAt(i));
		uint32_t op = item->getOperator();
		if((op == mopFunctor && !bindingToObject)
			|| (op != mopFunctor && bindingToObject)) {
			// If it's binding to class, we bind all operators exception functor,
			// because functor can be only bound to object.
			// If it's binding to object, we only bind functor, to optimize performance.
			continue;
		}
		if(boundOperators.find(op) == boundOperators.end()) {
			boundOperators.insert(op);
			helperBindOperator(context, objectData, metaClass, static_cast<GMetaOpType>(op));
		}
	}
}

void helperBindClass(const GContextPointer & context, IMetaClass * metaClass)
{
	lua_State * L = getLuaState(context);

	void * userData = lua_newuserdata(L, getGlueDataWrapperSize<GClassGlueData>());
	GClassGlueDataPointer classData(context->newClassData(metaClass));
	newGlueDataWrapper(userData, classData);

	lua_newtable(L);

	setMetaTableSignature(L);
	setMetaTableGC(L);
	setMetaTableCall(L, userData);

	initObjectMetaTable(L);

	lua_setmetatable(L, -2);
}

void helperBindMethodList(const GContextPointer & context, const GObjectGlueDataPointer & objectData, const GMethodGlueDataPointer & methodData)
{
	lua_State * L = getLuaState(context);

	void * userData = lua_newuserdata(L, getGlueDataWrapperSize<GObjectAndMethodGlueData>());
	GObjectAndMethodGlueDataPointer objectAndMethodData(context->newObjectAndMethodGlueData(objectData, methodData));
	newGlueDataWrapper(userData, objectAndMethodData);

	lua_newtable(L);
	
	setMetaTableSignature(L);
	setMetaTableGC(L);
	lua_setmetatable(L, -2);
	
	lua_pushcclosure(L, &callbackInvokeMethodList, 1);
}

void helperBindMethodList(const GContextPointer & context, IMetaList * methodList)
{
	GMethodGlueDataPointer methodData = context->newMethodGlueData(GClassGlueDataPointer(), methodList);
	helperBindMethodList(context, GObjectGlueDataPointer(), methodData);
}

bool helperBindValue(const GContextPointer & context, const char * name,
	const GScriptValue & value, const GlobalAccessorGetter & globalAccessorGetter)
{
	lua_State * L = getLuaState(context);

	switch(value.getType()) {
		case GScriptValue::typeNull:
			lua_pushnil(L);
			break;

		case GScriptValue::typeFundamental:
			if(! variantToLua(context, value.toFundamental(), GBindValueFlags(bvfAllowRaw), nullptr)) {
				raiseCoreException(Error_ScriptBinding_CantBindFundamental);
			}
			break;

		case GScriptValue::typeString:
			lua_pushstring(L, value.toString().c_str());
			break;

		case GScriptValue::typeClass: {
			GScopedInterface<IMetaClass> metaClass(value.toClass());
			helperBindClass(context, metaClass.get());
			break;
		}

		case GScriptValue::typeObject: {
			IMetaClass * metaClass;
			bool transferOwnership;
			void * instance = objectAddressFromVariant(value.toObject(&metaClass, &transferOwnership));
			GScopedInterface<IMetaClass> metaClassGuard(metaClass);
			
			GBindValueFlags flags;
			flags.setByBool(bvfAllowGC, transferOwnership);
			objectToLua(context, context->getClassData(metaClass), instance, flags, opcvNone, nullptr);
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

			helperBindMethodList(context, methodList.get());
			break;
		}

		case GScriptValue::typeOverloadedMethods: {
			GScopedInterface<IMetaList> methodList(value.toOverloadedMethods());
			helperBindMethodList(context, methodList.get());
			break;
		}

		case GScriptValue::typeEnum: {
			GScopedInterface<IMetaEnum> metaEnum(value.toEnum());
			helperBindEnum(context, metaEnum.get());
			break;
		}

		case GScriptValue::typeRaw:
			if(! rawToLua(context, value.toRaw(), nullptr)) {
				raiseCoreException(Error_ScriptBinding_CantBindRaw);
			}
			break;

		case GScriptValue::typeAccessible: {
			void * instance;
			GScopedInterface<IMetaAccessible> accessible(value.toAccessible(&instance));
			globalAccessorGetter()->bindAccessible(name, instance, accessible.get());
			return false;
		}

	}
	
	return true;
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

	GEnumGlueDataPointer userData = static_cast<GGlueDataWrapper *>(lua_touserdata(L, -2))->getAs<GEnumGlueData>();
	
	const char * name = lua_tostring(L, -1);

	int index = userData->getMetaEnum()->findKey(name);
	if(index < 0) {
		raiseCoreException(Error_ScriptBinding_CantFindEnumKey, name);
	}
	else {
		GVariant value(metaGetEnumValue(userData->getMetaEnum(), index));
		lua_pushinteger(L, fromVariant<lua_Integer>(value));
	}
	
	return true;
	
	LEAVE_LUA(L, return false)
}

int Enum_newindex(lua_State * L)
{
	ENTER_LUA()

	raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);

	return 0;
	
	LEAVE_LUA(L, return 0)
}

void helperBindEnum(const GContextPointer & context, IMetaEnum * metaEnum)
{
	lua_State * L = getLuaState(context);

	void * userData = lua_newuserdata(L, getGlueDataWrapperSize<GEnumGlueData>());
	GEnumGlueDataPointer enumData(context->newEnumGlueData(metaEnum));
	newGlueDataWrapper(userData, enumData);

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


GLuaScopeGuard::GLuaScopeGuard(GScriptObject * scope)
	: scope(gdynamic_cast<GLuaScriptObject *>(scope))
{
	this->top = lua_gettop(this->scope->luaState);
	if(! this->scope->isGlobal()) {
		this->scope->getTable();
	}
}
	
GLuaScopeGuard::~GLuaScopeGuard()
{
	if(this->top >= 0) {
		int currentTop = lua_gettop(this->scope->luaState);
		if(currentTop > this->top) {
			lua_pop(this->scope->luaState, currentTop - this->top);
		}
	}
}

void GLuaScopeGuard::keepStack()
{
	this->top = -1;
}

void GLuaScopeGuard::set(const char * name)
{
	if(scope->isGlobal()) {
		lua_setglobal(this->scope->luaState, name);
	}
	else {
		lua_setfield(this->scope->luaState, -2, name);
	}
}
	
void GLuaScopeGuard::get(const char * name)
{
	if(scope->isGlobal()) {
		lua_getglobal(this->scope->luaState, name);
	}
	else {
		lua_getfield(this->scope->luaState, -1, name);
	}
}

void GLuaScopeGuard::rawGet(const char * name)
{
#if HAS_LUA_GLOBALSINDEX
	lua_pushstring(this->scope->luaState, name);

	if(scope->isGlobal()) {
		lua_rawget(this->scope->luaState, LUA_GLOBALSINDEX);
	}
	else {
		lua_rawget(this->scope->luaState, -2);
	}
#else
	if(scope->isGlobal()) {
		lua_pushglobaltable(this->scope->luaState);
	}

	lua_pushstring(this->scope->luaState, name);
	lua_rawget(this->scope->luaState, -2);
	lua_remove(this->scope->luaState, -2); // remove the global table to balance the stace
#endif
}


// function is on stack top
GScriptValue invokeLuaFunctionIndirectly(const GContextPointer & context, GVariant const * const * params, size_t paramCount, const char * name)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	if(! context) {
		raiseCoreException(Error_ScriptBinding_NoContext);
	}

	lua_State * L = getLuaState(context);

	int top = lua_gettop(L) - 1;

	if(lua_isfunction(L, -1)) {
		for(size_t i = 0; i < paramCount; ++i) {
			if(!variantToLua(context, *params[i], GBindValueFlags(bvfAllowRaw), nullptr)) {
				if(i > 0) {
					lua_pop(L, static_cast<int>(i) - 1);
				}

				raiseCoreException(Error_ScriptBinding_ScriptMethodParamMismatch, i, name);
			}
		}

		int error = lua_pcall(L, static_cast<int>(paramCount), LUA_MULTRET, 0);
		if(error) {
			raiseCoreException(Error_ScriptBinding_ScriptFunctionReturnError, name, lua_tostring(L, -1));
		}
		else {
			int resultCount = lua_gettop(L) - top;
			if(resultCount > 1) {
				raiseCoreException(Error_ScriptBinding_CantReturnMultipleValue, name);
			}
			else {
				if(resultCount > 0) {
					return luaToScriptValue(context, -1, nullptr);
				}
			}
		}
	}
	else {
		raiseCoreException(Error_ScriptBinding_CantCallNonfunction);
	}
	
	return GScriptValue::fromNull();
}


GLuaScriptFunction::GLuaScriptFunction(const GContextPointer & context, int objectIndex)
	: super(context), ref(refLua(getLuaState(context), objectIndex))
{
}

GLuaScriptFunction::~GLuaScriptFunction()
{
	unrefLua(getLuaState(this->getBindingContext()), this->ref);
}
	
GScriptValue GLuaScriptFunction::invoke(const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GScriptValue GLuaScriptFunction::invokeIndirectly(GVariant const * const * params, size_t paramCount)
{
	lua_State * L = getLuaState(this->getBindingContext());

	getRefObject(L, this->ref);

	return invokeLuaFunctionIndirectly(this->getBindingContext(), params, paramCount, "");
}


GLuaScriptArray::GLuaScriptArray(GLuaScriptObject * scriptObject, int objectIndex)
	: super(scriptObject->getBindingContext()),
		ref(refLua(getLuaState(scriptObject->getBindingContext()), objectIndex)),
		scriptObject(scriptObject)
{
}

GLuaScriptArray::~GLuaScriptArray()
{
	unrefLua(getLuaState(this->getBindingContext()), this->ref);
}

size_t GLuaScriptArray::getLength()
{
	lua_State * L = getLuaState(this->getBindingContext());

	getRefObject(L, this->ref);

#if LUA_VERSION_NUM >= 502
	return lua_rawlen(L, -1);
#else
	return lua_objlen(L, -1);
#endif
}

GScriptValue GLuaScriptArray::getValue(size_t index)
{
	lua_State * L = getLuaState(this->getBindingContext());
	getRefObject(L, this->ref);
	lua_rawgeti(L, -1, (int)(index + 1));
	
	return luaToScriptValue(this->getBindingContext(), -1, nullptr);
}

void GLuaScriptArray::setValue(size_t index, const GScriptValue & value)
{
	if(value.isAccessible()) {
		raiseCoreException(Error_ScriptBinding_NotSupportedFeature, "Set Accessible Into Array", "Lua");
	}
	else {
		lua_State * L = getLuaState(this->getBindingContext());
		getRefObject(L, this->ref);
		bool shouldSet = helperBindValue(this->getBindingContext(), "", value,
			makeCallback(this->scriptObject, &GLuaScriptObject::getGlobalAccessor));
		if(shouldSet) {
			lua_rawseti(L, -2, (int)(index + 1));
		}
	}
}

bool GLuaScriptArray::maybeIsScriptArray(size_t index)
{
	lua_State * L = getLuaState(this->getBindingContext());
	getRefObject(L, this->ref);
	lua_rawgeti(L, -1, (int)(index + 1));
	return lua_type(L, -1) == LUA_TTABLE;
}
GScriptValue GLuaScriptArray::getAsScriptArray(size_t index)
{
	lua_State * L = getLuaState(this->getBindingContext());
	getRefObject(L, this->ref);
	lua_rawgeti(L, -1, (int)(index + 1));

	if(lua_type(L, -1) == LUA_TTABLE) {
		GScopedInterface<IScriptArray> scriptArray(new ImplScriptArray(new GLuaScriptArray(this->scriptObject, -1), true));
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
	else {
		return GScriptValue();
	}
}
GScriptValue GLuaScriptArray::createScriptArray(size_t index)
{
	lua_State * L = getLuaState(this->getBindingContext());

	lua_newtable(L);
	lua_rawseti(L, -2, (int)(index + 1));

	GScopedInterface<IScriptArray> scriptArray(new ImplScriptArray(new GLuaScriptArray(this->scriptObject, -1), true));
	return GScriptValue::fromScriptArray(scriptArray.get());
}	

GLuaScriptObject::GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config)
	: super(GContextPointer(new GLuaBindingContext(service, config, L)), config), luaState(L), ref(LUA_NOREF)
{
}

GLuaScriptObject::GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config, int objectIndex)
	: super(GContextPointer(new GLuaBindingContext(service, config, L)), config), luaState(L), ref(LUA_NOREF)
{
	this->ref = refLua(this->luaState, objectIndex);
}

GLuaScriptObject::GLuaScriptObject(const GLuaScriptObject & other)
	: super(other), luaState(other.luaState), ref(LUA_NOREF)
{
	this->ref = refLua(this->luaState, -1);
}

GLuaScriptObject::~GLuaScriptObject()
{
	if(this->ref != LUA_NOREF) {
		unrefLua(this->luaState, this->ref);
	}
}

bool GLuaScriptObject::isGlobal() const
{
	return this->ref == LUA_NOREF;
}

void GLuaScriptObject::getTable() const
{
	if(this->ref != LUA_NOREF) {
		getRefObject(this->luaState, this->ref);
	}
	else {
#if HAS_LUA_GLOBALSINDEX
		lua_pushvalue(this->luaState, LUA_GLOBALSINDEX);
#else
		lua_pushglobaltable(this->luaState);
#endif
	}
}

GScriptValue GLuaScriptObject::doGetValue(const char * name)
{
	GLuaScopeGuard scopeGuard(this);
	
	scopeGuard.get(name);

	return luaToScriptValue(this->getBindingContext(), -1, nullptr);
}

void GLuaScriptObject::doSetValue(const char * name, const GScriptValue & value)
{
	GLuaScopeGuard scopeGuard(this);

	bool shouldSet = helperBindValue(this->getBindingContext(), name, value,
		makeCallback(this, &GLuaScriptObject::getGlobalAccessor));

	if(shouldSet) {	
		scopeGuard.set(name);
	}
}

GMethodGlueDataPointer GLuaScriptObject::doGetMethodUserData()
{
	if(lua_type(this->luaState, -1) != LUA_TFUNCTION) {
		return GMethodGlueDataPointer();
	}

	lua_getupvalue(this->luaState, -1, 1);
	if(lua_isnil(this->luaState, -1)) {
		lua_pop(this->luaState, 1);
	}
	else {
		void * rawUserData = lua_touserdata(this->luaState, -1);
		GGlueDataPointer userData = static_cast<GGlueDataWrapper *>(rawUserData)->getData();

		if(userData->getType() == gdtObjectAndMethod) {
			return static_cast<GGlueDataWrapper *>(rawUserData)->getAs<GObjectAndMethodGlueData>()->getMethodData();
		}
	}

	return GMethodGlueDataPointer();
}

GLuaGlobalAccessor * GLuaScriptObject::getGlobalAccessor()
{
	if(! this->globalAccessor) {
		this->globalAccessor.reset(new GLuaGlobalAccessor(this));
	}

	return this->globalAccessor.get();
}

GScriptObject * GLuaScriptObject::doCreateScriptObject(const char * name)
{
	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	if(lua_isnil(this->luaState, -1)) {
		lua_pop(this->luaState, 1);
		lua_newtable(this->luaState);
		scopeGuard.set(name);
		scopeGuard.get(name);
	}
	else {
		if(this->getValue(name).getType() != GScriptValue::typeScriptObject) {
			lua_pop(this->luaState, 1);
			return nullptr;
		}
	}

	GLuaScriptObject * newScriptObject = new GLuaScriptObject(*this);
	newScriptObject->setOwner(this);
	newScriptObject->setName(name);
	
	return newScriptObject;
}

GScriptValue GLuaScriptObject::getScriptFunction(const char * name)
{
	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	GScopedInterface<IScriptFunction> scriptFunction(new ImplScriptFunction(new GLuaScriptFunction(this->getBindingContext(), -1), true));
	return GScriptValue::fromScriptFunction(scriptFunction.get());
}

GScriptValue GLuaScriptObject::invoke(const char * name, const GVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GScriptValue GLuaScriptObject::invokeIndirectly(const char * name, GVariant const * const * params, size_t paramCount)
{
	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	return invokeLuaFunctionIndirectly(this->getBindingContext(), params, paramCount, name);
}

void GLuaScriptObject::assignValue(const char * fromName, const char * toName)
{
	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(fromName);
	scopeGuard.set(toName);
}

bool GLuaScriptObject::maybeIsScriptArray(const char * name)
{
	lua_State * L = this->getLuaState();
	GLuaScopeGuard scopeGuard(this);
	scopeGuard.get(name);
	return lua_type(L, -1) == LUA_TTABLE;
}

GScriptValue GLuaScriptObject::getAsScriptArray(const char * name)
{
	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	if(lua_type(this->getLuaState(), -1) == LUA_TTABLE) {
		GScopedInterface<IScriptArray> scriptArray(new ImplScriptArray(new GLuaScriptArray(this, -1), true));
		return GScriptValue::fromScriptArray(scriptArray.get());
	}
	else {
		return GScriptValue();
	}
}

GScriptValue GLuaScriptObject::createScriptArray(const char * name)
{
	GLuaScopeGuard scopeGuard(this);

	lua_newtable(this->luaState);
	scopeGuard.set(name);
	scopeGuard.get(name);

	GScopedInterface<IScriptArray> scriptArray(new ImplScriptArray(new GLuaScriptArray(this, -1), true));
	return GScriptValue::fromScriptArray(scriptArray.get());
}



} // unnamed namespace


GScriptObject * createLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config)
{
	return new GLuaScriptObject(service, L, config);
}

IScriptObject * createLuaScriptInterface(IMetaService * service, lua_State * L, const GScriptConfig & config)
{
	return new ImplScriptObject(new GLuaScriptObject(service, L, config), true);
}


} // namespace cpgf



#if defined(_MSC_VER)
#pragma warning(pop)
#endif

