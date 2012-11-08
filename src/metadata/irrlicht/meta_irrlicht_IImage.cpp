// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IImage.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IImage.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iimage()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iimage(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IImage()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::IImage, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::video::IImage, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IImage");
        buildMetaClass_IImage(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


