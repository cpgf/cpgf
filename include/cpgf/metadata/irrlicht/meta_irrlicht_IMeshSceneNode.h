// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IMESHSCENENODE_H
#define __META_IRRLICHT_IMESHSCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IMeshSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setMesh", &D::ClassType::setMesh);
    _d.CPGF_MD_TEMPLATE _method("getMesh", &D::ClassType::getMesh);
    _d.CPGF_MD_TEMPLATE _method("addShadowVolumeSceneNode", &D::ClassType::addShadowVolumeSceneNode)
        ._default(copyVariantFromCopyable(1000.0f))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("setReadOnlyMaterials", &D::ClassType::setReadOnlyMaterials);
    _d.CPGF_MD_TEMPLATE _method("isReadOnlyMaterials", &D::ClassType::isReadOnlyMaterials);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
