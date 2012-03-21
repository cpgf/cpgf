#include "cpgf/scriptbind/gluabind.h"
#include "cpgf/gflags.h"
#include "cpgf/gexception.h"
#include "cpgf/gmetaclasstraveller.h"
#include "cpgf/gcallback.h"

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
using namespace cpgf::bind_internal;

#define ENTER_LUA() \
	char local_msg[256]; bool local_error = false; { \
	try {

#define LEAVE_LUA(L, ...) \
	} \
	catch(const GException & e) { strncpy(local_msg, e.getMessage(), 256); local_error = true; } \
	catch(...) { strcpy(local_msg, "Unknown exception occurred."); local_error = true; } \
	} if(local_error) { local_msg[255] = 0; error(L, local_msg); } \
	__VA_ARGS__;
	

namespace cpgf {


namespace {

class GLuaScriptObject;
class GLuaGlobalAccessor;

class GLuaScriptFunction : public GScriptFunction
{
public:
	GLuaScriptFunction(lua_State * L, GScriptBindingParam * bindingParam, int objectIndex);
	virtual ~GLuaScriptFunction();
	
	virtual GMetaVariant invoke(const GMetaVariant * params, size_t paramCount);
	virtual GMetaVariant invokeIndirectly(GMetaVariant const * const * params, size_t paramCount);

private:
	lua_State * luaState;
	GScriptBindingParam * bindingParam;
	int ref;
};

class GLuaScriptObjectImplement
{
public:
	GLuaScriptObjectImplement(GScriptBindingParam * param, GLuaScriptObject * binding, lua_State * L, int objectIndex, bool freeResource);
	
	~GLuaScriptObjectImplement();
	
	bool isGlobal() const;
	
	void getTable() const;

	GMethodListUserData * doGetMethodUserData();

	GLuaGlobalAccessor * getGlobalAccessor();

public:
	GScriptBindingParam * param;
	GLuaScriptObject * binding;
	lua_State * luaState;
	int ref;
	bool freeResource;

private:
	GScopedPointer<GLuaGlobalAccessor> globalAccessor;
};


class GLuaScriptObject : public GScriptObject
{
private:
	typedef GScriptObject super;

public:
	GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config);
	GLuaScriptObject(const GLuaScriptObject & other);
	GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config, int objectIndex);
	virtual ~GLuaScriptObject();

	virtual bool isGlobal() const;
	
	virtual void bindClass(const char * name, IMetaClass * metaClass);
	virtual void bindEnum(const char * name, IMetaEnum * metaEnum);

	virtual void bindFundamental(const char * name, const GVariant & value);
	virtual void bindAccessible(const char * name, void * instance, IMetaAccessible * accessible);
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

public:
	lua_State * getLuaState() const {
		return this->implement->luaState;
	}

	GScriptBindingParam * getParam() const {
		return this->implement->param;
	}

private:
	GScopedPointer<GLuaScriptObjectImplement> implement;

private:
	friend class GLuaScopeGuard;
	friend class GLuaScriptObjectImplement;
	friend class GLuaScriptNameData;
	friend class GLuaGlobalAccessor;
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
	~GLuaGlobalAccessor();
	void doBindAccessible(const char * name, void * instance, IMetaAccessible * accessible);

	int doIndex();
	int doNewIndex();

private:
	void initialize();
	void destroy();

	int doPreviousIndex(const char * name);
	int doPreviousNewIndex(const char * name);

private:
	MapType itemMap;
	GLuaScriptObject * scriptObject;
	bool hasPreviousMetatable;
};

int UserData_gc(lua_State * L);
int UserData_call(lua_State * L);
int UserData_index(lua_State * L);
int UserData_newindex(lua_State * L);
int UserData_operator(lua_State * L);

void doBindAllOperators(lua_State * L, GScriptBindingParam * param, void * instance, IMetaClass * metaClass);
void doBindClass(lua_State * L, GScriptBindingParam * param, IMetaClass * metaClass);
void doBindEnum(lua_State * L, GScriptBindingParam * param, IMetaEnum * metaEnum);
void doBindMethodList(lua_State * L, GScriptBindingParam * param, IMetaList * methodList, GUserDataMethodType methodType);

void initObjectMetaTable(lua_State * L);
void setMetaTableGC(lua_State * L);
void setMetaTableCall(lua_State * L, void * userData);
void setMetaTableSignature(lua_State * L);
bool isValidMetaTable(lua_State * L, int index);

bool variantToLua(lua_State * L, GScriptBindingParam * param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw);
GMetaVariant luaToVariant(lua_State * L, GScriptBindingParam * param, int index);

bool doIndexMemberData(lua_State * L, GScriptBindingParam * param, IMetaAccessible * data, void * instance, bool instanceIsConst);

void error(lua_State * L, const char * message);

const int RefTable = LUA_REGISTRYINDEX;

int refLua(lua_State * L, int objectIndex)
{
	lua_pushvalue(L, objectIndex);
	return luaL_ref(L, RefTable);
}

