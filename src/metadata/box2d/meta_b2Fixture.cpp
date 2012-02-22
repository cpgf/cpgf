// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Fixture.h"

#include "cpgf/metadata/box2d/meta_b2Fixture.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2Filter()
{
    GDefineMetaClass<b2Filter> _d = GDefineMetaClass<b2Filter>::declare("b2Filter");
    buildMetaClass_B2Filter(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2Fixture()
{
    GDefineMetaClass<b2Fixture> _d = GDefineMetaClass<b2Fixture>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent> >::declare("b2Fixture");
    buildMetaClass_B2Fixture(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2FixtureDef()
{
    GDefineMetaClass<b2FixtureDef> _d = GDefineMetaClass<b2FixtureDef>::declare("b2FixtureDef");
    buildMetaClass_B2FixtureDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


