// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IVideoDriver.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IVideoDriver.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_ivideodriver()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_ivideodriver(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IRenderTarget()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::IRenderTarget> _nd = GDefineMetaClass<irr::video::IRenderTarget>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("IRenderTarget");
        buildMetaClass_IRenderTarget(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SOverrideMaterial()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::SOverrideMaterial> _nd = GDefineMetaClass<irr::video::SOverrideMaterial>::declare("SOverrideMaterial");
        buildMetaClass_SOverrideMaterial(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


