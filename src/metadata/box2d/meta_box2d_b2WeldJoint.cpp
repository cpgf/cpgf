// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2WeldJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2WeldJoint.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2WeldJoint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2WeldJoint, b2Joint> _nd = GDefineMetaClass<b2WeldJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2WeldJoint");
        buildMetaClass_B2WeldJoint(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2WeldJointDef()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2WeldJointDef, b2JointDef> _nd = GDefineMetaClass<b2WeldJointDef, b2JointDef>::declare("b2WeldJointDef");
        buildMetaClass_B2WeldJointDef(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


