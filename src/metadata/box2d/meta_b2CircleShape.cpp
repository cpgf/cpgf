#include "Box2D/Box2D.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"

#include "cpgf/metadata/box2d/meta_b2CircleShape.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2CircleShape, b2Shape> _d = GDefineMetaClass<b2CircleShape, b2Shape>::declare("b2CircleShape");
    _ns._class(_d);
    buildMetaClass_B2CircleShape(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

