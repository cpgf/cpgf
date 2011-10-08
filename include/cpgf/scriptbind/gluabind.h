#ifndef __GLUABIND_H
#define __GLUABIND_H

#include "cpgf/scriptbind/gscriptbind.h"


#include "lua.hpp"

#include <iostream>
#include <string>


namespace cpgf {

class GLuaScriptObjectImplement;

class GLuaScriptObject : public GScriptObject
{
private:
	typedef GScriptObject super;

public:
	GLuaScriptObject(IMetaService * service, lua_State * L, const GScriptConfig & config);
	virtual ~GLuaScriptObject();

	virtual bool cacheName(GScriptName * name);

	virtual GScriptDataType getType(const GScriptName & name);

	virtual void bindClass(const GScriptName & name, IMetaClass * metaClass);
	virtual void bindEnum(const GScriptName & name, IMetaEnum * metaEnum);

	virtual GScriptObject * createScriptObject(const GScriptName & name);
	
	virtual GVariant invokeIndirectly(const GScriptName & name, GVariant const * const * params, size_t paramCount);
	virtual GVariant invoke(const GScriptName & name, const GVariant * params, size_t paramCount);

	virtual void setFundamental(const GScriptName & name, const GVariant & value);
	virtual void setString(const GScriptName & stringName, const char * s);
	virtual void setObject(const GScriptName & objectName, void * instance, IMetaClass * type, bool transferOwnership);
	virtual void setMethod(const GScriptName & name, void * instance, IMetaMethod * method);
	
	virtual GVariant getFundamental(const GScriptName & name);
	virtual std::string getString(const GScriptName & stringName);
	
	virtual void * getObject(const GScriptName & objectName);
	virtual IMetaClass * getObjectType(const GScriptName & objectName);
	virtual IMetaClass * getClass(const GScriptName & className);
	
	virtual IMetaMethod * getMethod(const GScriptName & methodName);
	virtual IMetaEnum * getEnum(const GScriptName & enumName);
	
	virtual void assignValue(const GScriptName & fromName, const GScriptName & toName);
	virtual bool valueIsNull(const GScriptName & name);
	virtual void nullifyValue(const GScriptName & name);

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
