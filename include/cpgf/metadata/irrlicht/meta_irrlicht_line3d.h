// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_LINE3D_H
#define __META_IRRLICHT_LINE3D_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <class T>
inline line3d< T > opErAToRWrapper_line3d__opAdd(const line3d<T> * self, const vector3d< T > & point) {
    return (*self) + point;
}
template <class T>
inline line3d< T > & opErAToRWrapper_line3d__opAddAssign(line3d<T> * self, const vector3d< T > & point) {
    return (*self) += point;
}
template <class T>
inline line3d< T > opErAToRWrapper_line3d__opSub(const line3d<T> * self, const vector3d< T > & point) {
    return (*self) - point;
}
template <class T>
inline line3d< T > & opErAToRWrapper_line3d__opSubAssign(line3d<T> * self, const vector3d< T > & point) {
    return (*self) -= point;
}
template <class T>
inline bool opErAToRWrapper_line3d__opEqual(const line3d<T> * self, const line3d< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_line3d__opNotEqual(const line3d<T> * self, const line3d< T > & other) {
    return (*self) != other;
}


template <typename D, class T>
void buildMetaClass_Line3d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T, T, T, T)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector3d< T > &, const vector3d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _field("start", &D::ClassType::start);
    _d.CPGF_MD_TEMPLATE _field("end", &D::ClassType::end);
    _d.CPGF_MD_TEMPLATE _method("setLine", (void (D::ClassType::*) (const T &, const T &, const T &, const T &, const T &, const T &))&D::ClassType::setLine);
    _d.CPGF_MD_TEMPLATE _method("setLine", (void (D::ClassType::*) (const vector3d< T > &, const vector3d< T > &))&D::ClassType::setLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("setLine", (void (D::ClassType::*) (const line3d< T > &))&D::ClassType::setLine);
    _d.CPGF_MD_TEMPLATE _method("getLength", &D::ClassType::getLength);
    _d.CPGF_MD_TEMPLATE _method("getLengthSQ", &D::ClassType::getLengthSQ);
    _d.CPGF_MD_TEMPLATE _method("getMiddle", &D::ClassType::getMiddle);
    _d.CPGF_MD_TEMPLATE _method("getVector", &D::ClassType::getVector);
    _d.CPGF_MD_TEMPLATE _method("isPointBetweenStartAndEnd", &D::ClassType::isPointBetweenStartAndEnd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getClosestPoint", &D::ClassType::getClosestPoint, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getIntersectionWithSphere", &D::ClassType::getIntersectionWithSphere);
    _d.CPGF_MD_TEMPLATE _operator<line3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAdd", (line3d< T > (*) (const line3d<T> *, const vector3d< T > &))&opErAToRWrapper_line3d__opAdd<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<line3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opAddAssign", (line3d< T > & (*) (line3d<T> *, const vector3d< T > &))&opErAToRWrapper_line3d__opAddAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<line3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opSub", (line3d< T > (*) (const line3d<T> *, const vector3d< T > &))&opErAToRWrapper_line3d__opSub<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<line3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opSubAssign", (line3d< T > & (*) (line3d<T> *, const vector3d< T > &))&opErAToRWrapper_line3d__opSubAssign<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const line3d< T > &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const line3d<T> *, const line3d< T > &))&opErAToRWrapper_line3d__opEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const line3d< T > &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const line3d<T> *, const line3d< T > &))&opErAToRWrapper_line3d__opNotEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
