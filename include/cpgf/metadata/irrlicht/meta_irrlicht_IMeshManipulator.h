// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IMESHMANIPULATOR_H
#define __META_IRRLICHT_IMESHMANIPULATOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_IMeshManipulator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("flipSurfaces", &D::ClassType::flipSurfaces);
    _d.CPGF_MD_TEMPLATE _method("setVertexColorAlpha", (void (D::ClassType::*) (IMesh *, s32) const)&D::ClassType::setVertexColorAlpha);
    _d.CPGF_MD_TEMPLATE _method("setVertexColorAlpha", (void (D::ClassType::*) (IMeshBuffer *, s32) const)&D::ClassType::setVertexColorAlpha);
    _d.CPGF_MD_TEMPLATE _method("setVertexColors", (void (D::ClassType::*) (IMesh *, video::SColor) const)&D::ClassType::setVertexColors);
    _d.CPGF_MD_TEMPLATE _method("setVertexColors", (void (D::ClassType::*) (IMeshBuffer *, video::SColor) const)&D::ClassType::setVertexColors);
    _d.CPGF_MD_TEMPLATE _method("recalculateNormals", (void (D::ClassType::*) (IMesh *, bool, bool) const)&D::ClassType::recalculateNormals)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("recalculateNormals", (void (D::ClassType::*) (IMeshBuffer *, bool, bool) const)&D::ClassType::recalculateNormals)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("recalculateTangents", (void (D::ClassType::*) (IMesh *, bool, bool, bool) const)&D::ClassType::recalculateTangents)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("recalculateTangents", (void (D::ClassType::*) (IMeshBuffer *, bool, bool, bool) const)&D::ClassType::recalculateTangents)
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("scale", (void (D::ClassType::*) (IMesh *, const core::vector3df &) const)&D::ClassType::scale);
    _d.CPGF_MD_TEMPLATE _method("scale", (void (D::ClassType::*) (IMeshBuffer *, const core::vector3df &) const)&D::ClassType::scale);
    _d.CPGF_MD_TEMPLATE _method("scaleMesh", &D::ClassType::scaleMesh);
    _d.CPGF_MD_TEMPLATE _method("scaleTCoords", (void (D::ClassType::*) (scene::IMesh *, const core::vector2df &, u32) const)&D::ClassType::scaleTCoords)
        ._default(copyVariantFromCopyable(1))
    ;
    _d.CPGF_MD_TEMPLATE _method("scaleTCoords", (void (D::ClassType::*) (scene::IMeshBuffer *, const core::vector2df &, u32) const)&D::ClassType::scaleTCoords)
        ._default(copyVariantFromCopyable(1))
    ;
    _d.CPGF_MD_TEMPLATE _method("transform", (void (D::ClassType::*) (IMesh *, const core::matrix4 &) const)&D::ClassType::transform);
    _d.CPGF_MD_TEMPLATE _method("transform", (void (D::ClassType::*) (IMeshBuffer *, const core::matrix4 &) const)&D::ClassType::transform);
    _d.CPGF_MD_TEMPLATE _method("transformMesh", &D::ClassType::transformMesh);
    _d.CPGF_MD_TEMPLATE _method("makePlanarTextureMapping", (void (D::ClassType::*) (IMesh *, f32) const)&D::ClassType::makePlanarTextureMapping)
        ._default(copyVariantFromCopyable(0.001f))
    ;
    _d.CPGF_MD_TEMPLATE _method("makePlanarTextureMapping", (void (D::ClassType::*) (scene::IMeshBuffer *, f32) const)&D::ClassType::makePlanarTextureMapping)
        ._default(copyVariantFromCopyable(0.001f))
    ;
    _d.CPGF_MD_TEMPLATE _method("makePlanarTextureMapping", (void (D::ClassType::*) (scene::IMesh *, f32, f32, u8, const core::vector3df &) const)&D::ClassType::makePlanarTextureMapping);
    _d.CPGF_MD_TEMPLATE _method("makePlanarTextureMapping", (void (D::ClassType::*) (scene::IMeshBuffer *, f32, f32, u8, const core::vector3df &) const)&D::ClassType::makePlanarTextureMapping);
    _d.CPGF_MD_TEMPLATE _method("createMeshCopy", &D::ClassType::createMeshCopy);
    _d.CPGF_MD_TEMPLATE _method("createMeshWithTangents", &D::ClassType::createMeshWithTangents)
        ._default(copyVariantFromCopyable(true))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
        ._default(copyVariantFromCopyable(false))
    ;
    _d.CPGF_MD_TEMPLATE _method("createMeshWith2TCoords", &D::ClassType::createMeshWith2TCoords);
    _d.CPGF_MD_TEMPLATE _method("createMeshWith1TCoords", &D::ClassType::createMeshWith1TCoords);
    _d.CPGF_MD_TEMPLATE _method("createMeshUniquePrimitives", &D::ClassType::createMeshUniquePrimitives);
    _d.CPGF_MD_TEMPLATE _method("createMeshWelded", &D::ClassType::createMeshWelded)
        ._default(copyVariantFromCopyable(core::ROUNDING_ERROR_f32))
    ;
    _d.CPGF_MD_TEMPLATE _method("getPolyCount", (s32 (D::ClassType::*) (IMesh *) const)&D::ClassType::getPolyCount);
    _d.CPGF_MD_TEMPLATE _method("getPolyCount", (s32 (D::ClassType::*) (IAnimatedMesh *) const)&D::ClassType::getPolyCount);
    _d.CPGF_MD_TEMPLATE _method("createAnimatedMesh", &D::ClassType::createAnimatedMesh)
        ._default(copyVariantFromCopyable(scene::EAMT_UNKNOWN))
    ;
    _d.CPGF_MD_TEMPLATE _method("createForsythOptimizedMesh", &D::ClassType::createForsythOptimizedMesh);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
