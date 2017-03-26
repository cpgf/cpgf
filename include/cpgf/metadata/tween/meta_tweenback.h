// Auto generated file, don't modify.

#ifndef CPGF_META_TWEENBACK_H
#define CPGF_META_TWEENBACK_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace cpgf;


namespace meta_tween { 


template <typename D>
void buildMetaClass_BackEase(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("easeIn", &D::ClassType::easeIn)
        ._default(copyVariantFromCopyable<GTweenNumber>(1.70158f))
    ;
    _d.CPGF_MD_TEMPLATE _method("easeOut", &D::ClassType::easeOut)
        ._default(copyVariantFromCopyable<GTweenNumber>(1.70158f))
    ;
    _d.CPGF_MD_TEMPLATE _method("easeInOut", &D::ClassType::easeInOut)
        ._default(copyVariantFromCopyable<GTweenNumber>(1.70158f))
    ;
}


} // namespace meta_tween




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
