// Auto generated file, don't modify.

#include "irrlicht.h"
#include "SColor.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_SColor.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_scolor()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_scolor(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SColor()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::SColor> _nd = GDefineMetaClass<irr::video::SColor>::declare("SColor");
        buildMetaClass_SColor(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SColorHSL()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::SColorHSL> _nd = GDefineMetaClass<irr::video::SColorHSL>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SColorHSL");
        buildMetaClass_SColorHSL(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SColorf()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::SColorf> _nd = GDefineMetaClass<irr::video::SColorf>::declare("SColorf");
        buildMetaClass_SColorf(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