void unrefLua(lua_State * L, int ref)
{
	if(ref == LUA_NOREF) {
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
	: scriptObject(scriptObject), hasPreviousMetatable(false)
{
	this->initialize();
}

GLuaGlobalAccessor::~GLuaGlobalAccessor()
{
	this->destroy();
}

void GLuaGlobalAccessor::doBindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	string sname(name);

	this->itemMap.insert(make_pair(sname, GLuaGlobalAccessorItem(instance, accessible)));
}

const char * GlobalAccessorPreviousMetaTableKey = "i_gL0b1_Meta_kEY_389172";

int GLuaGlobalAccessor::doIndex()
{
	lua_State * L = this->scriptObject->implement->luaState;

	const char * name = lua_tostring(L, -1);

	string sname(name);
	MapType::iterator it = this->itemMap.find(sname);
	if(it != this->itemMap.end()) {
		doIndexMemberData(L, this->scriptObject->implement->param, it->second.accessible.get(), it->second.instance, false);
		return 1;
	}

	return this->doPreviousIndex(name);
}

int GLuaGlobalAccessor::doNewIndex()
{
	lua_State * L = this->scriptObject->implement->luaState;

	const char * name = lua_tostring(L, -2);
	
	string sname(name);
	MapType::iterator it = this->itemMap.find(sname);
	if(it != this->itemMap.end()) {
		GVariant value = luaToVariant(L, this->scriptObject->implement->param, -1).getValue();
		GVariantData varData = value.getData();
		it->second.accessible->set(it->second.instance, &varData);
		metaCheckError(it->second.accessible);
		return 1;
	}

	return this->doPreviousNewIndex(name);
}

int GLuaGlobalAccessor::doPreviousIndex(const char * name)
{
	if(this->hasPreviousMetatable) {
		lua_State * L = this->scriptObject->implement->luaState;
		
		GLuaScopeGuard scopeGuard(this->scriptObject);
		
		scopeGuard.keepStack();

		scopeGuard.rawGet(GlobalAccessorPreviousMetaTableKey);

		if(lua_isfunction(L, -1)) {
			lua_pushstring(L, name);
			lua_call(L, 1, 1);
			return 1;
		}
		if(lua_istable(L, -1)) {
			lua_getfield(L, -1, name);
			return 1;
		}
	}

	return 0;
}

int GLuaGlobalAccessor::doPreviousNewIndex(const char * name)
{
	if(this->hasPreviousMetatable) {
		lua_State * L = this->scriptObject->implement->luaState;
		
		GLuaScopeGuard scopeGuard(this->scriptObject);

		scopeGuard.keepStack();

		scopeGuard.rawGet(GlobalAccessorPreviousMetaTableKey);

		if(lua_isfunction(L, -1)) {
			lua_pushstring(L, name);
			lua_pushvalue(L, -3);
			lua_call(L, 2, 0);
			return 1;
		}
		if(lua_istable(L, -1)) {
			lua_pushvalue(L, -2);
			lua_setfield(L, -2, name);
			return 1;
		}
	}

	return 0;
}

void GLuaGlobalAccessor::initialize()
{
	lua_State * L = this->scriptObject->implement->luaState;

	this->hasPreviousMetatable = false;

	{
		GLuaScopeGuard scopeGuard(this->scriptObject);
		bool ok = false;
		if(this->scriptObject->isGlobal()) {
			ok = !! lua_getmetatable(L, LUA_GLOBALSINDEX);
			if(!ok) {
				lua_newtable(L);
			}
			ok = true;
		}
		else {
			ok = !! lua_getmetatable(L, -1);
		}
		if(ok) {
			scopeGuard.set(GlobalAccessorPreviousMetaTableKey);

			this->hasPreviousMetatable = true;
		}
	}

	if(! this->scriptObject->isGlobal()) {
		this->scriptObject->implement->getTable();
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
		lua_setmetatable(L, LUA_GLOBALSINDEX);
	}
	else {
		lua_setmetatable(L, -2);
	}
}

void GLuaGlobalAccessor::destroy()
{
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
	if(fileName == NULL || *fileName != '@') {
		fileName = &dummy;
	}

	char s[1024];
	sprintf(s, "Error: file %.256s, line %d: %.700s", fileName, ar.currentline, message);

	lua_pushstring(L, s);
	lua_error(L);
}

void objectToLua(lua_State * L, GScriptBindingParam * param, void * instance, IMetaClass * metaClass, bool allowGC, ObjectPointerCV cv, ClassUserDataType dataType)
{
	if(instance == NULL) {
		lua_pushnil(L);

		return;
	}

	void * userData = lua_newuserdata(L, sizeof(GClassUserData));
	new (userData) GClassUserData(param, metaClass, instance, true, allowGC, cv, dataType);

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
				metaCheckError(classData->metaClass);
				*outType = GMetaType(typeData);
			}

			return classData->instance;
		}
	}

	return NULL;
}

