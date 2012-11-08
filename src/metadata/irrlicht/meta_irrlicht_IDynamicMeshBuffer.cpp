// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IDynamicMeshBuffer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IDynamicMeshBuffer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IDynamicMeshBuffer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IDynamicMeshBuffer, irr::scene::IMeshBuffer> _nd = GDefineMetaClass<irr::scene::IDynamicMeshBuffer, irr::scene::IMeshBuffer>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IDynamicMeshBuffer");
        buildMetaClass_IDynamicMeshBuffer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


