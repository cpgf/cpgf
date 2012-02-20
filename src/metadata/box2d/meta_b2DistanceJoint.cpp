// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/Joints/b2DistanceJoint.h"

#include "cpgf/metadata/box2d/meta_b2DistanceJoint.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2DistanceJoint, b2Joint> _d = GDefineMetaClass<b2DistanceJoint, b2Joint>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2DistanceJoint");
    _ns._class(_d);
    buildMetaClass_B2DistanceJoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2DistanceJointDef, b2JointDef> _d = GDefineMetaClass<b2DistanceJointDef, b2JointDef>::declare("b2DistanceJointDef");
    _ns._class(_d);
    buildMetaClass_B2DistanceJointDef(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

