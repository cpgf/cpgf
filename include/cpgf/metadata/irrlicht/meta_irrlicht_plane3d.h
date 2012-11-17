// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_PLANE3D_H
#define __META_IRRLICHT_PLANE3D_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_plane3d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<EIntersectionRelation3D>("EIntersectionRelation3D")
        ._element("ISREL3D_FRONT", irr::core::ISREL3D_FRONT)
        ._element("ISREL3D_BACK", irr::core::ISREL3D_BACK)
        ._element("ISREL3D_PLANAR", irr::core::ISREL3D_PLANAR)
        ._element("ISREL3D_SPANNING", irr::core::ISREL3D_SPANNING)
        ._element("ISREL3D_CLIPPED", irr::core::ISREL3D_CLIPPED)
    ;
}


template <class T>
inline bool opErAToRWrapper_plane3d__opEqual(const plane3d<T> * self, const plane3d< T > & other) {
    return (*self) == other;
}
template <class T>
inline bool opErAToRWrapper_plane3d__opNotEqual(const plane3d<T> * self, const plane3d< T > & other) {
    return (*self) != other;
}


template <typename D, class T>
void buildMetaClass_Plane3d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector3d< T > &, const vector3d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (T, T, T, T, T, T)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector3d< T > &, const vector3d< T > &, const vector3d< T > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleCopyConstReference<2> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const vector3d< T > &, const T)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _field("Normal", &D::ClassType::Normal);
    _d.CPGF_MD_TEMPLATE _field("D", &D::ClassType::D);
    _d.CPGF_MD_TEMPLATE _method("setPlane", (void (D::ClassType::*) (const vector3d< T > &, const vector3d< T > &))&D::ClassType::setPlane, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("setPlane", (void (D::ClassType::*) (const vector3d< T > &, T))&D::ClassType::setPlane, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setPlane", (void (D::ClassType::*) (const vector3d< T > &, const vector3d< T > &, const vector3d< T > &))&D::ClassType::setPlane, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleCopyConstReference<2> >());
    _d.CPGF_MD_TEMPLATE _method("getIntersectionWithLine", &D::ClassType::getIntersectionWithLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getKnownIntersectionWithLine", &D::ClassType::getKnownIntersectionWithLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getIntersectionWithLimitedLine", &D::ClassType::getIntersectionWithLimitedLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("classifyPointRelation", &D::ClassType::classifyPointRelation, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("recalculateD", &D::ClassType::recalculateD, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getMemberPoint", &D::ClassType::getMemberPoint);
    _d.CPGF_MD_TEMPLATE _method("existsIntersection", &D::ClassType::existsIntersection);
    _d.CPGF_MD_TEMPLATE _method("getIntersectionWithPlane", &D::ClassType::getIntersectionWithPlane);
    _d.CPGF_MD_TEMPLATE _method("getIntersectionWithPlanes", &D::ClassType::getIntersectionWithPlanes);
    _d.CPGF_MD_TEMPLATE _method("isFrontFacing", &D::ClassType::isFrontFacing, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getDistanceTo", &D::ClassType::getDistanceTo, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const plane3d< T > &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const plane3d<T> *, const plane3d< T > &))&opErAToRWrapper_plane3d__opEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const plane3d< T > &)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const plane3d<T> *, const plane3d< T > &))&opErAToRWrapper_plane3d__opNotEqual<T>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
