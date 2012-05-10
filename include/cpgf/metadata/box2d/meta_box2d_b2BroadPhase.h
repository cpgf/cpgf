// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2BROADPHASE_H
#define __META_BOX2D_B2BROADPHASE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2broadphase(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2PairLessThan", _r), (bool (*) (const b2Pair &, const b2Pair &))&b2PairLessThan);
}


template <typename D>
void buildMetaClass_B2BroadPhase(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateProxy", _r), &D::ClassType::CreateProxy);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DestroyProxy", _r), &D::ClassType::DestroyProxy);
    _d.CPGF_MD_TEMPLATE _method(replaceName("MoveProxy", _r), &D::ClassType::MoveProxy);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFatAABB", _r), &D::ClassType::GetFatAABB);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUserData", _r), &D::ClassType::GetUserData);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TestOverlap", _r), &D::ClassType::TestOverlap);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetProxyCount", _r), &D::ClassType::GetProxyCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeHeight", _r), &D::ClassType::ComputeHeight);
    _d.CPGF_MD_TEMPLATE _enum<long long>(replaceName("GlobalEnum_box2d_2", _r))
        ._element(replaceName("e_nullProxy", _r), D::ClassType::e_nullProxy)
    ;
}


template <typename D>
void buildMetaClass_B2Pair(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyIdA", _r), &D::ClassType::proxyIdA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyIdB", _r), &D::ClassType::proxyIdB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("next", _r), &D::ClassType::next);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
