#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/scriptbind/gscriptbindapi.h"
#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/gclassutil.h"
#include "cpgf/gapiutil.h"


namespace cpgf {


GScriptWrapper::GScriptWrapper()
	:
		scriptDataStorage(),
		scriptContext()
{
}

GScriptWrapper::~GScriptWrapper()
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

void GScriptWrapper::initializeScriptWrapper(IScriptDataStorage * scriptDataStorage, IScriptContext * scriptContext)
{
	this->scriptDataStorage.reset(scriptDataStorage);
	this->scriptContext.reset(scriptContext);
}

IScriptContext * GScriptWrapper::getScriptContext() const
{
	IScriptContext * context = this->scriptContext.get();
	context->addReference();
	return context;
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

	virtual void G_API_CC initializeScriptWrapper(void * instance, IScriptDataStorage * scriptDataStorage, IScriptContext * scriptContext) {
		static_cast<GScriptWrapper *>(this->caster(instance))->initializeScriptWrapper(scriptDataStorage, scriptContext);
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


