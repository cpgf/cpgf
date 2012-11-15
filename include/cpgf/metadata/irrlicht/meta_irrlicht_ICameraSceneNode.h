// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ICAMERASCENENODE_H
#define __META_IRRLICHT_ICAMERASCENENODE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ICameraSceneNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("setProjectionMatrix", &D::ClassType::setProjectionMatrix)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("getProjectionMatrix", &D::ClassType::getProjectionMatrix);
    _d.CPGF_MD_TEMPLATE _method("getViewMatrix", &D::ClassType::getViewMatrix);
    _d.CPGF_MD_TEMPLATE _method("setViewMatrixAffector", &D::ClassType::setViewMatrixAffector);
    _d.CPGF_MD_TEMPLATE _method("getViewMatrixAffector", &D::ClassType::getViewMatrixAffector);
    _d.CPGF_MD_TEMPLATE _method("OnEvent", &D::ClassType::OnEvent);
    _d.CPGF_MD_TEMPLATE _method("setTarget", &D::ClassType::setTarget);
    _d.CPGF_MD_TEMPLATE _method("setRotation", &D::ClassType::setRotation);
    _d.CPGF_MD_TEMPLATE _method("getTarget", &D::ClassType::getTarget);
    _d.CPGF_MD_TEMPLATE _method("setUpVector", &D::ClassType::setUpVector);
    _d.CPGF_MD_TEMPLATE _method("getUpVector", &D::ClassType::getUpVector);
    _d.CPGF_MD_TEMPLATE _method("getNearValue", &D::ClassType::getNearValue);
    _d.CPGF_MD_TEMPLATE _method("getFarValue", &D::ClassType::getFarValue);
    _d.CPGF_MD_TEMPLATE _method("getAspectRatio", &D::ClassType::getAspectRatio);
    _d.CPGF_MD_TEMPLATE _method("getFOV", &D::ClassType::getFOV);
    _d.CPGF_MD_TEMPLATE _method("setNearValue", &D::ClassType::setNearValue);
    _d.CPGF_MD_TEMPLATE _method("setFarValue", &D::ClassType::setFarValue);
    _d.CPGF_MD_TEMPLATE _method("setAspectRatio", &D::ClassType::setAspectRatio);
    _d.CPGF_MD_TEMPLATE _method("setFOV", &D::ClassType::setFOV);
    _d.CPGF_MD_TEMPLATE _method("getViewFrustum", &D::ClassType::getViewFrustum);
    _d.CPGF_MD_TEMPLATE _method("setInputReceiverEnabled", &D::ClassType::setInputReceiverEnabled);
    _d.CPGF_MD_TEMPLATE _method("isInputReceiverEnabled", &D::ClassType::isInputReceiverEnabled);
    _d.CPGF_MD_TEMPLATE _method("isOrthogonal", &D::ClassType::isOrthogonal);
    _d.CPGF_MD_TEMPLATE _method("bindTargetAndRotation", &D::ClassType::bindTargetAndRotation);
    _d.CPGF_MD_TEMPLATE _method("getTargetAndRotationBinding", &D::ClassType::getTargetAndRotationBinding);
    _d.CPGF_MD_TEMPLATE _method("serializeAttributes", &D::ClassType::serializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("deserializeAttributes", &D::ClassType::deserializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
