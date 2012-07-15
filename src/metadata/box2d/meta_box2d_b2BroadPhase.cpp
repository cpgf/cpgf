// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2BroadPhase.h"

#include "cpgf/metadata/box2d/meta_box2d_b2BroadPhase.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2broadphase()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2broadphase(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2BroadPhase()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2BroadPhase> _nd = GDefineMetaClass<b2BroadPhase>::declare("b2BroadPhase");
        buildMetaClass_B2BroadPhase(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2Pair()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2Pair> _nd = GDefineMetaClass<b2Pair>::declare("b2Pair");
        buildMetaClass_B2Pair(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


