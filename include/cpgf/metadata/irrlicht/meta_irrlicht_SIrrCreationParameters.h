// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SIRRCREATIONPARAMETERS_H
#define __META_IRRLICHT_SIRRCREATIONPARAMETERS_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


inline SIrrlichtCreationParameters & opErAToRWrapper_SIrrlichtCreationParameters__opAssign(SIrrlichtCreationParameters * self, const SIrrlichtCreationParameters & other) {
    return (*self) = other;
}


template <typename D>
void buildMetaClass_SIrrlichtCreationParameters(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const SIrrlichtCreationParameters &)>();
    _d.CPGF_MD_TEMPLATE _field("DeviceType", &D::ClassType::DeviceType);
    _d.CPGF_MD_TEMPLATE _field("DriverType", &D::ClassType::DriverType);
    _d.CPGF_MD_TEMPLATE _field("WindowSize", &D::ClassType::WindowSize);
    _d.CPGF_MD_TEMPLATE _field("Bits", &D::ClassType::Bits);
    _d.CPGF_MD_TEMPLATE _field("ZBufferBits", &D::ClassType::ZBufferBits);
    _d.CPGF_MD_TEMPLATE _field("Fullscreen", &D::ClassType::Fullscreen);
    _d.CPGF_MD_TEMPLATE _field("Stencilbuffer", &D::ClassType::Stencilbuffer);
    _d.CPGF_MD_TEMPLATE _field("Vsync", &D::ClassType::Vsync);
    _d.CPGF_MD_TEMPLATE _field("AntiAlias", &D::ClassType::AntiAlias);
    _d.CPGF_MD_TEMPLATE _field("HandleSRGB", &D::ClassType::HandleSRGB);
    _d.CPGF_MD_TEMPLATE _field("WithAlphaChannel", &D::ClassType::WithAlphaChannel);
    _d.CPGF_MD_TEMPLATE _field("Doublebuffer", &D::ClassType::Doublebuffer);
    _d.CPGF_MD_TEMPLATE _field("IgnoreInput", &D::ClassType::IgnoreInput);
    _d.CPGF_MD_TEMPLATE _field("Stereobuffer", &D::ClassType::Stereobuffer);
    _d.CPGF_MD_TEMPLATE _field("HighPrecisionFPU", &D::ClassType::HighPrecisionFPU);
    _d.CPGF_MD_TEMPLATE _field("EventReceiver", &D::ClassType::EventReceiver);
    _d.CPGF_MD_TEMPLATE _field("WindowId", &D::ClassType::WindowId);
    _d.CPGF_MD_TEMPLATE _field("LoggingLevel", &D::ClassType::LoggingLevel);
    _d.CPGF_MD_TEMPLATE _field("DisplayAdapter", &D::ClassType::DisplayAdapter);
    _d.CPGF_MD_TEMPLATE _field("DriverMultithreaded", &D::ClassType::DriverMultithreaded);
    _d.CPGF_MD_TEMPLATE _field("UsePerformanceTimer", &D::ClassType::UsePerformanceTimer);
    _d.CPGF_MD_TEMPLATE _field("SDK_version_do_not_use", &D::ClassType::SDK_version_do_not_use);
    _d.CPGF_MD_TEMPLATE _operator<SIrrlichtCreationParameters & (*)(cpgf::GMetaSelf, const SIrrlichtCreationParameters &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (SIrrlichtCreationParameters & (*) (SIrrlichtCreationParameters *, const SIrrlichtCreationParameters &))&opErAToRWrapper_SIrrlichtCreationParameters__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
