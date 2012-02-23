// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2BroadPhase.h"

#include "cpgf/metadata/box2d/meta_box2d_b2BroadPhase.h"

#include "cpgf/gmetapolicy.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2broadphase()
{
    GDefineMetaDangle _d = GDefineMetaDangle::dangle();
    buildMetaClass_Global_b2broadphase(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2BroadPhase()
{
    GDefineMetaClass<b2BroadPhase> _d = GDefineMetaClass<b2BroadPhase>::declare("b2BroadPhase");
    buildMetaClass_B2BroadPhase(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2Pair()
{
    GDefineMetaClass<b2Pair> _d = GDefineMetaClass<b2Pair>::declare("b2Pair");
    buildMetaClass_B2Pair(0, _d, NULL, GMetaPolicyCopyAllConstReference());
    return _d.getMetaInfo();
}


} // namespace meta_box2d 


