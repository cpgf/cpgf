// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISCENENODEANIMATORCOLLISIONRESPONSE_H
#define __META_IRRLICHT_ISCENENODEANIMATORCOLLISIONRESPONSE_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ICollisionCallback(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("onCollision", &D::ClassType::onCollision);
}


template <typename D>
void buildMetaClass_ISceneNodeAnimatorCollisionResponse(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("isFalling", &D::ClassType::isFalling);
    _d.CPGF_MD_TEMPLATE _method("setEllipsoidRadius", &D::ClassType::setEllipsoidRadius);
    _d.CPGF_MD_TEMPLATE _method("getEllipsoidRadius", &D::ClassType::getEllipsoidRadius);
    _d.CPGF_MD_TEMPLATE _method("setGravity", &D::ClassType::setGravity);
    _d.CPGF_MD_TEMPLATE _method("getGravity", &D::ClassType::getGravity);
    _d.CPGF_MD_TEMPLATE _method("jump", &D::ClassType::jump);
    _d.CPGF_MD_TEMPLATE _method("setAnimateTarget", &D::ClassType::setAnimateTarget);
    _d.CPGF_MD_TEMPLATE _method("getAnimateTarget", &D::ClassType::getAnimateTarget);
    _d.CPGF_MD_TEMPLATE _method("setEllipsoidTranslation", &D::ClassType::setEllipsoidTranslation);
    _d.CPGF_MD_TEMPLATE _method("getEllipsoidTranslation", &D::ClassType::getEllipsoidTranslation);
    _d.CPGF_MD_TEMPLATE _method("setWorld", &D::ClassType::setWorld);
    _d.CPGF_MD_TEMPLATE _method("getWorld", &D::ClassType::getWorld);
    _d.CPGF_MD_TEMPLATE _method("setTargetNode", &D::ClassType::setTargetNode);
    _d.CPGF_MD_TEMPLATE _method("getTargetNode", &D::ClassType::getTargetNode);
    _d.CPGF_MD_TEMPLATE _method("collisionOccurred", &D::ClassType::collisionOccurred);
    _d.CPGF_MD_TEMPLATE _method("getCollisionPoint", &D::ClassType::getCollisionPoint);
    _d.CPGF_MD_TEMPLATE _method("getCollisionTriangle", &D::ClassType::getCollisionTriangle);
    _d.CPGF_MD_TEMPLATE _method("getCollisionResultPosition", &D::ClassType::getCollisionResultPosition);
    _d.CPGF_MD_TEMPLATE _method("getCollisionNode", &D::ClassType::getCollisionNode);
    _d.CPGF_MD_TEMPLATE _method("setCollisionCallback", &D::ClassType::setCollisionCallback);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
