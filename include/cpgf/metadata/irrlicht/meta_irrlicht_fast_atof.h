// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_FAST_ATOF_H
#define __META_IRRLICHT_FAST_ATOF_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_fast_atof(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("fast_atof_table", &fast_atof_table);
    _d.CPGF_MD_TEMPLATE _method("fast_atof", (float (*) (const char *))&fast_atof);
    _d.CPGF_MD_TEMPLATE _method("fast_atof_move", (const char * (*) (const char *, f32 &))&fast_atof_move);
    _d.CPGF_MD_TEMPLATE _method("strtof10", (f32 (*) (const char *, const char **))&strtof10)
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("strtol10", (s32 (*) (const char *, const char **))&strtol10)
        ._default(copyVariantFromCopyable(0))
    ;
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
