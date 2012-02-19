#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2LineJoint.h"

#include "cpgf/metadata/box2d/meta_b2LineJoint.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2LineJoint, b2Joint> _d = GDefineMetaClass<b2LineJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2LineJoint");
    _ns._class(_d);
    buildMetaClass_B2LineJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2LineJointDef, b2JointDef> _d = GDefineMetaClass<b2LineJointDef, b2JointDef>::declare("b2LineJointDef");
    _ns._class(_d);
    buildMetaClass_B2LineJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

