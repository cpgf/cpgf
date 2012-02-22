// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2MATH_H
#define __META_BOX2D_B2MATH_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_Global_b2math(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2Vec2_zero", _r), &b2Vec2_zero, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2Mat22_identity", _r), &b2Mat22_identity, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2Transform_identity", _r), &b2Transform_identity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2IsValid", _r), (bool (*) (float32))&b2IsValid, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2InvSqrt", _r), (float32 (*) (float32))&b2InvSqrt, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Abs", _r), (float32 (*) (float32))&b2Abs, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Dot", _r), (float32 (*) (const b2Vec2 &, const b2Vec2 &))&b2Dot, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Cross", _r), (float32 (*) (const b2Vec2 &, const b2Vec2 &))&b2Cross, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Cross", _r), (b2Vec2 (*) (const b2Vec2 &, float32))&b2Cross, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Cross", _r), (b2Vec2 (*) (float32, const b2Vec2 &))&b2Cross, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Mul", _r), (b2Vec2 (*) (const b2Mat22 &, const b2Vec2 &))&b2Mul, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2MulT", _r), (b2Vec2 (*) (const b2Mat22 &, const b2Vec2 &))&b2MulT, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Distance", _r), (float32 (*) (const b2Vec2 &, const b2Vec2 &))&b2Distance, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2DistanceSquared", _r), (float32 (*) (const b2Vec2 &, const b2Vec2 &))&b2DistanceSquared, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Dot", _r), (float32 (*) (const b2Vec3 &, const b2Vec3 &))&b2Dot, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Cross", _r), (b2Vec3 (*) (const b2Vec3 &, const b2Vec3 &))&b2Cross, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Mul", _r), (b2Mat22 (*) (const b2Mat22 &, const b2Mat22 &))&b2Mul, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2MulT", _r), (b2Mat22 (*) (const b2Mat22 &, const b2Mat22 &))&b2MulT, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Mul", _r), (b2Vec3 (*) (const b2Mat33 &, const b2Vec3 &))&b2Mul, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Mul", _r), (b2Vec2 (*) (const b2Transform &, const b2Vec2 &))&b2Mul, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2MulT", _r), (b2Vec2 (*) (const b2Transform &, const b2Vec2 &))&b2MulT, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Abs", _r), (b2Vec2 (*) (const b2Vec2 &))&b2Abs, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Abs", _r), (b2Mat22 (*) (const b2Mat22 &))&b2Abs, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Min", _r), (b2Vec2 (*) (const b2Vec2 &, const b2Vec2 &))&b2Min, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Max", _r), (b2Vec2 (*) (const b2Vec2 &, const b2Vec2 &))&b2Max, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Clamp", _r), (b2Vec2 (*) (const b2Vec2 &, const b2Vec2 &, const b2Vec2 &))&b2Clamp, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2NextPowerOfTwo", _r), (uint32 (*) (uint32))&b2NextPowerOfTwo, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2IsPowerOfTwo", _r), (bool (*) (uint32))&b2IsPowerOfTwo, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec2 (*)(const b2Vec2 &, const b2Vec2 &)>(mopHolder + mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec2 (*)(const b2Vec2 &, const b2Vec2 &)>(mopHolder - mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec2 (*)(float32, const b2Vec2 &)>(mopHolder * mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const b2Vec2 &, const b2Vec2 &)>(mopHolder == mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec3 (*)(float32, const b2Vec3 &)>(mopHolder * mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec3 (*)(const b2Vec3 &, const b2Vec3 &)>(mopHolder + mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec3 (*)(const b2Vec3 &, const b2Vec3 &)>(mopHolder - mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Mat22 (*)(const b2Mat22 &, const b2Mat22 &)>(mopHolder + mopHolder, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2Mat22(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec2 &, const b2Vec2 &)>(_p);
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32, float32, float32)>(_p);
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32)>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("col1", _r), &D::ClassType::col1, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("col2", _r), &D::ClassType::col2, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), (void (D::ClassType::*) (const b2Vec2 &, const b2Vec2 &))&D::ClassType::Set, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), (void (D::ClassType::*) (float32))&D::ClassType::Set, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetIdentity", _r), &D::ClassType::SetIdentity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetZero", _r), &D::ClassType::SetZero, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAngle", _r), &D::ClassType::GetAngle, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetInverse", _r), &D::ClassType::GetInverse, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Solve", _r), &D::ClassType::Solve, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2Mat33(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec3 &, const b2Vec3 &, const b2Vec3 &)>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("col1", _r), &D::ClassType::col1, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("col2", _r), &D::ClassType::col2, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("col3", _r), &D::ClassType::col3, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetZero", _r), &D::ClassType::SetZero, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Solve33", _r), &D::ClassType::Solve33, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Solve22", _r), &D::ClassType::Solve22, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2Sweep(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("localCenter", _r), &D::ClassType::localCenter, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("c0", _r), &D::ClassType::c0, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("c", _r), &D::ClassType::c, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("a0", _r), &D::ClassType::a0, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("a", _r), &D::ClassType::a, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetTransform", _r), &D::ClassType::GetTransform, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Advance", _r), &D::ClassType::Advance, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Normalize", _r), &D::ClassType::Normalize, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2Transform(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _constructor<void * (const b2Vec2 &, const b2Mat22 &)>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("position", _r), &D::ClassType::position, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("R", _r), &D::ClassType::R, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetIdentity", _r), &D::ClassType::SetIdentity, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAngle", _r), &D::ClassType::GetAngle, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2Vec2(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32)>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("x", _r), &D::ClassType::x, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("y", _r), &D::ClassType::y, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetZero", _r), &D::ClassType::SetZero, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Length", _r), &D::ClassType::Length, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("LengthSquared", _r), &D::ClassType::LengthSquared, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Normalize", _r), &D::ClassType::Normalize, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsValid", _r), &D::ClassType::IsValid, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec2 (*)(const cpgf::GMetaSelf &)>(-mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<float32 (*)(int32)>(mopHolder(mopHolder), _p);
    _d.CPGF_MD_TEMPLATE _operator<float32 & (*)(int32)>(mopHolder(mopHolder), _p);
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const b2Vec2 &)>(mopHolder += mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const b2Vec2 &)>(mopHolder -= mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, float32)>(mopHolder *= mopHolder, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2Vec3(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _constructor<void * (float32, float32, float32)>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("x", _r), &D::ClassType::x, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("y", _r), &D::ClassType::y, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("z", _r), &D::ClassType::z, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetZero", _r), &D::ClassType::SetZero, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set, _p);
    _d.CPGF_MD_TEMPLATE _operator<b2Vec3 (*)(const cpgf::GMetaSelf &)>(-mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const b2Vec3 &)>(mopHolder += mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const b2Vec3 &)>(mopHolder -= mopHolder, _p);
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, float32)>(mopHolder *= mopHolder, _p);
}


} // namespace meta_box2d 




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif