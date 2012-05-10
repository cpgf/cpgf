// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CONTACTSOLVER_H
#define __META_BOX2D_B2CONTACTSOLVER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2ContactConstraint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("points", _r), &D::ClassType::points);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localNormal", _r), &D::ClassType::localNormal);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localPoint", _r), &D::ClassType::localPoint);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normal", _r), &D::ClassType::normal);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalMass", _r), &D::ClassType::normalMass);
    _d.CPGF_MD_TEMPLATE _field(replaceName("K", _r), &D::ClassType::K);
    _d.CPGF_MD_TEMPLATE _field(replaceName("bodyA", _r), &D::ClassType::bodyA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("bodyB", _r), &D::ClassType::bodyB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("type", _r), &D::ClassType::type);
    _d.CPGF_MD_TEMPLATE _field(replaceName("radius", _r), &D::ClassType::radius);
    _d.CPGF_MD_TEMPLATE _field(replaceName("friction", _r), &D::ClassType::friction);
    _d.CPGF_MD_TEMPLATE _field(replaceName("pointCount", _r), &D::ClassType::pointCount);
    _d.CPGF_MD_TEMPLATE _field(replaceName("manifold", _r), &D::ClassType::manifold);
}


template <typename D>
void buildMetaClass_B2ContactConstraintPoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("localPoint", _r), &D::ClassType::localPoint);
    _d.CPGF_MD_TEMPLATE _field(replaceName("rA", _r), &D::ClassType::rA);
    _d.CPGF_MD_TEMPLATE _field(replaceName("rB", _r), &D::ClassType::rB);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalImpulse", _r), &D::ClassType::normalImpulse);
    _d.CPGF_MD_TEMPLATE _field(replaceName("tangentImpulse", _r), &D::ClassType::tangentImpulse);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalMass", _r), &D::ClassType::normalMass);
    _d.CPGF_MD_TEMPLATE _field(replaceName("tangentMass", _r), &D::ClassType::tangentMass);
    _d.CPGF_MD_TEMPLATE _field(replaceName("velocityBias", _r), &D::ClassType::velocityBias);
}


template <typename D>
void buildMetaClass_B2ContactSolver(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (b2Contact **, int32, b2StackAllocator *, float32)>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_allocator", _r), &D::ClassType::m_allocator);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_constraints", _r), &D::ClassType::m_constraints);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_constraintCount", _r), &D::ClassType::m_constraintCount);
    _d.CPGF_MD_TEMPLATE _method(replaceName("WarmStart", _r), &D::ClassType::WarmStart);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SolveVelocityConstraints", _r), &D::ClassType::SolveVelocityConstraints);
    _d.CPGF_MD_TEMPLATE _method(replaceName("StoreImpulses", _r), &D::ClassType::StoreImpulses);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SolvePositionConstraints", _r), &D::ClassType::SolvePositionConstraints);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
