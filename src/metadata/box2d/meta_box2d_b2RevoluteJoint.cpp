// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2RevoluteJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2RevoluteJoint.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2RevoluteJoint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2RevoluteJoint, b2Joint> _nd = GDefineMetaClass<b2RevoluteJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2RevoluteJoint");
        buildMetaClass_B2RevoluteJoint(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2RevoluteJointDef()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2RevoluteJointDef, b2JointDef> _nd = GDefineMetaClass<b2RevoluteJointDef, b2JointDef>::declare("b2RevoluteJointDef");
        buildMetaClass_B2RevoluteJointDef(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


