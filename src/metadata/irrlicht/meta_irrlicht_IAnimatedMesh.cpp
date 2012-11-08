// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IAnimatedMesh.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IAnimatedMesh.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ianimatedmesh()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ianimatedmesh(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IAnimatedMesh()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IAnimatedMesh, irr::scene::IMesh> _nd = GDefineMetaClass<irr::scene::IAnimatedMesh, irr::scene::IMesh>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IAnimatedMesh");
        buildMetaClass_IAnimatedMesh(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


