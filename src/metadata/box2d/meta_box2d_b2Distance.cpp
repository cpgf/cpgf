// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2Distance.h"

#include "cpgf/metadata/box2d/meta_box2d_b2Distance.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2distance()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2distance(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2DistanceInput()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2DistanceInput> _nd = GDefineMetaClass<b2DistanceInput>::declare("b2DistanceInput");
        buildMetaClass_B2DistanceInput(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2DistanceOutput()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2DistanceOutput> _nd = GDefineMetaClass<b2DistanceOutput>::declare("b2DistanceOutput");
        buildMetaClass_B2DistanceOutput(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2DistanceProxy()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2DistanceProxy> _nd = GDefineMetaClass<b2DistanceProxy>::declare("b2DistanceProxy");
        buildMetaClass_B2DistanceProxy(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2SimplexCache()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2SimplexCache> _nd = GDefineMetaClass<b2SimplexCache>::declare("b2SimplexCache");
        buildMetaClass_B2SimplexCache(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


