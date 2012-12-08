// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ICOLLADAMESHWRITER_H
#define __META_IRRLICHT_ICOLLADAMESHWRITER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_icolladameshwriter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_COLLADA_TECHNIQUE_FX>("E_COLLADA_TECHNIQUE_FX")
        ._element("ECTF_BLINN", irr::scene::ECTF_BLINN)
        ._element("ECTF_PHONG", irr::scene::ECTF_PHONG)
        ._element("ECTF_LAMBERT", irr::scene::ECTF_LAMBERT)
        ._element("ECTF_CONSTANT", irr::scene::ECTF_CONSTANT)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_COLLADA_TRANSPARENT_FX>("E_COLLADA_TRANSPARENT_FX")
        ._element("ECOF_A_ONE", irr::scene::ECOF_A_ONE)
        ._element("ECOF_RGB_ZERO", irr::scene::ECOF_RGB_ZERO)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_COLLADA_COLOR_SAMPLER>("E_COLLADA_COLOR_SAMPLER")
        ._element("ECCS_DIFFUSE", irr::scene::ECCS_DIFFUSE)
        ._element("ECCS_AMBIENT", irr::scene::ECCS_AMBIENT)
        ._element("ECCS_EMISSIVE", irr::scene::ECCS_EMISSIVE)
        ._element("ECCS_SPECULAR", irr::scene::ECCS_SPECULAR)
        ._element("ECCS_TRANSPARENT", irr::scene::ECCS_TRANSPARENT)
        ._element("ECCS_REFLECTIVE", irr::scene::ECCS_REFLECTIVE)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_COLLADA_IRR_COLOR>("E_COLLADA_IRR_COLOR")
        ._element("ECIC_NONE", irr::scene::ECIC_NONE)
        ._element("ECIC_CUSTOM", irr::scene::ECIC_CUSTOM)
        ._element("ECIC_DIFFUSE", irr::scene::ECIC_DIFFUSE)
        ._element("ECIC_AMBIENT", irr::scene::ECIC_AMBIENT)
        ._element("ECIC_EMISSIVE", irr::scene::ECIC_EMISSIVE)
        ._element("ECIC_SPECULAR", irr::scene::ECIC_SPECULAR)
    ;
    _d.CPGF_MD_TEMPLATE _enum<E_COLLADA_GEOMETRY_WRITING>("E_COLLADA_GEOMETRY_WRITING")
        ._element("ECGI_PER_MESH", irr::scene::ECGI_PER_MESH)
        ._element("ECGI_PER_MESH_AND_MATERIAL", irr::scene::ECGI_PER_MESH_AND_MATERIAL)
    ;
}


template <typename D>
void buildMetaClass_IColladaMeshWriter(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("writeScene", &D::ClassType::writeScene);
    _d.CPGF_MD_TEMPLATE _method("setWriteTextures", &D::ClassType::setWriteTextures);
    _d.CPGF_MD_TEMPLATE _method("getWriteTextures", &D::ClassType::getWriteTextures);
    _d.CPGF_MD_TEMPLATE _method("setWriteDefaultScene", &D::ClassType::setWriteDefaultScene);
    _d.CPGF_MD_TEMPLATE _method("getWriteDefaultScene", &D::ClassType::getWriteDefaultScene);
    _d.CPGF_MD_TEMPLATE _method("setAmbientLight", &D::ClassType::setAmbientLight);
    _d.CPGF_MD_TEMPLATE _method("getAmbientLight", &D::ClassType::getAmbientLight);
    _d.CPGF_MD_TEMPLATE _method("setGeometryWriting", &D::ClassType::setGeometryWriting);
    _d.CPGF_MD_TEMPLATE _method("getGeometryWriting", &D::ClassType::getGeometryWriting);
    _d.CPGF_MD_TEMPLATE _method("setExportSMaterialsOnlyOnce", &D::ClassType::setExportSMaterialsOnlyOnce);
    _d.CPGF_MD_TEMPLATE _method("getExportSMaterialsOnlyOnce", &D::ClassType::getExportSMaterialsOnlyOnce);
    _d.CPGF_MD_TEMPLATE _method("setProperties", &D::ClassType::setProperties);
    _d.CPGF_MD_TEMPLATE _method("getProperties", &D::ClassType::getProperties);
    _d.CPGF_MD_TEMPLATE _method("getDefaultProperties", &D::ClassType::getDefaultProperties);
    _d.CPGF_MD_TEMPLATE _method("setNameGenerator", &D::ClassType::setNameGenerator);
    _d.CPGF_MD_TEMPLATE _method("getNameGenerator", &D::ClassType::getNameGenerator);
    _d.CPGF_MD_TEMPLATE _method("getDefaultNameGenerator", &D::ClassType::getDefaultNameGenerator);
    _d.CPGF_MD_TEMPLATE _method("toNCName", &D::ClassType::toNCName, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0>, cpgf::GMetaRuleCopyConstReference<1> >())
        ._default(copyVariantFromCopyable(irr::core::stringw(L"_NC_")))
    ;
}


template <typename D>
void buildMetaClass_IColladaMeshWriterNames(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("nameForMesh", &D::ClassType::nameForMesh);
    _d.CPGF_MD_TEMPLATE _method("nameForNode", &D::ClassType::nameForNode);
    _d.CPGF_MD_TEMPLATE _method("nameForMaterial", &D::ClassType::nameForMaterial);
}


template <typename D>
void buildMetaClass_IColladaMeshWriterProperties(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getTechniqueFx", &D::ClassType::getTechniqueFx);
    _d.CPGF_MD_TEMPLATE _method("getTextureIdx", &D::ClassType::getTextureIdx);
    _d.CPGF_MD_TEMPLATE _method("getColorMapping", &D::ClassType::getColorMapping);
    _d.CPGF_MD_TEMPLATE _method("getCustomColor", &D::ClassType::getCustomColor);
    _d.CPGF_MD_TEMPLATE _method("getTransparentFx", &D::ClassType::getTransparentFx);
    _d.CPGF_MD_TEMPLATE _method("getTransparency", &D::ClassType::getTransparency);
    _d.CPGF_MD_TEMPLATE _method("getReflectivity", &D::ClassType::getReflectivity);
    _d.CPGF_MD_TEMPLATE _method("getIndexOfRefraction", &D::ClassType::getIndexOfRefraction);
    _d.CPGF_MD_TEMPLATE _method("isExportable", &D::ClassType::isExportable);
    _d.CPGF_MD_TEMPLATE _method("getMesh", &D::ClassType::getMesh);
    _d.CPGF_MD_TEMPLATE _method("useNodeMaterial", &D::ClassType::useNodeMaterial);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
