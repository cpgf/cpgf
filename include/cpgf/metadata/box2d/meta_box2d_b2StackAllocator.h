// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2STACKALLOCATOR_H
#define __META_BOX2D_B2STACKALLOCATOR_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2stackallocator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("b2_stackSize", &b2_stackSize);
    _d.CPGF_MD_TEMPLATE _field("b2_maxStackEntries", &b2_maxStackEntries);
}


template <typename D>
void buildMetaClass_B2StackAllocator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("Allocate", &D::ClassType::Allocate);
    _d.CPGF_MD_TEMPLATE _method("Free", &D::ClassType::Free);
    _d.CPGF_MD_TEMPLATE _method("GetMaxAllocation", &D::ClassType::GetMaxAllocation);
}


template <typename D>
void buildMetaClass_B2StackEntry(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("data", &D::ClassType::data);
    _d.CPGF_MD_TEMPLATE _field("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _field("usedMalloc", &D::ClassType::usedMalloc);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
