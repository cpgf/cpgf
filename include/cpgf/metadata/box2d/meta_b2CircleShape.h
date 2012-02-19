//auto generated file, don't modify.
#ifndef __META_B2CIRCLESHAPE_H
#define __META_B2CIRCLESHAPE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_B2CircleShape(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_p", _r), &D::ClassType::m_p, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clone", _r), &D::ClassType::Clone, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("TestPoint", _r), &D::ClassType::TestPoint, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeAABB", _r), &D::ClassType::ComputeAABB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("ComputeMass", _r), &D::ClassType::ComputeMass, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupport", _r), &D::ClassType::GetSupport, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupportVertex", _r), &D::ClassType::GetSupportVertex, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertexCount", _r), &D::ClassType::GetVertexCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertex", _r), &D::ClassType::GetVertex, _p);
}


template <typename D, typename Policy>
void buildMetaFile_B2CircleShape(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    buildMetaClass_B2CircleShape(config, _d, _r, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif