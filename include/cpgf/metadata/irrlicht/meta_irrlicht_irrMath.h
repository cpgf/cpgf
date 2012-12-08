// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRRMATH_H
#define __META_IRRLICHT_IRRMATH_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_irrmath(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("ROUNDING_ERROR_S32", &ROUNDING_ERROR_S32);
    _d.CPGF_MD_TEMPLATE _field("ROUNDING_ERROR_S64", &ROUNDING_ERROR_S64);
    _d.CPGF_MD_TEMPLATE _field("ROUNDING_ERROR_f32", &ROUNDING_ERROR_f32);
    _d.CPGF_MD_TEMPLATE _field("ROUNDING_ERROR_f64", &ROUNDING_ERROR_f64);
    _d.CPGF_MD_TEMPLATE _field("PI", &PI);
    _d.CPGF_MD_TEMPLATE _field("RECIPROCAL_PI", &RECIPROCAL_PI);
    _d.CPGF_MD_TEMPLATE _field("HALF_PI", &HALF_PI);
    _d.CPGF_MD_TEMPLATE _field("PI64", &PI64);
    _d.CPGF_MD_TEMPLATE _field("RECIPROCAL_PI64", &RECIPROCAL_PI64);
    _d.CPGF_MD_TEMPLATE _field("DEGTORAD", &DEGTORAD);
    _d.CPGF_MD_TEMPLATE _field("RADTODEG", &RADTODEG);
    _d.CPGF_MD_TEMPLATE _field("DEGTORAD64", &DEGTORAD64);
    _d.CPGF_MD_TEMPLATE _field("RADTODEG64", &RADTODEG64);
    _d.CPGF_MD_TEMPLATE _method("radToDeg", (f32 (*) (f32))&radToDeg);
    _d.CPGF_MD_TEMPLATE _method("radToDeg", (f64 (*) (f64))&radToDeg);
    _d.CPGF_MD_TEMPLATE _method("degToRad", (f32 (*) (f32))&degToRad);
    _d.CPGF_MD_TEMPLATE _method("degToRad", (f64 (*) (f64))&degToRad);
    _d.CPGF_MD_TEMPLATE _method("equals", (bool (*) (const f64, const f64, const f64))&equals)
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_f64))
    ;
    _d.CPGF_MD_TEMPLATE _method("equals", (bool (*) (const f32, const f32, const f32))&equals)
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_f32))
    ;
    _d.CPGF_MD_TEMPLATE _method("equalsByUlp", (bool (*) (f32, f32, int))&equalsByUlp);
    _d.CPGF_MD_TEMPLATE _method("equals", (bool (*) (const s32, const s32, const s32))&equals)
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_S32))
    ;
    _d.CPGF_MD_TEMPLATE _method("equals", (bool (*) (const u32, const u32, const s32))&equals)
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_S32))
    ;
    _d.CPGF_MD_TEMPLATE _method("equals", (bool (*) (const s64, const s64, const s64))&equals)
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_S64))
    ;
    _d.CPGF_MD_TEMPLATE _method("iszero", (bool (*) (const f64, const f64))&iszero)
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_f64))
    ;
    _d.CPGF_MD_TEMPLATE _method("iszero", (bool (*) (const f32, const f32))&iszero)
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_f32))
    ;
    _d.CPGF_MD_TEMPLATE _method("isnotzero", (bool (*) (const f32, const f32))&isnotzero)
        ._default(copyVariantFromCopyable(ROUNDING_ERROR_f32))
    ;
    _d.CPGF_MD_TEMPLATE _method("iszero", (bool (*) (const s32, const s32))&iszero)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("iszero", (bool (*) (const u32, const u32))&iszero)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("iszero", (bool (*) (const s64, const s64))&iszero)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("s32_min", (s32 (*) (s32, s32))&s32_min);
    _d.CPGF_MD_TEMPLATE _method("s32_max", (s32 (*) (s32, s32))&s32_max);
    _d.CPGF_MD_TEMPLATE _method("s32_clamp", (s32 (*) (s32, s32, s32))&s32_clamp);
    _d.CPGF_MD_TEMPLATE _method("IR", (u32 (*) (f32))&IR);
    _d.CPGF_MD_TEMPLATE _method("FR", (f32 (*) (u32))&FR);
    _d.CPGF_MD_TEMPLATE _method("FR", (f32 (*) (s32))&FR);
    _d.CPGF_MD_TEMPLATE _method("if_c_a_else_b", (u32 (*) (const s32, const u32, const u32))&if_c_a_else_b);
    _d.CPGF_MD_TEMPLATE _method("if_c_a_else_b", (u16 (*) (const s16, const u16, const u16))&if_c_a_else_b);
    _d.CPGF_MD_TEMPLATE _method("if_c_a_else_0", (u32 (*) (const s32, const u32))&if_c_a_else_0);
    _d.CPGF_MD_TEMPLATE _method("setbit_cond", (void (*) (u32 &, s32, u32))&setbit_cond);
    _d.CPGF_MD_TEMPLATE _method("round_", (f32 (*) (f32))&round_);
    _d.CPGF_MD_TEMPLATE _method("clearFPUException", (void (*) ())&clearFPUException);
    _d.CPGF_MD_TEMPLATE _method("squareroot", (f32 (*) (const f32))&squareroot);
    _d.CPGF_MD_TEMPLATE _method("squareroot", (f64 (*) (const f64))&squareroot);
    _d.CPGF_MD_TEMPLATE _method("squareroot", (s32 (*) (const s32))&squareroot);
    _d.CPGF_MD_TEMPLATE _method("squareroot", (s64 (*) (const s64))&squareroot);
    _d.CPGF_MD_TEMPLATE _method("reciprocal_squareroot", (f64 (*) (const f64))&reciprocal_squareroot);
    _d.CPGF_MD_TEMPLATE _method("reciprocal_squareroot", (f32 (*) (const f32))&reciprocal_squareroot);
    _d.CPGF_MD_TEMPLATE _method("reciprocal_squareroot", (s32 (*) (const s32))&reciprocal_squareroot);
    _d.CPGF_MD_TEMPLATE _method("reciprocal", (f32 (*) (const f32))&reciprocal);
    _d.CPGF_MD_TEMPLATE _method("reciprocal", (f64 (*) (const f64))&reciprocal);
    _d.CPGF_MD_TEMPLATE _method("reciprocal_approxim", (f32 (*) (const f32))&reciprocal_approxim);
    _d.CPGF_MD_TEMPLATE _method("floor32", (s32 (*) (f32))&floor32);
    _d.CPGF_MD_TEMPLATE _method("ceil32", (s32 (*) (f32))&ceil32);
    _d.CPGF_MD_TEMPLATE _method("round32", (s32 (*) (f32))&round32);
    _d.CPGF_MD_TEMPLATE _method("f32_max3", (f32 (*) (const f32, const f32, const f32))&f32_max3);
    _d.CPGF_MD_TEMPLATE _method("f32_min3", (f32 (*) (const f32, const f32, const f32))&f32_min3);
    _d.CPGF_MD_TEMPLATE _method("fract", (f32 (*) (f32))&fract);
    _d.CPGF_MD_TEMPLATE _enum<long long>("GlobalDefine_irrlicht_7")
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
