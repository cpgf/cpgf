// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRRSTRING_H
#define __META_IRRLICHT_IRRSTRING_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_Global_irrstring(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("locale_current", &locale_current);
    _d.CPGF_MD_TEMPLATE _method("locale_set", (void (*) (eLocaleID))&locale_set);
    _d.CPGF_MD_TEMPLATE _method("locale_lower", (u32 (*) (u32))&locale_lower);
    _d.CPGF_MD_TEMPLATE _method("locale_upper", (u32 (*) (u32))&locale_upper);
    _d.CPGF_MD_TEMPLATE _enum<eLocaleID>("eLocaleID")
        ._element("IRR_LOCALE_ANSI", irr::core::IRR_LOCALE_ANSI)
        ._element("IRR_LOCALE_GERMAN", irr::core::IRR_LOCALE_GERMAN)
    ;
}


template <typename D, typename T, typename TAlloc>
void buildMetaClass_String(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const string< T, TAlloc > &)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _constructor<void * (const double)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (int)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (unsigned int)>();
    _d.CPGF_MD_TEMPLATE _method("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _method("c_str", &D::ClassType::c_str);
    _d.CPGF_MD_TEMPLATE _method("make_lower", &D::ClassType::make_lower);
    _d.CPGF_MD_TEMPLATE _method("make_upper", &D::ClassType::make_upper);
    _d.CPGF_MD_TEMPLATE _method("equals_ignore_case", &D::ClassType::equals_ignore_case, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("equals_substring_ignore_case", &D::ClassType::equals_substring_ignore_case, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(0))
    ;
    _d.CPGF_MD_TEMPLATE _method("lower_ignore_case", &D::ClassType::lower_ignore_case, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("equalsn", (bool (D::ClassType::*) (const string< T, TAlloc > &, u32) const)&D::ClassType::equalsn, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("equalsn", (bool (D::ClassType::*) (const T *const, u32) const)&D::ClassType::equalsn);
    _d.CPGF_MD_TEMPLATE _method("append", (void (D::ClassType::*) (T))&D::ClassType::append);
    _d.CPGF_MD_TEMPLATE _method("append", (void (D::ClassType::*) (const T *const))&D::ClassType::append);
    _d.CPGF_MD_TEMPLATE _method("append", (void (D::ClassType::*) (const string< T, TAlloc > &))&D::ClassType::append, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("append", (void (D::ClassType::*) (const string< T, TAlloc > &, u32))&D::ClassType::append, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("reserve", &D::ClassType::reserve);
    _d.CPGF_MD_TEMPLATE _method("findFirst", &D::ClassType::findFirst);
    _d.CPGF_MD_TEMPLATE _method("findFirstChar", &D::ClassType::findFirstChar);
    _d.CPGF_MD_TEMPLATE _method("findNext", &D::ClassType::findNext);
    _d.CPGF_MD_TEMPLATE _method("findLast", &D::ClassType::findLast)
        ._default(copyVariantFromCopyable(-1))
    ;
    _d.CPGF_MD_TEMPLATE _method("findLastChar", &D::ClassType::findLastChar);
    _d.CPGF_MD_TEMPLATE _method("subString", &D::ClassType::subString);
    _d.CPGF_MD_TEMPLATE _method("replace", &D::ClassType::replace);
    _d.CPGF_MD_TEMPLATE _method("remove", (void (D::ClassType::*) (T))&D::ClassType::remove);
    _d.CPGF_MD_TEMPLATE _method("remove", (void (D::ClassType::*) (const string< T, TAlloc >))&D::ClassType::remove);
    _d.CPGF_MD_TEMPLATE _method("removeChars", &D::ClassType::removeChars, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("trim", &D::ClassType::trim, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >())
        ._default(copyVariantFromCopyable(" \t\n\r"))
    ;
    _d.CPGF_MD_TEMPLATE _method("erase", &D::ClassType::erase);
    _d.CPGF_MD_TEMPLATE _method("validate", &D::ClassType::validate);
    _d.CPGF_MD_TEMPLATE _method("lastChar", &D::ClassType::lastChar);
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const string< T, TAlloc > &)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > (*)(const cpgf::GMetaSelf &, const string< T, TAlloc > &)>(mopHolder + mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<T & (*)(cpgf::GMetaSelf, const u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _operator<const T & (*)(const cpgf::GMetaSelf &, const u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const T *const)>(mopHolder == mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const string< T, TAlloc > &)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const string< T, TAlloc > &)>(mopHolder < mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const T *const)>(mopHolder != mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const string< T, TAlloc > &)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, T)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const T *const)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const string< T, TAlloc > &)>(mopHolder += mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const int)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const unsigned int)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const long)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const unsigned long &)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const double)>(mopHolder += mopHolder);
    _d.CPGF_MD_TEMPLATE _operator<string< T, TAlloc > & (*)(cpgf::GMetaSelf, const float)>(mopHolder += mopHolder);
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
