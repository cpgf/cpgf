// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ILIGHTSCENENODE_H
#define __META_IRRLICHT_ILIGHTSCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ILightSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setLightData", &D::ClassType::setLightData);
    _d.CPGF_MD_TEMPLATE _method("getLightData", (const video::SLight & (D::ClassType::*) () const)&D::ClassType::getLightData);
    _d.CPGF_MD_TEMPLATE _method("getLightData", (video::SLight & (D::ClassType::*) ())&D::ClassType::getLightData);
    _d.CPGF_MD_TEMPLATE _method("setVisible", &D::ClassType::setVisible);
    _d.CPGF_MD_TEMPLATE _method("setRadius", &D::ClassType::setRadius);
    _d.CPGF_MD_TEMPLATE _method("getRadius", &D::ClassType::getRadius);
    _d.CPGF_MD_TEMPLATE _method("setLightType", &D::ClassType::setLightType);
    _d.CPGF_MD_TEMPLATE _method("getLightType", &D::ClassType::getLightType);
    _d.CPGF_MD_TEMPLATE _method("enableCastShadow", &D::ClassType::enableCastShadow)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("getCastShadow", &D::ClassType::getCastShadow);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
