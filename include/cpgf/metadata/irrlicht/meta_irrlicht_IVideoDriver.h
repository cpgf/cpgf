// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IVIDEODRIVER_H
#define __META_IRRLICHT_IVIDEODRIVER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::scene;
using namespace irr::video;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_ivideodriver(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("FogTypeNames", &FogTypeNames);
    _d.CPGF_MD_TEMPLATE _enum<E_FOG_TYPE>("E_FOG_TYPE")
        ._element("EFT_FOG_EXP", irr::video::EFT_FOG_EXP)
        ._element("EFT_FOG_LINEAR", irr::video::EFT_FOG_LINEAR)
        ._element("EFT_FOG_EXP2", irr::video::EFT_FOG_EXP2)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_LOST_RESSOURCE>("E_LOST_RESSOURCE")
        ._element("ELR_DEVICE", irr::video::ELR_DEVICE)
        ._element("ELR_TEXTURES", irr::video::ELR_TEXTURES)
        ._element("ELR_RTTS", irr::video::ELR_RTTS)
        ._element("ELR_HW_BUFFERS", irr::video::ELR_HW_BUFFERS)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_RENDER_TARGET>("E_RENDER_TARGET")
        ._element("ERT_FRAME_BUFFER", irr::video::ERT_FRAME_BUFFER)
        ._element("ERT_RENDER_TEXTURE", irr::video::ERT_RENDER_TEXTURE)
        ._element("ERT_MULTI_RENDER_TEXTURES", irr::video::ERT_MULTI_RENDER_TEXTURES)
        ._element("ERT_STEREO_LEFT_BUFFER", irr::video::ERT_STEREO_LEFT_BUFFER)
        ._element("ERT_STEREO_RIGHT_BUFFER", irr::video::ERT_STEREO_RIGHT_BUFFER)
        ._element("ERT_STEREO_BOTH_BUFFERS", irr::video::ERT_STEREO_BOTH_BUFFERS)
        ._element("ERT_AUX_BUFFER0", irr::video::ERT_AUX_BUFFER0)
        ._element("ERT_AUX_BUFFER1", irr::video::ERT_AUX_BUFFER1)
        ._element("ERT_AUX_BUFFER2", irr::video::ERT_AUX_BUFFER2)
        ._element("ERT_AUX_BUFFER3", irr::video::ERT_AUX_BUFFER3)
        ._element("ERT_AUX_BUFFER4", irr::video::ERT_AUX_BUFFER4)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_TRANSFORMATION_STATE>("E_TRANSFORMATION_STATE")
        ._element("ETS_VIEW", irr::video::ETS_VIEW)
        ._element("ETS_WORLD", irr::video::ETS_WORLD)
        ._element("ETS_PROJECTION", irr::video::ETS_PROJECTION)
        ._element("ETS_TEXTURE_0", irr::video::ETS_TEXTURE_0)
        ._element("ETS_TEXTURE_1", irr::video::ETS_TEXTURE_1)
        ._element("ETS_TEXTURE_2", irr::video::ETS_TEXTURE_2)
        ._element("ETS_TEXTURE_3", irr::video::ETS_TEXTURE_3)
        ._element("ETS_COUNT", irr::video::ETS_COUNT)
    ;
}


inline E_BLEND_FACTOR bItFiEldWrapper_IRenderTarget_BlendFuncDst_getter(IRenderTarget * self) {
    return self->BlendFuncDst;
}

inline void bItFiEldWrapper_IRenderTarget_BlendFuncDst_setter(IRenderTarget * self, E_BLEND_FACTOR value) {
    self->BlendFuncDst = value;
}
inline E_BLEND_FACTOR bItFiEldWrapper_IRenderTarget_BlendFuncSrc_getter(IRenderTarget * self) {
    return self->BlendFuncSrc;
}

