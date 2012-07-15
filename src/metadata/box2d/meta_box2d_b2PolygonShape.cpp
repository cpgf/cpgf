// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"

#include "cpgf/metadata/box2d/meta_box2d_b2PolygonShape.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2PolygonShape()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2PolygonShape, b2Shape> _nd = GDefineMetaClass<b2PolygonShape, b2Shape>::declare("b2PolygonShape");
        buildMetaClass_B2PolygonShape(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


