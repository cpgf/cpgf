// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IOSOPERATOR_H
#define __META_IRRLICHT_IOSOPERATOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IOSOperator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getOperatingSystemVersion", &D::ClassType::getOperatingSystemVersion, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("getOperationSystemVersion", &D::ClassType::getOperationSystemVersion);
    _d.CPGF_MD_TEMPLATE _method("copyToClipboard", &D::ClassType::copyToClipboard);
    _d.CPGF_MD_TEMPLATE _method("getTextFromClipboard", &D::ClassType::getTextFromClipboard);
    _d.CPGF_MD_TEMPLATE _method("getProcessorSpeedMHz", &D::ClassType::getProcessorSpeedMHz);
    _d.CPGF_MD_TEMPLATE _method("getSystemMemory", &D::ClassType::getSystemMemory);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
