// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISkinnedMesh.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISkinnedMesh.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iskinnedmesh()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iskinnedmesh(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_ISkinnedMesh()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISkinnedMesh, irr::scene::IAnimatedMesh> _nd = GDefineMetaClass<irr::scene::ISkinnedMesh, irr::scene::IAnimatedMesh>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISkinnedMesh");
        buildMetaClass_ISkinnedMesh(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


