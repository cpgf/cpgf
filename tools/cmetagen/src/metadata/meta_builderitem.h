/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderItem
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERITEM_H
#define METADATA_META_BUILDERITEM_H

#include "model/cppitem.h"
#include "buildermodel/builderwriter.h"
#include "project.h"
#include "codewriter/codeblock.h"
#include "buildermodel/buildercontainer.h"
#include "buildermodel/builderitem.h"
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
metagen::BuilderItem &oPeRat0rWrapPer_metagen_BuilderItem_opAssign_0(TsE1f * sE1F, const metagen::BuilderItem &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_BuilderItem(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    

    _d.CPGF_MD_TEMPLATE _method("getCppItem", &D_d::ClassType::getCppItem);
    _d.CPGF_MD_TEMPLATE _method("canBind", &D_d::ClassType::canBind);
    _d.CPGF_MD_TEMPLATE _method("writeMetaData", &D_d::ClassType::writeMetaData);
    _d.CPGF_MD_TEMPLATE _method("setProject", &D_d::ClassType::setProject);
    _d.CPGF_MD_TEMPLATE _method("getProject", &D_d::ClassType::getProject);
    _d.CPGF_MD_TEMPLATE _method("getParent", &D_d::ClassType::getParent);
    _d.CPGF_MD_TEMPLATE _method("setSkipBind", &D_d::ClassType::setSkipBind);
    _d.CPGF_MD_TEMPLATE _method("shouldSkipBind", &D_d::ClassType::shouldSkipBind);
    _d.CPGF_MD_TEMPLATE _method("setWrapClass", &D_d::ClassType::setWrapClass);
    _d.CPGF_MD_TEMPLATE _method("shouldWrapClass", &D_d::ClassType::shouldWrapClass);

    _d.CPGF_MD_TEMPLATE _operator<metagen::BuilderItem &(*)(cpgf::GMetaSelf, const metagen::BuilderItem &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_BuilderItem_opAssign_0<typename D_d::ClassType >);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderItem();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
