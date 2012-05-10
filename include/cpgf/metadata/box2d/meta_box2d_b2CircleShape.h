// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CIRCLESHAPE_H
#define __META_BOX2D_B2CIRCLESHAPE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2CircleShape(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_p", _r), &D::ClassType::m_p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clone", _r), &D::ClassType::Clone);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TestPoint", _r), &D::ClassType::TestPoint);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeAABB", _r), &D::ClassType::ComputeAABB);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeMass", _r), &D::ClassType::ComputeMass);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupport", _r), &D::ClassType::GetSupport);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupportVertex", _r), &D::ClassType::GetSupportVertex);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertexCount", _r), &D::ClassType::GetVertexCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertex", _r), &D::ClassType::GetVertex);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
