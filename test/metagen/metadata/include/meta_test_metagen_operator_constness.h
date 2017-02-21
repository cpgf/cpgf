// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_OPERATOR_CONSTNESS_H
#define CPGF_META_TEST_METAGEN_OPERATOR_CONSTNESS_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


inline MetagenOperatorConstness opErAToRWrapper_MetagenOperatorConstness__opAdd(MetagenOperatorConstness * self, int n) {
    return (*self) + n;
}
inline MetagenOperatorConstness opErAToRWrapper_MetagenOperatorConstness__opSub(const MetagenOperatorConstness * self, int n) {
    return (*self) - n;
}


template <typename D>
void buildMetaClass_MetagenOperatorConstness(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
    _d.CPGF_MD_TEMPLATE _method("constSelf", &D::ClassType::constSelf);
    _d.CPGF_MD_TEMPLATE _method("constSelfRef", &D::ClassType::constSelfRef, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<-1> >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorConstness (*)(cpgf::GMetaSelf, int)>(mopHolder + mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAdd", (MetagenOperatorConstness (*) (MetagenOperatorConstness *, int))&opErAToRWrapper_MetagenOperatorConstness__opAdd, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorConstness (*)(const cpgf::GMetaSelf &, int)>(mopHolder - mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opSub", (MetagenOperatorConstness (*) (const MetagenOperatorConstness *, int))&opErAToRWrapper_MetagenOperatorConstness__opSub, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
