// Auto generated file, don't modify.

#include "Box2D/Box2D.h"
#include "Box2D/Common/b2Settings.h"

#include "cpgf/metadata/box2d/meta_box2d_b2Settings.h"

using namespace cpgf;

namespace meta_box2d { 


GDefineMetaInfo createMetaClass_Global_b2settings()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_b2settings(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_B2Version()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<b2Version> _nd = GDefineMetaClass<b2Version>::declare("b2Version");
        buildMetaClass_B2Version(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_box2d


