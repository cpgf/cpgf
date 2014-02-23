/*@@cpgf@@*/
// Auto generated file, don't modify.

/*@cpgf@creations@@
createMetaClass_metagen_BuilderMethod
createMetaClass_metagen_namespace_buildermethod
@@cpgf@creations@*/

#ifndef METADATA_META_BUILDERMETHOD_H
#define METADATA_META_BUILDERMETHOD_H

#include "model/cppmethod.h"
#include "buildermodel/builderwriter.h"
#include "model/cppcontainer.h"
#include "buildermodel/buildermethod.h"
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
metagen::BuilderMethod &oPeRat0rWrapPer_metagen_BuilderMethod_opAssign_0(TsE1f * sE1F, const metagen::BuilderMethod &pAr9_Am0)
{
    return (*sE1F) = pAr9_Am0;
}

template <typename D_d >
void buildMetaClass_metagen_BuilderMethod(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _base<metagen::BuilderInvokable >();

    _d.CPGF_MD_TEMPLATE _constructor<void * (const metagen::CppItem *)>();

    _d.CPGF_MD_TEMPLATE _method("getCppMethod", &D_d::ClassType::getCppMethod);
    _d.CPGF_MD_TEMPLATE _method("canBind", &D_d::ClassType::canBind);

    _d.CPGF_MD_TEMPLATE _operator<metagen::BuilderMethod &(*)(cpgf::GMetaSelf, const metagen::BuilderMethod &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", &oPeRat0rWrapPer_metagen_BuilderMethod_opAssign_0<typename D_d::ClassType >);

}

template <typename D_d >
void buildMetaClass_metagen_namespace_buildermethod(D_d & _d)
{
    using namespace cpgf;
    
    (void)_d;
    
    _d.CPGF_MD_TEMPLATE _method("writeMethodClassWrapper", &metagen::writeMethodClassWrapper);

}

cpgf::GDefineMetaInfo createMetaClass_metagen_BuilderMethod();
cpgf::GDefineMetaInfo createMetaClass_metagen_namespace_buildermethod();

} // namespace metadata
#include "cpgf/metadata/private/gmetadata_footer.h"

#endif
