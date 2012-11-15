// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IATTRIBUTEEXCHANGINGOBJECT_H
#define __META_IRRLICHT_IATTRIBUTEEXCHANGINGOBJECT_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::io;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_iattributeexchangingobject(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<E_ATTRIBUTE_READ_WRITE_FLAGS>("E_ATTRIBUTE_READ_WRITE_FLAGS")
        ._element("EARWF_FOR_FILE", irr::io::EARWF_FOR_FILE)
        ._element("EARWF_FOR_EDITOR", irr::io::EARWF_FOR_EDITOR)
        ._element("EARWF_USE_RELATIVE_PATHS", irr::io::EARWF_USE_RELATIVE_PATHS)
    ;
}


template <typename D>
void buildMetaClass_IAttributeExchangingObject(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("serializeAttributes", &D::ClassType::serializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("deserializeAttributes", &D::ClassType::deserializeAttributes)
        ._default(copyVariantFromCopyable(0))
    ;
}


template <typename D>
void buildMetaClass_SAttributeReadWriteOptions(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("Flags", &D::ClassType::Flags);
    _d.CPGF_MD_TEMPLATE _field("Filename", &D::ClassType::Filename);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
