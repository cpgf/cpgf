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
void buildMetaClass_Global_quaternion(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<long long>("GlobalDefine_irrlicht_9")
    ;
}


inline bool opErAToRWrapper_quaternion__opEqual(const quaternion * self, const quaternion & other) {
    return (*self) == other;
}
inline bool opErAToRWrapper_quaternion__opNotEqual(const quaternion * self, const quaternion & other) {
    return (*self) != other;
}
inline quaternion & opErAToRWrapper_quaternion__opAssign(quaternion * self, const quaternion & other) {
    return (*self) = other;
}
inline quaternion & opErAToRWrapper_quaternion__opAssign(quaternion * self, const matrix4 & other) {
    return (*self) = other;
}
inline quaternion opErAToRWrapper_quaternion__opAdd(const quaternion * self, const quaternion & other) {
    return (*self) + other;
}
inline quaternion opErAToRWrapper_quaternion__opMul(const quaternion * self, const quaternion & other) {
    return (*self) * other;
}
inline quaternion opErAToRWrapper_quaternion__opMul(const quaternion * self, f32 s) {
    return (*self) * s;
}
inline quaternion & opErAToRWrapper_quaternion__opMulAssign(quaternion * self, f32 s) {
    return (*self) *= s;
}
inline vector3df opErAToRWrapper_quaternion__opMul(const quaternion * self, const vector3df & v) {
    return (*self) * v;
}
inline quaternion & opErAToRWrapper_quaternion__opMulAssign(quaternion * self, const quaternion & other) {
    return (*self) *= other;
}


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
    _d.CPGF_MD_TEMPLATE _method("getMatrix", (void (D::ClassType::*) (matrix4 &, const core::vector3df &) const)&D::ClassType::getMatrix)
        ._default(copyVariantFromCopyable(core::vector3df()))
    ;
    _d.CPGF_MD_TEMPLATE _method("getMatrixCenter", &D::ClassType::getMatrixCenter);
    _d.CPGF_MD_TEMPLATE _method("getMatrix_transposed", &D::ClassType::getMatrix_transposed);
    _d.CPGF_MD_TEMPLATE _method("makeInverse", &D::ClassType::makeInverse);
    _d.CPGF_MD_TEMPLATE _method("lerp", &D::ClassType::lerp);
    _d.CPGF_MD_TEMPLATE _method("slerp", &D::ClassType::slerp)
        ._default(copyVariantFromCopyable(.05f))
    ;
    _d.CPGF_MD_TEMPLATE _method("fromAngleAxis", &D::ClassType::fromAngleAxis);
    _d.CPGF_MD_TEMPLATE _method("toAngleAxis", &D::ClassType::toAngleAxis);
    _d.CPGF_MD_TEMPLATE _method("toEuler", &D::ClassType::toEuler);
    _d.CPGF_MD_TEMPLATE _method("makeIdentity", &D::ClassType::makeIdentity);
    _d.CPGF_MD_TEMPLATE _method("rotationFromTo", &D::ClassType::rotationFromTo);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const quaternion &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const quaternion *, const quaternion &))&opErAToRWrapper_quaternion__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const quaternion &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const quaternion *, const quaternion &))&opErAToRWrapper_quaternion__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion & (*)(cpgf::GMetaSelf, const quaternion &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (quaternion & (*) (quaternion *, const quaternion &))&opErAToRWrapper_quaternion__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion & (*)(cpgf::GMetaSelf, const matrix4 &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (quaternion & (*) (quaternion *, const matrix4 &))&opErAToRWrapper_quaternion__opAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion (*)(const cpgf::GMetaSelf &, const quaternion &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAdd", (quaternion (*) (const quaternion *, const quaternion &))&opErAToRWrapper_quaternion__opAdd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion (*)(const cpgf::GMetaSelf &, const quaternion &)>(mopHolder * mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMul", (quaternion (*) (const quaternion *, const quaternion &))&opErAToRWrapper_quaternion__opMul, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion (*)(const cpgf::GMetaSelf &, f32)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMul", (quaternion (*) (const quaternion *, f32))&opErAToRWrapper_quaternion__opMul, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion & (*)(cpgf::GMetaSelf, f32)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (quaternion & (*) (quaternion *, f32))&opErAToRWrapper_quaternion__opMulAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3df (*)(const cpgf::GMetaSelf &, const vector3df &)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMul", (vector3df (*) (const quaternion *, const vector3df &))&opErAToRWrapper_quaternion__opMul, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<quaternion & (*)(cpgf::GMetaSelf, const quaternion &)>(mopHolder *= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (quaternion & (*) (quaternion *, const quaternion &))&opErAToRWrapper_quaternion__opMulAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
