/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderFile
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERFILE_H
#define METADATA_META_BUILDERFILE_H

#include "model/cppfile.h"
#include "buildermodel/builderwriter.h"
#include "project.h"
#include "buildermodel/builderfile.h"
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

template <typename TsE1f>
metagen::BuilderFile &oPeRat0rWrapPer_metagen_BuilderFile_opAssign_0(TsE1f * sE1F, const metagen::BuilderFile &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_BuilderFile(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::BuilderContainer >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppItem *)>();

    _d.CPGF_MD_TEMPLATE _method("getCppFile", &D_d::ClassType::getCppFile);

    _d.CPGF_MD_TEMPLATE _operator<metagen::BuilderFile &(*)(cpgf::GMetaSelf, const metagen::BuilderFile &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_BuilderFile_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderFile();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
