// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ITriangleSelector.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ITriangleSelector.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ITriangleSelector()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ITriangleSelector, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::ITriangleSelector, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ITriangleSelector");
        buildMetaClass_ITriangleSelector(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


