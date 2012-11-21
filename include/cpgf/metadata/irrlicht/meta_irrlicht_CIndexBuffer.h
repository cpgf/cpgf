// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_CINDEXBUFFER_H
#define __META_IRRLICHT_CINDEXBUFFER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::scene;


namespace meta_irrlicht { 


inline u32 opErAToRWrapper_CIndexBuffer__opArrayGet(const CIndexBuffer * self, u32 index) {
    return (*self)[index];
}


template <typename D>
void buildMetaClass_CIndexBuffer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * (video::E_INDEX_TYPE)>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const IIndexBuffer &)>();
    _d.CPGF_MD_TEMPLATE _field("Indices", &D::ClassType::Indices);
    _d.CPGF_MD_TEMPLATE _field("MappingHint", &D::ClassType::MappingHint);
    _d.CPGF_MD_TEMPLATE _field("ChangedID", &D::ClassType::ChangedID);
    _d.CPGF_MD_TEMPLATE _method("setType", &D::ClassType::setType);
    _d.CPGF_MD_TEMPLATE _method("getData", &D::ClassType::getData);
    _d.CPGF_MD_TEMPLATE _method("getType", &D::ClassType::getType);
    _d.CPGF_MD_TEMPLATE _method("stride", &D::ClassType::stride);
    _d.CPGF_MD_TEMPLATE _method("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _method("push_back", &D::ClassType::push_back);
    _d.CPGF_MD_TEMPLATE _method("getLast", &D::ClassType::getLast);
    _d.CPGF_MD_TEMPLATE _method("setValue", &D::ClassType::setValue);
    _d.CPGF_MD_TEMPLATE _method("set_used", &D::ClassType::set_used);
    _d.CPGF_MD_TEMPLATE _method("reallocate", &D::ClassType::reallocate);
    _d.CPGF_MD_TEMPLATE _method("allocated_size", &D::ClassType::allocated_size);
    _d.CPGF_MD_TEMPLATE _method("pointer", &D::ClassType::pointer);
    _d.CPGF_MD_TEMPLATE _method("getHardwareMappingHint", &D::ClassType::getHardwareMappingHint);
    _d.CPGF_MD_TEMPLATE _method("setHardwareMappingHint", &D::ClassType::setHardwareMappingHint);
    _d.CPGF_MD_TEMPLATE _method("setDirty", &D::ClassType::setDirty);
    _d.CPGF_MD_TEMPLATE _method("getChangedID", &D::ClassType::getChangedID);
    _d.CPGF_MD_TEMPLATE _operator<u32 (*)(const cpgf::GMetaSelf &, u32)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _method("_opArrayGet", (u32 (*) (const CIndexBuffer *, u32))&opErAToRWrapper_CIndexBuffer__opArrayGet, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
