// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2Joint.h"

#include "cpgf/metadata/box2d/meta_b2Joint.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaNamespace _d = GDefineMetaNamespace::define(_mEta_nS_box2d);
    buildMetaClass_Global_b2joint(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaNamespace _ns = GDefineMetaNamespace::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Jacobian> _d = GDefineMetaClass<b2Jacobian>::declare("b2Jacobian");
    _ns._class(_d);
    buildMetaClass_B2Jacobian(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaNamespace _ns = GDefineMetaNamespace::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Joint> _d = GDefineMetaClass<b2Joint>::Policy<MakePolicy<GMetaRuleDestructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("b2Joint");
    _ns._class(_d);
    buildMetaClass_B2Joint(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaNamespace _ns = GDefineMetaNamespace::define(_mEta_nS_box2d);
    GDefineMetaClass<b2JointDef> _d = GDefineMetaClass<b2JointDef>::declare("b2JointDef");
    _ns._class(_d);
    buildMetaClass_B2JointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaNamespace _ns = GDefineMetaNamespace::define(_mEta_nS_box2d);
    GDefineMetaClass<b2JointEdge> _d = GDefineMetaClass<b2JointEdge>::declare("b2JointEdge");
    _ns._class(_d);
    buildMetaClass_B2JointEdge(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

