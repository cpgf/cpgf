/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderInvokable
createMetaClass_metagen_namespace_builderinvokable
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERINVOKABLE_H
#define METADATA_META_BUILDERINVOKABLE_H

#include "model/cppinvokable.h"
#include "buildermodel/builderwriter.h"
#include "buildermodel/builderinvokable.h"
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
metagen::BuilderInvokable &oPeRat0rWrapPer_metagen_BuilderInvokable_opAssign_0(TsE1f * sE1F, const metagen::BuilderInvokable &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_BuilderInvokable(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::BuilderItem >();

    _d.CPGF_MD_TEMPLATE _method("doCanBind", &D_d::ClassType::doCanBind);

    _d.CPGF_MD_TEMPLATE _operator<metagen::BuilderInvokable &(*)(cpgf::GMetaSelf, const metagen::BuilderInvokable &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_BuilderInvokable_opAssign_0<typename D_d::ClassType >);

}

template <typename D_d >
void buildMetaClass_metagen_namespace_builderinvokable(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _method("getInvokablePolicyText", &metagen::getInvokablePolicyText);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderInvokable();
cpgf::GDefineMetaInfo createMetaClass_metagen_namespace_builderinvokable();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
