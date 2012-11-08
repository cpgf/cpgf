// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IParticleSystemSceneNode.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IParticleSystemSceneNode.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IParticleSystemSceneNode()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IParticleSystemSceneNode, irr::scene::ISceneNode> _nd = GDefineMetaClass<irr::scene::IParticleSystemSceneNode, irr::scene::ISceneNode>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IParticleSystemSceneNode");
        buildMetaClass_IParticleSystemSceneNode(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


