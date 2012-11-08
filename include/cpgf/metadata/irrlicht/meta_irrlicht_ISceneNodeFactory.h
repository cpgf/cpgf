// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISCENENODEFACTORY_H
#define __META_IRRLICHT_ISCENENODEFACTORY_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ISceneNodeFactory(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("addSceneNode", (ISceneNode * (D::ClassType::*) (ESCENE_NODE_TYPE, ISceneNode *))&D::ClassType::addSceneNode)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addSceneNode", (ISceneNode * (D::ClassType::*) (const c8 *, ISceneNode *))&D::ClassType::addSceneNode)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getCreatableSceneNodeTypeCount", &D::ClassType::getCreatableSceneNodeTypeCount);
    _d.CPGF_MD_TEMPLATE _method("getCreateableSceneNodeType", &D::ClassType::getCreateableSceneNodeType);
    _d.CPGF_MD_TEMPLATE _method("getCreateableSceneNodeTypeName", (const c8 * (D::ClassType::*) (u32) const)&D::ClassType::getCreateableSceneNodeTypeName);
    _d.CPGF_MD_TEMPLATE _method("getCreateableSceneNodeTypeName", (const c8 * (D::ClassType::*) (ESCENE_NODE_TYPE) const)&D::ClassType::getCreateableSceneNodeTypeName);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
