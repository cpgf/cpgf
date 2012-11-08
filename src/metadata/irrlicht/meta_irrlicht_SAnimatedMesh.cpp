// Auto generated file, don't modify.

#include "irrlicht.h"
#include "SAnimatedMesh.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_SAnimatedMesh.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_SAnimatedMesh()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::SAnimatedMesh, irr::scene::IAnimatedMesh> _nd = GDefineMetaClass<irr::scene::SAnimatedMesh, irr::scene::IAnimatedMesh>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SAnimatedMesh");
        buildMetaClass_SAnimatedMesh(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


