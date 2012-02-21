// Auto generated file, don't modify.

#ifndef __META_B2DISTANCE_H
#define __META_B2DISTANCE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_Global_b2distance(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Distance", _r), (void (*) (b2DistanceOutput *, b2SimplexCache *, const b2DistanceInput *))&b2Distance, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2DistanceInput(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyA", _r), &D::ClassType::proxyA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyB", _r), &D::ClassType::proxyB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("transformA", _r), &D::ClassType::transformA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("transformB", _r), &D::ClassType::transformB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("useRadii", _r), &D::ClassType::useRadii, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2DistanceOutput(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("pointA", _r), &D::ClassType::pointA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("pointB", _r), &D::ClassType::pointB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("distance", _r), &D::ClassType::distance, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("iterations", _r), &D::ClassType::iterations, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2DistanceProxy(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_vertices", _r), &D::ClassType::m_vertices, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_count", _r), &D::ClassType::m_count, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_radius", _r), &D::ClassType::m_radius, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupport", _r), &D::ClassType::GetSupport, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupportVertex", _r), &D::ClassType::GetSupportVertex, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertexCount", _r), &D::ClassType::GetVertexCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertex", _r), &D::ClassType::GetVertex, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2SimplexCache(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("metric", _r), &D::ClassType::metric, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("count", _r), &D::ClassType::count, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("indexA", _r), &D::ClassType::indexA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("indexB", _r), &D::ClassType::indexB, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif