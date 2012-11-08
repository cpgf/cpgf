// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IGeometryCreator.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IGeometryCreator.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IGeometryCreator()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IGeometryCreator, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IGeometryCreator, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IGeometryCreator");
        buildMetaClass_IGeometryCreator(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