GMetaVariant luaUserDataToVariant(lua_State * L, GScriptBindingParam * param, int index)
{
	(void)param;

	if(isValidMetaTable(L, index)) {
		void * userData = lua_touserdata(L, index);
		return userDataToVariant(static_cast<GScriptUserData *>(userData));
	}

	return GMetaVariant();
}

GMetaVariant functionToVariant(lua_State * L, GScriptBindingParam * param, int index)
{
	GScopedInterface<IScriptFunction> func(new ImplScriptFunction(new GLuaScriptFunction(L, param, index), true));
	
	return GMetaVariant(func.get(), GMetaType());
}

GMetaVariant tableToVariant(lua_State * L, GScriptBindingParam * param, int index)
{
	GScopedInterface<IScriptObject> scriptObject(new ImplScriptObject(new GLuaScriptObject(param->getService(), L, param->getConfig(), index), true));
	
	return GMetaVariant(scriptObject.get(), GMetaType());
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
			return GMetaVariant(createStringVariant(lua_tostring(L, index)), createMetaType<char *>());

		case LUA_TUSERDATA:
			return luaUserDataToVariant(L, param, index);

		case LUA_TFUNCTION:
			return functionToVariant(L, param, index);

		case LUA_TLIGHTUSERDATA:
			break;

		case LUA_TTABLE:
			return tableToVariant(L, param, index);
			
		case LUA_TTHREAD:
			break;
	}

	return GMetaVariant();
}

bool rawToLua(lua_State * L, GScriptBindingParam * param, const GVariant & value)
{
	GVariantType vt = value.getType();

	if(param->getConfig().allowAccessRawData() && variantIsRawData(vt)) {
		void * userData = lua_newuserdata(L, sizeof(GRawUserData));
		new (userData) GRawUserData(param, value);

		lua_newtable(L);

		setMetaTableSignature(L);
		setMetaTableGC(L);

		lua_setmetatable(L, -2);

		return true;
	}

	return false;
}

bool variantToLua(lua_State * L, GScriptBindingParam * param, const GVariant & value, const GMetaType & type, bool allowGC, bool allowRaw)
{
	GVariantType vt = static_cast<GVariantType>(value.getType() & ~byReference);
	
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

	if(!vtIsByteArray(vt) && canFromVariant<void *>(value) && objectAddressFromVariant(value) == NULL) {
		lua_pushnil(L);

		return true;
	}

	if(variantIsString(value)) {
		lua_pushstring(L, fromVariant<char *>(value));

		return true;
	}

	if(! type.isEmpty() && type.getPointerDimension() <= 1) {
		GScopedInterface<IMetaTypedItem> typedItem(param->getService()->findTypedItemByName(type.getBaseName()));
		if(typedItem) {
			bool isReference = type.isReference();

			if(type.getPointerDimension() == 0 && !isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");
				GASSERT_MSG(type.baseIsClass(), "Unknown type");

				IMetaClass * metaClass = gdynamic_cast<IMetaClass *>(typedItem.get());
				void * instance = metaClass->cloneInstance(objectAddressFromVariant(value));
				objectToLua(L, param, instance, gdynamic_cast<IMetaClass *>(typedItem.get()), true, metaTypeToCV(type), cudtNormal);

				return true;
			}

			if(type.getPointerDimension() == 1 || isReference) {
				GASSERT_MSG(!! metaIsClass(typedItem->getCategory()), "Unknown type");

				if(vtIsByteArray(vt)) {
					GScopedInterface<IByteArray> ba(value.data.valueByteArray); // free the byte array after return
					objectToLua(L, param, value.data.valueByteArray, gdynamic_cast<IMetaClass *>(typedItem.get()),
						allowGC, metaTypeToCV(type), cudtByteArray);
				}
				else {
					objectToLua(L, param, fromVariant<void *>(value), gdynamic_cast<IMetaClass *>(typedItem.get()),
						allowGC, metaTypeToCV(type), cudtNormal);
				}

				return true;
			}
		}

		if(bind_internal::shouldRemoveReference(type)) {
			GMetaType newType(type);
			newType.removeReference();

			return variantToLua(L, param, value, newType, allowGC, allowRaw);
		}
	}

	if(allowRaw) {
		return rawToLua(L, param, value);
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
						*typeItem = gdynamic_cast<GClassUserData *>(userData)->metaClass;
						(*typeItem)->addReference();
					}

					if(gdynamic_cast<GClassUserData *>(userData)->isInstance) {
						return sdtObject;
					}
					else {
						return sdtClass;
					}

				case udtMethodList:
					return methodTypeToUserDataType(gdynamic_cast<GMethodListUserData *>(userData)->methodType);

				case udtEnum:
					if(typeItem != NULL) {
						*typeItem = gdynamic_cast<GEnumUserData *>(userData)->metaEnum;
						(*typeItem)->addReference();
					}
					return sdtEnum;

				case udtRaw:
					return sdtRaw;

				default:
					break;
				}
			}
			break;

		case LUA_TTABLE:
			return sdtScriptObject;

		case LUA_TFUNCTION: {
			lua_getupvalue(L, index, 1);
			if(lua_isnil(L, -1)) {
				lua_pop(L, 1);
			}
			else {
				void * rawUserData = lua_touserdata(L, -1);
				GScriptUserData * userData = static_cast<GScriptUserData *>(rawUserData);

				if(userData != NULL) {
					switch(userData->getType()) {
					case udtMethodList:
						return methodTypeToUserDataType(gdynamic_cast<GMethodListUserData *>(userData)->methodType);

					default:
						break;
					}
				}
			}
			return sdtScriptMethod;
		}

	}
	
	return sdtUnknown;
}

