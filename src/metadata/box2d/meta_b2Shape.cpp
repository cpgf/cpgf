// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/Shapes/b2Shape.h"

#include "cpgf/metadata/box2d/meta_b2Shape.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2MassData()
{
    GDefineMetaClass<b2MassData> _d = GDefineMetaClass<b2MassData>::declare("b2MassData");
    buildMetaClass_B2MassData(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2Shape()
{
    GDefineMetaClass<b2Shape> _d = GDefineMetaClass<b2Shape>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("b2Shape");
    buildMetaClass_B2Shape(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


