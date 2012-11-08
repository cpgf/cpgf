// Auto generated file, don't modify.

#include "irrlicht.h"
#include "SLight.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_SLight.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_slight()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_slight(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SLight()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::SLight> _nd = GDefineMetaClass<irr::video::SLight>::declare("SLight");
        buildMetaClass_SLight(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


