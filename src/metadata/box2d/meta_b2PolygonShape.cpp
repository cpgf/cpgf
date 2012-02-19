#include "Box2D/Box2D.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"

#include "cpgf/metadata/box2d/meta_b2PolygonShape.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

extern const char * _mEta_nS_box2d;

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define(_mEta_nS_box2d);
    GDefineMetaClass<b2PolygonShape, b2Shape> _d = GDefineMetaClass<b2PolygonShape, b2Shape>::declare("b2PolygonShape");
    _ns._class(_d);
    buildMetaClass_B2PolygonShape(0, _d, NULL, GMetaPolicyCopyAllConstReference());

    }
    

}

} // unnamed namespace