void loadMethodParameters(lua_State * L, GScriptBindingParam * param, GVariantData * outputParams, int startIndex, size_t paramCount)
{
	for(size_t i = 0; i < paramCount; ++i) {
		outputParams[i] = luaToVariant(L, param, static_cast<int>(i) + startIndex).takeData().varData;
	}
}

void loadMethodParamTypes(lua_State * L, GBindDataType * outputTypes, int startIndex, size_t paramCount)
{
	for(size_t i = 0; i < paramCount; ++i) {
		IMetaTypedItem * typeItem;
		outputTypes[i].dataType = getLuaType(L, static_cast<int>(i) + startIndex, &typeItem);
		outputTypes[i].typeItem.reset(typeItem);
	}
}

bool doPushInvokeResult(lua_State * L, GScriptBindingParam * param, IMetaCallable * callable, InvokeCallableResult * result)
{
	if(result->resultCount > 0) {
		GMetaTypeData typeData;
	
		callable->getResultType(&typeData);
		metaCheckError(callable);

		GVariant value = GVariant(result->resultData);
		bool success = variantToLua(L, param, value, GMetaType(typeData), !! callable->isResultTransferOwnership(), false);
		if(!success) {
			GScopedInterface<IMetaConverter> converter(metaGetResultExtendType(callable, GExtendTypeCreateFlag_Converter).getConverter());
			success = converterToLua(L, param, value, converter.get());
		}
		if(!success) {
			success = rawToLua(L, param, value);
		}
		if(!success) {
			raiseCoreException(Error_ScriptBinding_FailVariantToScript);
		}
	}
	
	return true;
}

void loadCallableParam(lua_State * L, GScriptBindingParam * param, InvokeCallableParam * callableParam, int startIndex, size_t paramCount)
{
	if(paramCount > REF_MAX_ARITY) {
		raiseCoreException(Error_ScriptBinding_CallMethodWithTooManyParameters);
	}

	callableParam->paramCount = paramCount;
	loadMethodParameters(L, param, callableParam->paramsData, startIndex, paramCount);
	loadMethodParamTypes(L, callableParam->paramsType, startIndex, paramCount);
}

int callbackInvokeMethodList(lua_State * L)
{
	ENTER_LUA()

	GMethodListUserData * userData = static_cast<GMethodListUserData *>(lua_touserdata(L, lua_upvalueindex(1)));
	IMetaList * methodList = userData->methodList;

	InvokeCallableParam callableParam;
	loadCallableParam(L, userData->getParam(), &callableParam, 1, lua_gettop(L));
	
	int maxRankIndex = findAppropriateCallable(userData->getParam()->getService(),
		makeCallback(methodList, &IMetaList::getAt), methodList->getCount(),
		&callableParam, FindCallablePredict());

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;
		GScopedInterface<IMetaCallable> callable(gdynamic_cast<IMetaCallable *>(methodList->getAt(maxRankIndex)));
		doInvokeCallable(methodList->getInstanceAt(static_cast<uint32_t>(maxRankIndex)), callable.get(), callableParam.paramsData, lua_gettop(L), &result);
		doPushInvokeResult(L, userData->getParam(), callable.get(), &result);
		return result.resultCount;
	}

	raiseCoreException(Error_ScriptBinding_CantFindMatchedMethod);

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
		InvokeCallableParam callableParam;
		loadCallableParam(L, param, &callableParam, 2, paramCount);
	
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
		objectToLua(L, param, instance, metaClass, true, opcvNone, cudtNormal);
	}
	else {
		raiseCoreException(Error_ScriptBinding_FailConstructObject);
	}

	return 1;
}

int invokeOperator(lua_State * L, GScriptBindingParam * param, void * instance, IMetaClass * metaClass, GMetaOpType op)
{
	int paramCount = lua_gettop(L);
	int startIndex = 1;

	if(op == mopFunctor) { // skip the first "func" parameter
		++startIndex;
		--paramCount;
	}
	
	if(op == mopNeg) {
		paramCount = 1; // Lua pass two parameters to __unm...
	}
		
	InvokeCallableParam callableParam;
	loadCallableParam(L, param, &callableParam, startIndex, paramCount);
	
	int maxRankIndex = findAppropriateCallable(param->getService(),
		makeCallback(metaClass, &IMetaClass::getOperatorAt), metaClass->getOperatorCount(),
		&callableParam, OperatorCallablePredict(op));

	if(maxRankIndex >= 0) {
		InvokeCallableResult result;

		GScopedInterface<IMetaOperator> metaOperator(metaClass->getOperatorAt(maxRankIndex));
		doInvokeCallable(instance, metaOperator.get(), callableParam.paramsData, paramCount, &result);
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
		raiseCoreException(Error_ScriptBinding_InternalError_WrongFunctor);

		return 0;
	}
	
	LEAVE_LUA(L, return 0)
}

