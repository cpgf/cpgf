// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2PulleyJoint.h"

#include "cpgf/metadata/box2d/meta_b2PulleyJoint.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2pulleyjoint()
{
    GDefineMetaNamespace _d = GDefineMetaNamespace::dangle("");
    buildMetaClass_Global_b2pulleyjoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2PulleyJoint()
{
    GDefineMetaClass<b2PulleyJoint, b2Joint> _d = GDefineMetaClass<b2PulleyJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2PulleyJoint");
    buildMetaClass_B2PulleyJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2PulleyJointDef()
{
    GDefineMetaClass<b2PulleyJointDef, b2JointDef> _d = GDefineMetaClass<b2PulleyJointDef, b2JointDef>::declare("b2PulleyJointDef");
    buildMetaClass_B2PulleyJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


