// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2JOINT_H
#define __META_BOX2D_B2JOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"




namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_Global_b2joint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<b2JointType>(replaceName("b2JointType", _r))
        ._element(replaceName("e_unknownJoint", _r), b2JointType::e_unknownJoint)
        ._element(replaceName("e_revoluteJoint", _r), b2JointType::e_revoluteJoint)
        ._element(replaceName("e_prismaticJoint", _r), b2JointType::e_prismaticJoint)
        ._element(replaceName("e_distanceJoint", _r), b2JointType::e_distanceJoint)
        ._element(replaceName("e_pulleyJoint", _r), b2JointType::e_pulleyJoint)
        ._element(replaceName("e_mouseJoint", _r), b2JointType::e_mouseJoint)
        ._element(replaceName("e_gearJoint", _r), b2JointType::e_gearJoint)
        ._element(replaceName("e_lineJoint", _r), b2JointType::e_lineJoint)
        ._element(replaceName("e_weldJoint", _r), b2JointType::e_weldJoint)
        ._element(replaceName("e_frictionJoint", _r), b2JointType::e_frictionJoint)
    ;
    _d.CPGF_MD_TEMPLATE _enum<b2LimitState>(replaceName("b2LimitState", _r))
        ._element(replaceName("e_inactiveLimit", _r), b2LimitState::e_inactiveLimit)
        ._element(replaceName("e_atLowerLimit", _r), b2LimitState::e_atLowerLimit)
        ._element(replaceName("e_atUpperLimit", _r), b2LimitState::e_atUpperLimit)
        ._element(replaceName("e_equalLimits", _r), b2LimitState::e_equalLimits)
    ;
}


template <typename D, typename Policy>
void buildMetaClass_B2Jacobian(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("linearA", _r), &D::ClassType::linearA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("angularA", _r), &D::ClassType::angularA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("linearB", _r), &D::ClassType::linearB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("angularB", _r), &D::ClassType::angularB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetZero", _r), &D::ClassType::SetZero, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Set", _r), &D::ClassType::Set, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Compute", _r), &D::ClassType::Compute, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2Joint(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetType", _r), &D::ClassType::GetType, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBodyA", _r), &D::ClassType::GetBodyA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetBodyB", _r), &D::ClassType::GetBodyB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorA", _r), &D::ClassType::GetAnchorA, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetAnchorB", _r), &D::ClassType::GetAnchorB, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionForce", _r), &D::ClassType::GetReactionForce, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetReactionTorque", _r), &D::ClassType::GetReactionTorque, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetNext", _r), &D::ClassType::GetNext, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetUserData", _r), &D::ClassType::GetUserData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("SetUserData", _r), &D::ClassType::SetUserData, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("IsActive", _r), &D::ClassType::IsActive, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2JointDef(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("type", _r), &D::ClassType::type, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("userData", _r), &D::ClassType::userData, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("bodyA", _r), &D::ClassType::bodyA, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("bodyB", _r), &D::ClassType::bodyB, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("collideConnected", _r), &D::ClassType::collideConnected, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2JointEdge(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("other", _r), &D::ClassType::other, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("joint", _r), &D::ClassType::joint, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("prev", _r), &D::ClassType::prev, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("next", _r), &D::ClassType::next, _p);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
