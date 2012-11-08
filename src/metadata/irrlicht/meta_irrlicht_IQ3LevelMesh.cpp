// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IQ3LevelMesh.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IQ3LevelMesh.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IQ3LevelMesh()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IQ3LevelMesh, irr::scene::IAnimatedMesh> _nd = GDefineMetaClass<irr::scene::IQ3LevelMesh, irr::scene::IAnimatedMesh>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IQ3LevelMesh");
        buildMetaClass_IQ3LevelMesh(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


