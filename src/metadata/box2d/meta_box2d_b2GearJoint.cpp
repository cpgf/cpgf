// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2GearJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2GearJoint.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2GearJoint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2GearJoint, b2Joint> _nd = GDefineMetaClass<b2GearJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2GearJoint");
        buildMetaClass_B2GearJoint(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2GearJointDef()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2GearJointDef, b2JointDef> _nd = GDefineMetaClass<b2GearJointDef, b2JointDef>::declare("b2GearJointDef");
        buildMetaClass_B2GearJointDef(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


