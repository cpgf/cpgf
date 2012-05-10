// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2DISTANCE_H
#define __META_BOX2D_B2DISTANCE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2distance(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2Distance", _r), (void (*) (b2DistanceOutput *, b2SimplexCache *, const b2DistanceInput *))&b2Distance);
}


template <typename D>
void buildMetaClass_B2DistanceInput(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyA", _r), &D::ClassType::proxyA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("proxyB", _r), &D::ClassType::proxyB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("transformA", _r), &D::ClassType::transformA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("transformB", _r), &D::ClassType::transformB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("useRadii", _r), &D::ClassType::useRadii);
}


template <typename D>
void buildMetaClass_B2DistanceOutput(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("pointA", _r), &D::ClassType::pointA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("pointB", _r), &D::ClassType::pointB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("distance", _r), &D::ClassType::distance);
    _d.CPGF_MD_TEMPLATE _field(replaceName("iterations", _r), &D::ClassType::iterations);
}


template <typename D>
void buildMetaClass_B2DistanceProxy(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_vertices", _r), &D::ClassType::m_vertices);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_count", _r), &D::ClassType::m_count);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_radius", _r), &D::ClassType::m_radius);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupport", _r), &D::ClassType::GetSupport);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetSupportVertex", _r), &D::ClassType::GetSupportVertex);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertexCount", _r), &D::ClassType::GetVertexCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetVertex", _r), &D::ClassType::GetVertex);
}


template <typename D>
void buildMetaClass_B2SimplexCache(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("metric", _r), &D::ClassType::metric);
    _d.CPGF_MD_TEMPLATE _field(replaceName("count", _r), &D::ClassType::count);
    _d.CPGF_MD_TEMPLATE _field(replaceName("indexA", _r), &D::ClassType::indexA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("indexB", _r), &D::ClassType::indexB);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
