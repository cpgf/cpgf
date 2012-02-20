// Auto generated file, don't modify.

#ifndef __META_B2BROADPHASE_H
#define __META_B2BROADPHASE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_Global_b2broadphase(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2PairLessThan", _r), (bool (*) (const b2Pair &, const b2Pair &))&b2PairLessThan, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2BroadPhase(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("CreateProxy", _r), &D::ClassType::CreateProxy, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("DestroyProxy", _r), &D::ClassType::DestroyProxy, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("MoveProxy", _r), &D::ClassType::MoveProxy, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetFatAABB", _r), &D::ClassType::GetFatAABB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUserData", _r), &D::ClassType::GetUserData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TestOverlap", _r), &D::ClassType::TestOverlap, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetProxyCount", _r), &D::ClassType::GetProxyCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeHeight", _r), &D::ClassType::ComputeHeight, _p);
    _d.CPGF_MD_TEMPLATE _enum<long long>(replaceName("GlobalEnum_box2d_3", _r))
        ._element(replaceName("e_nullProxy", _r), D::ClassType::e_nullProxy)
    ;
}


template <typename D, typename Policy>
void buildMetaClass_B2Pair(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyIdA", _r), &D::ClassType::proxyIdA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyIdB", _r), &D::ClassType::proxyIdB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("next", _r), &D::ClassType::next, _p);
}


template <typename D, typename Policy>
void buildMetaFile_B2BroadPhase(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    buildMetaClass_Global_b2broadphase(config, _d, _r, _p);
    buildMetaClass_B2BroadPhase(config, _d, _r, _p);
    buildMetaClass_B2Pair(config, _d, _r, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif