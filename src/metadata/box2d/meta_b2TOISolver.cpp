// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Contacts/b2TOISolver.h"

#include "cpgf/metadata/box2d/meta_b2TOISolver.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2TOISolver> _d = GDefineMetaClass<b2TOISolver>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2TOISolver");
    _ns._class(_d);
    buildMetaClass_B2TOISolver(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

