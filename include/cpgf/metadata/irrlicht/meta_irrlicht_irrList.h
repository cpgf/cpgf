// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_IRRLIST_H
#define __META_IRRLICHT_IRRLIST_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;
using namespace irr::core;


namespace meta_irrlicht { 


template <class T>
inline void opErAToRWrapper_list__opAssign(list<T> * self, const list< T > & other) {
    (*self) = other;
}


template <typename D, class T>
void buildMetaClass_List(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _constructor<void * (const list< T > &)>();
    _d.CPGF_MD_TEMPLATE _method("size", &D::ClassType::size);
    _d.CPGF_MD_TEMPLATE _method("getSize", &D::ClassType::getSize);
    _d.CPGF_MD_TEMPLATE _method("clear", &D::ClassType::clear);
    _d.CPGF_MD_TEMPLATE _method("empty", &D::ClassType::empty);
    _d.CPGF_MD_TEMPLATE _method("push_back", &D::ClassType::push_back);
    _d.CPGF_MD_TEMPLATE _method("push_front", &D::ClassType::push_front);
    _d.CPGF_MD_TEMPLATE _method("begin", (typename list<T >::Iterator (D::ClassType::*) ())&D::ClassType::begin);
    _d.CPGF_MD_TEMPLATE _method("begin", (typename list<T >::ConstIterator (D::ClassType::*) () const)&D::ClassType::begin);
    _d.CPGF_MD_TEMPLATE _method("end", (typename list<T >::Iterator (D::ClassType::*) ())&D::ClassType::end);
    _d.CPGF_MD_TEMPLATE _method("end", (typename list<T >::ConstIterator (D::ClassType::*) () const)&D::ClassType::end);
    _d.CPGF_MD_TEMPLATE _method("getLast", (typename list<T >::Iterator (D::ClassType::*) ())&D::ClassType::getLast);
    _d.CPGF_MD_TEMPLATE _method("getLast", (typename list<T >::ConstIterator (D::ClassType::*) () const)&D::ClassType::getLast);
    _d.CPGF_MD_TEMPLATE _method("insert_after", &D::ClassType::insert_after);
    _d.CPGF_MD_TEMPLATE _method("insert_before", &D::ClassType::insert_before);
    _d.CPGF_MD_TEMPLATE _method("erase", &D::ClassType::erase);
    _d.CPGF_MD_TEMPLATE _method("swap", &D::ClassType::swap);
    _d.CPGF_MD_TEMPLATE _operator<void (*)(cpgf::GMetaSelf, const list< T > &)>(mopHolder = mopHolder);
    _d.CPGF_MD_TEMPLATE _method("_opAssign", (void (*) (list<T> *, const list< T > &))&opErAToRWrapper_list__opAssign<T>);
    {
        GDefineMetaClass<typename list<T >::ConstIterator> _nd = GDefineMetaClass<typename list<T >::ConstIterator>::declare("ConstIterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _constructor<void * (const typename list<T >::Iterator&)>();
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator& (*)(cpgf::GMetaSelf)>(++mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opInc", (typename list<T >::ConstIterator& (*) (list::ConstIterator *))&opErAToRWrapper_list_ConstIterator__opInc);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator& (*)(cpgf::GMetaSelf)>(--mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opDec", (typename list<T >::ConstIterator& (*) (list::ConstIterator *))&opErAToRWrapper_list_ConstIterator__opDec);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _method("_opInc", (typename list<T >::ConstIterator (*) (list::ConstIterator *))&opErAToRWrapper_list_ConstIterator__opInc);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator (*)(cpgf::GMetaSelf)>(mopHolder--);
        _nd.CPGF_MD_TEMPLATE _method("_opDec", (typename list<T >::ConstIterator (*) (list::ConstIterator *))&opErAToRWrapper_list_ConstIterator__opDec);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator& (*)(cpgf::GMetaSelf, s32)>(mopHolder += mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opPlusAssign", (typename list<T >::ConstIterator& (*) (list::ConstIterator *, s32))&opErAToRWrapper_list_ConstIterator__opPlusAssign);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator (*)(const cpgf::GMetaSelf &, s32)>(mopHolder + mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opPlus", (typename list<T >::ConstIterator (*) (const list::ConstIterator *, s32))&opErAToRWrapper_list_ConstIterator__opPlus);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator& (*)(const cpgf::GMetaSelf &, s32)>(mopHolder -= mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opMinusAssign", (typename list<T >::ConstIterator& (*) (const list::ConstIterator *, s32))&opErAToRWrapper_list_ConstIterator__opMinusAssign);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator (*)(const cpgf::GMetaSelf &, s32)>(mopHolder - mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opMinus", (typename list<T >::ConstIterator (*) (const list::ConstIterator *, s32))&opErAToRWrapper_list_ConstIterator__opMinus);
        _nd.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const typename list<T >::ConstIterator&)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const list::ConstIterator *, const typename list<T >::ConstIterator&))&opErAToRWrapper_list_ConstIterator__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const typename list<T >::ConstIterator&)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const list::ConstIterator *, const typename list<T >::ConstIterator&))&opErAToRWrapper_list_ConstIterator__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const typename list<T >::Iterator&)>(mopHolder == mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const list::ConstIterator *, const typename list<T >::Iterator&))&opErAToRWrapper_list_ConstIterator__opEqual);
        _nd.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const typename list<T >::Iterator&)>(mopHolder != mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const list::ConstIterator *, const typename list<T >::Iterator&))&opErAToRWrapper_list_ConstIterator__opNotEqual);
        _nd.CPGF_MD_TEMPLATE _operator<const T & (*)(cpgf::GMetaSelf)>(*mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opStar", (const T & (*) (list::ConstIterator *))&opErAToRWrapper_list_ConstIterator__opStar);
        _nd.CPGF_MD_TEMPLATE _operator<const T * (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::ConstIterator& (*)(cpgf::GMetaSelf, const typename list<T >::Iterator&)>(mopHolder = mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opAssign", (typename list<T >::ConstIterator& (*) (list::ConstIterator *, const typename list<T >::Iterator&))&opErAToRWrapper_list_ConstIterator__opAssign);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
    {
        GDefineMetaClass<typename list<T >::Iterator> _nd = GDefineMetaClass<typename list<T >::Iterator>::declare("Iterator");
        _nd.CPGF_MD_TEMPLATE _constructor<void * ()>();
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::Iterator& (*)(cpgf::GMetaSelf)>(++mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opInc", (typename list<T >::Iterator& (*) (list::Iterator *))&opErAToRWrapper_list_Iterator__opInc);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::Iterator& (*)(cpgf::GMetaSelf)>(--mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opDec", (typename list<T >::Iterator& (*) (list::Iterator *))&opErAToRWrapper_list_Iterator__opDec);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::Iterator (*)(cpgf::GMetaSelf)>(mopHolder++);
        _nd.CPGF_MD_TEMPLATE _method("_opInc", (typename list<T >::Iterator (*) (list::Iterator *))&opErAToRWrapper_list_Iterator__opInc);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::Iterator (*)(cpgf::GMetaSelf)>(mopHolder--);
        _nd.CPGF_MD_TEMPLATE _method("_opDec", (typename list<T >::Iterator (*) (list::Iterator *))&opErAToRWrapper_list_Iterator__opDec);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::Iterator& (*)(cpgf::GMetaSelf, s32)>(mopHolder += mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opPlusAssign", (typename list<T >::Iterator& (*) (list::Iterator *, s32))&opErAToRWrapper_list_Iterator__opPlusAssign);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::Iterator (*)(const cpgf::GMetaSelf &, s32)>(mopHolder + mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opPlus", (typename list<T >::Iterator (*) (const list::Iterator *, s32))&opErAToRWrapper_list_Iterator__opPlus);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::Iterator& (*)(const cpgf::GMetaSelf &, s32)>(mopHolder -= mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opMinusAssign", (typename list<T >::Iterator& (*) (const list::Iterator *, s32))&opErAToRWrapper_list_Iterator__opMinusAssign);
        _nd.CPGF_MD_TEMPLATE _operator<typename list<T >::Iterator (*)(const cpgf::GMetaSelf &, s32)>(mopHolder - mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opMinus", (typename list<T >::Iterator (*) (const list::Iterator *, s32))&opErAToRWrapper_list_Iterator__opMinus);
        _nd.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const typename list<T >::Iterator&)>(mopHolder == mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const list::Iterator *, const typename list<T >::Iterator&))&opErAToRWrapper_list_Iterator__opEqual);
        _nd.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const typename list<T >::Iterator&)>(mopHolder != mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const list::Iterator *, const typename list<T >::Iterator&))&opErAToRWrapper_list_Iterator__opNotEqual);
        _nd.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const typename list<T >::ConstIterator&)>(mopHolder == mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opEqual", (bool (*) (const list::Iterator *, const typename list<T >::ConstIterator&))&opErAToRWrapper_list_Iterator__opEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _operator<bool (*)(const cpgf::GMetaSelf &, const typename list<T >::ConstIterator&)>(mopHolder != mopHolder, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _method("_opNotEqual", (bool (*) (const list::Iterator *, const typename list<T >::ConstIterator&))&opErAToRWrapper_list_Iterator__opNotEqual, cpgf::MakePolicy<cpgf::GMetaRuleCopyConstReference<1> >());
        _nd.CPGF_MD_TEMPLATE _operator<T & (*)(cpgf::GMetaSelf)>(*mopHolder);
        _nd.CPGF_MD_TEMPLATE _method("_opStar", (T & (*) (list::Iterator *))&opErAToRWrapper_list_Iterator__opStar);
        _nd.CPGF_MD_TEMPLATE _operator<T * (*)(cpgf::GMetaSelf)>(mopHolder -> mopHolder);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
