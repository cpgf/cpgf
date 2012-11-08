// Auto generated file, don't modify.

#include "irrlicht.h"
#include "IAttributeExchangingObject.h"

#include "cpgf/metadata/irrlicht/meta_irrlicht_IAttributeExchangingObject.h"

using namespace cpgf;

namespace meta_irrlicht { 


GDefineMetaInfo createMetaClass_Global_iattributeexchangingobject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    buildMetaClass_Global_iattributeexchangingobject(0, _d);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_IAttributeExchangingObject()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::IAttributeExchangingObject, irr::IReferenceCounted> _nd = GDefineMetaClass<irr::io::IAttributeExchangingObject, irr::IReferenceCounted>::declare("IAttributeExchangingObject");
        buildMetaClass_IAttributeExchangingObject(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SAttributeReadWriteOptions()
{
    GDefineMetaGlobalDangle _d = GDefineMetaGlobalDangle::dangle();
    {
        GDefineMetaClass<irr::io::SAttributeReadWriteOptions> _nd = GDefineMetaClass<irr::io::SAttributeReadWriteOptions>::declare("SAttributeReadWriteOptions");
        buildMetaClass_SAttributeReadWriteOptions(0, _nd);
        _d._class(_nd);
    }
    return _d.getMetaInfo();
}


} // namespace meta_irrlicht


