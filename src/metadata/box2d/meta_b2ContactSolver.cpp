// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Contacts/b2ContactSolver.h"

#include "cpgf/metadata/box2d/meta_b2ContactSolver.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaNamespace _ns = GDefineMetaNamespace::define(_mEta_nS_box2d);
    GDefineMetaClass<b2ContactConstraint> _d = GDefineMetaClass<b2ContactConstraint>::declare("b2ContactConstraint");
    _ns._class(_d);
    buildMetaClass_B2ContactConstraint(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaNamespace _ns = GDefineMetaNamespace::define(_mEta_nS_box2d);
    GDefineMetaClass<b2ContactConstraintPoint> _d = GDefineMetaClass<b2ContactConstraintPoint>::declare("b2ContactConstraintPoint");
    _ns._class(_d);
    buildMetaClass_B2ContactConstraintPoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaNamespace _ns = GDefineMetaNamespace::define(_mEta_nS_box2d);
    GDefineMetaClass<b2ContactSolver> _d = GDefineMetaClass<b2ContactSolver>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2ContactSolver");
    _ns._class(_d);
    buildMetaClass_B2ContactSolver(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

