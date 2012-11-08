// Auto generated file, don't modify.

#include "irrlicht.h"
#include "CDynamicMeshBuffer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_CDynamicMeshBuffer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_CDynamicMeshBuffer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::CDynamicMeshBuffer, irr::scene::IDynamicMeshBuffer> _nd = GDefineMetaClass<irr::scene::CDynamicMeshBuffer, irr::scene::IDynamicMeshBuffer>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("CDynamicMeshBuffer");
        buildMetaClass_CDynamicMeshBuffer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


