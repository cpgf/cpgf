// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IVertexBuffer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IVertexBuffer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IVertexBuffer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IVertexBuffer, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IVertexBuffer, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IVertexBuffer");
        buildMetaClass_IVertexBuffer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


