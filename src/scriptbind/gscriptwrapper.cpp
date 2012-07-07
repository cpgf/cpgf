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
	
	virtual void G_API_CC setScriptObject(void * instance, IScriptObject * scriptObject) {
		static_cast<GScriptWrapper *>(instance)->setScriptObject(scriptObject);
	}

private:
	const GScriptWrapper & wrapper;	

	GMAKE_NONCOPYABLE(GMetaScriptWrapper);
};


}

IScriptFunction * GScriptWrapper::getScriptFunction(const char * name)
{
	if(this->scriptObject) {
		return this->scriptObject->gainScriptFunction(name);
	}
	else {
		return NULL;
	}
}

void GScriptWrapper::setScriptObject(IScriptObject * scriptObject)
{
	this->scriptObject.reset(scriptObject);
}


IMetaScriptWrapper * metaTraitsCreateScriptWrapper(const GScriptWrapper & wrapper, const GMetaTraitsParam &)
{
	return new GMetaScriptWrapper(wrapper);
}



} // namespace cpgf




