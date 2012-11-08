// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IFileList.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IFileList.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_IFileList()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IFileList, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IFileList, irr::IReferenceCounted>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent, GMetaRuleCopyConstructorAbsent> >::declare("IFileList");
        buildMetaClass_IFileList(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


