// Auto generated file, don't modify.

#ifndef __META_B2CONTACTMANAGER_H
#define __META_B2CONTACTMANAGER_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gnamereplacer.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"


namespace meta_box2d { 


template <typename D, typename Policy>
void buildMetaClass_B2ContactManager(const cpgf::GMetaDataConfigFlags & config, D _d, const cpgf::GMetaDataNameReplacer * _r, const Policy & _p)
{
    (void)config; (void)_d; (void)_r; (void)_d; (void)_p;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>(_p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_broadPhase", _r), &D::ClassType::m_broadPhase, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_contactList", _r), &D::ClassType::m_contactList, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_contactCount", _r), &D::ClassType::m_contactCount, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_contactFilter", _r), &D::ClassType::m_contactFilter, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_contactListener", _r), &D::ClassType::m_contactListener, _p);
    _d.CPGF_MD_TEMPLATE _field(replaceName("m_allocator", _r), &D::ClassType::m_allocator, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("AddPair", _r), &D::ClassType::AddPair, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("FindNewContacts", _r), &D::ClassType::FindNewContacts, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Destroy", _r), &D::ClassType::Destroy, _p);
    _d.CPGF_MD_TEMPLATE _method(replaceName("Collide", _r), &D::ClassType::Collide, _p);
}


} // namespace meta_box2d 




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif