// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Contacts/b2PolygonContact.h"

#include "cpgf/metadata/box2d/meta_box2d_b2PolygonContact.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2PolygonContact()
{
    GDefineMetaClass<b2PolygonContact, b2Contact> _d = GDefineMetaClass<b2PolygonContact, b2Contact>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2PolygonContact");
    buildMetaClass_B2PolygonContact(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 

