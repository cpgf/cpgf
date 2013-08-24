/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderContext
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERCONTEXT_H
#define METADATA_META_BUILDERCONTEXT_H

#include "model/cppcontext.h"
#include "buildermodel/builderwriter.h"
#include "model/cppsourcefile.h"
#include "project.h"
#include "buildermodel/buildercontext.h"
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
void buildMetaClass_metagen_BuilderContext(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::Project *, const metagen::CppSourceFile &, bool)>();

    _d.CPGF_MD_TEMPLATE _method("process", &D_d::ClassType::process);
    _d.CPGF_MD_TEMPLATE _method("getProject", &D_d::ClassType::getProject);
    _d.CPGF_MD_TEMPLATE _method("shouldOverwriteEvenIfNoChange", &D_d::ClassType::shouldOverwriteEvenIfNoChange);
    _d.CPGF_MD_TEMPLATE _method("getSourceFileName", &D_d::ClassType::getSourceFileName);
    _d.CPGF_MD_TEMPLATE _method("getSourceBaseFileName", &D_d::ClassType::getSourceBaseFileName);
    _d.CPGF_MD_TEMPLATE _method("getItemList", &D_d::ClassType::getItemList);
    _d.CPGF_MD_TEMPLATE _method("getSectionList", &D_d::ClassType::getSectionList);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderContext();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
