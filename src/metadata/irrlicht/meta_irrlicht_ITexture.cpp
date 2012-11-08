// Auto generated file, don't modify.

#include "irrlicht.h"
#include "ITexture.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_ITexture.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_itexture()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_itexture(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_ITexture()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::ITexture, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::video::ITexture, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("ITexture");
        buildMetaClass_ITexture(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


