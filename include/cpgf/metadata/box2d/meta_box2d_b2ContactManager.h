// Auto generated file, don't modify.

#ifndef __META_BOX2D_B2CONTACTMANAGER_H
#define __META_BOX2D_B2CONTACTMANAGER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_box2d { 


template <typename D>
void buildMetaClass_B2ContactManager(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r)
{
    (void)config; (void)_d; (void)_r; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_broadPhase", _r), &D::ClassType::m_broadPhase);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_contactList", _r), &D::ClassType::m_contactList);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_contactCount", _r), &D::ClassType::m_contactCount);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_contactFilter", _r), &D::ClassType::m_contactFilter);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_contactListener", _r), &D::ClassType::m_contactListener);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_allocator", _r), &D::ClassType::m_allocator);
    _d.CPGF_MD_TEMPLATE _method(replaceName("AddPair", _r), &D::ClassType::AddPair);
    _d.CPGF_MD_TEMPLATE _method(replaceName("FindNewContacts", _r), &D::ClassType::FindNewContacts);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Destroy", _r), &D::ClassType::Destroy);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Collide", _r), &D::ClassType::Collide);
}


} // namespace meta_box2d




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
