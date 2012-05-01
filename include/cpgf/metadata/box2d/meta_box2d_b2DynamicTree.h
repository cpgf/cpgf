// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2DYNAMICTREE_H
#define __META_BOX2D_B2DYNAMICTREE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_Global_b2dynamictree(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<long long>(replaceName("GlobalDefine_box2d_3", _r))
        ._element(replaceName("b2_nullNode", _r), b2_nullNode)
    ;
}


template <typename D, typename Policy>
void buildMetaClass_B2DynamicTree(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateProxy", _r), &D::ClassType::CreateProxy, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DestroyProxy", _r), &D::ClassType::DestroyProxy, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("MoveProxy", _r), &D::ClassType::MoveProxy, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Rebalance", _r), &D::ClassType::Rebalance, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUserData", _r), &D::ClassType::GetUserData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFatAABB", _r), &D::ClassType::GetFatAABB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeHeight", _r), (int32 (D::ClassType::*) () const)&D::ClassType::ComputeHeight, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2DynamicTreeNode(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("aabb", _r), &D::ClassType::aabb, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("userData", _r), &D::ClassType::userData, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("parent", _r), &D::ClassType::parent, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("next", _r), &D::ClassType::next, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("child1", _r), &D::ClassType::child1, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("child2", _r), &D::ClassType::child2, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsLeaf", _r), &D::ClassType::IsLeaf, _p);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
