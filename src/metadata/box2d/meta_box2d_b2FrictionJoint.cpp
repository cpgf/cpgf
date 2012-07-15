// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2FrictionJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2FrictionJoint.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2FrictionJoint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2FrictionJoint, b2Joint> _nd = GDefineMetaClass<b2FrictionJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2FrictionJoint");
        buildMetaClass_B2FrictionJoint(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2FrictionJointDef()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2FrictionJointDef, b2JointDef> _nd = GDefineMetaClass<b2FrictionJointDef, b2JointDef>::declare("b2FrictionJointDef");
        buildMetaClass_B2FrictionJointDef(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


