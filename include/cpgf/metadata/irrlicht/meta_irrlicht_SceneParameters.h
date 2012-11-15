// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SCENEPARAMETERS_H
#define __META_IRRLICHT_SCENEPARAMETERS_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_sceneparameters(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("ALLOW_ZWRITE_ON_TRANSPARENT", &ALLOW_ZWRITE_ON_TRANSPARENT);
    _d.CPGF_MD_TEMPLATE _field("CSM_TEXTURE_PATH", &CSM_TEXTURE_PATH);
    _d.CPGF_MD_TEMPLATE _field("LMTS_TEXTURE_PATH", &LMTS_TEXTURE_PATH);
    _d.CPGF_MD_TEMPLATE _field("MY3D_TEXTURE_PATH", &MY3D_TEXTURE_PATH);
    _d.CPGF_MD_TEMPLATE _field("COLLADA_CREATE_SCENE_INSTANCES", &COLLADA_CREATE_SCENE_INSTANCES);
    _d.CPGF_MD_TEMPLATE _field("DMF_TEXTURE_PATH", &DMF_TEXTURE_PATH);
    _d.CPGF_MD_TEMPLATE _field("DMF_IGNORE_MATERIALS_DIRS", &DMF_IGNORE_MATERIALS_DIRS);
    _d.CPGF_MD_TEMPLATE _field("DMF_ALPHA_CHANNEL_REF", &DMF_ALPHA_CHANNEL_REF);
    _d.CPGF_MD_TEMPLATE _field("DMF_FLIP_ALPHA_TEXTURES", &DMF_FLIP_ALPHA_TEXTURES);
    _d.CPGF_MD_TEMPLATE _field("OBJ_TEXTURE_PATH", &OBJ_TEXTURE_PATH);
    _d.CPGF_MD_TEMPLATE _field("OBJ_LOADER_IGNORE_GROUPS", &OBJ_LOADER_IGNORE_GROUPS);
    _d.CPGF_MD_TEMPLATE _field("OBJ_LOADER_IGNORE_MATERIAL_FILES", &OBJ_LOADER_IGNORE_MATERIAL_FILES);
    _d.CPGF_MD_TEMPLATE _field("B3D_LOADER_IGNORE_MIPMAP_FLAG", &B3D_LOADER_IGNORE_MIPMAP_FLAG);
    _d.CPGF_MD_TEMPLATE _field("B3D_TEXTURE_PATH", &B3D_TEXTURE_PATH);
    _d.CPGF_MD_TEMPLATE _field("IRR_SCENE_MANAGER_IS_EDITOR", &IRR_SCENE_MANAGER_IS_EDITOR);
    _d.CPGF_MD_TEMPLATE _field("DEBUG_NORMAL_LENGTH", &DEBUG_NORMAL_LENGTH);
    _d.CPGF_MD_TEMPLATE _field("DEBUG_NORMAL_COLOR", &DEBUG_NORMAL_COLOR);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
