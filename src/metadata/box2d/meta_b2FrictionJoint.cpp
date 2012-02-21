// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2FrictionJoint.h"

#include "cpgf/metadata/box2d/meta_b2FrictionJoint.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

GDefineMetaInfo createMetaClass_B2FrictionJoint()
{
    GDefineMetaClass<b2FrictionJoint, b2Joint> _d = GDefineMetaClass<b2FrictionJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2FrictionJoint");
    buildMetaClass_B2FrictionJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2FrictionJointDef()
{
    GDefineMetaClass<b2FrictionJointDef, b2JointDef> _d = GDefineMetaClass<b2FrictionJointDef, b2JointDef>::declare("b2FrictionJointDef");
    buildMetaClass_B2FrictionJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


