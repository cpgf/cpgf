// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IImageWriter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IImageWriter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IImageWriter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::video::IImageWriter, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::video::IImageWriter, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IImageWriter");
        buildMetaClass_IImageWriter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


