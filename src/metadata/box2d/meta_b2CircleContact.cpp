#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Contacts/b2CircleContact.h"

#include "cpgf/metadata/box2d/meta_b2CircleContact.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2CircleContact, b2Contact> _d = GDefineMetaClass<b2CircleContact, b2Contact>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent> >::declare("b2CircleContact");
    _ns._class(_d);
    buildMetaClass_B2CircleContact(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

