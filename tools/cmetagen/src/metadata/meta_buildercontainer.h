/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderContainer
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERCONTAINER_H
#define METADATA_META_BUILDERCONTAINER_H

#include "model/cppcontainer.h"
#include "buildermodel/builderwriter.h"
#include "buildermodel/buildercontainer.h"
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
metagen::BuilderContainer &oPeRat0rWrapPer_metagen_BuilderContainer_opAssign_0(TsE1f * sE1F, const metagen::BuilderContainer &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_BuilderContainer(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::BuilderItem >();

    _d.CPGF_MD_TEMPLATE _method("addItem", &D_d::ClassType::addItem);
    _d.CPGF_MD_TEMPLATE _method("getCppContainer", &D_d::ClassType::getCppContainer);

    _d.CPGF_MD_TEMPLATE _operator<metagen::BuilderContainer &(*)(cpgf::GMetaSelf, const metagen::BuilderContainer &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_BuilderContainer_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderContainer();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
