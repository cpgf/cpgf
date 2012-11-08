// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IMesh.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IMesh.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IMesh()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IMesh, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IMesh, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IMesh");
        buildMetaClass_IMesh(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


