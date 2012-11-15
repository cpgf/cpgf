// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IPARTICLEANIMATEDMESHSCENENODEEMITTER_H
#define __META_IRRLICHT_IPARTICLEANIMATEDMESHSCENENODEEMITTER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IParticleAnimatedMeshSceneNodeEmitter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setAnimatedMeshSceneNode", &D::ClassType::setAnimatedMeshSceneNode);
    _d.CPGF_MD_TEMPLATE _method("setUseNormalDirection", &D::ClassType::setUseNormalDirection)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("setNormalDirectionModifier", &D::ClassType::setNormalDirectionModifier);
    _d.CPGF_MD_TEMPLATE _method("setEveryMeshVertex", &D::ClassType::setEveryMeshVertex)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("getAnimatedMeshSceneNode", &D::ClassType::getAnimatedMeshSceneNode);
    _d.CPGF_MD_TEMPLATE _method("isUsingNormalDirection", &D::ClassType::isUsingNormalDirection);
    _d.CPGF_MD_TEMPLATE _method("getNormalDirectionModifier", &D::ClassType::getNormalDirectionModifier);
    _d.CPGF_MD_TEMPLATE _method("getEveryMeshVertex", &D::ClassType::getEveryMeshVertex);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
