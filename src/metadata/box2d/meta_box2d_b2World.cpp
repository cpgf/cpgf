// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2World.h"

#include "cpgf/metadata/box2d/meta_box2d_b2World.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_B2World()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2World> _nd = GDefineMetaClass<b2World>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("b2World");
        buildMetaClass_B2World(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


