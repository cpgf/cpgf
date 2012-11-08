// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISCENENODEANIMATORFACTORY_H
#define __META_IRRLICHT_ISCENENODEANIMATORFACTORY_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ISceneNodeAnimatorFactory(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("createSceneNodeAnimator", (ISceneNodeAnimator * (D::ClassType::*) (ESCENE_NODE_ANIMATOR_TYPE, ISceneNode *))&D::ClassType::createSceneNodeAnimator);
    _d.CPGF_MD_TEMPLATE _method("createSceneNodeAnimator", (ISceneNodeAnimator * (D::ClassType::*) (const c8 *, ISceneNode *))&D::ClassType::createSceneNodeAnimator);
    _d.CPGF_MD_TEMPLATE _method("getCreatableSceneNodeAnimatorTypeCount", &D::ClassType::getCreatableSceneNodeAnimatorTypeCount);
    _d.CPGF_MD_TEMPLATE _method("getCreateableSceneNodeAnimatorType", &D::ClassType::getCreateableSceneNodeAnimatorType);
    _d.CPGF_MD_TEMPLATE _method("getCreateableSceneNodeAnimatorTypeName", (const c8 * (D::ClassType::*) (u32) const)&D::ClassType::getCreateableSceneNodeAnimatorTypeName);
    _d.CPGF_MD_TEMPLATE _method("getCreateableSceneNodeAnimatorTypeName", (const c8 * (D::ClassType::*) (ESCENE_NODE_ANIMATOR_TYPE) const)&D::ClassType::getCreateableSceneNodeAnimatorTypeName);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
