#ifndef __GLUABIND_H
#define __GLUABIND_H

#include "cpgf/scriptbind/gscriptbind.h"


#include "lua.hpp"

#include <iostream>
#include <string>


namespace cpgf {

class GLuaScriptObjectImplement;

GMAKE_FINAL(GLuaScriptObject)

class GLuaScriptObject : public GScriptObject, GFINAL_BASE(GLuaScriptObject)
{
private:
	typedef GScriptObject super;

public:
	GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config);
	virtual ~GLuaScriptObject();

	virtual void bindClass(const GScriptName & name, IMetaClass * metaClass);
	virtual void bindEnum(const GScriptName & name, IMetaEnum * metaEnum);

	virtual void bindFundamental(const GScriptName & name, const GVariant & value);
	virtual void bindString(const GScriptName & stringName, const char * s);
	virtual void bindObject(const GScriptName & objectName, void * instance, IMetaClass * type, bool transferOwnership);
	virtual void bindMethod(const GScriptName & name, void * instance, IMetaMethod * method);
	virtual void bindMethodList(const GScriptName & name, IMetaList * methodList);

	virtual IMetaClass * getClass(const GScriptName & className);
	virtual IMetaEnum * getEnum(const GScriptName & enumName);

	virtual GVariant getFundamental(const GScriptName & name);
	virtual std::string getString(const GScriptName & stringName);
	virtual void * getObject(const GScriptName & objectName);
	virtual IMetaMethod * getMethod(const GScriptName & methodName);
	virtual IMetaList * getMethodList(const GScriptName & methodName);
	
	virtual bool cacheName(GScriptName * name);

	virtual GScriptDataType getType(const GScriptName & name, IMetaTypedItem ** outMetaTypeItem);

	virtual GScriptObject * createScriptObject(const GScriptName & name);
	
	virtual GMetaVariant invoke(const GScriptName & name, const GMetaVariant * params, size_t paramCount);
	virtual GMetaVariant invokeIndirectly(const GScriptName & name, GMetaVariant const * const * params, size_t paramCount);

	virtual void assignValue(const GScriptName & fromName, const GScriptName & toName);
	virtual bool valueIsNull(const GScriptName & name);
	virtual void nullifyValue(const GScriptName & name);

private:
	GLuaScriptObject(const GLuaScriptObject & other);
	
private:
	GScopedPointer<GLuaScriptObjectImplement> implement;

private:
	friend class GLuaScopeGuard;
	friend class GLuaScriptObjectImplement;
	friend class GLuaScriptNameData;
};


IScriptObject * createLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config);



} // namespace cpgf



#endif
