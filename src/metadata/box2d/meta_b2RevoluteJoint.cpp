#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2RevoluteJoint.h"

#include "cpgf/metadata/box2d/meta_b2RevoluteJoint.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2RevoluteJoint, b2Joint> _d = GDefineMetaClass<b2RevoluteJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2RevoluteJoint");
    _ns._class(_d);
    buildMetaClass_B2RevoluteJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2RevoluteJointDef, b2JointDef> _d = GDefineMetaClass<b2RevoluteJointDef, b2JointDef>::declare("b2RevoluteJointDef");
    _ns._class(_d);
    buildMetaClass_B2RevoluteJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

