// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2COLLISION_H
#define __META_BOX2D_B2COLLISION_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2collision(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_nullFeature", _r), &b2_nullFeature);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2GetPointStates", _r), (void (*) (b2PointState[b2_maxManifoldPoints], b2PointState[b2_maxManifoldPoints], const b2Manifold *, const b2Manifold *))&b2GetPointStates);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2CollideCircles", _r), (void (*) (b2Manifold *, const b2CircleShape *, const b2Transform &, const b2CircleShape *, const b2Transform &))&b2CollideCircles);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2CollidePolygonAndCircle", _r), (void (*) (b2Manifold *, const b2PolygonShape *, const b2Transform &, const b2CircleShape *, const b2Transform &))&b2CollidePolygonAndCircle);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2CollidePolygons", _r), (void (*) (b2Manifold *, const b2PolygonShape *, const b2Transform &, const b2PolygonShape *, const b2Transform &))&b2CollidePolygons);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2ClipSegmentToLine", _r), (int32 (*) (b2ClipVertex[2], const b2ClipVertex[2], const b2Vec2 &, float32))&b2ClipSegmentToLine);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2TestOverlap", _r), (bool (*) (const b2Shape *, const b2Shape *, const b2Transform &, const b2Transform &))&b2TestOverlap);
    _d.CPGF_MD_TEMPLATE _method(replaceName("b2TestOverlap", _r), (bool (*) (const b2AABB &, const b2AABB &))&b2TestOverlap);
    _d.CPGF_MD_TEMPLATE _enum<b2PointState>(replaceName("b2PointState", _r))
        ._element(replaceName("b2_nullState", _r), b2_nullState)
        ._element(replaceName("b2_addState", _r), b2_addState)
        ._element(replaceName("b2_persistState", _r), b2_persistState)
        ._element(replaceName("b2_removeState", _r), b2_removeState)
    ;
}


template <typename D>
void buildMetaClass_Features(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("referenceEdge", _r), &D::ClassType::referenceEdge);
    _d.CPGF_MD_TEMPLATE _field(replaceName("incidentEdge", _r), &D::ClassType::incidentEdge);
    _d.CPGF_MD_TEMPLATE _field(replaceName("incidentVertex", _r), &D::ClassType::incidentVertex);
    _d.CPGF_MD_TEMPLATE _field(replaceName("flip", _r), &D::ClassType::flip);
}


template <typename D>
void buildMetaClass_B2AABB(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("lowerBound", _r), &D::ClassType::lowerBound);
    _d.CPGF_MD_TEMPLATE _field(replaceName("upperBound", _r), &D::ClassType::upperBound);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsValid", _r), &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetCenter", _r), &D::ClassType::GetCenter);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetExtents", _r), &D::ClassType::GetExtents);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Combine", _r), &D::ClassType::Combine);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Contains", _r), &D::ClassType::Contains);
    _d.CPGF_MD_TEMPLATE _method(replaceName("RayCast", _r), &D::ClassType::RayCast);
}


template <typename D>
void buildMetaClass_B2ClipVertex(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("v", _r), &D::ClassType::v);
    _d.CPGF_MD_TEMPLATE _field(replaceName("id", _r), &D::ClassType::id);
}


template <typename D>
void buildMetaClass_B2Manifold(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("points", _r), &D::ClassType::points);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localNormal", _r), &D::ClassType::localNormal);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localPoint", _r), &D::ClassType::localPoint);
    _d.CPGF_MD_TEMPLATE _field(replaceName("type", _r), &D::ClassType::type);
    _d.CPGF_MD_TEMPLATE _field(replaceName("pointCount", _r), &D::ClassType::pointCount);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Type>(replaceName("Type", _r))
        ._element(replaceName("e_circles", _r), D::ClassType::e_circles)
        ._element(replaceName("e_faceA", _r), D::ClassType::e_faceA)
        ._element(replaceName("e_faceB", _r), D::ClassType::e_faceB)
    ;
}


template <typename D>
void buildMetaClass_B2ManifoldPoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("localPoint", _r), &D::ClassType::localPoint);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalImpulse", _r), &D::ClassType::normalImpulse);
    _d.CPGF_MD_TEMPLATE _field(replaceName("tangentImpulse", _r), &D::ClassType::tangentImpulse);
    _d.CPGF_MD_TEMPLATE _field(replaceName("id", _r), &D::ClassType::id);
}


template <typename D>
void buildMetaClass_B2RayCastInput(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("p1", _r), &D::ClassType::p1);
    _d.CPGF_MD_TEMPLATE _field(replaceName("p2", _r), &D::ClassType::p2);
    _d.CPGF_MD_TEMPLATE _field(replaceName("maxFraction", _r), &D::ClassType::maxFraction);
}


template <typename D>
void buildMetaClass_B2RayCastOutput(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("normal", _r), &D::ClassType::normal);
    _d.CPGF_MD_TEMPLATE _field(replaceName("fraction", _r), &D::ClassType::fraction);
}


template <typename D>
void buildMetaClass_B2WorldManifold(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("normal", _r), &D::ClassType::normal);
    _d.CPGF_MD_TEMPLATE _field(replaceName("points", _r), &D::ClassType::points);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Initialize", _r), &D::ClassType::Initialize);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
