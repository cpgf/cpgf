// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IVIDEOMODELIST_H
#define __META_IRRLICHT_IVIDEOMODELIST_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IVideoModeList(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getVideoModeCount", &D::ClassType::getVideoModeCount);
    _d.CPGF_MD_TEMPLATE _method("getVideoModeResolution", (core::dimension2d< u32 > (D::ClassType::*) (s32) const)&D::ClassType::getVideoModeResolution);
    _d.CPGF_MD_TEMPLATE _method("getVideoModeResolution", (core::dimension2d< u32 > (D::ClassType::*) (const core::dimension2d< u32 > &, const core::dimension2d< u32 > &) const)&D::ClassType::getVideoModeResolution);
    _d.CPGF_MD_TEMPLATE _method("getVideoModeDepth", &D::ClassType::getVideoModeDepth);
    _d.CPGF_MD_TEMPLATE _method("getDesktopResolution", &D::ClassType::getDesktopResolution);
    _d.CPGF_MD_TEMPLATE _method("getDesktopDepth", &D::ClassType::getDesktopDepth);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
