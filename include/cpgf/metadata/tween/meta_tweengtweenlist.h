// Auto generated file, don't modify.

#ifndef CPGF_META_TWEENGTWEENLIST_H
#define CPGF_META_TWEENGTWEENLIST_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace cpgf;


namespace meta_tween { 


template <typename D>
void buildMetaClass_GTweenList(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("getInstance", &D::ClassType::getInstance);
    _d.CPGF_MD_TEMPLATE _method("tween", &D::ClassType::tween);
    _d.CPGF_MD_TEMPLATE _method("timeline", &D::ClassType::timeline);
    _d.CPGF_MD_TEMPLATE _method("getTweenableCount", &D::ClassType::getTweenableCount);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("remove", &D::ClassType::remove);
    _d.CPGF_MD_TEMPLATE _method("getDuration", &D::ClassType::getDuration);
    _d.CPGF_MD_TEMPLATE _method("removeForInstance", &D::ClassType::removeForInstance);
}


} // namespace meta_tween




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
