// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IMetaTriangleSelector.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IMetaTriangleSelector.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IMetaTriangleSelector()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IMetaTriangleSelector, irr::scene::ITriangleSelector> _nd = GDefineMetaClass<irr::scene::IMetaTriangleSelector, irr::scene::ITriangleSelector>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IMetaTriangleSelector");
        buildMetaClass_IMetaTriangleSelector(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


