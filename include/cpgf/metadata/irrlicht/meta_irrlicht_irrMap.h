// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRRMAP_H
#define __META_IRRLICHT_IRRMAP_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <typename D, typename KeyType, typename ValueType>
void buildMetaClass_Map(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("delink", &D::ClassType::delink);
    _d.CPGF_MD_TEMPLATE _method("empty", &D::ClassType::empty);
    _d.CPGF_MD_TEMPLATE _method("find", &D::ClassType::find);
    _d.CPGF_MD_TEMPLATE _method("getIterator", &D::ClassType::getIterator);
    _d.CPGF_MD_TEMPLATE _method("getParentFirstIterator", &D::ClassType::getParentFirstIterator);
    _d.CPGF_MD_TEMPLATE _method("getParentLastIterator", &D::ClassType::getParentLastIterator);
    _d.CPGF_MD_TEMPLATE _method("getRoot", &D::ClassType::getRoot);
    _d.CPGF_MD_TEMPLATE _method("insert", (bool (D::ClassType::*) (const KeyType &, const ValueType &))&D::ClassType::insert);
    _d.CPGF_MD_TEMPLATE _method("isEmpty", &D::ClassType::isEmpty);
    _d.CPGF_MD_TEMPLATE _method("remove", &D::ClassType::remove);
    _d.CPGF_MD_TEMPLATE _method("set", &D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _method("swap", &D::ClassType::swap, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<0> >());
    _d.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::AccessClass (*)(cpgf::GMetaSelf, const KeyType &)>(mopHolder[0]);
    {
        GDefineMetaClass<map::AccessClass> _nd = GDefineMetaClass<map::AccessClass>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("AccessClass");
        _nd.CPGF_MD_TEMPLATE _operator< ValueType (cpgf::GMetaSelf)>(mopHolder());
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const ValueType &)>(mopHolder = mopHolder);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<map::Iterator> _nd = GDefineMetaClass<map::Iterator>::declare("Iterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (map<KeyType, ValueType >::Node*)>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const list<T >::Iterator&)>();
        _nd.CPGF_MD_TEMPLATE _method("atEnd", &irr::core::map::Iterator::atEnd);
        _nd.CPGF_MD_TEMPLATE _method("getNode", &irr::core::map::Iterator::getNode);
        _nd.CPGF_MD_TEMPLATE _method("reset", &irr::core::map::Iterator::reset)
            ._default(copyVariantFromCopyable(true))
        ;
        _nd.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::Node& (*)(cpgf::GMetaSelf)>(*mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder--);
        _nd.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::Node* (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<list<T >::Iterator& (*)(cpgf::GMetaSelf, const list<T >::Iterator&)>(mopHolder = mopHolder);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<map::ParentFirstIterator> _nd = GDefineMetaClass<map::ParentFirstIterator>::declare("ParentFirstIterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (map<KeyType, ValueType >::Node*)>();
        _nd.CPGF_MD_TEMPLATE _method("atEnd", &irr::core::map::ParentFirstIterator::atEnd);
        _nd.CPGF_MD_TEMPLATE _method("getNode", &irr::core::map::ParentFirstIterator::getNode);
        _nd.CPGF_MD_TEMPLATE _method("reset", &irr::core::map::ParentFirstIterator::reset);
        _nd.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::Node& (*)(cpgf::GMetaSelf)>(*mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::Node* (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::ParentFirstIterator& (*)(cpgf::GMetaSelf, const map<KeyType, ValueType >::ParentFirstIterator&)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<map::ParentLastIterator> _nd = GDefineMetaClass<map::ParentLastIterator>::declare("ParentLastIterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (map<KeyType, ValueType >::Node*)>();
        _nd.CPGF_MD_TEMPLATE _method("atEnd", &irr::core::map::ParentLastIterator::atEnd);
        _nd.CPGF_MD_TEMPLATE _method("getNode", &irr::core::map::ParentLastIterator::getNode);
        _nd.CPGF_MD_TEMPLATE _method("reset", &irr::core::map::ParentLastIterator::reset);
        _nd.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::Node& (*)(cpgf::GMetaSelf)>(*mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::Node* (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<map<KeyType, ValueType >::ParentLastIterator& (*)(cpgf::GMetaSelf, const map<KeyType, ValueType >::ParentLastIterator&)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
