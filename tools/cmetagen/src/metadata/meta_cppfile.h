/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_CppFile
@@cpgf@creations@*/

#ifndef METADATA_META_CPPFILE_H
#define METADATA_META_CPPFILE_H

#include "cpgf/gmetadefine.h"
#include "cpgf/gmetapolicy.h"
#include "cpgf/gscopedinterface.h"
#include "cpgf/gselectFunctionByArity.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/scriptbind/gscriptbindutil.h"
#include "cpgf/scriptbind/gscriptwrapper.h"
#include "model/cppfile.h"

namespace metadata {

template <typename TsE1f>
metagen::CppFile &oPeRat0rWrapPer_metagen_CppFile_opAssign_0(TsE1f * sE1F, const metagen::CppFile &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_CppFile(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::CppContainer >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const clang::Decl *)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppFile &)>(cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());

    _d.CPGF_MD_TEMPLATE _operator<metagen::CppFile &(*)(cpgf::GMetaSelf, const metagen::CppFile &)>(mopHolder = mopHolder, cpgf::MakePolicy<GMetaRuleCopyConstReference<0> >());
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_CppFile_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_CppFile();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
