// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h"

#include "cpgf/metadata/box2d/meta_box2d_b2PolygonAndCircleContact.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2PolygonAndCircleContact()
{
    GDefineMetaClass<b2PolygonAndCircleContact, b2Contact> _d = GDefineMetaClass<b2PolygonAndCircleContact, b2Contact>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2PolygonAndCircleContact");
    buildMetaClass_B2PolygonAndCircleContact(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


