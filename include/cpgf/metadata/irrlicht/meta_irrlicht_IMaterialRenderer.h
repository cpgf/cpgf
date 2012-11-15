// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IMATERIALRENDERER_H
#define __META_IRRLICHT_IMATERIALRENDERER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IMaterialRenderer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("OnSetMaterial", &D::ClassType::OnSetMaterial);
    _d.CPGF_MD_TEMPLATE _method("OnRender", &D::ClassType::OnRender);
    _d.CPGF_MD_TEMPLATE _method("OnUnsetMaterial", &D::ClassType::OnUnsetMaterial);
    _d.CPGF_MD_TEMPLATE _method("isTransparent", &D::ClassType::isTransparent);
    _d.CPGF_MD_TEMPLATE _method("getRenderCapability", &D::ClassType::getRenderCapability);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
