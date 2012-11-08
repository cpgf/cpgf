// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUIContextMenu.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUIContextMenu.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iguicontextmenu()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iguicontextmenu(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGUIContextMenu()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUIContextMenu, irr::gui::IGUIElement> _nd = GDefineMetaClass<irr::gui::IGUIContextMenu, irr::gui::IGUIElement>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUIContextMenu");
        buildMetaClass_IGUIContextMenu(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


