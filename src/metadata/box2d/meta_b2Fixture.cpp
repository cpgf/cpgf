// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Fixture.h"

#include "cpgf/metadata/box2d/meta_b2Fixture.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Filter> _d = GDefineMetaClass<b2Filter>::declare("b2Filter");
    _ns._class(_d);
    buildMetaClass_B2Filter(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Fixture> _d = GDefineMetaClass<b2Fixture>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent> >::declare("b2Fixture");
    _ns._class(_d);
    buildMetaClass_B2Fixture(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2FixtureDef> _d = GDefineMetaClass<b2FixtureDef>::declare("b2FixtureDef");
    _ns._class(_d);
    buildMetaClass_B2FixtureDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

