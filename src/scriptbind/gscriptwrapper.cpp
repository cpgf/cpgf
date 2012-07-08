#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gapiutil.h"


namespace cpgf {


namespace {

class GMetaScriptWrapper : public IMetaScriptWrapper
{
	G_INTERFACE_IMPL_OBJECT

public:
	explicit GMetaScriptWrapper(const GScriptWrapper & wrapper) : wrapper(wrapper) {
	}
	
	virtual void G_API_CC setScriptDataStorage(void * instance, IScriptDataStorage * scriptDataStorage) {
		static_cast<GScriptWrapper *>(instance)->setScriptDataStorage(scriptDataStorage);
	}

private:
	const GScriptWrapper & wrapper;	

	GMAKE_NONCOPYABLE(GMetaScriptWrapper);
};


}

IScriptFunction * GScriptWrapper::getScriptFunction(const char * name)
{
	if(this->scriptDataStorage) {
		return this->scriptDataStorage->getScriptFunction(name);
	}
	else {
		return NULL;
	}
}

void GScriptWrapper::setScriptDataStorage(IScriptDataStorage * scriptDataStorage)
{
	this->scriptDataStorage.reset(scriptDataStorage);
}


IMetaScriptWrapper * metaTraitsCreateScriptWrapper(const GScriptWrapper & wrapper, const GMetaTraitsParam &)
{
	return new GMetaScriptWrapper(wrapper);
}



} // namespace cpgf




