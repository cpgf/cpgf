// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_QUATERNION_H
#define __META_IRRLICHT_QUATERNION_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Quaternion(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32, f32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, f32, f32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector3df &)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const matrix4 &)>();
    _d.CPGF_MD_TEMPLATE _field("X", &D::ClassType::X);
    _d.CPGF_MD_TEMPLATE _field("Y", &D::ClassType::Y);
    _d.CPGF_MD_TEMPLATE _field("Z", &D::ClassType::Z);
    _d.CPGF_MD_TEMPLATE _field("W", &D::ClassType::W);
    _d.CPGF_MD_TEMPLATE _method("dotProduct", &D::ClassType::dotProduct, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("set", (quaternion & (D::ClassType::*) (f32, f32, f32, f32))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("set", (quaternion & (D::ClassType::*) (f32, f32, f32))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("set", (quaternion & (D::ClassType::*) (const core::vector3df &))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("set", (quaternion & (D::ClassType::*) (const core::quaternion &))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("equals", &D::ClassType::equals, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_f32))
    ;
    _d.CPGF_MD_TEMPLATE _method("normalize", &D::ClassType::normalize);
    _d.CPGF_MD_TEMPLATE _method("getMatrix", (matrix4 (D::ClassType::*) () const)&D::ClassType::getMatrix);
    _d.CPGF_MD_TEMPLATE _method("getMatrix", (void (D::ClassType::*) (matrix4 &, const core::vector3df &) const)&D::ClassType::getMatrix);
    _d.CPGF_MD_TEMPLATE _method("getMatrixCenter", &D::ClassType::getMatrixCenter);
    _d.CPGF_MD_TEMPLATE _method("getMatrix_transposed", &D::ClassType::getMatrix_transposed);
    _d.CPGF_MD_TEMPLATE _method("makeInverse", &D::ClassType::makeInverse);
    _d.CPGF_MD_TEMPLATE _method("slerp", &D::ClassType::slerp);
    _d.CPGF_MD_TEMPLATE _method("fromAngleAxis", &D::ClassType::fromAngleAxis);
    _d.CPGF_MD_TEMPLATE _method("toAngleAxis", &D::ClassType::toAngleAxis);
    _d.CPGF_MD_TEMPLATE _method("toEuler", &D::ClassType::toEuler);
    _d.CPGF_MD_TEMPLATE _method("makeIdentity", &D::ClassType::makeIdentity);
    _d.CPGF_MD_TEMPLATE _method("rotationFromTo", &D::ClassType::rotationFromTo);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const quaternion &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const quaternion &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion & (*)(cpgf::GMetaSelf, const quaternion &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion & (*)(cpgf::GMetaSelf, const matrix4 &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<quaternion (*)(const cpgf::GMetaSelf &, const quaternion &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion (*)(const cpgf::GMetaSelf &, const quaternion &)>(mopHolder * mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion (*)(const cpgf::GMetaSelf &, f32)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<quaternion & (*)(cpgf::GMetaSelf, f32)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3df (*)(const cpgf::GMetaSelf &, const vector3df &)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<quaternion & (*)(cpgf::GMetaSelf, const quaternion &)>(mopHolder *= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
