// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2BLOCKALLOCATOR_H
#define __META_BOX2D_B2BLOCKALLOCATOR_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2blockallocator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("b2_chunkSize", &b2_chunkSize);
    _d.CPGF_MD_TEMPLATE _field("b2_maxBlockSize", &b2_maxBlockSize);
    _d.CPGF_MD_TEMPLATE _field("b2_blockSizes", &b2_blockSizes);
    _d.CPGF_MD_TEMPLATE _field("b2_chunkArrayIncrement", &b2_chunkArrayIncrement);
}


template <typename D>
void buildMetaClass_B2BlockAllocator(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("Allocate", &D::ClassType::Allocate);
    _d.CPGF_MD_TEMPLATE _method("Free", &D::ClassType::Free);
    _d.CPGF_MD_TEMPLATE _method("Clear", &D::ClassType::Clear);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
