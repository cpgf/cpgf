// Auto generated file, don't modify.

#ifndef __META_B2BLOCKALLOCATOR_H
#define __META_B2BLOCKALLOCATOR_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_Global_b2blockallocator(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_chunkSize", _r), &b2_chunkSize, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_maxBlockSize", _r), &b2_maxBlockSize, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_blockSizes", _r), &b2_blockSizes, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_chunkArrayIncrement", _r), &b2_chunkArrayIncrement, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2BlockAllocator(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Allocate", _r), &D::ClassType::Allocate, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Free", _r), &D::ClassType::Free, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Clear", _r), &D::ClassType::Clear, _p);
}


template <typename D, typename Policy>
void buildMetaFile_B2BlockAllocator(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    buildMetaClass_Global_b2blockallocator(config, _d, _r, _p);
    buildMetaClass_B2BlockAllocator(config, _d, _r, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif