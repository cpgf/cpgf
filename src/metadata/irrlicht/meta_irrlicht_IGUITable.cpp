// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGUITable.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGUITable.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iguitable()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iguitable(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IGUITable()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::gui::IGUITable, irr::gui::IGUIElement> _nd = GDefineMetaClass<irr::gui::IGUITable, irr::gui::IGUIElement>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGUITable");
        buildMetaClass_IGUITable(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


