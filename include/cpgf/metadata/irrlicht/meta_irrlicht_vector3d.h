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


template <typename D, typename T>
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
    _d.CPGF_MD_TEMPLATE _method("crossProduct", &D::ClassType::crossProduct, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("dotProduct", &D::ClassType::dotProduct, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("equals", &D::ClassType::equals, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable((T) ROUNDING_ERROR_f32))
    ;
    _d.CPGF_MD_TEMPLATE _method("getAs4Values", &D::ClassType::getAs4Values);
    _d.CPGF_MD_TEMPLATE _method("getDistanceFrom", &D::ClassType::getDistanceFrom, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getDistanceFromSQ", &D::ClassType::getDistanceFromSQ, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getHorizontalAngle", &D::ClassType::getHorizontalAngle);
    _d.CPGF_MD_TEMPLATE _method("getInterpolated", &D::ClassType::getInterpolated, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getInterpolated_quadratic", &D::ClassType::getInterpolated_quadratic, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("getLength", &D::ClassType::getLength);
    _d.CPGF_MD_TEMPLATE _method("getLengthSQ", &D::ClassType::getLengthSQ);
    _d.CPGF_MD_TEMPLATE _method("getSphericalCoordinateAngles", (vector3d< T > (D::ClassType::*) ())&D::ClassType::getSphericalCoordinateAngles);
    _d.CPGF_MD_TEMPLATE _method("getSphericalCoordinateAngles", (vector3d< s32 > (D::ClassType::*) ())&D::ClassType::getSphericalCoordinateAngles);
    _d.CPGF_MD_TEMPLATE _method("interpolate", &D::ClassType::interpolate, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("invert", &D::ClassType::invert);
    _d.CPGF_MD_TEMPLATE _method("isBetweenPoints", &D::ClassType::isBetweenPoints, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("normalize", &D::ClassType::normalize);
    _d.CPGF_MD_TEMPLATE _method("rotateXYBy", &D::ClassType::rotateXYBy, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(vector3d< T >()))
    ;
    _d.CPGF_MD_TEMPLATE _method("rotateXZBy", &D::ClassType::rotateXZBy, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(vector3d< T >()))
    ;
    _d.CPGF_MD_TEMPLATE _method("rotateYZBy", &D::ClassType::rotateYZBy, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(vector3d< T >()))
    ;
    _d.CPGF_MD_TEMPLATE _method("rotationToDirection", &D::ClassType::rotationToDirection, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(vector3d< T >(0, 0, 1)))
    ;
    _d.CPGF_MD_TEMPLATE _method("set", (vector3d< T > & (D::ClassType::*) (const T, const T, const T))&D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("set", (vector3d< T > & (D::ClassType::*) (const vector3d< T > &))&D::ClassType::set, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setLength", &D::ClassType::setLength);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder * mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder *= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder *= mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &)>(-mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder - mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder -= mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder / mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > (*)(const cpgf::GMetaSelf &, const T)>(mopHolder / mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< s32 > (*)(const vector3d<s32> &, s32)>(mopHolder / mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder /= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const T)>(mopHolder /= mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<vector3d< s32 > & (*)(vector3d<s32>, s32)>(mopHolder /= mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder <= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<vector3d< T > & (*)(cpgf::GMetaSelf, const vector3d< T > &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder > mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const vector3d< T > &)>(mopHolder >= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
