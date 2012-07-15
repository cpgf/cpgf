// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2BROADPHASE_H
#define __META_BOX2D_B2BROADPHASE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2broadphase(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("b2PairLessThan", (bool (*) (const b2Pair &, const b2Pair &))&b2PairLessThan);
}


template <typename D>
void buildMetaClass_B2BroadPhase(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("CreateProxy", &D::ClassType::CreateProxy);
    _d.CPGF_MD_TEMPLATE _method("DestroyProxy", &D::ClassType::DestroyProxy);
    _d.CPGF_MD_TEMPLATE _method("MoveProxy", &D::ClassType::MoveProxy);
    _d.CPGF_MD_TEMPLATE _method("GetFatAABB", &D::ClassType::GetFatAABB);
    _d.CPGF_MD_TEMPLATE _method("GetUserData", &D::ClassType::GetUserData);
    _d.CPGF_MD_TEMPLATE _method("TestOverlap", &D::ClassType::TestOverlap);
    _d.CPGF_MD_TEMPLATE _method("GetProxyCount", &D::ClassType::GetProxyCount);
    _d.CPGF_MD_TEMPLATE _method("ComputeHeight", &D::ClassType::ComputeHeight);
    _d.CPGF_MD_TEMPLATE _enum<long long>("GlobalEnum_box2d_2")
        ._element("e_nullProxy", D::ClassType::e_nullProxy)
    ;
}


template <typename D>
void buildMetaClass_B2Pair(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("proxyIdA", &D::ClassType::proxyIdA);
    _d.CPGF_MD_TEMPLATE _field("proxyIdB", &D::ClassType::proxyIdB);
    _d.CPGF_MD_TEMPLATE _field("next", &D::ClassType::next);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
