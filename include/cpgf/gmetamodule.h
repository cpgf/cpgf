#ifndef __GMETAMODULE_H
#define __GMETAMODULE_H

#include "cpgf/gscopedptr.h"


namespace cpgf {

class GMetaClass;
class GMetaEnum;
class GMetaTypedItem;
class GTypeInfo;

class GMetaModuleImplement;

class GMetaModule
{
public:
	GMetaModule();
	~GMetaModule();

	void registerMetaClass(const GMetaClass * metaClass);
	void unregisterMetaClass(const GMetaClass * metaClass);
	
	const GMetaTypedItem * findItemByType(const GTypeInfo & type) const;
	const GMetaTypedItem * findItemByName(const char * name) const;

	const GMetaClass * findClassByType(const GTypeInfo & type) const;
	const GMetaClass * findClassByName(const char * name) const;

private:
	void unregisterAll();

	void doRegisterMetaClass(const GMetaClass * metaClass);
	void doRegisterMetaEnum(const GMetaEnum * metaEnum);
	
	void doUnregisterMetaClass(const GMetaClass * metaClass);
	void doUnregisterMetaEnum(const GMetaEnum * metaEnum);
	
private:
	GScopedPointer<GMetaModuleImplement> implement;
};



} // namespace cpgf



#endif
