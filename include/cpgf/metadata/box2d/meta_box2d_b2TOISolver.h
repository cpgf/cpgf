// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2TOISOLVER_H
#define __META_BOX2D_B2TOISOLVER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2TOISolver(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (b2StackAllocator *)>();
    _d.CPGF_MD_TEMPLATE _method("Initialize", &D::ClassType::Initialize);
    _d.CPGF_MD_TEMPLATE _method("Clear", &D::ClassType::Clear);
    _d.CPGF_MD_TEMPLATE _method("Solve", &D::ClassType::Solve);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
