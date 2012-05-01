// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2MouseJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2MouseJoint.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2MouseJoint()
{
    GDefineMetaClass<b2MouseJoint, b2Joint> _d = GDefineMetaClass<b2MouseJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2MouseJoint");
    buildMetaClass_B2MouseJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2MouseJointDef()
{
    GDefineMetaClass<b2MouseJointDef, b2JointDef> _d = GDefineMetaClass<b2MouseJointDef, b2JointDef>::declare("b2MouseJointDef");
    buildMetaClass_B2MouseJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d


