// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IXMLWriter.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IXMLWriter.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IXMLWriter()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IXMLWriter, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IXMLWriter, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IXMLWriter");
        buildMetaClass_IXMLWriter(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


