// Auto generated file, don't modify.

#include "irrlicht.h"
#include "CVertexBuffer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_CVertexBuffer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_CVertexBuffer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::CVertexBuffer, irr::scene::IVertexBuffer> _nd = GDefineMetaClass<irr::scene::CVertexBuffer, irr::scene::IVertexBuffer>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("CVertexBuffer");
        buildMetaClass_CVertexBuffer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