bool doIndexMemberData(lua_State * L, GScriptBindingParam * param, IMetaAccessible * data, void * instance, bool instanceIsConst)
{
	GMetaType type;
	GVariant value = getAccessibleValueAndType(instance, data, &type, instanceIsConst);

	bool success = variantToLua(L, param, value, type, false, false);
	if(!success) {
		GScopedInterface<IMetaConverter> converter(metaGetItemExtendType(data, GExtendTypeCreateFlag_Converter).getConverter());
		success = converterToLua(L, param, value, converter.get());
	}
	if(!success) {
		success = rawToLua(L, param, value);
	}

	return success;
}

bool indexMemberData(lua_State * L, GClassUserData * userData, IMetaAccessible * data, void * instance)
{
	return doIndexMemberData(L, userData->getParam(), data, instance, userData->cv == opcvConst);
}

bool indexMemberEnumType(lua_State * L, GClassUserData * userData, GMetaMapItem * mapItem)
{
	GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));

	doBindEnum(L, userData->getParam(), metaEnum.get());

	return true;
}

bool indexMemberEnumValue(lua_State * L, GClassUserData * userData, GMetaMapItem * mapItem)
{
	(void)userData;

	GScopedInterface<IMetaEnum> metaEnum(gdynamic_cast<IMetaEnum *>(mapItem->getItem()));

	GVariantData data;
	metaEnum->getValue(&data, static_cast<uint32_t>(mapItem->getEnumIndex()));
	metaCheckError(metaEnum);
	variantToLua(L, userData->getParam(), GVariant(data), GMetaType(), false, true);

	return true;
}

bool indexMemberClass(lua_State * L, GClassUserData * userData, GMetaMapItem * mapItem)
{
	GScopedInterface<IMetaClass> metaClass(gdynamic_cast<IMetaClass *>(mapItem->getItem()));

	doBindClass(L, userData->getParam(), metaClass.get());

	return true;
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
			lua_pushnil(L);
			return false;
		}
		
		GMetaMapItem * mapItem = findMetaMapItem(userData->getParam()->getMetaMap(), metaClass.get(), name);
		if(mapItem == NULL) {
			continue;
		}
		
		switch(mapItem->getType()) {
			case mmitField:
			case mmitProperty: {
				GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));
				if(allowAccessData(userData, data.get())) {
					if(indexMemberData(L, userData, data.get(), instance)) {
						return true;
					}
					else {
						lua_pushnil(L);
						return false;
					}
				}
			}
			   break;

			case mmitMethod:
			case mmitMethodList: {
				GScopedInterface<IMetaList> metaList(createMetaList());
				loadMethodList(&traveller, metaList.get(), userData->getParam()->getMetaMap(), mapItem, instance, userData, name);
				doBindMethodList(L, userData->getParam(), metaList.get(), udmtInternal);
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

	LEAVE_LUA(L, lua_pushnil(L); return false)
}

