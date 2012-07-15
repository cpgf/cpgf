// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2MouseJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2MouseJoint.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2MouseJoint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2MouseJoint, b2Joint> _nd = GDefineMetaClass<b2MouseJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2MouseJoint");
        buildMetaClass_B2MouseJoint(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2MouseJointDef()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2MouseJointDef, b2JointDef> _nd = GDefineMetaClass<b2MouseJointDef, b2JointDef>::declare("b2MouseJointDef");
        buildMetaClass_B2MouseJointDef(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


