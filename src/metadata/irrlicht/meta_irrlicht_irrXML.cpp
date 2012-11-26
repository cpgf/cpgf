// Auto generated file, don't modify.

#include "irrlicht.h"
#include "irrXML.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_irrXML.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_irrxml()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_irrxml(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IFileReadCallBack()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IFileReadCallBack> _nd = GDefineMetaClass<irr::io::IFileReadCallBack>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IFileReadCallBack");
        buildMetaClass_IFileReadCallBack(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IXMLBase()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IXMLBase> _nd = GDefineMetaClass<irr::io::IXMLBase>::declare("IXMLBase");
        buildMetaClass_IXMLBase(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