bool newindexMemberData(lua_State * L, GClassUserData * userData, const char * name, const GVariant & value)
{
	(void)L;

	if(userData->cv == opcvConst) {
		raiseCoreException(Error_ScriptBinding_CantWriteToConstObject);

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
		
		GScopedInterface<IMetaAccessible> data(gdynamic_cast<IMetaAccessible *>(mapItem->getItem()));

		if(allowAccessData(userData, data.get())) {
			GVariantData varData = value.getData();
			data->set(instance, &varData);
			metaCheckError(data);

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

/* No need to setup GC as long as GOperatorUserData doesn't need to free any resource.
			lua_newtable(L);
			setMetaTableSignature(L);
			setMetaTableGC(L);
			lua_setmetatable(L, -2);
*/

			lua_pushcclosure(L, &UserData_operator, 1);
			lua_rawset(L, -3);

			return;
		}
	}
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

void doBindClass(lua_State * L, GScriptBindingParam * param, IMetaClass * metaClass)
{
	void * userData = lua_newuserdata(L, sizeof(GClassUserData));
	new (userData) GClassUserData(param, metaClass, NULL, false, false, opcvNone, cudtNormal);

	lua_newtable(L);

	setMetaTableSignature(L);
	setMetaTableGC(L);
	setMetaTableCall(L, userData);

	initObjectMetaTable(L);

	lua_setmetatable(L, -2);
}

void doBindMethodList(lua_State * L, GScriptBindingParam * param, IMetaList * methodList, GUserDataMethodType methodType)
{
	void * userData = lua_newuserdata(L, sizeof(GMethodListUserData));
	new (userData) GMethodListUserData(param, methodList, methodType);
	
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
		raiseCoreException(Error_ScriptBinding_CantFindEnumKey, name);
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

	raiseCoreException(Error_ScriptBinding_CantAssignToEnumMethodClass);

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


GLuaScriptObjectImplement::GLuaScriptObjectImplement(GScriptBindingParam * param, GLuaScriptObject * binding, lua_State * L, int objectIndex, bool freeResource)
		: param(param), binding(binding), luaState(L), ref(LUA_NOREF), freeResource(freeResource)
{
	if(objectIndex != 0) {
		this->ref = refLua(this->luaState, objectIndex);
	}
}
	
GLuaScriptObjectImplement::~GLuaScriptObjectImplement()
{
	if(this->ref != LUA_NOREF) {
		unrefLua(this->luaState, this->ref);
	}

	if(this->freeResource) {
		delete this->param;
	}
}
	
bool GLuaScriptObjectImplement::isGlobal() const
{
	return this->ref == LUA_NOREF;
}

void GLuaScriptObjectImplement::getTable() const
{
	if(this->ref != LUA_NOREF) {
		getRefObject(this->luaState, this->ref);
	}
}

GMethodListUserData * GLuaScriptObjectImplement::doGetMethodUserData()
{
	if(lua_type(this->luaState, -1) != LUA_TFUNCTION) {
		return NULL;
	}

	lua_getupvalue(this->luaState, -1, 1);
	if(lua_isnil(this->luaState, -1)) {
		lua_pop(this->luaState, 1);
	}
	else {
		void * rawUserData = lua_touserdata(this->luaState, -1);
		GScriptUserData * userData = static_cast<GScriptUserData *>(rawUserData);

		if(userData->getType() == udtMethodList) {
			GMethodListUserData * methodListData = static_cast<GMethodListUserData *>(userData);
			return methodListData;
		}
	}

	return NULL;
}

GLuaGlobalAccessor * GLuaScriptObjectImplement::getGlobalAccessor()
{
	if(! this->globalAccessor) {
		this->globalAccessor.reset(new GLuaGlobalAccessor(this->binding));
	}

	return this->globalAccessor.get();
}


GLuaScopeGuard::GLuaScopeGuard(GScriptObject * scope)
	: scope(gdynamic_cast<GLuaScriptObject *>(scope))
{
	this->top = lua_gettop(this->scope->implement->luaState);
	if(! this->scope->isGlobal()) {
		this->scope->implement->getTable();
	}
}
	
GLuaScopeGuard::~GLuaScopeGuard()
{
	if(this->top >= 0) {
		int currentTop = lua_gettop(this->scope->implement->luaState);
		if(currentTop > this->top) {
			lua_pop(this->scope->implement->luaState, currentTop - this->top);
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
		lua_setglobal(this->scope->implement->luaState, name);
	}
	else {
		lua_setfield(this->scope->implement->luaState, -2, name);
	}
}
	
void GLuaScopeGuard::get(const char * name)
{
	if(scope->isGlobal()) {
		lua_getglobal(this->scope->implement->luaState, name);
	}
	else {
		lua_getfield(this->scope->implement->luaState, -1, name);
	}
}

void GLuaScopeGuard::rawGet(const char * name)
{
	lua_pushstring(this->scope->implement->luaState, name);

	if(scope->isGlobal()) {
		lua_rawget(this->scope->implement->luaState, LUA_GLOBALSINDEX);
	}
	else {
		lua_rawget(this->scope->implement->luaState, -2);
	}
}

// function is on stack top
GMetaVariant invokeLuaFunctionIndirectly(lua_State * L, GScriptBindingParam * bindingParam, GMetaVariant const * const * params, size_t paramCount, const char * name)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	int top = lua_gettop(L) - 1;

	if(lua_isfunction(L, -1)) {
		for(size_t i = 0; i < paramCount; ++i) {
			if(!variantToLua(L, bindingParam, params[i]->getValue(), params[i]->getType(), false, true)) {
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
					return luaToVariant(L, bindingParam, -1);
				}
			}
		}
	}
	else {
		raiseCoreException(Error_ScriptBinding_CantCallNonfunction);
	}
	
	return GMetaVariant();
}


GLuaScriptFunction::GLuaScriptFunction(lua_State * L, GScriptBindingParam * bindingParam, int objectIndex)
	: luaState(L), bindingParam(bindingParam), ref(refLua(L, objectIndex))
{
}

GLuaScriptFunction::~GLuaScriptFunction()
{
	unrefLua(this->luaState, this->ref);
}
	
GMetaVariant GLuaScriptFunction::invoke(const GMetaVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GMetaVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(variantPointers, paramCount);
}

GMetaVariant GLuaScriptFunction::invokeIndirectly(GMetaVariant const * const * params, size_t paramCount)
{
	ENTER_LUA()

	getRefObject(this->luaState, this->ref);

	return invokeLuaFunctionIndirectly(this->luaState, this->bindingParam, params, paramCount, "");
	
	LEAVE_LUA(this->luaState, return GMetaVariant())
}


GLuaScriptObject::GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config)
	: super(config)
{
	this->implement.reset(new GLuaScriptObjectImplement(new GScriptBindingParam(service, super::getConfig()), this, L, 0, true));
}

