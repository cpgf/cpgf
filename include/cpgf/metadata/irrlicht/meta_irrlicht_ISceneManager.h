// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ISCENEMANAGER_H
#define __META_IRRLICHT_ISCENEMANAGER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::gui;
using namespace irr::video;
using namespace irr::scene;
using namespace irr::scene::quake3;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iscenemanager(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_SCENE_NODE_RENDER_PASS>("E_SCENE_NODE_RENDER_PASS")
        ._element("ESNRP_NONE", irr::scene::ESNRP_NONE)
        ._element("ESNRP_CAMERA", irr::scene::ESNRP_CAMERA)
        ._element("ESNRP_LIGHT", irr::scene::ESNRP_LIGHT)
        ._element("ESNRP_SKY_BOX", irr::scene::ESNRP_SKY_BOX)
        ._element("ESNRP_AUTOMATIC", irr::scene::ESNRP_AUTOMATIC)
        ._element("ESNRP_SOLID", irr::scene::ESNRP_SOLID)
        ._element("ESNRP_TRANSPARENT", irr::scene::ESNRP_TRANSPARENT)
        ._element("ESNRP_TRANSPARENT_EFFECT", irr::scene::ESNRP_TRANSPARENT_EFFECT)
        ._element("ESNRP_SHADOW", irr::scene::ESNRP_SHADOW)
    ;
}


template <typename D>
void buildMetaClass_ISceneManager(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getMesh", (IAnimatedMesh * (D::ClassType::*) (const io::path &))&D::ClassType::getMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("getMesh", (IAnimatedMesh * (D::ClassType::*) (io::IReadFile *))&D::ClassType::getMesh);
    _d.CPGF_MD_TEMPLATE _method("getMeshCache", &D::ClassType::getMeshCache);
    _d.CPGF_MD_TEMPLATE _method("getVideoDriver", &D::ClassType::getVideoDriver);
    _d.CPGF_MD_TEMPLATE _method("getGUIEnvironment", &D::ClassType::getGUIEnvironment);
    _d.CPGF_MD_TEMPLATE _method("getFileSystem", &D::ClassType::getFileSystem);
    _d.CPGF_MD_TEMPLATE _method("addVolumeLightSceneNode", &D::ClassType::addVolumeLightSceneNode)
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(video::SColor(0, 0, 0, 0)))
        ._default(copyVariantFromCopyable(video::SColor(51, 0, 230, 180)))
        ._default(copyVariantFromCopyable(32))
        ._default(copyVariantFromCopyable(32))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addCubeSceneNode", &D::ClassType::addCubeSceneNode)
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(10.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("addSphereSceneNode", &D::ClassType::addSphereSceneNode)
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(16))
        ._default(copyVariantFromCopyable(5.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("addAnimatedMeshSceneNode", &D::ClassType::addAnimatedMeshSceneNode)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addMeshSceneNode", &D::ClassType::addMeshSceneNode)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addWaterSurfaceSceneNode", &D::ClassType::addWaterSurfaceSceneNode)
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(10.0f))
        ._default(copyVariantFromCopyable(300.0f))
        ._default(copyVariantFromCopyable(2.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("addOctreeSceneNode", (IMeshSceneNode * (D::ClassType::*) (IAnimatedMesh *, ISceneNode *, s32, s32, bool))&D::ClassType::addOctreeSceneNode)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(512))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addOctTreeSceneNode", (_IRR_DEPRECATED_ IMeshSceneNode * (D::ClassType::*) (IAnimatedMesh *, ISceneNode *, s32, s32, bool))&D::ClassType::addOctTreeSceneNode)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(512))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addOctreeSceneNode", (IMeshSceneNode * (D::ClassType::*) (IMesh *, ISceneNode *, s32, s32, bool))&D::ClassType::addOctreeSceneNode)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(256))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addOctTreeSceneNode", (_IRR_DEPRECATED_ IMeshSceneNode * (D::ClassType::*) (IMesh *, ISceneNode *, s32, s32, bool))&D::ClassType::addOctTreeSceneNode)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(256))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addCameraSceneNode", &D::ClassType::addCameraSceneNode)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 100)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addCameraSceneNodeMaya", &D::ClassType::addCameraSceneNodeMaya)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(70.f))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(1500.f))
        ._default(copyVariantFromCopyable(200.f))
        ._default(copyVariantFromCopyable(-1500.f))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addCameraSceneNodeFPS", &D::ClassType::addCameraSceneNodeFPS)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0.5f))
        ._default(copyVariantFromCopyable(100.0f))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addLightSceneNode", &D::ClassType::addLightSceneNode)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(100.0f))
        ._default(copyVariantFromCopyable(video::SColorf(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addBillboardSceneNode", &D::ClassType::addBillboardSceneNode)
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::dimension2d< f32 >(10.0f, 10.0f)))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addSkyBoxSceneNode", &D::ClassType::addSkyBoxSceneNode)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addSkyDomeSceneNode", &D::ClassType::addSkyDomeSceneNode)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(1000.f))
        ._default(copyVariantFromCopyable(2.0))
        ._default(copyVariantFromCopyable(0.9))
        ._default(copyVariantFromCopyable(8))
        ._default(copyVariantFromCopyable(16))
    ;
    _d.CPGF_MD_TEMPLATE _method("addParticleSystemSceneNode", &D::ClassType::addParticleSystemSceneNode)
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTerrainSceneNode", (ITerrainSceneNode * (D::ClassType::*) (const io::path &, ISceneNode *, s32, const core::vector3df &, const core::vector3df &, const core::vector3df &, video::SColor, s32, E_TERRAIN_PATCH_SIZE, s32, bool))&D::ClassType::addTerrainSceneNode, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(ETPS_17))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.0f, 0.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.0f, 0.0f)))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTerrainSceneNode", (ITerrainSceneNode * (D::ClassType::*) (io::IReadFile *, ISceneNode *, s32, const core::vector3df &, const core::vector3df &, const core::vector3df &, video::SColor, s32, E_TERRAIN_PATCH_SIZE, s32, bool))&D::ClassType::addTerrainSceneNode)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(ETPS_17))
        ._default(copyVariantFromCopyable(5))
        ._default(copyVariantFromCopyable(video::SColor(255, 255, 255, 255)))
        ._default(copyVariantFromCopyable(core::vector3df(1.0f, 1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.0f, 0.0f)))
        ._default(copyVariantFromCopyable(core::vector3df(0.0f, 0.0f, 0.0f)))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addQuake3SceneNode", &D::ClassType::addQuake3SceneNode)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addEmptySceneNode", &D::ClassType::addEmptySceneNode)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addDummyTransformationSceneNode", &D::ClassType::addDummyTransformationSceneNode)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTextSceneNode", &D::ClassType::addTextSceneNode)
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(video::SColor(100, 255, 255, 255)))
    ;
    _d.CPGF_MD_TEMPLATE _method("addBillboardTextSceneNode", &D::ClassType::addBillboardTextSceneNode)
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
        ._default(copyVariantFromCopyable(-1))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::dimension2d< f32 >(10.0f, 10.0f)))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addHillPlaneMesh", &D::ClassType::addHillPlaneMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(core::dimension2d< f32 >(1.0f, 1.0f)))
        ._default(copyVariantFromCopyable(core::dimension2d< f32 >(0.0f, 0.0f)))
        ._default(copyVariantFromCopyable(0.0f))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addTerrainMesh", &D::ClassType::addTerrainMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(core::dimension2d< u32 >(64, 64)))
        ._default(copyVariantFromCopyable(200.0f))
        ._default(copyVariantFromCopyable(core::dimension2d< f32 >(10.0f, 10.0f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("addArrowMesh", &D::ClassType::addArrowMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0.3f))
        ._default(copyVariantFromCopyable(0.05f))
        ._default(copyVariantFromCopyable(0.6f))
        ._default(copyVariantFromCopyable(1.f))
        ._default(copyVariantFromCopyable(8))
        ._default(copyVariantFromCopyable(4))
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
        ._default(copyVariantFromCopyable(0xFFFFFFFF))
    ;
    _d.CPGF_MD_TEMPLATE _method("addSphereMesh", &D::ClassType::addSphereMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(16))
        ._default(copyVariantFromCopyable(16))
        ._default(copyVariantFromCopyable(5.f))
    ;
    _d.CPGF_MD_TEMPLATE _method("addVolumeLightMesh", &D::ClassType::addVolumeLightMesh, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(video::SColor(0, 0, 0, 0)))
        ._default(copyVariantFromCopyable(video::SColor(51, 0, 230, 180)))
        ._default(copyVariantFromCopyable(32))
        ._default(copyVariantFromCopyable(32))
    ;
    _d.CPGF_MD_TEMPLATE _method("getRootSceneNode", &D::ClassType::getRootSceneNode);
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeFromId", &D::ClassType::getSceneNodeFromId)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeFromName", &D::ClassType::getSceneNodeFromName)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeFromType", &D::ClassType::getSceneNodeFromType)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSceneNodesFromType", &D::ClassType::getSceneNodesFromType)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("getActiveCamera", &D::ClassType::getActiveCamera);
    _d.CPGF_MD_TEMPLATE _method("setActiveCamera", &D::ClassType::setActiveCamera);
    _d.CPGF_MD_TEMPLATE _method("setShadowColor", &D::ClassType::setShadowColor)
        ._default(copyVariantFromCopyable(video::SColor(150, 0, 0, 0)))
    ;
    _d.CPGF_MD_TEMPLATE _method("getShadowColor", &D::ClassType::getShadowColor);
    _d.CPGF_MD_TEMPLATE _method("registerNodeForRendering", &D::ClassType::registerNodeForRendering)
        ._default(copyVariantFromCopyable(ESNRP_AUTOMATIC))
    ;
    _d.CPGF_MD_TEMPLATE _method("drawAll", &D::ClassType::drawAll);
    _d.CPGF_MD_TEMPLATE _method("createRotationAnimator", &D::ClassType::createRotationAnimator);
    _d.CPGF_MD_TEMPLATE _method("createFlyCircleAnimator", &D::ClassType::createFlyCircleAnimator)
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(0.f))
        ._default(copyVariantFromCopyable(core::vector3df(0.f, 1.f, 0.f)))
        ._default(copyVariantFromCopyable(0.001f))
        ._default(copyVariantFromCopyable(100.f))
        ._default(copyVariantFromCopyable(core::vector3df(0.f, 0.f, 0.f)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createFlyStraightAnimator", &D::ClassType::createFlyStraightAnimator)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("createTextureAnimator", &D::ClassType::createTextureAnimator)
        ._default(copyVariantFromCopyable(true))
    ;
    _d.CPGF_MD_TEMPLATE _method("createDeleteAnimator", &D::ClassType::createDeleteAnimator);
    _d.CPGF_MD_TEMPLATE _method("createCollisionResponseAnimator", &D::ClassType::createCollisionResponseAnimator)
        ._default(copyVariantFromCopyable(0.0005f))
        ._default(copyVariantFromCopyable(core::vector3df(0, 0, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(0,-10.0f, 0)))
        ._default(copyVariantFromCopyable(core::vector3df(30, 60, 30)))
    ;
    _d.CPGF_MD_TEMPLATE _method("createFollowSplineAnimator", &D::ClassType::createFollowSplineAnimator)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(0.5f))
        ._default(copyVariantFromCopyable(1.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("createTriangleSelector", (ITriangleSelector * (D::ClassType::*) (IMesh *, ISceneNode *))&D::ClassType::createTriangleSelector);
    _d.CPGF_MD_TEMPLATE _method("createTriangleSelector", (ITriangleSelector * (D::ClassType::*) (IAnimatedMeshSceneNode *))&D::ClassType::createTriangleSelector);
    _d.CPGF_MD_TEMPLATE _method("createTriangleSelectorFromBoundingBox", &D::ClassType::createTriangleSelectorFromBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("createOctreeTriangleSelector", &D::ClassType::createOctreeTriangleSelector)
        ._default(copyVariantFromCopyable(32))
    ;
    _d.CPGF_MD_TEMPLATE _method("createOctTreeTriangleSelector", &D::ClassType::createOctTreeTriangleSelector)
        ._default(copyVariantFromCopyable(32))
    ;
    _d.CPGF_MD_TEMPLATE _method("createMetaTriangleSelector", &D::ClassType::createMetaTriangleSelector);
    _d.CPGF_MD_TEMPLATE _method("createTerrainTriangleSelector", &D::ClassType::createTerrainTriangleSelector)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("addExternalMeshLoader", &D::ClassType::addExternalMeshLoader);
    _d.CPGF_MD_TEMPLATE _method("getMeshLoaderCount", &D::ClassType::getMeshLoaderCount);
    _d.CPGF_MD_TEMPLATE _method("getMeshLoader", &D::ClassType::getMeshLoader);
    _d.CPGF_MD_TEMPLATE _method("addExternalSceneLoader", &D::ClassType::addExternalSceneLoader);
    _d.CPGF_MD_TEMPLATE _method("getSceneLoaderCount", &D::ClassType::getSceneLoaderCount);
    _d.CPGF_MD_TEMPLATE _method("getSceneLoader", &D::ClassType::getSceneLoader);
    _d.CPGF_MD_TEMPLATE _method("getSceneCollisionManager", &D::ClassType::getSceneCollisionManager);
    _d.CPGF_MD_TEMPLATE _method("getMeshManipulator", &D::ClassType::getMeshManipulator);
    _d.CPGF_MD_TEMPLATE _method("addToDeletionQueue", &D::ClassType::addToDeletionQueue);
    _d.CPGF_MD_TEMPLATE _method("postEventFromUser", &D::ClassType::postEventFromUser);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("getParameters", &D::ClassType::getParameters);
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeRenderPass", &D::ClassType::getSceneNodeRenderPass);
    _d.CPGF_MD_TEMPLATE _method("getDefaultSceneNodeFactory", &D::ClassType::getDefaultSceneNodeFactory);
    _d.CPGF_MD_TEMPLATE _method("registerSceneNodeFactory", &D::ClassType::registerSceneNodeFactory);
    _d.CPGF_MD_TEMPLATE _method("getRegisteredSceneNodeFactoryCount", &D::ClassType::getRegisteredSceneNodeFactoryCount);
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeFactory", &D::ClassType::getSceneNodeFactory);
    _d.CPGF_MD_TEMPLATE _method("getDefaultSceneNodeAnimatorFactory", &D::ClassType::getDefaultSceneNodeAnimatorFactory);
    _d.CPGF_MD_TEMPLATE _method("registerSceneNodeAnimatorFactory", &D::ClassType::registerSceneNodeAnimatorFactory);
    _d.CPGF_MD_TEMPLATE _method("getRegisteredSceneNodeAnimatorFactoryCount", &D::ClassType::getRegisteredSceneNodeAnimatorFactoryCount);
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeAnimatorFactory", &D::ClassType::getSceneNodeAnimatorFactory);
    _d.CPGF_MD_TEMPLATE _method("getSceneNodeTypeName", &D::ClassType::getSceneNodeTypeName);
    _d.CPGF_MD_TEMPLATE _method("getAnimatorTypeName", &D::ClassType::getAnimatorTypeName);
    _d.CPGF_MD_TEMPLATE _method("addSceneNode", &D::ClassType::addSceneNode)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("createSceneNodeAnimator", &D::ClassType::createSceneNodeAnimator)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("createNewSceneManager", &D::ClassType::createNewSceneManager)
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("saveScene", (bool (D::ClassType::*) (const io::path &, ISceneUserDataSerializer *, ISceneNode *))&D::ClassType::saveScene, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("saveScene", (bool (D::ClassType::*) (io::IWriteFile *, ISceneUserDataSerializer *, ISceneNode *))&D::ClassType::saveScene)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("saveScene", (bool (D::ClassType::*) (io::IXMLWriter *, const io::path &, ISceneUserDataSerializer *, ISceneNode *))&D::ClassType::saveScene, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("loadScene", (bool (D::ClassType::*) (const io::path &, ISceneUserDataSerializer *, ISceneNode *))&D::ClassType::loadScene, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("loadScene", (bool (D::ClassType::*) (io::IReadFile *, ISceneUserDataSerializer *, ISceneNode *))&D::ClassType::loadScene)
        ._default(copyVariantFromCopyable(0))
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("createMeshWriter", &D::ClassType::createMeshWriter);
    _d.CPGF_MD_TEMPLATE _method("createSkinnedMesh", &D::ClassType::createSkinnedMesh);
    _d.CPGF_MD_TEMPLATE _method("setAmbientLight", &D::ClassType::setAmbientLight);
    _d.CPGF_MD_TEMPLATE _method("getAmbientLight", &D::ClassType::getAmbientLight);
    _d.CPGF_MD_TEMPLATE _method("setLightManager", &D::ClassType::setLightManager);
    _d.CPGF_MD_TEMPLATE _method("getGeometryCreator", &D::ClassType::getGeometryCreator);
    _d.CPGF_MD_TEMPLATE _method("isCulled", &D::ClassType::isCulled);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
