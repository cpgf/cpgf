#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2WorldCallbacks.h"

#include "cpgf/metadata/box2d/meta_b2WorldCallbacks.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2Color> _d = GDefineMetaClass<b2Color>::declare("b2Color");
    _ns._class(_d);
    buildMetaClass_B2Color(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2ContactFilter> _d = GDefineMetaClass<b2ContactFilter>::Policy<MakePolicy<GMetaRuleDestructorAbsent> >::declare("b2ContactFilter");
    _ns._class(_d);
    buildMetaClass_B2ContactFilter(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2ContactImpulse> _d = GDefineMetaClass<b2ContactImpulse>::declare("b2ContactImpulse");
    _ns._class(_d);
    buildMetaClass_B2ContactImpulse(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2ContactListener> _d = GDefineMetaClass<b2ContactListener>::Policy<MakePolicy<GMetaRuleDestructorAbsent> >::declare("b2ContactListener");
    _ns._class(_d);
    buildMetaClass_B2ContactListener(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2DebugDraw> _d = GDefineMetaClass<b2DebugDraw>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("b2DebugDraw");
    _ns._class(_d);
    buildMetaClass_B2DebugDraw(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2DestructionListener> _d = GDefineMetaClass<b2DestructionListener>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("b2DestructionListener");
    _ns._class(_d);
    buildMetaClass_B2DestructionListener(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2QueryCallback> _d = GDefineMetaClass<b2QueryCallback>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("b2QueryCallback");
    _ns._class(_d);
    buildMetaClass_B2QueryCallback(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2RayCastCallback> _d = GDefineMetaClass<b2RayCastCallback>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("b2RayCastCallback");
    _ns._class(_d);
    buildMetaClass_B2RayCastCallback(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