GLuaScriptObject::GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config, int objectIndex)
	: super(config)
{
	this->implement.reset(new GLuaScriptObjectImplement(new GScriptBindingParam(service, super::getConfig()), this, L, objectIndex, true));
}

GLuaScriptObject::GLuaScriptObject(const GLuaScriptObject & other)
	: super(other.implement->param->getConfig())
{
	this->implement.reset(new GLuaScriptObjectImplement(other.implement->param, this, other.implement->luaState, -1, false));
}

GLuaScriptObject::~GLuaScriptObject()
{
}

bool GLuaScriptObject::isGlobal() const
{
	return this->implement->isGlobal();
}

GScriptDataType GLuaScriptObject::getType(const char * name, IMetaTypedItem ** outMetaTypeItem)
{
	ENTER_LUA()
	
	GLuaScopeGuard scopeGuard(this);
	
	scopeGuard.get(name);

	return getLuaType(this->implement->luaState, -1, outMetaTypeItem);
	
	LEAVE_LUA(this->implement->luaState, return sdtUnknown)
}

void GLuaScriptObject::bindClass(const char * name, IMetaClass * metaClass)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	doBindClass(this->implement->luaState, this->implement->param, metaClass);
	
	scopeGuard.set(name);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindEnum(const char * name, IMetaEnum * metaEnum)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	doBindEnum(this->implement->luaState, this->implement->param, metaEnum);
	
	scopeGuard.set(name);

	LEAVE_LUA(this->implement->luaState)
}

GScriptObject * GLuaScriptObject::createScriptObject(const char * name)
{
	ENTER_LUA()
	
	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	if(lua_isnil(this->implement->luaState, -1)) {
		lua_pop(this->implement->luaState, 1);
		lua_newtable(this->implement->luaState);
		scopeGuard.set(name);
		scopeGuard.get(name);
	}
	else {
		if(isValidMetaTable(this->implement->luaState, -1)) {
			lua_pop(this->implement->luaState, 1);
			return NULL;
		}
	}

	GLuaScriptObject * binding = new GLuaScriptObject(*this);
	binding->owner = this;
	binding->name = name;
	
	return binding;

	LEAVE_LUA(this->implement->luaState, return NULL)
}

GScriptObject * GLuaScriptObject::gainScriptObject(const char * name)
{
	ENTER_LUA()
	
	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	if(lua_isnil(this->implement->luaState, -1)) {
		lua_pop(this->implement->luaState, 1);
		
		return NULL;
	}

	if(isValidMetaTable(this->implement->luaState, -1)) {
		lua_pop(this->implement->luaState, 1);
		
		return NULL;
	}

	GLuaScriptObject * binding = new GLuaScriptObject(*this);
	binding->owner = this;
	binding->name = name;
	
	return binding;

	LEAVE_LUA(this->implement->luaState, return NULL)
}

GScriptFunction * GLuaScriptObject::gainScriptFunction(const char * name)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	return new GLuaScriptFunction(this->getLuaState(), this->getParam(), -1);
	
	LEAVE_LUA(this->implement->luaState, return NULL)
}

GMetaVariant GLuaScriptObject::invoke(const char * name, const GMetaVariant * params, size_t paramCount)
{
	GASSERT_MSG(paramCount <= REF_MAX_ARITY, "Too many parameters.");

	const cpgf::GMetaVariant * variantPointers[REF_MAX_ARITY];

	for(size_t i = 0; i < paramCount; ++i) {
		variantPointers[i] = &params[i];
	}

	return this->invokeIndirectly(name, variantPointers, paramCount);
}

GMetaVariant GLuaScriptObject::invokeIndirectly(const char * name, GMetaVariant const * const * params, size_t paramCount)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	return invokeLuaFunctionIndirectly(this->getLuaState(), this->getParam(), params, paramCount, name);
	
	LEAVE_LUA(this->implement->luaState, return GMetaVariant())
}

