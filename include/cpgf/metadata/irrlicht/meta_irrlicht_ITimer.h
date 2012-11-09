// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ITIMER_H
#define __META_IRRLICHT_ITIMER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ITimer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getRealTime", &D::ClassType::getRealTime);
    _d.CPGF_MD_TEMPLATE _method("getTime", &D::ClassType::getTime);
    _d.CPGF_MD_TEMPLATE _method("setTime", &D::ClassType::setTime);
    _d.CPGF_MD_TEMPLATE _method("stop", &D::ClassType::stop);
    _d.CPGF_MD_TEMPLATE _method("start", &D::ClassType::start);
    _d.CPGF_MD_TEMPLATE _method("setSpeed", &D::ClassType::setSpeed)
        ._default(copyVariantFromCopyable(1.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSpeed", &D::ClassType::getSpeed);
    _d.CPGF_MD_TEMPLATE _method("isStopped", &D::ClassType::isStopped);
    _d.CPGF_MD_TEMPLATE _method("tick", &D::ClassType::tick);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
