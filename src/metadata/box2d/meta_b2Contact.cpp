#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Contacts/b2Contact.h"

#include "cpgf/metadata/box2d/meta_b2Contact.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2Contact> _d = GDefineMetaClass<b2Contact>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("b2Contact");
    _ns._class(_d);
    buildMetaClass_B2Contact(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2ContactEdge> _d = GDefineMetaClass<b2ContactEdge>::declare("b2ContactEdge");
    _ns._class(_d);
    buildMetaClass_B2ContactEdge(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2ContactRegister> _d = GDefineMetaClass<b2ContactRegister>::declare("b2ContactRegister");
    _ns._class(_d);
    buildMetaClass_B2ContactRegister(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

