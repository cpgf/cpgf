// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IImageLoader.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IImageLoader.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IImageLoader()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::IImageLoader, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::video::IImageLoader, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IImageLoader");
        buildMetaClass_IImageLoader(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


