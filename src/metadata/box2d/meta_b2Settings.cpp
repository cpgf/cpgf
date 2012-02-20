// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Common/b2Settings.h"

#include "cpgf/metadata/box2d/meta_b2Settings.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _d = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    buildMetaClass_Global_b2settings(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2Version> _d = GDefineMetaClass<b2Version>::declare("b2Version");
    _ns._class(_d);
    buildMetaClass_B2Version(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

