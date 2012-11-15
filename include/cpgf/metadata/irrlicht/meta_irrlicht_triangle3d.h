// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_TRIANGLE3D_H
#define __META_IRRLICHT_TRIANGLE3D_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D, class T>
void buildMetaClass_Triangle3d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (vector3d< T >, vector3d< T >, vector3d< T >)>();
    _d.CPGF_MD_TEMPLATE _field("pointA", &D::ClassType::pointA);
    _d.CPGF_MD_TEMPLATE _field("pointB", &D::ClassType::pointB);
    _d.CPGF_MD_TEMPLATE _field("pointC", &D::ClassType::pointC);
    _d.CPGF_MD_TEMPLATE _method("isTotalInsideBox", &D::ClassType::isTotalInsideBox, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isTotalOutsideBox", &D::ClassType::isTotalOutsideBox, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("closestPointOnTriangle", &D::ClassType::closestPointOnTriangle);
    _d.CPGF_MD_TEMPLATE _method("isPointInside", &D::ClassType::isPointInside, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("isPointInsideFast", &D::ClassType::isPointInsideFast, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getIntersectionWithLimitedLine", &D::ClassType::getIntersectionWithLimitedLine);
    _d.CPGF_MD_TEMPLATE _method("getIntersectionWithLine", &D::ClassType::getIntersectionWithLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getIntersectionOfPlaneWithLine", &D::ClassType::getIntersectionOfPlaneWithLine, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getNormal", &D::ClassType::getNormal);
    _d.CPGF_MD_TEMPLATE _method("isFrontFacing", &D::ClassType::isFrontFacing, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getPlane", &D::ClassType::getPlane);
    _d.CPGF_MD_TEMPLATE _method("getArea", &D::ClassType::getArea);
    _d.CPGF_MD_TEMPLATE _method("set", &D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const triangle3d< T > &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const triangle3d< T > &)>(mopHolder != mopHolder);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
