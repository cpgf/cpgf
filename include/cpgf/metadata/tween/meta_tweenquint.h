// Auto generated file, don't modify.

#ifndef CPGF_META_TWEENQUINT_H
#define CPGF_META_TWEENQUINT_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace cpgf;


namespace meta_tween { 


template <typename D>
void buildMetaClass_QuintEase(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("easeIn", &D::ClassType::easeIn);
    _d.CPGF_MD_TEMPLATE _method("easeOut", &D::ClassType::easeOut);
    _d.CPGF_MD_TEMPLATE _method("easeInOut", &D::ClassType::easeInOut);
}


} // namespace meta_tween




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
