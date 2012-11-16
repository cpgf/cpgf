// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_VECTOR2D_H
#define __META_IRRLICHT_VECTOR2D_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_vector2d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opMinus(const vector2d<T> * self) {
    return -(*self);
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opAssign(vector2d<T> * self, const vector2d< T > & other) {
    return (*self) = other;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opAssign(vector2d<T> * self, const dimension2d< T > & other) {
    return (*self) = other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opPlus(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) + other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opPlus(const vector2d<T> * self, const dimension2d< T > & other) {
    return (*self) + other;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opPlusAssign(vector2d<T> * self, const vector2d< T > & other) {
    return (*self) += other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opPlus(const vector2d<T> * self, const T v) {
    return (*self) + v;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opPlusAssign(vector2d<T> * self, const T v) {
    return (*self) += v;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opPlusAssign(vector2d<T> * self, const dimension2d< T > & other) {
    return (*self) += other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opMinus(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) - other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opMinus(const vector2d<T> * self, const dimension2d< T > & other) {
    return (*self) - other;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opMinusAssign(vector2d<T> * self, const vector2d< T > & other) {
    return (*self) -= other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opMinus(const vector2d<T> * self, const T v) {
    return (*self) - v;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opMinusAssign(vector2d<T> * self, const T v) {
    return (*self) -= v;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opMinusAssign(vector2d<T> * self, const dimension2d< T > & other) {
    return (*self) -= other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opStar(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) * other;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opStarAssign(vector2d<T> * self, const vector2d< T > & other) {
    return (*self) *= other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opStar(const vector2d<T> * self, const T v) {
    return (*self) * v;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opStarAssign(vector2d<T> * self, const T v) {
    return (*self) *= v;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opDiv(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) / other;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opDivAssign(vector2d<T> * self, const vector2d< T > & other) {
    return (*self) /= other;
}
template <class T>
inline vector2d< T > opErAToRWrapper_vector2d__opDiv(const vector2d<T> * self, const T v) {
    return (*self) / v;
}
template <class T>
inline vector2d< T > & opErAToRWrapper_vector2d__opDivAssign(vector2d<T> * self, const T v) {
    return (*self) /= v;
}
template <class T>
inline bool opErAToRWrapper_vector2d_null(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) <= other;
}
template <class T>
inline bool opErAToRWrapper_vector2d__opGreaterEqual(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) >= other;
}
template <class T>
inline bool opErAToRWrapper_vector2d__opLesserEqual(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) < other;
}
template <class T>
inline bool opErAToRWrapper_vector2d__opGreater(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) > other;
}
template <class T>
inline bool opErAToRWrapper_vector2d__opEqual(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_vector2d__opNotEqual(const vector2d<T> * self, const vector2d< T > & other) {
    return (*self) != other;
}


template <typename D, class T>
void buildMetaClass_Vector2d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector2d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const dimension2d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _field("X", &D::ClassType::X);
    _d.CPGF_MD_TEMPLATE _field("Y", &D::ClassType::Y);
    _d.CPGF_MD_TEMPLATE _method("equals", &D::ClassType::equals, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("set", (vector2d< T > & (D::ClassType::*) (T, T))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("set", (vector2d< T > & (D::ClassType::*) (const vector2d< T > &))&D::ClassType::set, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getLength", &D::ClassType::getLength);
    _d.CPGF_MD_TEMPLATE _method("getLengthSQ", &D::ClassType::getLengthSQ);
    _d.CPGF_MD_TEMPLATE _method("dotProduct", &D::ClassType::dotProduct, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getDistanceFrom", &D::ClassType::getDistanceFrom, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getDistanceFromSQ", &D::ClassType::getDistanceFromSQ, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("rotateBy", &D::ClassType::rotateBy, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(vector2d< T >()))
    ;
    _d.CPGF_MD_TEMPLATE _method("normalize", &D::ClassType::normalize);
    _d.CPGF_MD_TEMPLATE _method("getAngleTrig", &D::ClassType::getAngleTrig);
    _d.CPGF_MD_TEMPLATE _method("getAngle", &D::ClassType::getAngle);
    _d.CPGF_MD_TEMPLATE _method("getAngleWith", &D::ClassType::getAngleWith, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isBetweenPoints", &D::ClassType::isBetweenPoints, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getInterpolated", &D::ClassType::getInterpolated, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getInterpolated_quadratic", &D::ClassType::getInterpolated_quadratic, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("interpolate", &D::ClassType::interpolate, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &)>(-mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (vector2d< T > (*) (const vector2d<T> *))&opErAToRWrapper_vector2d__opMinus<T>);
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const vector2d< T > &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (vector2d< T > & (*) (vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const dimension2d< T > &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (vector2d< T > & (*) (vector2d<T> *, const dimension2d< T > &))&opErAToRWrapper_vector2d__opAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opPlus", (vector2d< T > (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opPlus<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const dimension2d< T > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opPlus", (vector2d< T > (*) (const vector2d<T> *, const dimension2d< T > &))&opErAToRWrapper_vector2d__opPlus<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const vector2d< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opPlusAssign", (vector2d< T > & (*) (vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opPlusAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opPlus", (vector2d< T > (*) (const vector2d<T> *, const T))&opErAToRWrapper_vector2d__opPlus<T>);
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opPlusAssign", (vector2d< T > & (*) (vector2d<T> *, const T))&opErAToRWrapper_vector2d__opPlusAssign<T>);
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const dimension2d< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opPlusAssign", (vector2d< T > & (*) (vector2d<T> *, const dimension2d< T > &))&opErAToRWrapper_vector2d__opPlusAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (vector2d< T > (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opMinus<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const dimension2d< T > &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (vector2d< T > (*) (const vector2d<T> *, const dimension2d< T > &))&opErAToRWrapper_vector2d__opMinus<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const vector2d< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMinusAssign", (vector2d< T > & (*) (vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opMinusAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder - mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (vector2d< T > (*) (const vector2d<T> *, const T))&opErAToRWrapper_vector2d__opMinus<T>);
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder -= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMinusAssign", (vector2d< T > & (*) (vector2d<T> *, const T))&opErAToRWrapper_vector2d__opMinusAssign<T>);
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const dimension2d< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMinusAssign", (vector2d< T > & (*) (vector2d<T> *, const dimension2d< T > &))&opErAToRWrapper_vector2d__opMinusAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder * mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opStar", (vector2d< T > (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opStar<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const vector2d< T > &)>(mopHolder *= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opStarAssign", (vector2d< T > & (*) (vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opStarAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opStar", (vector2d< T > (*) (const vector2d<T> *, const T))&opErAToRWrapper_vector2d__opStar<T>);
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opStarAssign", (vector2d< T > & (*) (vector2d<T> *, const T))&opErAToRWrapper_vector2d__opStarAssign<T>);
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder / mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opDiv", (vector2d< T > (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opDiv<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const vector2d< T > &)>(mopHolder /= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opDivAssign", (vector2d< T > & (*) (vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opDivAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder / mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opDiv", (vector2d< T > (*) (const vector2d<T> *, const T))&opErAToRWrapper_vector2d__opDiv<T>);
    _d.CPGF_MD_TEMPLATE _operator<vector2d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder /= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opDivAssign", (vector2d< T > & (*) (vector2d<T> *, const T))&opErAToRWrapper_vector2d__opDivAssign<T>);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder <= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("null", (bool (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d_null<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder >= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreaterEqual", (bool (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opGreaterEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLesserEqual", (bool (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opLesserEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder > mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opGreater", (bool (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opGreater<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const vector2d<T> *, const vector2d< T > &))&opErAToRWrapper_vector2d__opNotEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
