// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IIndexBuffer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IIndexBuffer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IIndexBuffer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IIndexBuffer, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IIndexBuffer, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IIndexBuffer");
        buildMetaClass_IIndexBuffer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


