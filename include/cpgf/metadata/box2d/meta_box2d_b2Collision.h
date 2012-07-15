// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2COLLISION_H
#define __META_BOX2D_B2COLLISION_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2collision(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("b2_nullFeature", &b2_nullFeature);
    _d.CPGF_MD_TEMPLATE _method("b2GetPointStates", (void (*) (b2PointState[b2_maxManifoldPoints], b2PointState[b2_maxManifoldPoints], const b2Manifold *, const b2Manifold *))&b2GetPointStates);
    _d.CPGF_MD_TEMPLATE _method("b2CollideCircles", (void (*) (b2Manifold *, const b2CircleShape *, const b2Transform &, const b2CircleShape *, const b2Transform &))&b2CollideCircles);
    _d.CPGF_MD_TEMPLATE _method("b2CollidePolygonAndCircle", (void (*) (b2Manifold *, const b2PolygonShape *, const b2Transform &, const b2CircleShape *, const b2Transform &))&b2CollidePolygonAndCircle);
    _d.CPGF_MD_TEMPLATE _method("b2CollidePolygons", (void (*) (b2Manifold *, const b2PolygonShape *, const b2Transform &, const b2PolygonShape *, const b2Transform &))&b2CollidePolygons);
    _d.CPGF_MD_TEMPLATE _method("b2ClipSegmentToLine", (int32 (*) (b2ClipVertex[2], const b2ClipVertex[2], const b2Vec2 &, float32))&b2ClipSegmentToLine);
    _d.CPGF_MD_TEMPLATE _method("b2TestOverlap", (bool (*) (const b2Shape *, const b2Shape *, const b2Transform &, const b2Transform &))&b2TestOverlap);
    _d.CPGF_MD_TEMPLATE _method("b2TestOverlap", (bool (*) (const b2AABB &, const b2AABB &))&b2TestOverlap);
    _d.CPGF_MD_TEMPLATE _enum<b2PointState>("b2PointState")
        ._element("b2_nullState", b2_nullState)
        ._element("b2_addState", b2_addState)
        ._element("b2_persistState", b2_persistState)
        ._element("b2_removeState", b2_removeState)
    ;
}


template <typename D>
void buildMetaClass_Features(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("referenceEdge", &D::ClassType::referenceEdge);
    _d.CPGF_MD_TEMPLATE _field("incidentEdge", &D::ClassType::incidentEdge);
    _d.CPGF_MD_TEMPLATE _field("incidentVertex", &D::ClassType::incidentVertex);
    _d.CPGF_MD_TEMPLATE _field("flip", &D::ClassType::flip);
}


template <typename D>
void buildMetaClass_B2AABB(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("lowerBound", &D::ClassType::lowerBound);
    _d.CPGF_MD_TEMPLATE _field("upperBound", &D::ClassType::upperBound);
    _d.CPGF_MD_TEMPLATE _method("IsValid", &D::ClassType::IsValid);
    _d.CPGF_MD_TEMPLATE _method("GetCenter", &D::ClassType::GetCenter);
    _d.CPGF_MD_TEMPLATE _method("GetExtents", &D::ClassType::GetExtents);
    _d.CPGF_MD_TEMPLATE _method("Combine", &D::ClassType::Combine);
    _d.CPGF_MD_TEMPLATE _method("Contains", &D::ClassType::Contains);
    _d.CPGF_MD_TEMPLATE _method("RayCast", &D::ClassType::RayCast);
}


template <typename D>
void buildMetaClass_B2ClipVertex(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("v", &D::ClassType::v);
    _d.CPGF_MD_TEMPLATE _field("id", &D::ClassType::id);
}


template <typename D>
void buildMetaClass_B2Manifold(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("points", &D::ClassType::points);
    _d.CPGF_MD_TEMPLATE _field("localNormal", &D::ClassType::localNormal);
    _d.CPGF_MD_TEMPLATE _field("localPoint", &D::ClassType::localPoint);
    _d.CPGF_MD_TEMPLATE _field("type", &D::ClassType::type);
    _d.CPGF_MD_TEMPLATE _field("pointCount", &D::ClassType::pointCount);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::Type>("Type")
        ._element("e_circles", D::ClassType::e_circles)
        ._element("e_faceA", D::ClassType::e_faceA)
        ._element("e_faceB", D::ClassType::e_faceB)
    ;
}


template <typename D>
void buildMetaClass_B2ManifoldPoint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("localPoint", &D::ClassType::localPoint);
    _d.CPGF_MD_TEMPLATE _field("normalImpulse", &D::ClassType::normalImpulse);
    _d.CPGF_MD_TEMPLATE _field("tangentImpulse", &D::ClassType::tangentImpulse);
    _d.CPGF_MD_TEMPLATE _field("id", &D::ClassType::id);
}


template <typename D>
void buildMetaClass_B2RayCastInput(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("p1", &D::ClassType::p1);
    _d.CPGF_MD_TEMPLATE _field("p2", &D::ClassType::p2);
    _d.CPGF_MD_TEMPLATE _field("maxFraction", &D::ClassType::maxFraction);
}


template <typename D>
void buildMetaClass_B2RayCastOutput(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("normal", &D::ClassType::normal);
    _d.CPGF_MD_TEMPLATE _field("fraction", &D::ClassType::fraction);
}


template <typename D>
void buildMetaClass_B2WorldManifold(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("normal", &D::ClassType::normal);
    _d.CPGF_MD_TEMPLATE _field("points", &D::ClassType::points);
    _d.CPGF_MD_TEMPLATE _method("Initialize", &D::ClassType::Initialize);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
