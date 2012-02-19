#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"

#include "cpgf/metadata/box2d/meta_b2Body.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _d = GDefineMetaClass<void>::define("box2d");
    buildMetaClass_Global_b2body(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2Body> _d = GDefineMetaClass<b2Body>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent> >::declare("b2Body");
    _ns._class(_d);
    buildMetaClass_B2Body(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2BodyDef> _d = GDefineMetaClass<b2BodyDef>::declare("b2BodyDef");
    _ns._class(_d);
    buildMetaClass_B2BodyDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

