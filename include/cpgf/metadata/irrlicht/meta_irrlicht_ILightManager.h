// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ILIGHTMANAGER_H
#define __META_IRRLICHT_ILIGHTMANAGER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ILightManager(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("OnNodePostRender", &D::ClassType::OnNodePostRender);
    _d.CPGF_MD_TEMPLATE _method("OnNodePreRender", &D::ClassType::OnNodePreRender);
    _d.CPGF_MD_TEMPLATE _method("OnPostRender", &D::ClassType::OnPostRender);
    _d.CPGF_MD_TEMPLATE _method("OnPreRender", &D::ClassType::OnPreRender);
    _d.CPGF_MD_TEMPLATE _method("OnRenderPassPostRender", &D::ClassType::OnRenderPassPostRender);
    _d.CPGF_MD_TEMPLATE _method("OnRenderPassPreRender", &D::ClassType::OnRenderPassPreRender);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
