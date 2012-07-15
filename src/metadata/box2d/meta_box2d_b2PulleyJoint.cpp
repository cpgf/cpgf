// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2PulleyJoint.h"

#include "cpgf/metadata/box2d/meta_box2d_b2PulleyJoint.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2pulleyjoint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2pulleyjoint(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2PulleyJoint()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2PulleyJoint, b2Joint> _nd = GDefineMetaClass<b2PulleyJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2PulleyJoint");
        buildMetaClass_B2PulleyJoint(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2PulleyJointDef()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2PulleyJointDef, b2JointDef> _nd = GDefineMetaClass<b2PulleyJointDef, b2JointDef>::declare("b2PulleyJointDef");
        buildMetaClass_B2PulleyJointDef(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


