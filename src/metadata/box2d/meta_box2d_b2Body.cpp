// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"

#include "cpgf/metadata/box2d/meta_box2d_b2Body.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2body()
{
    GDefineMetaNamespace _d = GDefineMetaNamespace::dangle("");
    buildMetaClass_Global_b2body(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2Body()
{
    GDefineMetaClass<b2Body> _d = GDefineMetaClass<b2Body>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent> >::declare("b2Body");
    buildMetaClass_B2Body(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2BodyDef()
{
    GDefineMetaClass<b2BodyDef> _d = GDefineMetaClass<b2BodyDef>::declare("b2BodyDef");
    buildMetaClass_B2BodyDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


