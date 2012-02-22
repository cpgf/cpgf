// Auto generated file, don't modify.

#ifndef __META_B2POLYGONSHAPE_H
#define __META_B2POLYGONSHAPE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_B2PolygonShape(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_centroid", _r), &D::ClassType::m_centroid, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_vertices", _r), &D::ClassType::m_vertices, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_normals", _r), &D::ClassType::m_normals, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_vertexCount", _r), &D::ClassType::m_vertexCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clone", _r), &D::ClassType::Clone, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAsBox", _r), (void (D::ClassType::*) (float32, float32))&D::ClassType::SetAsBox, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAsBox", _r), (void (D::ClassType::*) (float32, float32, const b2Vec2 &, float32))&D::ClassType::SetAsBox, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetAsEdge", _r), &D::ClassType::SetAsEdge, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TestPoint", _r), &D::ClassType::TestPoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeAABB", _r), &D::ClassType::ComputeAABB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeMass", _r), &D::ClassType::ComputeMass, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupport", _r), &D::ClassType::GetSupport, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupportVertex", _r), &D::ClassType::GetSupportVertex, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertexCount", _r), &D::ClassType::GetVertexCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertex", _r), &D::ClassType::GetVertex, _p);
}


} // namespace meta_box2d 




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif