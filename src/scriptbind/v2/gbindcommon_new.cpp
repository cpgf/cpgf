#include "gbindcommon_new.h"


namespace cpgf {

namespace _bind_internal {

GBindingContext::GBindingContext(IMetaService * service, const GScriptConfig & config)
	: service(service), config(config)
{
}

GBindingContext::~GBindingContext()
{
}


} // namespace _bind_internal


} // namespace cpgf



