// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"

#include "cpgf/metadata/box2d/meta_box2d_b2CircleShape.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2CircleShape()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2CircleShape, b2Shape> _nd = GDefineMetaClass<b2CircleShape, b2Shape>::declare("b2CircleShape");
        buildMetaClass_B2CircleShape(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


