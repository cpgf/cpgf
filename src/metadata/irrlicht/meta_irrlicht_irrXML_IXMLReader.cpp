// Auto generated file, don't modify.

#include "irrlicht.h"
#include "irrXML.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_irrXML.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IIrrXMLReader_TemplateInstance_IXMLReader()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<IIrrXMLReader<wchar_t, IReferenceCounted>, IReferenceCounted > _nd = GDefineMetaClass<IIrrXMLReader<wchar_t, IReferenceCounted>, IReferenceCounted >::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IXMLReader");
        buildMetaClass_IIrrXMLReader<GDefineMetaClass<IIrrXMLReader<wchar_t, IReferenceCounted>, IReferenceCounted >, wchar_t, IReferenceCounted >(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}
} // namespace meta_irrlicht


