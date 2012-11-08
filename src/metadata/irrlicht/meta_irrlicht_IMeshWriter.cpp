// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IMeshWriter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IMeshWriter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IMeshWriter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::scene::IMeshWriter, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::scene::IMeshWriter, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IMeshWriter");
        buildMetaClass_IMeshWriter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


