/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderDestructor
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERDESTRUCTOR_H
#define METADATA_META_BUILDERDESTRUCTOR_H

#include "model/cppdestructor.h"
#include "buildermodel/builderwriter.h"
#include "buildermodel/builderdestructor.h"
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
metagen::BuilderDestructor &oPeRat0rWrapPer_metagen_BuilderDestructor_opAssign_0(TsE1f * sE1F, const metagen::BuilderDestructor &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_BuilderDestructor(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::BuilderItem >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppItem *)>();

    _d.CPGF_MD_TEMPLATE _method("getCppDestructor", &D_d::ClassType::getCppDestructor);
    _d.CPGF_MD_TEMPLATE _method("doCanBind", &D_d::ClassType::doCanBind);

    _d.CPGF_MD_TEMPLATE _operator<metagen::BuilderDestructor &(*)(cpgf::GMetaSelf, const metagen::BuilderDestructor &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_BuilderDestructor_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderDestructor();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
