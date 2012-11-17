// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_AABBOX3D_H
#define __META_IRRLICHT_AABBOX3D_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <class T>
inline bool opErAToRWrapper_aabbox3d__opEqual(const aabbox3d<T> * self, const aabbox3d< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_aabbox3d__opNotEqual(const aabbox3d<T> * self, const aabbox3d< T > & other) {
    return (*self) != other;
}


template <typename D, class T>
void buildMetaClass_Aabbox3d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector3d< T > &, const vector3d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector3d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T, T, T, T)>();
    _d.CPGF_MD_TEMPLATE _field("MinEdge", &D::ClassType::MinEdge);
    _d.CPGF_MD_TEMPLATE _field("MaxEdge", &D::ClassType::MaxEdge);
    _d.CPGF_MD_TEMPLATE _method("reset", (void (D::ClassType::*) (T, T, T))&D::ClassType::reset);
    _d.CPGF_MD_TEMPLATE _method("reset", (void (D::ClassType::*) (const aabbox3d< T > &))&D::ClassType::reset, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("reset", (void (D::ClassType::*) (const vector3d< T > &))&D::ClassType::reset, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("addInternalPoint", (void (D::ClassType::*) (const vector3d< T > &))&D::ClassType::addInternalPoint, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("addInternalBox", &D::ClassType::addInternalBox, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("addInternalPoint", (void (D::ClassType::*) (T, T, T))&D::ClassType::addInternalPoint);
    _d.CPGF_MD_TEMPLATE _method("getCenter", &D::ClassType::getCenter);
    _d.CPGF_MD_TEMPLATE _method("getExtent", &D::ClassType::getExtent);
    _d.CPGF_MD_TEMPLATE _method("isEmpty", &D::ClassType::isEmpty);
    _d.CPGF_MD_TEMPLATE _method("getVolume", &D::ClassType::getVolume);
    _d.CPGF_MD_TEMPLATE _method("getArea", &D::ClassType::getArea);
    _d.CPGF_MD_TEMPLATE _method("getEdges", &D::ClassType::getEdges);
    _d.CPGF_MD_TEMPLATE _method("repair", &D::ClassType::repair);
    _d.CPGF_MD_TEMPLATE _method("getInterpolated", &D::ClassType::getInterpolated, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isPointInside", &D::ClassType::isPointInside, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isPointTotalInside", &D::ClassType::isPointTotalInside, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isFullInside", &D::ClassType::isFullInside, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("intersectsWithBox", &D::ClassType::intersectsWithBox, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("intersectsWithLine", (bool (D::ClassType::*) (const line3d< T > &) const)&D::ClassType::intersectsWithLine);
    _d.CPGF_MD_TEMPLATE _method("intersectsWithLine", (bool (D::ClassType::*) (const vector3d< T > &, const vector3d< T > &, T) const)&D::ClassType::intersectsWithLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("classifyPlaneRelation", &D::ClassType::classifyPlaneRelation);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const aabbox3d< T > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const aabbox3d<T> *, const aabbox3d< T > &))&opErAToRWrapper_aabbox3d__opEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const aabbox3d< T > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const aabbox3d<T> *, const aabbox3d< T > &))&opErAToRWrapper_aabbox3d__opNotEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
