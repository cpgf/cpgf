// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_OPERATOR_BITWISE_H
#define CPGF_META_TEST_METAGEN_OPERATOR_BITWISE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitAnd(const MetagenOperatorBitwise * self, int n) {
    return (*self) & n;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitAnd(const MetagenOperatorBitwise * self, const MetagenOperatorBitwise & other) {
    return (*self) & other;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitAnd(const MetagenOperatorBitwise * self, const std::string & s) {
    return (*self) & s;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitOr(MetagenOperatorBitwise * self, int n) {
    return (*self) | n;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitOr(MetagenOperatorBitwise * self, const MetagenOperatorBitwise & other) {
    return (*self) | other;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitOr(MetagenOperatorBitwise * self, const std::string & s) {
    return (*self) | s;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitXor(MetagenOperatorBitwise * self, int n) {
    return (*self) ^ n;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitXor(MetagenOperatorBitwise * self, const MetagenOperatorBitwise & other) {
    return (*self) ^ other;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitXor(MetagenOperatorBitwise * self, const std::string & s) {
    return (*self) ^ s;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opLeftShift(MetagenOperatorBitwise * self, int n) {
    return (*self) << n;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opLeftShift(MetagenOperatorBitwise * self, const MetagenOperatorBitwise & other) {
    return (*self) << other;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opLeftShift(MetagenOperatorBitwise * self, const std::string & s) {
    return (*self) << s;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opRightShift(MetagenOperatorBitwise * self, int n) {
    return (*self) >> n;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opRightShift(MetagenOperatorBitwise * self, const MetagenOperatorBitwise & other) {
    return (*self) >> other;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opRightShift(MetagenOperatorBitwise * self, const std::string & s) {
    return (*self) >> s;
}
inline MetagenOperatorBitwise opErAToRWrapper_MetagenOperatorBitwise__opBitNot(MetagenOperatorBitwise * self) {
    return ~(*self);
}


template <typename D>
void buildMetaClass_MetagenOperatorBitwise(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(const cpgf::GMetaSelf &, int)>(mopHolder & mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opBitAnd", (MetagenOperatorBitwise (*) (const MetagenOperatorBitwise *, int))&opErAToRWrapper_MetagenOperatorBitwise__opBitAnd, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(const cpgf::GMetaSelf &, const MetagenOperatorBitwise &)>(mopHolder & mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitAnd", (MetagenOperatorBitwise (*) (const MetagenOperatorBitwise *, const MetagenOperatorBitwise &))&opErAToRWrapper_MetagenOperatorBitwise__opBitAnd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(const cpgf::GMetaSelf &, const std::string &)>(mopHolder & mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitAnd", (MetagenOperatorBitwise (*) (const MetagenOperatorBitwise *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwise__opBitAnd, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, int)>(mopHolder | mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opBitOr", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, int))&opErAToRWrapper_MetagenOperatorBitwise__opBitOr, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, const MetagenOperatorBitwise &)>(mopHolder | mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitOr", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, const MetagenOperatorBitwise &))&opErAToRWrapper_MetagenOperatorBitwise__opBitOr, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder | mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitOr", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwise__opBitOr, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, int)>(mopHolder ^ mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opBitXor", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, int))&opErAToRWrapper_MetagenOperatorBitwise__opBitXor, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, const MetagenOperatorBitwise &)>(mopHolder ^ mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitXor", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, const MetagenOperatorBitwise &))&opErAToRWrapper_MetagenOperatorBitwise__opBitXor, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder ^ mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitXor", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwise__opBitXor, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, int)>(mopHolder << mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, int))&opErAToRWrapper_MetagenOperatorBitwise__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, const MetagenOperatorBitwise &)>(mopHolder << mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, const MetagenOperatorBitwise &))&opErAToRWrapper_MetagenOperatorBitwise__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder << mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLeftShift", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwise__opLeftShift, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, int)>(mopHolder >> mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, int))&opErAToRWrapper_MetagenOperatorBitwise__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, const MetagenOperatorBitwise &)>(mopHolder >> mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, const MetagenOperatorBitwise &))&opErAToRWrapper_MetagenOperatorBitwise__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder >> mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opRightShift", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwise__opRightShift, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwise (*)(cpgf::GMetaSelf)>(~mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opBitNot", (MetagenOperatorBitwise (*) (MetagenOperatorBitwise *))&opErAToRWrapper_MetagenOperatorBitwise__opBitNot, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
