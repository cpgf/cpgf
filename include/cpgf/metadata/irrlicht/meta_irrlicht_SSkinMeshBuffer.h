// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_SSKINMESHBUFFER_H
#define __META_IRRLICHT_SSKINMESHBUFFER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


inline E_HARDWARE_MAPPING bItFiEldWrapper_SSkinMeshBuffer_MappingHint_Vertex_getter(SSkinMeshBuffer * self) {
    return self->MappingHint_Vertex;
}

inline void bItFiEldWrapper_SSkinMeshBuffer_MappingHint_Vertex_setter(SSkinMeshBuffer * self, E_HARDWARE_MAPPING value) {
    self->MappingHint_Vertex = value;
}
inline E_HARDWARE_MAPPING bItFiEldWrapper_SSkinMeshBuffer_MappingHint_Index_getter(SSkinMeshBuffer * self) {
    return self->MappingHint_Index;
}

inline void bItFiEldWrapper_SSkinMeshBuffer_MappingHint_Index_setter(SSkinMeshBuffer * self, E_HARDWARE_MAPPING value) {
    self->MappingHint_Index = value;
}
inline bool bItFiEldWrapper_SSkinMeshBuffer_BoundingBoxNeedsRecalculated_getter(SSkinMeshBuffer * self) {
    return self->BoundingBoxNeedsRecalculated;
}

inline void bItFiEldWrapper_SSkinMeshBuffer_BoundingBoxNeedsRecalculated_setter(SSkinMeshBuffer * self, bool value) {
    self->BoundingBoxNeedsRecalculated = value;
}


