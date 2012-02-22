// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CONTACTSOLVER_H
#define __META_BOX2D_B2CONTACTSOLVER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_B2ContactConstraint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("points", _r), &D::ClassType::points, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localNormal", _r), &D::ClassType::localNormal, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("localPoint", _r), &D::ClassType::localPoint, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normal", _r), &D::ClassType::normal, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalMass", _r), &D::ClassType::normalMass, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("K", _r), &D::ClassType::K, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("bodyA", _r), &D::ClassType::bodyA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("bodyB", _r), &D::ClassType::bodyB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("type", _r), &D::ClassType::type, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("radius", _r), &D::ClassType::radius, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("friction", _r), &D::ClassType::friction, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("pointCount", _r), &D::ClassType::pointCount, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("manifold", _r), &D::ClassType::manifold, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2ContactConstraintPoint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("localPoint", _r), &D::ClassType::localPoint, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("rA", _r), &D::ClassType::rA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("rB", _r), &D::ClassType::rB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalImpulse", _r), &D::ClassType::normalImpulse, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("tangentImpulse", _r), &D::ClassType::tangentImpulse, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("normalMass", _r), &D::ClassType::normalMass, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("tangentMass", _r), &D::ClassType::tangentMass, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("velocityBias", _r), &D::ClassType::velocityBias, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2ContactSolver(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (b2Contact **, int32, b2StackAllocator *, float32)>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_allocator", _r), &D::ClassType::m_allocator, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_constraints", _r), &D::ClassType::m_constraints, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_constraintCount", _r), &D::ClassType::m_constraintCount, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("WarmStart", _r), &D::ClassType::WarmStart, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SolveVelocityConstraints", _r), &D::ClassType::SolveVelocityConstraints, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("StoreImpulses", _r), &D::ClassType::StoreImpulses, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SolvePositionConstraints", _r), &D::ClassType::SolvePositionConstraints, _p);
}


} // namespace meta_box2d 




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif