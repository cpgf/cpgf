#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gapiutil.h"


namespace cpgf {


GScriptWrapper::GScriptWrapper()
	: scriptDataStorage(nullptr)
{
}

IScriptFunction * GScriptWrapper::getScriptFunction(const char * name) const
{
	if(this->scriptDataStorage) {
		return const_cast<IScriptDataStorage *>(this->scriptDataStorage.get())->getScriptFunction(name);
	}
	else {
		return nullptr;
	}
}

void GScriptWrapper::setScriptDataStorage(IScriptDataStorage * scriptDataStorage)
{
	this->scriptDataStorage.reset(scriptDataStorage);
}


namespace scriptbind_internal {

class GMetaScriptWrapper : public IMetaScriptWrapper
{
	G_INTERFACE_IMPL_OBJECT

public:
	explicit GMetaScriptWrapper(CasterType caster) : caster(caster) {
	}

	virtual ~GMetaScriptWrapper() {
	}

	virtual void G_API_CC setScriptDataStorage(void * instance, IScriptDataStorage * scriptDataStorage) {
		static_cast<GScriptWrapper *>(this->caster(instance))->setScriptDataStorage(scriptDataStorage);
	}

private:
	CasterType caster;

	GMAKE_NONCOPYABLE(GMetaScriptWrapper);
};


IMetaScriptWrapper * doCreateScriptWrapper(CasterType caster)
{
	return new GMetaScriptWrapper(caster);
}

} // namespace scriptbind_internal



} // namespace cpgf


