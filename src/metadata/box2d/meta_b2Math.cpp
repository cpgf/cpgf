// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Common/b2Math.h"

#include "cpgf/metadata/box2d/meta_b2Math.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _d = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    buildMetaClass_Global_b2math(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Mat22> _d = GDefineMetaClass<b2Mat22>::declare("b2Mat22");
    _ns._class(_d);
    buildMetaClass_B2Mat22(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Mat33> _d = GDefineMetaClass<b2Mat33>::declare("b2Mat33");
    _ns._class(_d);
    buildMetaClass_B2Mat33(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Sweep> _d = GDefineMetaClass<b2Sweep>::declare("b2Sweep");
    _ns._class(_d);
    buildMetaClass_B2Sweep(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Transform> _d = GDefineMetaClass<b2Transform>::declare("b2Transform");
    _ns._class(_d);
    buildMetaClass_B2Transform(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Vec2> _d = GDefineMetaClass<b2Vec2>::declare("b2Vec2");
    _ns._class(_d);
    buildMetaClass_B2Vec2(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Vec3> _d = GDefineMetaClass<b2Vec3>::declare("b2Vec3");
    _ns._class(_d);
    buildMetaClass_B2Vec3(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

