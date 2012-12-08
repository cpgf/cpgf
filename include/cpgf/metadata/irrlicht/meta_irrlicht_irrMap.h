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


template <class KeyType, class ValueType>
inline typename map<KeyType, ValueType >::AccessClass opErAToRWrapper_map__opArrayGet(map<KeyType, ValueType> * self, const KeyType & k) {
    return (*self)[k];
}


template <typename D, class KeyType, class ValueType>
void buildMetaClass_Map(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("insert", (bool (D::ClassType::*) (const KeyType &, const ValueType &))&D::ClassType::insert);
    _d.CPGF_MD_TEMPLATE _method("set", &D::ClassType::set);
    _d.CPGF_MD_TEMPLATE _method("delink", &D::ClassType::delink);
    _d.CPGF_MD_TEMPLATE _method("remove", (bool (D::ClassType::*) (const KeyType &))&D::ClassType::remove);
    _d.CPGF_MD_TEMPLATE _method("remove", (bool (D::ClassType::*) (typename map<KeyType, ValueType >::Node*))&D::ClassType::remove);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("empty", &D::ClassType::empty);
    _d.CPGF_MD_TEMPLATE _method("isEmpty", &D::ClassType::isEmpty);
    _d.CPGF_MD_TEMPLATE _method("find", &D::ClassType::find);
    _d.CPGF_MD_TEMPLATE _method("getRoot", &D::ClassType::getRoot);
    _d.CPGF_MD_TEMPLATE _method("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _method("swap", &D::ClassType::swap, cpgf::MakePolicy<cpgf::GMetaRuleParamNoncopyable<0> >());
    _d.CPGF_MD_TEMPLATE _method("getIterator", &D::ClassType::getIterator);
    _d.CPGF_MD_TEMPLATE _method("getConstIterator", &D::ClassType::getConstIterator);
    _d.CPGF_MD_TEMPLATE _method("getParentFirstIterator", &D::ClassType::getParentFirstIterator);
    _d.CPGF_MD_TEMPLATE _method("getParentLastIterator", &D::ClassType::getParentLastIterator);
    _d.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::AccessClass (*)(cpgf::GMetaSelf, const KeyType &)>(mopHolder[0]);
    _d.CPGF_MD_TEMPLATE _method("_opArrayGet", (typename map<KeyType, ValueType >::AccessClass (*) (map<KeyType, ValueType> *, const KeyType &))&opErAToRWrapper_map__opArrayGet<KeyType, ValueType>, cpgf::MakePolicy<cpgf::GMetaRuleExplicitThis >());
    {
        GDefineMetaClass<typename map<KeyType, ValueType >::ConstIterator> _nd = GDefineMetaClass<typename map<KeyType, ValueType >::ConstIterator>::declare("ConstIterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const typename map<KeyType, ValueType >::Node*)>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const typename map<KeyType, ValueType >::ConstIterator&)>(cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<0> >());
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const typename map<KeyType, ValueType >::Iterator&)>();
        _nd.CPGF_MD_TEMPLATE _method("reset", &map<KeyType, ValueType >::ConstIterator::reset)
            ._default(copyVariantFromCopyable(true))
        ;
        _nd.CPGF_MD_TEMPLATE _method("atEnd", &map<KeyType, ValueType >::ConstIterator::atEnd);
        _nd.CPGF_MD_TEMPLATE _method("getNode", &map<KeyType, ValueType >::ConstIterator::getNode);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::ConstIterator& (*)(cpgf::GMetaSelf, const typename map<KeyType, ValueType >::ConstIterator&)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder--);
        _nd.CPGF_MD_TEMPLATE _operator<const typename map<KeyType, ValueType >::Node* (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<const typename map<KeyType, ValueType >::Node& (*)(cpgf::GMetaSelf)>(*mopHolder);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<typename map<KeyType, ValueType >::Iterator> _nd = GDefineMetaClass<typename map<KeyType, ValueType >::Iterator>::declare("Iterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (typename map<KeyType, ValueType >::Node*)>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const typename map<KeyType, ValueType >::Iterator&)>();
        _nd.CPGF_MD_TEMPLATE _method("reset", &map<KeyType, ValueType >::Iterator::reset)
            ._default(copyVariantFromCopyable(true))
        ;
        _nd.CPGF_MD_TEMPLATE _method("atEnd", &map<KeyType, ValueType >::Iterator::atEnd);
        _nd.CPGF_MD_TEMPLATE _method("getNode", &map<KeyType, ValueType >::Iterator::getNode);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::Iterator& (*)(cpgf::GMetaSelf, const typename map<KeyType, ValueType >::Iterator&)>(mopHolder = mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder--);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::Node* (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::Node& (*)(cpgf::GMetaSelf)>(*mopHolder);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<typename map<KeyType, ValueType >::ParentFirstIterator> _nd = GDefineMetaClass<typename map<KeyType, ValueType >::ParentFirstIterator>::declare("ParentFirstIterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (typename map<KeyType, ValueType >::Node*)>();
        _nd.CPGF_MD_TEMPLATE _method("reset", &map<KeyType, ValueType >::ParentFirstIterator::reset);
        _nd.CPGF_MD_TEMPLATE _method("atEnd", &map<KeyType, ValueType >::ParentFirstIterator::atEnd);
        _nd.CPGF_MD_TEMPLATE _method("getNode", &map<KeyType, ValueType >::ParentFirstIterator::getNode);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::ParentFirstIterator& (*)(cpgf::GMetaSelf, const typename map<KeyType, ValueType >::ParentFirstIterator&)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::Node* (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::Node& (*)(cpgf::GMetaSelf)>(*mopHolder);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<typename map<KeyType, ValueType >::ParentLastIterator> _nd = GDefineMetaClass<typename map<KeyType, ValueType >::ParentLastIterator>::declare("ParentLastIterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (typename map<KeyType, ValueType >::Node*)>();
        _nd.CPGF_MD_TEMPLATE _method("reset", &map<KeyType, ValueType >::ParentLastIterator::reset);
        _nd.CPGF_MD_TEMPLATE _method("atEnd", &map<KeyType, ValueType >::ParentLastIterator::atEnd);
        _nd.CPGF_MD_TEMPLATE _method("getNode", &map<KeyType, ValueType >::ParentLastIterator::getNode);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::ParentLastIterator& (*)(cpgf::GMetaSelf, const typename map<KeyType, ValueType >::ParentLastIterator&)>(mopHolder = mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::Node* (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<typename map<KeyType, ValueType >::Node& (*)(cpgf::GMetaSelf)>(*mopHolder);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
