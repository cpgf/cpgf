// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2BLOCKALLOCATOR_H
#define __META_BOX2D_B2BLOCKALLOCATOR_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_Global_b2blockallocator(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_chunkSize", _r), &b2_chunkSize);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_maxBlockSize", _r), &b2_maxBlockSize);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_blockSizes", _r), &b2_blockSizes);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_chunkArrayIncrement", _r), &b2_chunkArrayIncrement);
}


template <typename D>
void buildMetaClass_B2BlockAllocator(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method(replaceName("Allocate", _r), &D::ClassType::Allocate);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Free", _r), &D::ClassType::Free);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clear", _r), &D::ClassType::Clear);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
