// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2PrismaticJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2PrismaticJoint.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2PrismaticJoint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2PrismaticJoint, b2Joint> _nd = GDefineMetaClass<b2PrismaticJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2PrismaticJoint");
        buildMetaClass_B2PrismaticJoint(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2PrismaticJointDef()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2PrismaticJointDef, b2JointDef> _nd = GDefineMetaClass<b2PrismaticJointDef, b2JointDef>::declare("b2PrismaticJointDef");
        buildMetaClass_B2PrismaticJointDef(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