inline void bItFiEldWrapper_IRenderTarget_BlendFuncSrc_setter(IRenderTarget * self, E_BLEND_FACTOR value) {
    self->BlendFuncSrc = value;
}
inline E_COLOR_PLANE bItFiEldWrapper_IRenderTarget_ColorMask_getter(IRenderTarget * self) {
    return self->ColorMask;
}

inline void bItFiEldWrapper_IRenderTarget_ColorMask_setter(IRenderTarget * self, E_COLOR_PLANE value) {
    self->ColorMask = value;
}
inline E_RENDER_TARGET bItFiEldWrapper_IRenderTarget_TargetType_getter(IRenderTarget * self) {
    return self->TargetType;
}

inline void bItFiEldWrapper_IRenderTarget_TargetType_setter(IRenderTarget * self, E_RENDER_TARGET value) {
    self->TargetType = value;
}


template <typename D>
void buildMetaClass_IRenderTarget(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (ITexture *, E_COLOR_PLANE, E_BLEND_FACTOR, E_BLEND_FACTOR, bool)>()
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(EBF_ONE_MINUS_SRC_ALPHA))
        ._default(copyVariantFromCopyable(EBF_ONE))
        ._default(copyVariantFromCopyable(ECP_ALL))
    ;
    _d.CPGF_MD_TEMPLATE _constructor<void * (E_RENDER_TARGET, E_COLOR_PLANE, E_BLEND_FACTOR, E_BLEND_FACTOR, bool)>()
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(EBF_ONE_MINUS_SRC_ALPHA))
        ._default(copyVariantFromCopyable(EBF_ONE))
        ._default(copyVariantFromCopyable(ECP_ALL))
    ;
    _d.CPGF_MD_TEMPLATE _field("BlendEnable", &D::ClassType::BlendEnable);
    _d.CPGF_MD_TEMPLATE _property("BlendFuncDst", &bItFiEldWrapper_IRenderTarget_BlendFuncDst_getter, &bItFiEldWrapper_IRenderTarget_BlendFuncDst_setter, MakePolicy<GMetaRuleGetterExplicitThis, GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("BlendFuncSrc", &bItFiEldWrapper_IRenderTarget_BlendFuncSrc_getter, &bItFiEldWrapper_IRenderTarget_BlendFuncSrc_setter, MakePolicy<GMetaRuleGetterExplicitThis, GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("ColorMask", &bItFiEldWrapper_IRenderTarget_ColorMask_getter, &bItFiEldWrapper_IRenderTarget_ColorMask_setter, MakePolicy<GMetaRuleGetterExplicitThis, GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _field("RenderTexture", &D::ClassType::RenderTexture);
    _d.CPGF_MD_TEMPLATE _property("TargetType", &bItFiEldWrapper_IRenderTarget_TargetType_getter, &bItFiEldWrapper_IRenderTarget_TargetType_setter, MakePolicy<GMetaRuleGetterExplicitThis, GMetaRuleSetterExplicitThis>());
}


template <typename D>
void buildMetaClass_IVideoDriver(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("addDynamicLight", &D::ClassType::addDynamicLight);
    _d.CPGF_MD_TEMPLATE _method("addExternalImageLoader", &D::ClassType::addExternalImageLoader);
    _d.CPGF_MD_TEMPLATE _method("addExternalImageWriter", &D::ClassType::addExternalImageWriter);
    _d.CPGF_MD_TEMPLATE _method("addMaterialRenderer", &D::ClassType::addMaterialRenderer)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addRenderTargetTexture", &D::ClassType::addRenderTargetTexture, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(ECF_UNKNOWN))
        ._default(copyVariantFromCopyable("rt"))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTexture", (ITexture * (D::ClassType::*) (const core::dimension2d< u32 > &, const io::path &, ECOLOR_FORMAT))&D::ClassType::addTexture, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(ECF_A8R8G8B8))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTexture", (ITexture * (D::ClassType::*) (const io::path &, IImage *, void *))&D::ClassType::addTexture, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("beginScene", &D::ClassType::beginScene, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<3> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(SExposedVideoData()))
        ._default(copyVariantFromCopyable(SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("checkDriverReset", &D::ClassType::checkDriverReset);
    _d.CPGF_MD_TEMPLATE _method("clearZBuffer", &D::ClassType::clearZBuffer);
    _d.CPGF_MD_TEMPLATE _method("createAttributesFromMaterial", &D::ClassType::createAttributesFromMaterial);
    _d.CPGF_MD_TEMPLATE _method("createImage", (IImage * (D::ClassType::*) (ECOLOR_FORMAT, const core::dimension2d< u32 > &))&D::ClassType::createImage);
    _d.CPGF_MD_TEMPLATE _method("createImage", (IImage * (D::ClassType::*) (ECOLOR_FORMAT, IImage *))&D::ClassType::createImage);
    _d.CPGF_MD_TEMPLATE _method("createImage", (IImage * (D::ClassType::*) (IImage *, const core::position2d< s32 > &, const core::dimension2d< u32 > &))&D::ClassType::createImage);
    _d.CPGF_MD_TEMPLATE _method("createImage", (IImage * (D::ClassType::*) (ITexture *, const core::position2d< s32 > &, const core::dimension2d< u32 > &))&D::ClassType::createImage);
    _d.CPGF_MD_TEMPLATE _method("createImageFromData", &D::ClassType::createImageFromData)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("createImageFromFile", (IImage * (D::ClassType::*) (const io::path &))&D::ClassType::createImageFromFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("createImageFromFile", (IImage * (D::ClassType::*) (io::IReadFile *))&D::ClassType::createImageFromFile);
    _d.CPGF_MD_TEMPLATE _method("createScreenShot", &D::ClassType::createScreenShot);
    _d.CPGF_MD_TEMPLATE _method("deleteAllDynamicLights", &D::ClassType::deleteAllDynamicLights);
    _d.CPGF_MD_TEMPLATE _method("disableFeature", &D::ClassType::disableFeature)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DImage", (void (D::ClassType::*) (const video::ITexture *, const core::position2d< s32 > &))&D::ClassType::draw2DImage);
    _d.CPGF_MD_TEMPLATE _method("draw2DImage", (void (D::ClassType::*) (const video::ITexture *, const core::position2d< s32 > &, const core::rect< s32 > &, const core::rect< s32 > *, SColor, bool))&D::ClassType::draw2DImage)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DImage", (void (D::ClassType::*) (const video::ITexture *, const core::rect< s32 > &, const core::rect< s32 > &, const core::rect< s32 > *, const video::SColor *const, bool))&D::ClassType::draw2DImage)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DImageBatch", (void (D::ClassType::*) (const video::ITexture *, const core::position2d< s32 > &, const core::array< core::rect< s32 > > &, const core::array< s32 > &, s32, const core::rect< s32 > *, SColor, bool))&D::ClassType::draw2DImageBatch)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DImageBatch", (void (D::ClassType::*) (const video::ITexture *, const core::array< core::position2d< s32 > > &, const core::array< core::rect< s32 > > &, const core::rect< s32 > *, SColor, bool))&D::ClassType::draw2DImageBatch)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DLine", &D::ClassType::draw2DLine)
        ._default(copyVariantFromCopyable(SColor(255, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DPolygon", &D::ClassType::draw2DPolygon)
        ._default(copyVariantFromCopyable(10))
        ._default(copyVariantFromCopyable(SColor(100, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DRectangle", (void (D::ClassType::*) (SColor, const core::rect< s32 > &, const core::rect< s32 > *))&D::ClassType::draw2DRectangle)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DRectangle", (void (D::ClassType::*) (const core::rect< s32 > &, SColor, SColor, SColor, SColor, const core::rect< s32 > *))&D::ClassType::draw2DRectangle)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DRectangleOutline", &D::ClassType::draw2DRectangleOutline)
        ._default(copyVariantFromCopyable(SColor(255, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw2DVertexPrimitiveList", &D::ClassType::draw2DVertexPrimitiveList)
        ._default(copyVariantFromCopyable(EIT_16BIT))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLES))
        ._default(copyVariantFromCopyable(EVT_STANDARD))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DBox", &D::ClassType::draw3DBox)
        ._default(copyVariantFromCopyable(SColor(255, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DLine", &D::ClassType::draw3DLine)
        ._default(copyVariantFromCopyable(SColor(255, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("draw3DTriangle", &D::ClassType::draw3DTriangle)
        ._default(copyVariantFromCopyable(SColor(255, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("drawIndexedTriangleFan", (void (D::ClassType::*) (const S3DVertex *, u32, const u16 *, u32))&D::ClassType::drawIndexedTriangleFan);
    _d.CPGF_MD_TEMPLATE _method("drawIndexedTriangleFan", (void (D::ClassType::*) (const S3DVertex2TCoords *, u32, const u16 *, u32))&D::ClassType::drawIndexedTriangleFan);
    _d.CPGF_MD_TEMPLATE _method("drawIndexedTriangleFan", (void (D::ClassType::*) (const S3DVertexTangents *, u32, const u16 *, u32))&D::ClassType::drawIndexedTriangleFan);
    _d.CPGF_MD_TEMPLATE _method("drawIndexedTriangleList", (void (D::ClassType::*) (const S3DVertex *, u32, const u16 *, u32))&D::ClassType::drawIndexedTriangleList);
    _d.CPGF_MD_TEMPLATE _method("drawIndexedTriangleList", (void (D::ClassType::*) (const S3DVertex2TCoords *, u32, const u16 *, u32))&D::ClassType::drawIndexedTriangleList);
    _d.CPGF_MD_TEMPLATE _method("drawIndexedTriangleList", (void (D::ClassType::*) (const S3DVertexTangents *, u32, const u16 *, u32))&D::ClassType::drawIndexedTriangleList);
    _d.CPGF_MD_TEMPLATE _method("drawMeshBuffer", &D::ClassType::drawMeshBuffer);
    _d.CPGF_MD_TEMPLATE _method("drawPixel", &D::ClassType::drawPixel, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<2> >());
    _d.CPGF_MD_TEMPLATE _method("drawStencilShadow", &D::ClassType::drawStencilShadow)
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(video::SColor(255, 0, 0, 0)))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("drawStencilShadowVolume", &D::ClassType::drawStencilShadowVolume)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("drawVertexPrimitiveList", &D::ClassType::drawVertexPrimitiveList)
        ._default(copyVariantFromCopyable(EIT_16BIT))
        ._default(copyVariantFromCopyable(scene::EPT_TRIANGLES))
        ._default(copyVariantFromCopyable(EVT_STANDARD))
    ;
    _d.CPGF_MD_TEMPLATE _method("enableClipPlane", &D::ClassType::enableClipPlane);
    _d.CPGF_MD_TEMPLATE _method("enableMaterial2D", &D::ClassType::enableMaterial2D)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("endScene", &D::ClassType::endScene);
    _d.CPGF_MD_TEMPLATE _method("fillMaterialStructureFromAttributes", &D::ClassType::fillMaterialStructureFromAttributes);
    _d.CPGF_MD_TEMPLATE _method("findTexture", &D::ClassType::findTexture, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getColorFormat", &D::ClassType::getColorFormat);
    _d.CPGF_MD_TEMPLATE _method("getCurrentRenderTargetSize", &D::ClassType::getCurrentRenderTargetSize);
    _d.CPGF_MD_TEMPLATE _method("getDriverType", &D::ClassType::getDriverType);
    _d.CPGF_MD_TEMPLATE _method("getDynamicLight", &D::ClassType::getDynamicLight);
    _d.CPGF_MD_TEMPLATE _method("getDynamicLightCount", &D::ClassType::getDynamicLightCount);
    _d.CPGF_MD_TEMPLATE _method("getExposedVideoData", &D::ClassType::getExposedVideoData, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _method("getFog", &D::ClassType::getFog);
    _d.CPGF_MD_TEMPLATE _method("getFPS", &D::ClassType::getFPS);
    _d.CPGF_MD_TEMPLATE _method("getGPUProgrammingServices", &D::ClassType::getGPUProgrammingServices);
    _d.CPGF_MD_TEMPLATE _method("getImageLoader", &D::ClassType::getImageLoader);
    _d.CPGF_MD_TEMPLATE _method("getImageLoaderCount", &D::ClassType::getImageLoaderCount);
    _d.CPGF_MD_TEMPLATE _method("getImageWriter", &D::ClassType::getImageWriter);
    _d.CPGF_MD_TEMPLATE _method("getImageWriterCount", &D::ClassType::getImageWriterCount);
    _d.CPGF_MD_TEMPLATE _method("getMaterial2D", &D::ClassType::getMaterial2D);
    _d.CPGF_MD_TEMPLATE _method("getMaterialRenderer", &D::ClassType::getMaterialRenderer);
    _d.CPGF_MD_TEMPLATE _method("getMaterialRendererCount", &D::ClassType::getMaterialRendererCount);
    _d.CPGF_MD_TEMPLATE _method("getMaterialRendererName", &D::ClassType::getMaterialRendererName);
    _d.CPGF_MD_TEMPLATE _method("getMaximalDynamicLightAmount", &D::ClassType::getMaximalDynamicLightAmount);
    _d.CPGF_MD_TEMPLATE _method("getMaximalPrimitiveCount", &D::ClassType::getMaximalPrimitiveCount);
    _d.CPGF_MD_TEMPLATE _method("getMaxTextureSize", &D::ClassType::getMaxTextureSize);
    _d.CPGF_MD_TEMPLATE _method("getMeshManipulator", &D::ClassType::getMeshManipulator);
    _d.CPGF_MD_TEMPLATE _method("getName", &D::ClassType::getName);
    _d.CPGF_MD_TEMPLATE _method("getOverrideMaterial", &D::ClassType::getOverrideMaterial);
    _d.CPGF_MD_TEMPLATE _method("getPrimitiveCountDrawn", &D::ClassType::getPrimitiveCountDrawn)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getScreenSize", &D::ClassType::getScreenSize);
    _d.CPGF_MD_TEMPLATE _method("getTexture", (ITexture * (D::ClassType::*) (const io::path &))&D::ClassType::getTexture, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getTexture", (ITexture * (D::ClassType::*) (io::IReadFile *))&D::ClassType::getTexture);
    _d.CPGF_MD_TEMPLATE _method("getTextureByIndex", &D::ClassType::getTextureByIndex);
    _d.CPGF_MD_TEMPLATE _method("getTextureCount", &D::ClassType::getTextureCount);
    _d.CPGF_MD_TEMPLATE _method("getTextureCreationFlag", &D::ClassType::getTextureCreationFlag);
    _d.CPGF_MD_TEMPLATE _method("getTransform", &D::ClassType::getTransform);
    _d.CPGF_MD_TEMPLATE _method("getVendorInfo", &D::ClassType::getVendorInfo);
    _d.CPGF_MD_TEMPLATE _method("getViewPort", &D::ClassType::getViewPort);
    _d.CPGF_MD_TEMPLATE _method("makeColorKeyTexture", (void (D::ClassType::*) (video::ITexture *, video::SColor, bool) const)&D::ClassType::makeColorKeyTexture)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("makeColorKeyTexture", (void (D::ClassType::*) (video::ITexture *, core::position2d< s32 >, bool) const)&D::ClassType::makeColorKeyTexture)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("makeNormalMapTexture", &D::ClassType::makeNormalMapTexture)
        ._default(copyVariantFromCopyable(1.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("OnResize", &D::ClassType::OnResize);
    _d.CPGF_MD_TEMPLATE _method("queryFeature", &D::ClassType::queryFeature);
    _d.CPGF_MD_TEMPLATE _method("removeAllHardwareBuffers", &D::ClassType::removeAllHardwareBuffers);
    _d.CPGF_MD_TEMPLATE _method("removeAllTextures", &D::ClassType::removeAllTextures);
    _d.CPGF_MD_TEMPLATE _method("removeHardwareBuffer", &D::ClassType::removeHardwareBuffer);
    _d.CPGF_MD_TEMPLATE _method("removeTexture", &D::ClassType::removeTexture);
    _d.CPGF_MD_TEMPLATE _method("renameTexture", &D::ClassType::renameTexture, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("setAllowZWriteOnTransparent", &D::ClassType::setAllowZWriteOnTransparent);
    _d.CPGF_MD_TEMPLATE _method("setAmbientLight", &D::ClassType::setAmbientLight, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("setClipPlane", &D::ClassType::setClipPlane)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("setFog", &D::ClassType::setFog)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0.01f))
        ._default(copyVariantFromCopyable(100.0f))
        ._default(copyVariantFromCopyable(50.0f))
        ._default(copyVariantFromCopyable(EFT_FOG_LINEAR))
        ._default(copyVariantFromCopyable(SColor(0, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("setMaterial", &D::ClassType::setMaterial);
    _d.CPGF_MD_TEMPLATE _method("setMaterialRendererName", &D::ClassType::setMaterialRendererName);
    _d.CPGF_MD_TEMPLATE _method("setMinHardwareBufferVertexCount", &D::ClassType::setMinHardwareBufferVertexCount);
    _d.CPGF_MD_TEMPLATE _method("setRenderTarget", (bool (D::ClassType::*) (video::ITexture *, bool, bool, SColor))&D::ClassType::setRenderTarget)
        ._default(copyVariantFromCopyable(video::SColor(0, 0, 0, 0)))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("setRenderTarget", (bool (D::ClassType::*) (E_RENDER_TARGET, bool, bool, SColor))&D::ClassType::setRenderTarget)
        ._default(copyVariantFromCopyable(video::SColor(0, 0, 0, 0)))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("setRenderTarget", (bool (D::ClassType::*) (const core::array< video::IRenderTarget > &, bool, bool, SColor))&D::ClassType::setRenderTarget)
        ._default(copyVariantFromCopyable(video::SColor(0, 0, 0, 0)))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("setTextureCreationFlag", &D::ClassType::setTextureCreationFlag)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("setTransform", &D::ClassType::setTransform);
    _d.CPGF_MD_TEMPLATE _method("setViewPort", &D::ClassType::setViewPort);
    _d.CPGF_MD_TEMPLATE _method("turnLightOn", &D::ClassType::turnLightOn);
    _d.CPGF_MD_TEMPLATE _method("writeImageToFile", (bool (D::ClassType::*) (IImage *, const io::path &, u32))&D::ClassType::writeImageToFile, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("writeImageToFile", (bool (D::ClassType::*) (IImage *, io::IWriteFile *, u32))&D::ClassType::writeImageToFile)
        ._default(copyVariantFromCopyable(0))
    ;
}


template <typename D>
void buildMetaClass_SOverrideMaterial(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("Enabled", &D::ClassType::Enabled);
    _d.CPGF_MD_TEMPLATE _field("EnableFlags", &D::ClassType::EnableFlags);
    _d.CPGF_MD_TEMPLATE _field("EnablePasses", &D::ClassType::EnablePasses);
    _d.CPGF_MD_TEMPLATE _field("Material", &D::ClassType::Material);
    _d.CPGF_MD_TEMPLATE _method("apply", &D::ClassType::apply);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
