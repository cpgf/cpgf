/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppSourceFile
@@cpgf@creations@*/

#ifndef METADATA_META_CPPSOURCEFILE_H
#define METADATA_META_CPPSOURCEFILE_H

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include "clang/AST/Decl.h"
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include "model/cppsourcefile.h"
#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/metatraits/gmetaconverter_widestring.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gselectfunctionbyarity.h"
#include "cpgf/metadata/private/gmetadata_header.h"

namespace metadata {

template <typename D_d >
void buildMetaClass_metagen_CppSourceFile(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _constructor<void * (const std::basic_string<char, std::char_traits<char>, std::allocator<char> > &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _method("getFileName", &D_d::ClassType::getFileName);
    _d.CPGF_MD_TEMPLATE _method("getBaseFileName", &D_d::ClassType::getBaseFileName);
    _d.CPGF_MD_TEMPLATE _method("setSkipBind", &D_d::ClassType::setSkipBind);
    _d.CPGF_MD_TEMPLATE _method("shouldSkipBind", &D_d::ClassType::shouldSkipBind);
    _d.CPGF_MD_TEMPLATE _method("addInclude", &D_d::ClassType::addInclude);
    _d.CPGF_MD_TEMPLATE _method("getIncludeList", &D_d::ClassType::getIncludeList), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();
    _d.CPGF_MD_TEMPLATE _method("addMetaInclude", &D_d::ClassType::addMetaInclude);
    _d.CPGF_MD_TEMPLATE _method("getMetaIncludeList", &D_d::ClassType::getMetaIncludeList), cpgf::MakePolicy<GMetaRuleCopyConstReference<-1> >();

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppSourceFile();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
