// Auto generated file, don't modify.

#ifndef __META_B2STACKALLOCATOR_H
#define __META_B2STACKALLOCATOR_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


template <typename D, typename Policy>
void buildMetaClass_Global_b2stackallocator(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_stackSize", _r), &b2_stackSize, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("b2_maxStackEntries", _r), &b2_maxStackEntries, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2StackAllocator(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Allocate", _r), &D::ClassType::Allocate, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Free", _r), &D::ClassType::Free, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("GetMaxAllocation", _r), &D::ClassType::GetMaxAllocation, _p);
}


template <typename D, typename Policy>
void buildMetaClass_B2StackEntry(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field(replaceName("data", _r), &D::ClassType::data, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("size", _r), &D::ClassType::size, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("usedMalloc", _r), &D::ClassType::usedMalloc, _p);
}


template <typename D, typename Policy>
void buildMetaFile_B2StackAllocator(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    buildMetaClass_Global_b2stackallocator(config, _d, _r, _p);
    buildMetaClass_B2StackAllocator(config, _d, _r, _p);
    buildMetaClass_B2StackEntry(config, _d, _r, _p);
}




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif