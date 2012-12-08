// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ESCENENODETYPES_H
#define __META_IRRLICHT_ESCENENODETYPES_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_escenenodetypes(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<ESCENE_NODE_TYPE>("ESCENE_NODE_TYPE")
        ._element("ESNT_SCENE_MANAGER", irr::scene::ESNT_SCENE_MANAGER)
        ._element("ESNT_CUBE", irr::scene::ESNT_CUBE)
        ._element("ESNT_SPHERE", irr::scene::ESNT_SPHERE)
        ._element("ESNT_TEXT", irr::scene::ESNT_TEXT)
        ._element("ESNT_WATER_SURFACE", irr::scene::ESNT_WATER_SURFACE)
        ._element("ESNT_TERRAIN", irr::scene::ESNT_TERRAIN)
        ._element("ESNT_SKY_BOX", irr::scene::ESNT_SKY_BOX)
        ._element("ESNT_SKY_DOME", irr::scene::ESNT_SKY_DOME)
        ._element("ESNT_SHADOW_VOLUME", irr::scene::ESNT_SHADOW_VOLUME)
        ._element("ESNT_OCTREE", irr::scene::ESNT_OCTREE)
        ._element("ESNT_MESH", irr::scene::ESNT_MESH)
        ._element("ESNT_LIGHT", irr::scene::ESNT_LIGHT)
        ._element("ESNT_EMPTY", irr::scene::ESNT_EMPTY)
        ._element("ESNT_DUMMY_TRANSFORMATION", irr::scene::ESNT_DUMMY_TRANSFORMATION)
        ._element("ESNT_CAMERA", irr::scene::ESNT_CAMERA)
        ._element("ESNT_BILLBOARD", irr::scene::ESNT_BILLBOARD)
        ._element("ESNT_ANIMATED_MESH", irr::scene::ESNT_ANIMATED_MESH)
        ._element("ESNT_PARTICLE_SYSTEM", irr::scene::ESNT_PARTICLE_SYSTEM)
        ._element("ESNT_Q3SHADER_SCENE_NODE", irr::scene::ESNT_Q3SHADER_SCENE_NODE)
        ._element("ESNT_MD3_SCENE_NODE", irr::scene::ESNT_MD3_SCENE_NODE)
        ._element("ESNT_VOLUME_LIGHT", irr::scene::ESNT_VOLUME_LIGHT)
        ._element("ESNT_CAMERA_MAYA", irr::scene::ESNT_CAMERA_MAYA)
        ._element("ESNT_CAMERA_FPS", irr::scene::ESNT_CAMERA_FPS)
        ._element("ESNT_UNKNOWN", irr::scene::ESNT_UNKNOWN)
        ._element("ESNT_ANY", irr::scene::ESNT_ANY)
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
