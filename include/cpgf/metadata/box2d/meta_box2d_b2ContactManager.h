// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CONTACTMANAGER_H
#define __META_BOX2D_B2CONTACTMANAGER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2ContactManager(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("m_broadPhase", &D::ClassType::m_broadPhase);
    _d.CPGF_MD_TEMPLATE _field("m_contactList", &D::ClassType::m_contactList);
    _d.CPGF_MD_TEMPLATE _field("m_contactCount", &D::ClassType::m_contactCount);
    _d.CPGF_MD_TEMPLATE _field("m_contactFilter", &D::ClassType::m_contactFilter);
    _d.CPGF_MD_TEMPLATE _field("m_contactListener", &D::ClassType::m_contactListener);
    _d.CPGF_MD_TEMPLATE _field("m_allocator", &D::ClassType::m_allocator);
    _d.CPGF_MD_TEMPLATE _method("AddPair", &D::ClassType::AddPair);
    _d.CPGF_MD_TEMPLATE _method("FindNewContacts", &D::ClassType::FindNewContacts);
    _d.CPGF_MD_TEMPLATE _method("Destroy", &D::ClassType::Destroy);
    _d.CPGF_MD_TEMPLATE _method("Collide", &D::ClassType::Collide);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
