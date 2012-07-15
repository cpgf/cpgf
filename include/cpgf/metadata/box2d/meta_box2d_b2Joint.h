// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2JOINT_H
#define __META_BOX2D_B2JOINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2joint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<b2JointType>("b2JointType")
        ._element("e_unknownJoint", e_unknownJoint)
        ._element("e_revoluteJoint", e_revoluteJoint)
        ._element("e_prismaticJoint", e_prismaticJoint)
        ._element("e_distanceJoint", e_distanceJoint)
        ._element("e_pulleyJoint", e_pulleyJoint)
        ._element("e_mouseJoint", e_mouseJoint)
        ._element("e_gearJoint", e_gearJoint)
        ._element("e_lineJoint", e_lineJoint)
        ._element("e_weldJoint", e_weldJoint)
        ._element("e_frictionJoint", e_frictionJoint)
    ;
    _d.CPGF_MD_TEMPLATE _enum<b2LimitState>("b2LimitState")
        ._element("e_inactiveLimit", e_inactiveLimit)
        ._element("e_atLowerLimit", e_atLowerLimit)
        ._element("e_atUpperLimit", e_atUpperLimit)
        ._element("e_equalLimits", e_equalLimits)
    ;
}


template <typename D>
void buildMetaClass_B2Jacobian(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("linearA", &D::ClassType::linearA);
    _d.CPGF_MD_TEMPLATE _field("angularA", &D::ClassType::angularA);
    _d.CPGF_MD_TEMPLATE _field("linearB", &D::ClassType::linearB);
    _d.CPGF_MD_TEMPLATE _field("angularB", &D::ClassType::angularB);
    _d.CPGF_MD_TEMPLATE _method("SetZero", &D::ClassType::SetZero);
    _d.CPGF_MD_TEMPLATE _method("Set", &D::ClassType::Set);
    _d.CPGF_MD_TEMPLATE _method("Compute", &D::ClassType::Compute);
}


template <typename D>
void buildMetaClass_B2Joint(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("GetType", &D::ClassType::GetType);
    _d.CPGF_MD_TEMPLATE _method("GetBodyA", &D::ClassType::GetBodyA);
    _d.CPGF_MD_TEMPLATE _method("GetBodyB", &D::ClassType::GetBodyB);
    _d.CPGF_MD_TEMPLATE _method("GetAnchorA", &D::ClassType::GetAnchorA);
    _d.CPGF_MD_TEMPLATE _method("GetAnchorB", &D::ClassType::GetAnchorB);
    _d.CPGF_MD_TEMPLATE _method("GetReactionForce", &D::ClassType::GetReactionForce);
    _d.CPGF_MD_TEMPLATE _method("GetReactionTorque", &D::ClassType::GetReactionTorque);
    _d.CPGF_MD_TEMPLATE _method("GetNext", &D::ClassType::GetNext);
    _d.CPGF_MD_TEMPLATE _method("GetUserData", &D::ClassType::GetUserData);
    _d.CPGF_MD_TEMPLATE _method("SetUserData", &D::ClassType::SetUserData);
    _d.CPGF_MD_TEMPLATE _method("IsActive", &D::ClassType::IsActive);
}


template <typename D>
void buildMetaClass_B2JointDef(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("type", &D::ClassType::type);
    _d.CPGF_MD_TEMPLATE _field("userData", &D::ClassType::userData);
    _d.CPGF_MD_TEMPLATE _field("bodyA", &D::ClassType::bodyA);
    _d.CPGF_MD_TEMPLATE _field("bodyB", &D::ClassType::bodyB);
    _d.CPGF_MD_TEMPLATE _field("collideConnected", &D::ClassType::collideConnected);
}


template <typename D>
void buildMetaClass_B2JointEdge(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("other", &D::ClassType::other);
    _d.CPGF_MD_TEMPLATE _field("joint", &D::ClassType::joint);
    _d.CPGF_MD_TEMPLATE _field("prev", &D::ClassType::prev);
    _d.CPGF_MD_TEMPLATE _field("next", &D::ClassType::next);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
