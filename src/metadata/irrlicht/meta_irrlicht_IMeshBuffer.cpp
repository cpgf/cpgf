// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IMeshBuffer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IMeshBuffer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IMeshBuffer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IMeshBuffer, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IMeshBuffer, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IMeshBuffer");
        buildMetaClass_IMeshBuffer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


