// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUISkin.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUISkin.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iguiskin()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iguiskin(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGUISkin()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUISkin, irr::io::IAttributeExchangingObject> _nd = GDefineMetaClass<irr::gui::IGUISkin, irr::io::IAttributeExchangingObject>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUISkin");
        buildMetaClass_IGUISkin(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