template <typename D>
void buildMetaClass_SSkinMeshBuffer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::E_VERTEX_TYPE)>()
        ._default(copyVariantFromCopyable(video::EVT_STANDARD))
    ;
    _d.CPGF_MD_TEMPLATE _field("Vertices_Tangents", &D::ClassType::Vertices_Tangents);
    _d.CPGF_MD_TEMPLATE _field("Vertices_2TCoords", &D::ClassType::Vertices_2TCoords);
    _d.CPGF_MD_TEMPLATE _field("Vertices_Standard", &D::ClassType::Vertices_Standard);
    _d.CPGF_MD_TEMPLATE _field("Indices", &D::ClassType::Indices);
    _d.CPGF_MD_TEMPLATE _field("ChangedID_Vertex", &D::ClassType::ChangedID_Vertex);
    _d.CPGF_MD_TEMPLATE _field("ChangedID_Index", &D::ClassType::ChangedID_Index);
    _d.CPGF_MD_TEMPLATE _field("Transformation", &D::ClassType::Transformation);
    _d.CPGF_MD_TEMPLATE _field("Material", &D::ClassType::Material);
    _d.CPGF_MD_TEMPLATE _field("VertexType", &D::ClassType::VertexType);
    _d.CPGF_MD_TEMPLATE _field("BoundingBox", &D::ClassType::BoundingBox);
    _d.CPGF_MD_TEMPLATE _property("MappingHint_Vertex", &bItFiEldWrapper_SSkinMeshBuffer_MappingHint_Vertex_getter, &bItFiEldWrapper_SSkinMeshBuffer_MappingHint_Vertex_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("MappingHint_Index", &bItFiEldWrapper_SSkinMeshBuffer_MappingHint_Index_getter, &bItFiEldWrapper_SSkinMeshBuffer_MappingHint_Index_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _property("BoundingBoxNeedsRecalculated", &bItFiEldWrapper_SSkinMeshBuffer_BoundingBoxNeedsRecalculated_getter, &bItFiEldWrapper_SSkinMeshBuffer_BoundingBoxNeedsRecalculated_setter, cpgf::MakePolicy<cpgf::GMetaRuleGetterExplicitThis, cpgf::GMetaRuleSetterExplicitThis>());
    _d.CPGF_MD_TEMPLATE _method("getMaterial", (const video::SMaterial & (D::ClassType::*) () const)&D::ClassType::getMaterial);
    _d.CPGF_MD_TEMPLATE _method("getMaterial", (video::SMaterial & (D::ClassType::*) ())&D::ClassType::getMaterial);
    _d.CPGF_MD_TEMPLATE _method("getVertex", &D::ClassType::getVertex);
    _d.CPGF_MD_TEMPLATE _method("getVertices", (const void * (D::ClassType::*) () const)&D::ClassType::getVertices);
    _d.CPGF_MD_TEMPLATE _method("getVertices", (void * (D::ClassType::*) ())&D::ClassType::getVertices);
    _d.CPGF_MD_TEMPLATE _method("getVertexCount", &D::ClassType::getVertexCount);
    _d.CPGF_MD_TEMPLATE _method("getIndexType", &D::ClassType::getIndexType);
    _d.CPGF_MD_TEMPLATE _method("getIndices", (const u16 * (D::ClassType::*) () const)&D::ClassType::getIndices);
    _d.CPGF_MD_TEMPLATE _method("getIndices", (u16 * (D::ClassType::*) ())&D::ClassType::getIndices);
    _d.CPGF_MD_TEMPLATE _method("getIndexCount", &D::ClassType::getIndexCount);
    _d.CPGF_MD_TEMPLATE _method("getBoundingBox", &D::ClassType::getBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("setBoundingBox", &D::ClassType::setBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("recalculateBoundingBox", &D::ClassType::recalculateBoundingBox);
    _d.CPGF_MD_TEMPLATE _method("getVertexType", &D::ClassType::getVertexType);
    _d.CPGF_MD_TEMPLATE _method("convertTo2TCoords", &D::ClassType::convertTo2TCoords);
    _d.CPGF_MD_TEMPLATE _method("convertToTangents", &D::ClassType::convertToTangents);
    _d.CPGF_MD_TEMPLATE _method("getPosition", (const core::vector3df & (D::ClassType::*) (u32) const)&D::ClassType::getPosition);
    _d.CPGF_MD_TEMPLATE _method("getPosition", (core::vector3df & (D::ClassType::*) (u32))&D::ClassType::getPosition);
    _d.CPGF_MD_TEMPLATE _method("getNormal", (const core::vector3df & (D::ClassType::*) (u32) const)&D::ClassType::getNormal);
    _d.CPGF_MD_TEMPLATE _method("getNormal", (core::vector3df & (D::ClassType::*) (u32))&D::ClassType::getNormal);
    _d.CPGF_MD_TEMPLATE _method("getTCoords", (const core::vector2df & (D::ClassType::*) (u32) const)&D::ClassType::getTCoords);
    _d.CPGF_MD_TEMPLATE _method("getTCoords", (core::vector2df & (D::ClassType::*) (u32))&D::ClassType::getTCoords);
    _d.CPGF_MD_TEMPLATE _method("append", (void (D::ClassType::*) (const void *const, u32, const u16 *const, u32))&D::ClassType::append);
    _d.CPGF_MD_TEMPLATE _method("append", (void (D::ClassType::*) (const IMeshBuffer *const))&D::ClassType::append);
    _d.CPGF_MD_TEMPLATE _method("getHardwareMappingHint_Vertex", &D::ClassType::getHardwareMappingHint_Vertex);
    _d.CPGF_MD_TEMPLATE _method("getHardwareMappingHint_Index", &D::ClassType::getHardwareMappingHint_Index);
    _d.CPGF_MD_TEMPLATE _method("setHardwareMappingHint", &D::ClassType::setHardwareMappingHint)
        ._default(copyVariantFromCopyable(EBT_VERTEX_AND_INDEX))
    ;
    _d.CPGF_MD_TEMPLATE _method("setDirty", &D::ClassType::setDirty)
        ._default(copyVariantFromCopyable(EBT_VERTEX_AND_INDEX))
    ;
    _d.CPGF_MD_TEMPLATE _method("getChangedID_Vertex", &D::ClassType::getChangedID_Vertex);
    _d.CPGF_MD_TEMPLATE _method("getChangedID_Index", &D::ClassType::getChangedID_Index);
    _d.CPGF_MD_TEMPLATE _method("boundingBoxNeedsRecalculated", &D::ClassType::boundingBoxNeedsRecalculated);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
