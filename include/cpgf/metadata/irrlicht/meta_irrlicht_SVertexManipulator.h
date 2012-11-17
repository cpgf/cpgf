// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SVERTEXMANIPULATOR_H
#define __META_IRRLICHT_SVERTEXMANIPULATOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IVertexManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
}


inline void opErAToRWrapper_SVertexColorBrightnessManipulator__opFunction(const SVertexColorBrightnessManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorBrightnessManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (s32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorBrightnessManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorBrightnessManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorContrastBrightnessManipulator__opFunction(const SVertexColorContrastBrightnessManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorContrastBrightnessManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32, s32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorContrastBrightnessManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorContrastBrightnessManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorContrastManipulator__opFunction(const SVertexColorContrastManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorContrastManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorContrastManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorContrastManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorDesaturateToAverageManipulator__opFunction(const SVertexColorDesaturateToAverageManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorDesaturateToAverageManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorDesaturateToAverageManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorDesaturateToAverageManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorDesaturateToLightnessManipulator__opFunction(const SVertexColorDesaturateToLightnessManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorDesaturateToLightnessManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorDesaturateToLightnessManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorDesaturateToLightnessManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorDesaturateToLuminanceManipulator__opFunction(const SVertexColorDesaturateToLuminanceManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorDesaturateToLuminanceManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorDesaturateToLuminanceManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorDesaturateToLuminanceManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorGammaManipulator__opFunction(const SVertexColorGammaManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorGammaManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorGammaManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorGammaManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorInterpolateLinearManipulator__opFunction(const SVertexColorInterpolateLinearManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorInterpolateLinearManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::SColor, f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorInterpolateLinearManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorInterpolateLinearManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorInterpolateQuadraticManipulator__opFunction(const SVertexColorInterpolateQuadraticManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorInterpolateQuadraticManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::SColor, video::SColor, f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorInterpolateQuadraticManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorInterpolateQuadraticManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorInvertManipulator__opFunction(const SVertexColorInvertManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorInvertManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorInvertManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorInvertManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorScaleManipulator__opFunction(const SVertexColorScaleManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorScaleManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (f32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorScaleManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorScaleManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorSetAlphaManipulator__opFunction(const SVertexColorSetAlphaManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorSetAlphaManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (u32)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorSetAlphaManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorSetAlphaManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorSetManipulator__opFunction(const SVertexColorSetManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorSetManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::SColor)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorSetManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorSetManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


inline void opErAToRWrapper_SVertexColorThresholdManipulator__opFunction(const SVertexColorThresholdManipulator * self, video::S3DVertex & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexColorThresholdManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (u8, video::SColor, video::SColor)>();
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexColorThresholdManipulator *, video::S3DVertex &))&opErAToRWrapper_SVertexColorThresholdManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


template <typename D>
void buildMetaClass_SVertexPositionScaleAlongNormalsManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &)>();
}


template <typename D>
void buildMetaClass_SVertexPositionScaleManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector3df &)>();
}


template <typename D>
void buildMetaClass_SVertexPositionTransformManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::matrix4 &)>();
}


inline void opErAToRWrapper_SVertexTCoordsScaleManipulator__opFunction(const SVertexTCoordsScaleManipulator * self, video::S3DVertex2TCoords & vertex) {
    (*self)(vertex);
}


template <typename D>
void buildMetaClass_SVertexTCoordsScaleManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (const core::vector2df &, u32)>()
        ._default(copyVariantFromCopyable(1))
    ;
    _d.CPGF_MD_TEMPLATE _operator<void (*)(video::S3DVertex2TCoords &)>(mopHolder(mopHolder));
    _d.CPGF_MD_TEMPLATE _method("_opFunction", (void (*) (const SVertexTCoordsScaleManipulator *, video::S3DVertex2TCoords &))&opErAToRWrapper_SVertexTCoordsScaleManipulator__opFunction, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
