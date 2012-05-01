// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2LineJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2LineJoint.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2LineJoint()
{
    GDefineMetaClass<b2LineJoint, b2Joint> _d = GDefineMetaClass<b2LineJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2LineJoint");
    buildMetaClass_B2LineJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2LineJointDef()
{
    GDefineMetaClass<b2LineJointDef, b2JointDef> _d = GDefineMetaClass<b2LineJointDef, b2JointDef>::declare("b2LineJointDef");
    buildMetaClass_B2LineJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d


