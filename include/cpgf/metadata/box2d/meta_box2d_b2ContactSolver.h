// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CONTACTSOLVER_H
#define __META_BOX2D_B2CONTACTSOLVER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2ContactConstraint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("points", &D::ClassType::points);
    _d.CPGF_MD_TEMPLATE _field("localNormal", &D::ClassType::localNormal);
    _d.CPGF_MD_TEMPLATE _field("localPoint", &D::ClassType::localPoint);
    _d.CPGF_MD_TEMPLATE _field("normal", &D::ClassType::normal);
    _d.CPGF_MD_TEMPLATE _field("normalMass", &D::ClassType::normalMass);
    _d.CPGF_MD_TEMPLATE _field("K", &D::ClassType::K);
    _d.CPGF_MD_TEMPLATE _field("bodyA", &D::ClassType::bodyA);
    _d.CPGF_MD_TEMPLATE _field("bodyB", &D::ClassType::bodyB);
    _d.CPGF_MD_TEMPLATE _field("type", &D::ClassType::type);
    _d.CPGF_MD_TEMPLATE _field("radius", &D::ClassType::radius);
    _d.CPGF_MD_TEMPLATE _field("friction", &D::ClassType::friction);
    _d.CPGF_MD_TEMPLATE _field("pointCount", &D::ClassType::pointCount);
    _d.CPGF_MD_TEMPLATE _field("manifold", &D::ClassType::manifold);
}


template <typename D>
void buildMetaClass_B2ContactConstraintPoint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("localPoint", &D::ClassType::localPoint);
    _d.CPGF_MD_TEMPLATE _field("rA", &D::ClassType::rA);
    _d.CPGF_MD_TEMPLATE _field("rB", &D::ClassType::rB);
    _d.CPGF_MD_TEMPLATE _field("normalImpulse", &D::ClassType::normalImpulse);
    _d.CPGF_MD_TEMPLATE _field("tangentImpulse", &D::ClassType::tangentImpulse);
    _d.CPGF_MD_TEMPLATE _field("normalMass", &D::ClassType::normalMass);
    _d.CPGF_MD_TEMPLATE _field("tangentMass", &D::ClassType::tangentMass);
    _d.CPGF_MD_TEMPLATE _field("velocityBias", &D::ClassType::velocityBias);
}


template <typename D>
void buildMetaClass_B2ContactSolver(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (b2Contact **, int32, b2StackAllocator *, float32)>();
    _d.CPGF_MD_TEMPLATE _field("m_allocator", &D::ClassType::m_allocator);
    _d.CPGF_MD_TEMPLATE _field("m_constraints", &D::ClassType::m_constraints);
    _d.CPGF_MD_TEMPLATE _field("m_constraintCount", &D::ClassType::m_constraintCount);
    _d.CPGF_MD_TEMPLATE _method("WarmStart", &D::ClassType::WarmStart);
    _d.CPGF_MD_TEMPLATE _method("SolveVelocityConstraints", &D::ClassType::SolveVelocityConstraints);
    _d.CPGF_MD_TEMPLATE _method("StoreImpulses", &D::ClassType::StoreImpulses);
    _d.CPGF_MD_TEMPLATE _method("SolvePositionConstraints", &D::ClassType::SolvePositionConstraints);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
