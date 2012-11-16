// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_LINE2D_H
#define __META_IRRLICHT_LINE2D_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <class T>
inline line2d< T > opErAToRWrapper_line2d__opPlus(const line2d<T> * self, const vector2d< T > & point) {
    return (*self) + point;
}
template <class T>
inline line2d< T > & opErAToRWrapper_line2d__opPlusAssign(line2d<T> * self, const vector2d< T > & point) {
    return (*self) += point;
}
template <class T>
inline line2d< T > opErAToRWrapper_line2d__opMinus(const line2d<T> * self, const vector2d< T > & point) {
    return (*self) - point;
}
template <class T>
inline line2d< T > & opErAToRWrapper_line2d__opMinusAssign(line2d<T> * self, const vector2d< T > & point) {
    return (*self) -= point;
}
template <class T>
inline bool opErAToRWrapper_line2d__opEqual(const line2d<T> * self, const line2d< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_line2d__opNotEqual(const line2d<T> * self, const line2d< T > & other) {
    return (*self) != other;
}


template <typename D, class T>
void buildMetaClass_Line2d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T, T)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector2d< T > &, const vector2d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const line2d< T > &)>();
    _d.CPGF_MD_TEMPLATE _field("start", &D::ClassType::start);
    _d.CPGF_MD_TEMPLATE _field("end", &D::ClassType::end);
    _d.CPGF_MD_TEMPLATE _method("setLine", (void (D::ClassType::*) (const T &, const T &, const T &, const T &))&D::ClassType::setLine);
    _d.CPGF_MD_TEMPLATE _method("setLine", (void (D::ClassType::*) (const vector2d< T > &, const vector2d< T > &))&D::ClassType::setLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("setLine", (void (D::ClassType::*) (const line2d< T > &))&D::ClassType::setLine);
    _d.CPGF_MD_TEMPLATE _method("getLength", &D::ClassType::getLength);
    _d.CPGF_MD_TEMPLATE _method("getLengthSQ", &D::ClassType::getLengthSQ);
    _d.CPGF_MD_TEMPLATE _method("getMiddle", &D::ClassType::getMiddle);
    _d.CPGF_MD_TEMPLATE _method("getVector", &D::ClassType::getVector);
    _d.CPGF_MD_TEMPLATE _method("intersectWith", &D::ClassType::intersectWith);
    _d.CPGF_MD_TEMPLATE _method("getUnitVector", &D::ClassType::getUnitVector);
    _d.CPGF_MD_TEMPLATE _method("getAngleWith", &D::ClassType::getAngleWith);
    _d.CPGF_MD_TEMPLATE _method("getPointOrientation", &D::ClassType::getPointOrientation, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isPointOnLine", &D::ClassType::isPointOnLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isPointBetweenStartAndEnd", &D::ClassType::isPointBetweenStartAndEnd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getClosestPoint", &D::ClassType::getClosestPoint, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _operator<line2d< T > (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opPlus", (line2d< T > (*) (const line2d<T> *, const vector2d< T > &))&opErAToRWrapper_line2d__opPlus<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<line2d< T > & (*)(cpgf::GMetaSelf, const vector2d< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opPlusAssign", (line2d< T > & (*) (line2d<T> *, const vector2d< T > &))&opErAToRWrapper_line2d__opPlusAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<line2d< T > (*)(const cpgf::GMetaSelf &, const vector2d< T > &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (line2d< T > (*) (const line2d<T> *, const vector2d< T > &))&opErAToRWrapper_line2d__opMinus<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<line2d< T > & (*)(cpgf::GMetaSelf, const vector2d< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opMinusAssign", (line2d< T > & (*) (line2d<T> *, const vector2d< T > &))&opErAToRWrapper_line2d__opMinusAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const line2d< T > &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const line2d<T> *, const line2d< T > &))&opErAToRWrapper_line2d__opEqual<T>);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const line2d< T > &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const line2d<T> *, const line2d< T > &))&opErAToRWrapper_line2d__opNotEqual<T>);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
