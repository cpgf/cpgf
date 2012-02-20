// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2WeldJoint.h"

#include "cpgf/metadata/box2d/meta_b2WeldJoint.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2WeldJoint, b2Joint> _d = GDefineMetaClass<b2WeldJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2WeldJoint");
    _ns._class(_d);
    buildMetaClass_B2WeldJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2WeldJointDef, b2JointDef> _d = GDefineMetaClass<b2WeldJointDef, b2JointDef>::declare("b2WeldJointDef");
    _ns._class(_d);
    buildMetaClass_B2WeldJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

