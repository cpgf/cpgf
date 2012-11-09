// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_MATRIX4_H
#define __META_IRRLICHT_MATRIX4_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_matrix4(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("IdentityMatrix", &IdentityMatrix);
}


template <typename D, typename T>
void buildMetaClass_CMatrix4(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (typename CMatrix4<T >::eConstructor)>()
        ._default(copyVariantFromCopyable(CMatrix4<T >::EM4CONST_IDENTITY))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (const CMatrix4< T > &, typename CMatrix4<T >::eConstructor)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(CMatrix4<T >::EM4CONST_COPY))
    ;
    _d.CPGF_MD_TEMPLATE _method("pointer", (const T * (D::ClassType::*) () const)&D::ClassType::pointer);
    _d.CPGF_MD_TEMPLATE _method("pointer", (T * (D::ClassType::*) ())&D::ClassType::pointer);
    _d.CPGF_MD_TEMPLATE _method("setbyproduct", &D::ClassType::setbyproduct, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("setbyproduct_nocheck", &D::ClassType::setbyproduct_nocheck, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("makeIdentity", &D::ClassType::makeIdentity);
    _d.CPGF_MD_TEMPLATE _method("isIdentity", &D::ClassType::isIdentity);
    _d.CPGF_MD_TEMPLATE _method("isOrthogonal", &D::ClassType::isOrthogonal);
    _d.CPGF_MD_TEMPLATE _method("isIdentity_integer_base", &D::ClassType::isIdentity_integer_base);
    _d.CPGF_MD_TEMPLATE _method("setTranslation", &D::ClassType::setTranslation, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getTranslation", &D::ClassType::getTranslation);
    _d.CPGF_MD_TEMPLATE _method("setInverseTranslation", &D::ClassType::setInverseTranslation, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setRotationRadians", &D::ClassType::setRotationRadians, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setRotationDegrees", &D::ClassType::setRotationDegrees, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getRotationDegrees", &D::ClassType::getRotationDegrees);
    _d.CPGF_MD_TEMPLATE _method("setInverseRotationRadians", &D::ClassType::setInverseRotationRadians, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setInverseRotationDegrees", &D::ClassType::setInverseRotationDegrees, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setScale", (CMatrix4< T > & (D::ClassType::*) (const vector3d< T > &))&D::ClassType::setScale, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setScale", (CMatrix4< T > & (D::ClassType::*) (const T))&D::ClassType::setScale);
    _d.CPGF_MD_TEMPLATE _method("getScale", &D::ClassType::getScale);
    _d.CPGF_MD_TEMPLATE _method("inverseTranslateVect", &D::ClassType::inverseTranslateVect);
    _d.CPGF_MD_TEMPLATE _method("inverseRotateVect", &D::ClassType::inverseRotateVect);
    _d.CPGF_MD_TEMPLATE _method("rotateVect", (void (D::ClassType::*) (vector3df &) const)&D::ClassType::rotateVect);
    _d.CPGF_MD_TEMPLATE _method("rotateVect", (void (D::ClassType::*) (core::vector3df &, const core::vector3df &) const)&D::ClassType::rotateVect);
    _d.CPGF_MD_TEMPLATE _method("rotateVect", (void (D::ClassType::*) (T *, const core::vector3df &) const)&D::ClassType::rotateVect);
    _d.CPGF_MD_TEMPLATE _method("transformVect", (void (D::ClassType::*) (vector3df &) const)&D::ClassType::transformVect);
    _d.CPGF_MD_TEMPLATE _method("transformVect", (void (D::ClassType::*) (vector3df &, const vector3df &) const)&D::ClassType::transformVect);
    _d.CPGF_MD_TEMPLATE _method("transformVect", (void (D::ClassType::*) (T *, const core::vector3df &) const)&D::ClassType::transformVect);
    _d.CPGF_MD_TEMPLATE _method("translateVect", &D::ClassType::translateVect);
    _d.CPGF_MD_TEMPLATE _method("transformPlane", (void (D::ClassType::*) (core::plane3d< f32 > &) const)&D::ClassType::transformPlane);
    _d.CPGF_MD_TEMPLATE _method("transformPlane", (void (D::ClassType::*) (const core::plane3d< f32 > &, core::plane3d< f32 > &) const)&D::ClassType::transformPlane);
    _d.CPGF_MD_TEMPLATE _method("transformBox", &D::ClassType::transformBox);
    _d.CPGF_MD_TEMPLATE _method("transformBoxEx", &D::ClassType::transformBoxEx);
    _d.CPGF_MD_TEMPLATE _method("multiplyWith1x4Matrix", &D::ClassType::multiplyWith1x4Matrix);
    _d.CPGF_MD_TEMPLATE _method("makeInverse", &D::ClassType::makeInverse);
    _d.CPGF_MD_TEMPLATE _method("getInversePrimitive", &D::ClassType::getInversePrimitive);
    _d.CPGF_MD_TEMPLATE _method("getInverse", &D::ClassType::getInverse);
    _d.CPGF_MD_TEMPLATE _method("buildProjectionMatrixPerspectiveFovRH", &D::ClassType::buildProjectionMatrixPerspectiveFovRH);
    _d.CPGF_MD_TEMPLATE _method("buildProjectionMatrixPerspectiveFovLH", &D::ClassType::buildProjectionMatrixPerspectiveFovLH);
    _d.CPGF_MD_TEMPLATE _method("buildProjectionMatrixPerspectiveRH", &D::ClassType::buildProjectionMatrixPerspectiveRH);
    _d.CPGF_MD_TEMPLATE _method("buildProjectionMatrixPerspectiveLH", &D::ClassType::buildProjectionMatrixPerspectiveLH);
    _d.CPGF_MD_TEMPLATE _method("buildProjectionMatrixOrthoLH", &D::ClassType::buildProjectionMatrixOrthoLH);
    _d.CPGF_MD_TEMPLATE _method("buildProjectionMatrixOrthoRH", &D::ClassType::buildProjectionMatrixOrthoRH);
    _d.CPGF_MD_TEMPLATE _method("buildCameraLookAtMatrixLH", &D::ClassType::buildCameraLookAtMatrixLH);
    _d.CPGF_MD_TEMPLATE _method("buildCameraLookAtMatrixRH", &D::ClassType::buildCameraLookAtMatrixRH);
    _d.CPGF_MD_TEMPLATE _method("buildShadowMatrix", &D::ClassType::buildShadowMatrix)
        ._default(copyVariantFromCopyable(1.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("buildNDCToDCMatrix", &D::ClassType::buildNDCToDCMatrix);
    _d.CPGF_MD_TEMPLATE _method("interpolate", &D::ClassType::interpolate);
    _d.CPGF_MD_TEMPLATE _method("getTransposed", (CMatrix4< T > (D::ClassType::*) () const)&D::ClassType::getTransposed);
    _d.CPGF_MD_TEMPLATE _method("getTransposed", (void (D::ClassType::*) (CMatrix4< T > &) const)&D::ClassType::getTransposed);
    _d.CPGF_MD_TEMPLATE _method("buildRotateFromTo", &D::ClassType::buildRotateFromTo);
    _d.CPGF_MD_TEMPLATE _method("setRotationCenter", &D::ClassType::setRotationCenter);
    _d.CPGF_MD_TEMPLATE _method("buildAxisAlignedBillboard", &D::ClassType::buildAxisAlignedBillboard);
    _d.CPGF_MD_TEMPLATE _method("buildTextureTransform", &D::ClassType::buildTextureTransform);
    _d.CPGF_MD_TEMPLATE _method("setTextureRotationCenter", &D::ClassType::setTextureRotationCenter);
    _d.CPGF_MD_TEMPLATE _method("setTextureTranslate", &D::ClassType::setTextureTranslate);
    _d.CPGF_MD_TEMPLATE _method("setTextureTranslateTransposed", &D::ClassType::setTextureTranslateTransposed);
    _d.CPGF_MD_TEMPLATE _method("setTextureScale", &D::ClassType::setTextureScale);
    _d.CPGF_MD_TEMPLATE _method("setTextureScaleCenter", &D::ClassType::setTextureScaleCenter);
    _d.CPGF_MD_TEMPLATE _method("setM", &D::ClassType::setM);
    _d.CPGF_MD_TEMPLATE _method("setDefinitelyIdentityMatrix", &D::ClassType::setDefinitelyIdentityMatrix);
    _d.CPGF_MD_TEMPLATE _method("getDefinitelyIdentityMatrix", &D::ClassType::getDefinitelyIdentityMatrix);
    _d.CPGF_MD_TEMPLATE _method("equals", &D::ClassType::equals)
        ._default(copyVariantFromCopyable((T) ROUNDING_ERROR_f64))
    ;
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::eConstructor>("eConstructor")
        ._element("EM4CONST_NOTHING", D::ClassType::EM4CONST_NOTHING)
        ._element("EM4CONST_COPY", D::ClassType::EM4CONST_COPY)
        ._element("EM4CONST_IDENTITY", D::ClassType::EM4CONST_IDENTITY)
        ._element("EM4CONST_TRANSPOSED", D::ClassType::EM4CONST_TRANSPOSED)
        ._element("EM4CONST_INVERSE", D::ClassType::EM4CONST_INVERSE)
        ._element("EM4CONST_INVERSE_TRANSPOSED", D::ClassType::EM4CONST_INVERSE_TRANSPOSED)
    ;
    _d.CPGF_MD_TEMPLATE _operator<T & (*)(const s32, const s32)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _operator<const T & (*)(const s32, const s32)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _operator<T & (*)(cpgf::GMetaSelf, u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _operator<const T & (*)(const cpgf::GMetaSelf &, u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > & (*)(cpgf::GMetaSelf, const CMatrix4< T > &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > & (*)(cpgf::GMetaSelf, const T &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const CMatrix4< T > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const CMatrix4< T > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > (*)(const cpgf::GMetaSelf &, const CMatrix4< T > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > & (*)(cpgf::GMetaSelf, const CMatrix4< T > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > (*)(const cpgf::GMetaSelf &, const CMatrix4< T > &)>(mopHolder - mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > & (*)(cpgf::GMetaSelf, const CMatrix4< T > &)>(mopHolder -= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > (*)(const cpgf::GMetaSelf &, const CMatrix4< T > &)>(mopHolder * mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > & (*)(cpgf::GMetaSelf, const CMatrix4< T > &)>(mopHolder *= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > (*)(const cpgf::GMetaSelf &, const T &)>(mopHolder * mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<CMatrix4< T > & (*)(cpgf::GMetaSelf, const T &)>(mopHolder *= mopHolder);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
