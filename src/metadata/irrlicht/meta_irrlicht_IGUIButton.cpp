// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUIButton.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUIButton.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iguibutton()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iguibutton(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGUIButton()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUIButton, irr::gui::IGUIElement> _nd = GDefineMetaClass<irr::gui::IGUIButton, irr::gui::IGUIElement>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUIButton");
        buildMetaClass_IGUIButton(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


