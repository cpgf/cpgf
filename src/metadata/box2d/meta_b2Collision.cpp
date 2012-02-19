#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2Collision.h"

#include "cpgf/metadata/box2d/meta_b2Collision.h"

#include "cpgf/gmetapolicy.h"
#include "cpgf/goutmain.h"

using namespace cpgf;

namespace {

G_AUTO_RUN_BEFORE_MAIN()
{
    {
    GDefineMetaClass<void> _d = GDefineMetaClass<void>::define("box2d");
    buildMetaClass_Global_b2collision(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2AABB> _d = GDefineMetaClass<b2AABB>::declare("b2AABB");
    _ns._class(_d);
    buildMetaClass_B2AABB(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2ClipVertex> _d = GDefineMetaClass<b2ClipVertex>::declare("b2ClipVertex");
    _ns._class(_d);
    buildMetaClass_B2ClipVertex(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2Manifold> _d = GDefineMetaClass<b2Manifold>::declare("b2Manifold");
    _ns._class(_d);
    buildMetaClass_B2Manifold(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2ManifoldPoint> _d = GDefineMetaClass<b2ManifoldPoint>::declare("b2ManifoldPoint");
    _ns._class(_d);
    buildMetaClass_B2ManifoldPoint(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2RayCastInput> _d = GDefineMetaClass<b2RayCastInput>::declare("b2RayCastInput");
    _ns._class(_d);
    buildMetaClass_B2RayCastInput(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2RayCastOutput> _d = GDefineMetaClass<b2RayCastOutput>::declare("b2RayCastOutput");
    _ns._class(_d);
    buildMetaClass_B2RayCastOutput(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2WorldManifold> _d = GDefineMetaClass<b2WorldManifold>::declare("b2WorldManifold");
    _ns._class(_d);
    buildMetaClass_B2WorldManifold(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

    {
    GDefineMetaClass<void> _ns = GDefineMetaClass<void>::define("box2d");
    GDefineMetaClass<b2ContactID::Features> _d = GDefineMetaClass<b2ContactID::Features>::declare("Features");
    _ns._class(_d);
    buildMetaClass_Features(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    }
    

}

} // unnamed namespace

