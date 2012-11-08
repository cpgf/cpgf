// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneUserDataSerializer.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneUserDataSerializer.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_ISceneUserDataSerializer()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneUserDataSerializer> _nd = GDefineMetaClass<irr::scene::ISceneUserDataSerializer>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneUserDataSerializer");
        buildMetaClass_ISceneUserDataSerializer(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


