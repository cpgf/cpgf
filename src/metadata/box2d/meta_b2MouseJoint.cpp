// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2MouseJoint.h"

#include "cpgf/metadata/box2d/meta_b2MouseJoint.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2MouseJoint, b2Joint> _d = GDefineMetaClass<b2MouseJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2MouseJoint");
    _ns._class(_d);
    buildMetaClass_B2MouseJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2MouseJointDef, b2JointDef> _d = GDefineMetaClass<b2MouseJointDef, b2JointDef>::declare("b2MouseJointDef");
    _ns._class(_d);
    buildMetaClass_B2MouseJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

