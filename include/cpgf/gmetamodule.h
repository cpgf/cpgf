#ifndef CPGF_GMETAMODULE_H
#define CPGF_GMETAMODULE_H

#include "cpgf/gscopedptr.h"


namespace cpgf {

class GMetaItem;
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

	void initializeMetaClasses();

	bool hasInitializedMetaClasses() const {
		return this->initializedMetaClasses;
	}

private:
	void unregisterAll();

	void doRegisterMetaClass(const GMetaClass * metaClass);
	void doRegisterMetaEnum(const GMetaEnum * metaEnum);
	
	void doUnregisterMetaClass(const GMetaClass * metaClass);
	void doUnregisterMetaEnum(const GMetaEnum * metaEnum);
	
private:
	GScopedPointer<GMetaModuleImplement> implement;
	bool initializedMetaClasses;
};

GMetaModule * getItemModule(const GMetaItem * metaItem);


} // namespace cpgf



#endif
