// Auto generated file, don't modify.

#include "irrlicht.h"
#include "SKeyMap.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_SKeyMap.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_skeymap()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_skeymap(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SKeyMap()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::SKeyMap> _nd = GDefineMetaClass<irr::SKeyMap>::declare("SKeyMap");
        buildMetaClass_SKeyMap(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


