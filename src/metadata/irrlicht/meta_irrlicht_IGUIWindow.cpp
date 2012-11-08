// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUIWindow.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUIWindow.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IGUIWindow()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUIWindow, irr::gui::IGUIElement> _nd = GDefineMetaClass<irr::gui::IGUIWindow, irr::gui::IGUIElement>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUIWindow");
        buildMetaClass_IGUIWindow(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


