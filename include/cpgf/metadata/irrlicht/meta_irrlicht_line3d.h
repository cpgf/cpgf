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


template <typename D>
void buildMetaClass_Global_line3d(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


template <typename D, typename T>
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
    _d.CPGF_MD_TEMPLATE _operator<line3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<line3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<line3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const line3d< T > &)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const line3d< T > &)>(mopHolder != mopHolder);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
