// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_OPERATOR_BITWISE_ASSIGN_H
#define CPGF_META_TEST_METAGEN_OPERATOR_BITWISE_ASSIGN_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitAndAssign(MetagenOperatorBitwiseAssign * self, int n) {
    return (*self) &= n;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitAndAssign(MetagenOperatorBitwiseAssign * self, const MetagenOperatorBitwiseAssign & other) {
    return (*self) &= other;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitAndAssign(MetagenOperatorBitwiseAssign * self, const std::string & s) {
    return (*self) &= s;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitOrAssign(MetagenOperatorBitwiseAssign * self, int n) {
    return (*self) |= n;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitOrAssign(MetagenOperatorBitwiseAssign * self, const MetagenOperatorBitwiseAssign & other) {
    return (*self) |= other;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitOrAssign(MetagenOperatorBitwiseAssign * self, const std::string & s) {
    return (*self) |= s;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitXorAssign(MetagenOperatorBitwiseAssign * self, int n) {
    return (*self) ^= n;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitXorAssign(MetagenOperatorBitwiseAssign * self, const MetagenOperatorBitwiseAssign & other) {
    return (*self) ^= other;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitXorAssign(MetagenOperatorBitwiseAssign * self, const std::string & s) {
    return (*self) ^= s;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opLeftShiftAssign(MetagenOperatorBitwiseAssign * self, int n) {
    return (*self) <<= n;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opLeftShiftAssign(MetagenOperatorBitwiseAssign * self, const MetagenOperatorBitwiseAssign & other) {
    return (*self) <<= other;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opLeftShiftAssign(MetagenOperatorBitwiseAssign * self, const std::string & s) {
    return (*self) <<= s;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opRightShiftAssign(MetagenOperatorBitwiseAssign * self, int n) {
    return (*self) >>= n;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opRightShiftAssign(MetagenOperatorBitwiseAssign * self, const MetagenOperatorBitwiseAssign & other) {
    return (*self) >>= other;
}
inline MetagenOperatorBitwiseAssign & opErAToRWrapper_MetagenOperatorBitwiseAssign__opRightShiftAssign(MetagenOperatorBitwiseAssign * self, const std::string & s) {
    return (*self) >>= s;
}


template <typename D>
void buildMetaClass_MetagenOperatorBitwiseAssign(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _field("value", &D::ClassType::value);
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder &= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opBitAndAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, int))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitAndAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorBitwiseAssign &)>(mopHolder &= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitAndAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const MetagenOperatorBitwiseAssign &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitAndAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder &= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitAndAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitAndAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder |= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opBitOrAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, int))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitOrAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorBitwiseAssign &)>(mopHolder |= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitOrAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const MetagenOperatorBitwiseAssign &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitOrAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder |= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitOrAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitOrAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder ^= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opBitXorAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, int))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitXorAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorBitwiseAssign &)>(mopHolder ^= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitXorAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const MetagenOperatorBitwiseAssign &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitXorAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder ^= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opBitXorAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opBitXorAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder <<= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opLeftShiftAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, int))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opLeftShiftAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorBitwiseAssign &)>(mopHolder <<= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLeftShiftAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const MetagenOperatorBitwiseAssign &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opLeftShiftAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder <<= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opLeftShiftAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opLeftShiftAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, int)>(mopHolder >>= mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opRightShiftAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, int))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opRightShiftAssign, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const MetagenOperatorBitwiseAssign &)>(mopHolder >>= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opRightShiftAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const MetagenOperatorBitwiseAssign &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opRightShiftAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
    _d.CPGF_MD_TEMPLATE _operator<MetagenOperatorBitwiseAssign & (*)(cpgf::GMetaSelf, const std::string &)>(mopHolder >>= mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _method("_opRightShiftAssign", (MetagenOperatorBitwiseAssign & (*) (MetagenOperatorBitwiseAssign *, const std::string &))&opErAToRWrapper_MetagenOperatorBitwiseAssign__opRightShiftAssign, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1>, cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
