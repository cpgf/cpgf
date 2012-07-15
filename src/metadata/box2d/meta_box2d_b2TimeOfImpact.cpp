// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Collision/b2TimeOfImpact.h"

#include "cpgf/metadata/box2d/meta_box2d_b2TimeOfImpact.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2timeofimpact()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2timeofimpact(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2TOIInput()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2TOIInput> _nd = GDefineMetaClass<b2TOIInput>::declare("b2TOIInput");
        buildMetaClass_B2TOIInput(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2TOIOutput()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2TOIOutput> _nd = GDefineMetaClass<b2TOIOutput>::declare("b2TOIOutput");
        buildMetaClass_B2TOIOutput(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


