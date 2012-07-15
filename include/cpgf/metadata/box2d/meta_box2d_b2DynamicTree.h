// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2DYNAMICTREE_H
#define __META_BOX2D_B2DYNAMICTREE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2dynamictree(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<long long>("GlobalDefine_box2d_3")
        ._element("b2_nullNode", b2_nullNode)
    ;
}


template <typename D>
void buildMetaClass_B2DynamicTree(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("CreateProxy", &D::ClassType::CreateProxy);
    _d.CPGF_MD_TEMPLATE _method("DestroyProxy", &D::ClassType::DestroyProxy);
    _d.CPGF_MD_TEMPLATE _method("MoveProxy", &D::ClassType::MoveProxy);
    _d.CPGF_MD_TEMPLATE _method("Rebalance", &D::ClassType::Rebalance);
    _d.CPGF_MD_TEMPLATE _method("GetUserData", &D::ClassType::GetUserData);
    _d.CPGF_MD_TEMPLATE _method("GetFatAABB", &D::ClassType::GetFatAABB);
    _d.CPGF_MD_TEMPLATE _method("ComputeHeight", (int32 (D::ClassType::*) () const)&D::ClassType::ComputeHeight);
}


template <typename D>
void buildMetaClass_B2DynamicTreeNode(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("aabb", &D::ClassType::aabb);
    _d.CPGF_MD_TEMPLATE _field("userData", &D::ClassType::userData);
    _d.CPGF_MD_TEMPLATE _field("parent", &D::ClassType::parent);
    _d.CPGF_MD_TEMPLATE _field("next", &D::ClassType::next);
    _d.CPGF_MD_TEMPLATE _field("child1", &D::ClassType::child1);
    _d.CPGF_MD_TEMPLATE _field("child2", &D::ClassType::child2);
    _d.CPGF_MD_TEMPLATE _method("IsLeaf", &D::ClassType::IsLeaf);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
