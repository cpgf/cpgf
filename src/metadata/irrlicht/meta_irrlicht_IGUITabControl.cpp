// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUITabControl.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUITabControl.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IGUITab()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUITab, irr::gui::IGUIElement> _nd = GDefineMetaClass<irr::gui::IGUITab, irr::gui::IGUIElement>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUITab");
        buildMetaClass_IGUITab(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGUITabControl()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUITabControl, irr::gui::IGUIElement> _nd = GDefineMetaClass<irr::gui::IGUITabControl, irr::gui::IGUIElement>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUITabControl");
        buildMetaClass_IGUITabControl(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


