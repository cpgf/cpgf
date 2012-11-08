// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUIFont.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUIFont.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iguifont()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iguifont(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGUIFont()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUIFont, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::gui::IGUIFont, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUIFont");
        buildMetaClass_IGUIFont(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


