// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ISceneManager.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ISceneManager.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iscenemanager()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iscenemanager(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_ISceneManager()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::ISceneManager, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::ISceneManager, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ISceneManager");
        buildMetaClass_ISceneManager(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


