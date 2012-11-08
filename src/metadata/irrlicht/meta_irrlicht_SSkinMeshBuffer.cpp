// Auto generated file, don't modify.

#include "irrlicht.h"
#include "SSkinMeshBuffer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_SSkinMeshBuffer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_SSkinMeshBuffer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::SSkinMeshBuffer, irr::scene::IMeshBuffer> _nd = GDefineMetaClass<irr::scene::SSkinMeshBuffer, irr::scene::IMeshBuffer>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SSkinMeshBuffer");
        buildMetaClass_SSkinMeshBuffer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


