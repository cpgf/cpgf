#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2PulleyJoint.h"

#include "cpgf/metadata/box2d/meta_b2PulleyJoint.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _d = GDefineMetaClass<void>::define("box2d");
    buildMetaClass_Global_b2pulleyjoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2PulleyJoint, b2Joint> _d = GDefineMetaClass<b2PulleyJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2PulleyJoint");
    _ns._class(_d);
    buildMetaClass_B2PulleyJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2PulleyJointDef, b2JointDef> _d = GDefineMetaClass<b2PulleyJointDef, b2JointDef>::declare("b2PulleyJointDef");
    _ns._class(_d);
    buildMetaClass_B2PulleyJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

