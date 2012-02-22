// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2TIMEOFIMPACT_H
#define __META_BOX2D_B2TIMEOFIMPACT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_Global_b2timeofimpact(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2TimeOfImpact", _r), (void (*) (b2TOIOutput *, const b2TOIInput *))&b2TimeOfImpact, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2TOIInput(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyA", _r), &D::ClassType::proxyA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyB", _r), &D::ClassType::proxyB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("sweepA", _r), &D::ClassType::sweepA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("sweepB", _r), &D::ClassType::sweepB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("tMax", _r), &D::ClassType::tMax, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2TOIOutput(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("state", _r), &D::ClassType::state, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("t", _r), &D::ClassType::t, _p);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::State>(replaceName("State", _r))
        ._element(replaceName("e_unknown", _r), D::ClassType::e_unknown)
        ._element(replaceName("e_failed", _r), D::ClassType::e_failed)
        ._element(replaceName("e_overlapped", _r), D::ClassType::e_overlapped)
        ._element(replaceName("e_touching", _r), D::ClassType::e_touching)
        ._element(replaceName("e_separated", _r), D::ClassType::e_separated)
    ;
}


} // namespace meta_box2d 




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif