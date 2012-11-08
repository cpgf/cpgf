// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUIElement.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUIElement.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IGUIElement()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUIElement, irr::io::IAttributeExchangingObject, irr::IEventReceiver> _nd = GDefineMetaClass<irr::gui::IGUIElement, irr::io::IAttributeExchangingObject, irr::IEventReceiver>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("IGUIElement");
        buildMetaClass_IGUIElement(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


