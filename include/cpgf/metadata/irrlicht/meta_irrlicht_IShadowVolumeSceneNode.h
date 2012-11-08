// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISHADOWVOLUMESCENENODE_H
#define __META_IRRLICHT_ISHADOWVOLUMESCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IShadowVolumeSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setShadowMesh", &D::ClassType::setShadowMesh);
    _d.CPGF_MD_TEMPLATE _method("updateShadowVolumes", &D::ClassType::updateShadowVolumes);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
