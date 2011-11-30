#ifndef __GV8BIND_H
#define __GV8BIND_H


#include "cpgf/scriptbind/gscriptbind.h"

#include "v8.h"


namespace cpgf {


class GV8ScriptObjectImplement;

GMAKE_FINAL(GV8ScriptObject)

class GV8ScriptObject : public GScriptObject, GFINAL_BASE(GV8ScriptObject)
{
private:
	typedef GScriptObject super;

public:
	GV8ScriptObject(IMetaService * service, v8::Local<v8::Object> object, const GScriptConfig & config);
	GV8ScriptObject(const GV8ScriptObject & other, v8::Local<v8::Object> object);
	virtual ~GV8ScriptObject();

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
	virtual IMetaMethod * getMethod(const GScriptName & methodName, void ** outInstance);
	virtual IMetaList * getMethodList(const GScriptName & methodName);
	
	virtual bool cacheName(GScriptName * name);

	virtual GScriptDataType getType(const GScriptName & name, IMetaTypedItem ** outMetaTypeItem);

	virtual GScriptObject * createScriptObject(const GScriptName & name);
	virtual GScriptObject * getScriptObject(const GScriptName & name);
	
	virtual GMetaVariant invoke(const GScriptName & name, const GMetaVariant * params, size_t paramCount);
	virtual GMetaVariant invokeIndirectly(const GScriptName & name, GMetaVariant const * const * params, size_t paramCount);

	virtual void assignValue(const GScriptName & fromName, const GScriptName & toName);
	virtual bool valueIsNull(const GScriptName & name);
	virtual void nullifyValue(const GScriptName & name);

	virtual void bindAccessible(const GScriptName & name, void * instance, IMetaAccessible * accessible);
private:
	GScopedPointer<GV8ScriptObjectImplement> implement;

private:
};


} // namespace cpgf




#endif


