// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2MATH_H
#define __META_BOX2D_B2MATH_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2math(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("b2Vec2_zero", &b2Vec2_zero);
    _d.CPGF_MD_TEMPLATE _field("b2Mat22_identity", &b2Mat22_identity);
    _d.CPGF_MD_TEMPLATE _field("b2Transform_identity", &b2Transform_identity);
    _d.CPGF_MD_TEMPLATE _method("b2IsValid", (bool (*) (float32))&b2IsValid);
    _d.CPGF_MD_TEMPLATE _method("b2InvSqrt", (float32 (*) (float32))&b2InvSqrt);
    _d.CPGF_MD_TEMPLATE _method("b2Abs", (float32 (*) (float32))&b2Abs);
    _d.CPGF_MD_TEMPLATE _method("b2Dot", (float32 (*) (const b2Vec2 &, const b2Vec2 &))&b2Dot);
    _d.CPGF_MD_TEMPLATE _method("b2Cross", (float32 (*) (const b2Vec2 &, const b2Vec2 &))&b2Cross);
    _d.CPGF_MD_TEMPLATE _method("b2Cross", (b2Vec2 (*) (const b2Vec2 &, float32))&b2Cross);
    _d.CPGF_MD_TEMPLATE _method("b2Cross", (b2Vec2 (*) (float32, const b2Vec2 &))&b2Cross);
    _d.CPGF_MD_TEMPLATE _method("b2Mul", (b2Vec2 (*) (const b2Mat22 &, const b2Vec2 &))&b2Mul, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("b2MulT", (b2Vec2 (*) (const b2Mat22 &, const b2Vec2 &))&b2MulT, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("b2Distance", (float32 (*) (const b2Vec2 &, const b2Vec2 &))&b2Distance);
    _d.CPGF_MD_TEMPLATE _method("b2DistanceSquared", (float32 (*) (const b2Vec2 &, const b2Vec2 &))&b2DistanceSquared);
    _d.CPGF_MD_TEMPLATE _method("b2Dot", (float32 (*) (const b2Vec3 &, const b2Vec3 &))&b2Dot);
    _d.CPGF_MD_TEMPLATE _method("b2Cross", (b2Vec3 (*) (const b2Vec3 &, const b2Vec3 &))&b2Cross);
    _d.CPGF_MD_TEMPLATE _method("b2Mul", (b2Mat22 (*) (const b2Mat22 &, const b2Mat22 &))&b2Mul, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("b2MulT", (b2Mat22 (*) (const b2Mat22 &, const b2Mat22 &))&b2MulT, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("b2Mul", (b2Vec3 (*) (const b2Mat33 &, const b2Vec3 &))&b2Mul);
    _d.CPGF_MD_TEMPLATE _method("b2Mul", (b2Vec2 (*) (const b2Transform &, const b2Vec2 &))&b2Mul);
    _d.CPGF_MD_TEMPLATE _method("b2MulT", (b2Vec2 (*) (const b2Transform &, const b2Vec2 &))&b2MulT);
    _d.CPGF_MD_TEMPLATE _method("b2Abs", (b2Vec2 (*) (const b2Vec2 &))&b2Abs);
    _d.CPGF_MD_TEMPLATE _method("b2Abs", (b2Mat22 (*) (const b2Mat22 &))&b2Abs, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("b2Min", (b2Vec2 (*) (const b2Vec2 &, const b2Vec2 &))&b2Min);
    _d.CPGF_MD_TEMPLATE _method("b2Max", (b2Vec2 (*) (const b2Vec2 &, const b2Vec2 &))&b2Max);
    _d.CPGF_MD_TEMPLATE _method("b2Clamp", (b2Vec2 (*) (const b2Vec2 &, const b2Vec2 &, const b2Vec2 &))&b2Clamp);
    _d.CPGF_MD_TEMPLATE _method("b2NextPowerOfTwo", (uint32 (*) (uint32))&b2NextPowerOfTwo);
    _d.CPGF_MD_TEMPLATE _method("b2IsPowerOfTwo", (bool (*) (uint32))&b2IsPowerOfTwo);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec2 (*)(const b2Vec2 &, const b2Vec2 &)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec2 (*)(const b2Vec2 &, const b2Vec2 &)>(mopHolder - mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec2 (*)(float32, const b2Vec2 &)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const b2Vec2 &, const b2Vec2 &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec3 (*)(float32, const b2Vec3 &)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec3 (*)(const b2Vec3 &, const b2Vec3 &)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec3 (*)(const b2Vec3 &, const b2Vec3 &)>(mopHolder - mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<b2Mat22 (*)(const b2Mat22 &, const b2Mat22 &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
}


template <typename D>
void buildMetaClass_B2Mat22(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec2 &, const b2Vec2 &)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32, float32, float32)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32)>();
    _d.CPGF_MD_TEMPLATE _field("col1", &D::ClassType::col1);
    _d.CPGF_MD_TEMPLATE _field("col2", &D::ClassType::col2);
    _d.CPGF_MD_TEMPLATE _method("Set", (void (D::ClassType::*) (const b2Vec2 &, const b2Vec2 &))&D::ClassType::Set);
    _d.CPGF_MD_TEMPLATE _method("Set", (void (D::ClassType::*) (float32))&D::ClassType::Set);
    _d.CPGF_MD_TEMPLATE _method("SetIdentity", &D::ClassType::SetIdentity);
    _d.CPGF_MD_TEMPLATE _method("SetZero", &D::ClassType::SetZero);
    _d.CPGF_MD_TEMPLATE _method("GetAngle", &D::ClassType::GetAngle);
    _d.CPGF_MD_TEMPLATE _method("GetInverse", &D::ClassType::GetInverse);
    _d.CPGF_MD_TEMPLATE _method("Solve", &D::ClassType::Solve);
}


template <typename D>
void buildMetaClass_B2Mat33(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec3 &, const b2Vec3 &, const b2Vec3 &)>();
    _d.CPGF_MD_TEMPLATE _field("col1", &D::ClassType::col1);
    _d.CPGF_MD_TEMPLATE _field("col2", &D::ClassType::col2);
    _d.CPGF_MD_TEMPLATE _field("col3", &D::ClassType::col3);
    _d.CPGF_MD_TEMPLATE _method("SetZero", &D::ClassType::SetZero);
    _d.CPGF_MD_TEMPLATE _method("Solve33", &D::ClassType::Solve33);
    _d.CPGF_MD_TEMPLATE _method("Solve22", &D::ClassType::Solve22);
}


template <typename D>
void buildMetaClass_B2Sweep(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("localCenter", &D::ClassType::localCenter);
    _d.CPGF_MD_TEMPLATE _field("c0", &D::ClassType::c0);
    _d.CPGF_MD_TEMPLATE _field("c", &D::ClassType::c);
    _d.CPGF_MD_TEMPLATE _field("a0", &D::ClassType::a0);
    _d.CPGF_MD_TEMPLATE _field("a", &D::ClassType::a);
    _d.CPGF_MD_TEMPLATE _method("GetTransform", &D::ClassType::GetTransform);
    _d.CPGF_MD_TEMPLATE _method("Advance", &D::ClassType::Advance);
    _d.CPGF_MD_TEMPLATE _method("Normalize", &D::ClassType::Normalize);
}


template <typename D>
void buildMetaClass_B2Transform(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec2 &, const b2Mat22 &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _field("position", &D::ClassType::position);
    _d.CPGF_MD_TEMPLATE _field("R", &D::ClassType::R);
    _d.CPGF_MD_TEMPLATE _method("SetIdentity", &D::ClassType::SetIdentity);
    _d.CPGF_MD_TEMPLATE _method("Set", &D::ClassType::Set);
    _d.CPGF_MD_TEMPLATE _method("GetAngle", &D::ClassType::GetAngle);
}


inline b2Vec2 opErAToRWrapper_b2Vec2__opMinus(const b2Vec2 * self) {
    return -(*self);
}
inline float32 opErAToRWrapper_b2Vec2__opFunction(const b2Vec2 * self, int32 i) {
    return (*self)(i);
}
inline float32 & opErAToRWrapper_b2Vec2__opFunction(b2Vec2 * self, int32 i) {
    return (*self)(i);
}
inline void opErAToRWrapper_b2Vec2__opAddAssign(b2Vec2 * self, const b2Vec2 & v) {
    (*self) += v;
}
inline void opErAToRWrapper_b2Vec2__opSubAssign(b2Vec2 * self, const b2Vec2 & v) {
    (*self) -= v;
}
inline void opErAToRWrapper_b2Vec2__opMulAssign(b2Vec2 * self, float32 a) {
    (*self) *= a;
}


template <typename D>
void buildMetaClass_B2Vec2(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32)>();
    _d.CPGF_MD_TEMPLATE _field("x", &D::ClassType::x);
    _d.CPGF_MD_TEMPLATE _field("y", &D::ClassType::y);
    _d.CPGF_MD_TEMPLATE _method("SetZero", &D::ClassType::SetZero);
    _d.CPGF_MD_TEMPLATE _method("Set", &D::ClassType::Set);
    _d.CPGF_MD_TEMPLATE _method("Length", &D::ClassType::Length);
    _d.CPGF_MD_TEMPLATE _method("LengthSquared", &D::ClassType::LengthSquared);
    _d.CPGF_MD_TEMPLATE _method("Normalize", &D::ClassType::Normalize);
    _d.CPGF_MD_TEMPLATE _method("IsValid", &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec2 (*)(const cpgf::GMetaSelf &)>(-mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (b2Vec2 (*) (const b2Vec2 *))&opErAToRWrapper_b2Vec2__opMinus, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<float32 (*)(int32)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (float32 (*) (const b2Vec2 *, int32))&opErAToRWrapper_b2Vec2__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<float32 & (*)(int32)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (float32 & (*) (b2Vec2 *, int32))&opErAToRWrapper_b2Vec2__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const b2Vec2 &)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (void (*) (b2Vec2 *, const b2Vec2 &))&opErAToRWrapper_b2Vec2__opAddAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const b2Vec2 &)>(mopHolder -= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (void (*) (b2Vec2 *, const b2Vec2 &))&opErAToRWrapper_b2Vec2__opSubAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, float32)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (void (*) (b2Vec2 *, float32))&opErAToRWrapper_b2Vec2__opMulAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline b2Vec3 opErAToRWrapper_b2Vec3__opMinus(const b2Vec3 * self) {
    return -(*self);
}
inline void opErAToRWrapper_b2Vec3__opAddAssign(b2Vec3 * self, const b2Vec3 & v) {
    (*self) += v;
}
inline void opErAToRWrapper_b2Vec3__opSubAssign(b2Vec3 * self, const b2Vec3 & v) {
    (*self) -= v;
}
inline void opErAToRWrapper_b2Vec3__opMulAssign(b2Vec3 * self, float32 s) {
    (*self) *= s;
}


template <typename D>
void buildMetaClass_B2Vec3(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32, float32)>();
    _d.CPGF_MD_TEMPLATE _field("x", &D::ClassType::x);
    _d.CPGF_MD_TEMPLATE _field("y", &D::ClassType::y);
    _d.CPGF_MD_TEMPLATE _field("z", &D::ClassType::z);
    _d.CPGF_MD_TEMPLATE _method("SetZero", &D::ClassType::SetZero);
    _d.CPGF_MD_TEMPLATE _method("Set", &D::ClassType::Set);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec3 (*)(const cpgf::GMetaSelf &)>(-mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (b2Vec3 (*) (const b2Vec3 *))&opErAToRWrapper_b2Vec3__opMinus, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const b2Vec3 &)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (void (*) (b2Vec3 *, const b2Vec3 &))&opErAToRWrapper_b2Vec3__opAddAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const b2Vec3 &)>(mopHolder -= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (void (*) (b2Vec3 *, const b2Vec3 &))&opErAToRWrapper_b2Vec3__opSubAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, float32)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (void (*) (b2Vec3 *, float32))&opErAToRWrapper_b2Vec3__opMulAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
