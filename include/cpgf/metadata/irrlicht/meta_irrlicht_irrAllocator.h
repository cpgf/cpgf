// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRRALLOCATOR_H
#define __META_IRRLICHT_IRRALLOCATOR_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_irrallocator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _enum<eAllocStrategy>("eAllocStrategy")
        ._element("ALLOC_STRATEGY_SAFE", irr::core::ALLOC_STRATEGY_SAFE)
        ._element("ALLOC_STRATEGY_DOUBLE", irr::core::ALLOC_STRATEGY_DOUBLE)
        ._element("ALLOC_STRATEGY_SQRT", irr::core::ALLOC_STRATEGY_SQRT)
    ;
}


template <typename D, typename T>
void buildMetaClass_IrrAllocator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("allocate", &D::ClassType::allocate);
    _d.CPGF_MD_TEMPLATE _method("deallocate", &D::ClassType::deallocate);
    _d.CPGF_MD_TEMPLATE _method("construct", &D::ClassType::construct);
    _d.CPGF_MD_TEMPLATE _method("destruct", &D::ClassType::destruct);
}


template <typename D, typename T>
void buildMetaClass_IrrAllocatorFast(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("allocate", &D::ClassType::allocate);
    _d.CPGF_MD_TEMPLATE _method("deallocate", &D::ClassType::deallocate);
    _d.CPGF_MD_TEMPLATE _method("construct", &D::ClassType::construct);
    _d.CPGF_MD_TEMPLATE _method("destruct", &D::ClassType::destruct);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
