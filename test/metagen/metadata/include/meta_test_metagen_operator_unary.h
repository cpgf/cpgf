// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_OPERATOR_UNARY_H
#define CPGF_META_TEST_METAGEN_OPERATOR_UNARY_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


inline MetagenOperatorUnary opErAToRWrapper_MetagenOperatorUnary__opPlus(const MetagenOperatorUnary * self) {
    return +(*self);
}
inline MetagenOperatorUnary opErAToRWrapper_MetagenOperatorUnary__opMinus(const MetagenOperatorUnary * self) {
    return -(*self);
}
inline MetagenOperatorUnary opErAToRWrapper_MetagenOperatorUnary__opInc(MetagenOperatorUnary * self) {
    return ++(*self);
}
inline MetagenOperatorUnary opErAToRWrapper_MetagenOperatorUnary__opIncSuffix(MetagenOperatorUnary * self) {
    return (*self)++;
}
inline MetagenOperatorUnary opErAToRWrapper_MetagenOperatorUnary__opDec(MetagenOperatorUnary * self) {
    return --(*self);
}
inline MetagenOperatorUnary opErAToRWrapper_MetagenOperatorUnary__opDecSuffix(MetagenOperatorUnary * self) {
    return (*self)--;
}


template <typename D>
void buildMetaClass_MetagenOperatorUnary(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorUnary (*)(const cpgf::GMetaSelf &)>(+mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opPlus", (MetagenOperatorUnary (*) (const MetagenOperatorUnary *))&opErAToRWrapper_MetagenOperatorUnary__opPlus, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorUnary (*)(const cpgf::GMetaSelf &)>(-mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opMinus", (MetagenOperatorUnary (*) (const MetagenOperatorUnary *))&opErAToRWrapper_MetagenOperatorUnary__opMinus, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorUnary (*)(cpgf::GMetaSelf)>(++mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opInc", (MetagenOperatorUnary (*) (MetagenOperatorUnary *))&opErAToRWrapper_MetagenOperatorUnary__opInc, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorUnary (*)(cpgf::GMetaSelf)>(mopHolder++);
    _d.CPGF_MD_TEMPLATE _method("_opIncSuffix", (MetagenOperatorUnary (*) (MetagenOperatorUnary *))&opErAToRWrapper_MetagenOperatorUnary__opIncSuffix, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorUnary (*)(cpgf::GMetaSelf)>(--mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opDec", (MetagenOperatorUnary (*) (MetagenOperatorUnary *))&opErAToRWrapper_MetagenOperatorUnary__opDec, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorUnary (*)(cpgf::GMetaSelf)>(mopHolder--);
    _d.CPGF_MD_TEMPLATE _method("_opDecSuffix", (MetagenOperatorUnary (*) (MetagenOperatorUnary *))&opErAToRWrapper_MetagenOperatorUnary__opDecSuffix, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