void GLuaScriptObject::bindFundamental(const char * name, const GVariant & value)
{
	GASSERT_MSG(vtIsFundamental(vtGetType(value.data.typeData)), "Only fundamental value can be bound via bindFundamental");

	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	if(! variantToLua(this->implement->luaState, this->implement->param, value, GMetaType(), false, true)) {
		raiseCoreException(Error_ScriptBinding_CantBindFundamental);
	}
	
	scopeGuard.set(name);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindAccessible(const char * name, void * instance, IMetaAccessible * accessible)
{
	ENTER_LUA()

	this->implement->getGlobalAccessor()->doBindAccessible(name, instance, accessible);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindString(const char * stringName, const char * s)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	lua_pushstring(this->implement->luaState, s);

	scopeGuard.set(stringName);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindObject(const char * objectName, void * instance, IMetaClass * type, bool transferOwnership)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	objectToLua(this->implement->luaState, this->implement->param, instance, gdynamic_cast<IMetaClass *>(type), transferOwnership, opcvNone, cudtNormal);

	scopeGuard.set(objectName);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindRaw(const char * name, const GVariant & value)
{
	GASSERT_MSG(variantIsRawData(vtGetType(value.data.typeData)), "Only raw data (pointer, object) can be bound via bindRaw");

	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	if(! rawToLua(this->implement->luaState, this->implement->param, value)) {
		raiseCoreException(Error_ScriptBinding_CantBindRaw);
	}
	
	scopeGuard.set(name);

	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindMethod(const char * name, void * instance, IMetaMethod * method)
{
	ENTER_LUA()

	if(method->isStatic()) {
		instance = NULL;
	}

	GLuaScopeGuard scopeGuard(this);
	
	GScopedInterface<IMetaList> methodList(createMetaList());
	methodList->add(method, instance);

	doBindMethodList(this->implement->luaState, this->implement->param, methodList.get(), udmtMethod);
	
	scopeGuard.set(name);
	
	LEAVE_LUA(this->implement->luaState)
}

void GLuaScriptObject::bindMethodList(const char * name, IMetaList * methodList)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	doBindMethodList(this->implement->luaState, this->implement->param, methodList, udmtMethodList);
	
	scopeGuard.set(name);
	
	LEAVE_LUA(this->implement->luaState)
}

IMetaClass * GLuaScriptObject::getClass(const char * className)
{
	IMetaTypedItem * typedItem = NULL;

	GScriptDataType sdt = this->getType(className, &typedItem);
	GScopedInterface<IMetaTypedItem> item(typedItem);
	if(sdt == sdtClass) {
		return gdynamic_cast<IMetaClass *>(item.take());
	}

	return NULL;
}

IMetaEnum * GLuaScriptObject::getEnum(const char * enumName)
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

GVariant GLuaScriptObject::getFundamental(const char * name)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);
	
	if(getLuaType(this->implement->luaState, -1, NULL) == sdtFundamental) {
		return luaToVariant(this->implement->luaState, this->implement->param, -1).getValue();
	}
	else {
		lua_pop(this->implement->luaState, 1);
		
		return GVariant();
	}

	LEAVE_LUA(this->implement->luaState, return GVariant())
}

std::string GLuaScriptObject::getString(const char * stringName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(stringName);

	return lua_tostring(this->implement->luaState, -1);
	
	LEAVE_LUA(this->implement->luaState, return "")
}

void * GLuaScriptObject::getObject(const char * objectName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(objectName);

	return luaToObject(this->implement->luaState, this->implement->param, -1, NULL);
	
	LEAVE_LUA(this->implement->luaState, return NULL)
}

GVariant GLuaScriptObject::getRaw(const char * name)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);
	
	if(getLuaType(this->implement->luaState, -1, NULL) == sdtRaw) {
		return luaToVariant(this->implement->luaState, this->implement->param, -1).getValue();
	}
	else {
		lua_pop(this->implement->luaState, 1);
		
		return GVariant();
	}

	LEAVE_LUA(this->implement->luaState, return GVariant())
}

IMetaMethod * GLuaScriptObject::getMethod(const char * methodName, void ** outInstance)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(methodName);
	
	if(outInstance != NULL) {
		*outInstance = NULL;
	}

	GMethodListUserData * userData = this->implement->doGetMethodUserData();
	if(userData != NULL && userData->methodType == udmtMethod) {
		if(outInstance != NULL) {
			*outInstance = userData->methodList->getInstanceAt(0);
		}

		return gdynamic_cast<IMetaMethod *>(userData->methodList->getAt(0));
	}
	else {
		return NULL;
	}

	
	LEAVE_LUA(this->implement->luaState, return NULL)
}

IMetaList * GLuaScriptObject::getMethodList(const char * methodName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(methodName);

	GMethodListUserData * userData = this->implement->doGetMethodUserData();
	if(userData != NULL && userData->methodType == udmtMethodList) {
		userData->methodList->addReference();

		return userData->methodList;
	}
	else {
		return NULL;
	}

	LEAVE_LUA(this->implement->luaState, return NULL)
}

void GLuaScriptObject::assignValue(const char * fromName, const char * toName)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(fromName);
	scopeGuard.set(toName);

	LEAVE_LUA(this->implement->luaState)
}

bool GLuaScriptObject::valueIsNull(const char * name)
{
	ENTER_LUA()

	GLuaScopeGuard scopeGuard(this);

	scopeGuard.get(name);

	return lua_isnil(this->implement->luaState, -1);

	LEAVE_LUA(this->implement->luaState, return false)
}

void GLuaScriptObject::nullifyValue(const char * name)
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

