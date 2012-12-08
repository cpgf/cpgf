// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_VECTOR3D_H
#define __META_IRRLICHT_VECTOR3D_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_vector3d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opMinus(const vector3d<T> * self) {
    return -(*self);
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opAssign(vector3d<T> * self, const vector3d< T > & other) {
    return (*self) = other;
}
template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opAdd(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) + other;
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opAddAssign(vector3d<T> * self, const vector3d< T > & other) {
    return (*self) += other;
}
template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opAdd(const vector3d<T> * self, const T val) {
    return (*self) + val;
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opAddAssign(vector3d<T> * self, const T val) {
    return (*self) += val;
}
template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opSub(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) - other;
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opSubAssign(vector3d<T> * self, const vector3d< T > & other) {
    return (*self) -= other;
}
template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opSub(const vector3d<T> * self, const T val) {
    return (*self) - val;
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opSubAssign(vector3d<T> * self, const T val) {
    return (*self) -= val;
}
template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opMul(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) * other;
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opMulAssign(vector3d<T> * self, const vector3d< T > & other) {
    return (*self) *= other;
}
template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opMul(const vector3d<T> * self, const T v) {
    return (*self) * v;
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opMulAssign(vector3d<T> * self, const T v) {
    return (*self) *= v;
}
template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opDiv(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) / other;
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opDivAssign(vector3d<T> * self, const vector3d< T > & other) {
    return (*self) /= other;
}
template <class T>
inline vector3d< T > opErAToRWrapper_vector3d__opDiv(const vector3d<T> * self, const T v) {
    return (*self) / v;
}
template <class T>
inline vector3d< T > & opErAToRWrapper_vector3d__opDivAssign(vector3d<T> * self, const T v) {
    return (*self) /= v;
}
template <class T>
inline bool opErAToRWrapper_vector3d__opLessEqual(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) <= other;
}
template <class T>
inline bool opErAToRWrapper_vector3d__opGreaterEqual(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) >= other;
}
template <class T>
inline bool opErAToRWrapper_vector3d__opLess(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) < other;
}
template <class T>
inline bool opErAToRWrapper_vector3d__opGreater(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) > other;
}
template <class T>
inline bool opErAToRWrapper_vector3d__opEqual(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_vector3d__opNotEqual(const vector3d<T> * self, const vector3d< T > & other) {
    return (*self) != other;
}


template <typename D, class T>
void buildMetaClass_Vector3d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector3d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _field("X", &D::ClassType::X);
    _d.CPGF_MD_TEMPLATE _field("Y", &D::ClassType::Y);
    _d.CPGF_MD_TEMPLATE _field("Z", &D::ClassType::Z);
    _d.CPGF_MD_TEMPLATE _method("equals", &D::ClassType::equals, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable((T) ROUNDING_ERROR_f32))
    ;
    _d.CPGF_MD_TEMPLATE _method("set", (vector3d< T > & (D::ClassType::*) (const T, const T, const T))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("set", (vector3d< T > & (D::ClassType::*) (const vector3d< T > &))&D::ClassType::set, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getLength", &D::ClassType::getLength);
    _d.CPGF_MD_TEMPLATE _method("getLengthSQ", &D::ClassType::getLengthSQ);
    _d.CPGF_MD_TEMPLATE _method("dotProduct", &D::ClassType::dotProduct, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getDistanceFrom", &D::ClassType::getDistanceFrom, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getDistanceFromSQ", &D::ClassType::getDistanceFromSQ, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("crossProduct", &D::ClassType::crossProduct, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isBetweenPoints", &D::ClassType::isBetweenPoints, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("normalize", &D::ClassType::normalize);
    _d.CPGF_MD_TEMPLATE _method("setLength", &D::ClassType::setLength);
    _d.CPGF_MD_TEMPLATE _method("invert", &D::ClassType::invert);
    _d.CPGF_MD_TEMPLATE _method("rotateXZBy", &D::ClassType::rotateXZBy, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(vector3d< T >()))
    ;
    _d.CPGF_MD_TEMPLATE _method("rotateXYBy", &D::ClassType::rotateXYBy, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(vector3d< T >()))
    ;
    _d.CPGF_MD_TEMPLATE _method("rotateYZBy", &D::ClassType::rotateYZBy, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(vector3d< T >()))
    ;
    _d.CPGF_MD_TEMPLATE _method("getInterpolated", &D::ClassType::getInterpolated, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getInterpolated_quadratic", &D::ClassType::getInterpolated_quadratic, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("interpolate", &D::ClassType::interpolate, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getHorizontalAngle", &D::ClassType::getHorizontalAngle);
    _d.CPGF_MD_TEMPLATE _method("getSphericalCoordinateAngles", (vector3d< T > (D::ClassType::*) () const)&D::ClassType::getSphericalCoordinateAngles);
    _d.CPGF_MD_TEMPLATE _method("rotationToDirection", &D::ClassType::rotationToDirection, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(vector3d< T >(0, 0, 1)))
    ;
    _d.CPGF_MD_TEMPLATE _method("getAs4Values", &D::ClassType::getAs4Values);
    _d.CPGF_MD_TEMPLATE _method("getAs3Values", &D::ClassType::getAs3Values);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &)>(-mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (vector3d< T > (*) (const vector3d<T> *))&opErAToRWrapper_vector3d__opMinus<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (vector3d< T > & (*) (vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAdd", (vector3d< T > (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opAdd<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (vector3d< T > & (*) (vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opAddAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAdd", (vector3d< T > (*) (const vector3d<T> *, const T))&opErAToRWrapper_vector3d__opAdd<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (vector3d< T > & (*) (vector3d<T> *, const T))&opErAToRWrapper_vector3d__opAddAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opSub", (vector3d< T > (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opSub<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (vector3d< T > & (*) (vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opSubAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder - mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opSub", (vector3d< T > (*) (const vector3d<T> *, const T))&opErAToRWrapper_vector3d__opSub<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder -= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (vector3d< T > & (*) (vector3d<T> *, const T))&opErAToRWrapper_vector3d__opSubAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder * mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMul", (vector3d< T > (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opMul<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder *= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (vector3d< T > & (*) (vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opMulAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMul", (vector3d< T > (*) (const vector3d<T> *, const T))&opErAToRWrapper_vector3d__opMul<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMulAssign", (vector3d< T > & (*) (vector3d<T> *, const T))&opErAToRWrapper_vector3d__opMulAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder / mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opDiv", (vector3d< T > (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opDiv<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder /= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opDivAssign", (vector3d< T > & (*) (vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opDivAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder / mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opDiv", (vector3d< T > (*) (const vector3d<T> *, const T))&opErAToRWrapper_vector3d__opDiv<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder /= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opDivAssign", (vector3d< T > & (*) (vector3d<T> *, const T))&opErAToRWrapper_vector3d__opDivAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder <= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLessEqual", (bool (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opLessEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder >= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreaterEqual", (bool (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opGreaterEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLess", (bool (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opLess<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder > mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreater", (bool (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opGreater<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const vector3d<T> *, const vector3d< T > &))&opErAToRWrapper_vector3d__opNotEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
