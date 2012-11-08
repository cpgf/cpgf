// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IAnimatedMeshMD2.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IAnimatedMeshMD2.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ianimatedmeshmd2()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ianimatedmeshmd2(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IAnimatedMeshMD2()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IAnimatedMeshMD2, irr::scene::IAnimatedMesh> _nd = GDefineMetaClass<irr::scene::IAnimatedMeshMD2, irr::scene::IAnimatedMesh>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IAnimatedMeshMD2");
        buildMetaClass_IAnimatedMeshMD2(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


