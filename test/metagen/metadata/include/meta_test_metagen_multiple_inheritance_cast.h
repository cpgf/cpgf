// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_MULTIPLE_INHERITANCE_CAST_H
#define CPGF_META_TEST_METAGEN_MULTIPLE_INHERITANCE_CAST_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_Global_metagen_multiple_inheritance_cast(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getMultipleInheritanceCastD", (MultipleInheritanceCastD * (*) ())&getMultipleInheritanceCastD);
    _d.CPGF_MD_TEMPLATE _method("getMultipleInheritanceCastDSharedPointer", (cpgf::GSharedPointer< MultipleInheritanceCastD > (*) ())&getMultipleInheritanceCastDSharedPointer);
    _d.CPGF_MD_TEMPLATE _method("checkMultipleInheritanceCastDAsR", (bool (*) (MultipleInheritanceCastR *))&checkMultipleInheritanceCastDAsR);
    _d.CPGF_MD_TEMPLATE _method("getMultipleInheritanceCastDAsR", (MultipleInheritanceCastR * (*) ())&getMultipleInheritanceCastDAsR);
    _d.CPGF_MD_TEMPLATE _method("getMultipleInheritanceCastDAsRSharedPointer", (cpgf::GSharedPointer< MultipleInheritanceCastR > (*) ())&getMultipleInheritanceCastDAsRSharedPointer);
    _d.CPGF_MD_TEMPLATE _method("checkMultipleInheritanceCastRAsD", (bool (*) (MultipleInheritanceCastD *))&checkMultipleInheritanceCastRAsD);
}


template <typename D>
void buildMetaClass_MultipleInheritanceCastA(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("a", &D::ClassType::a);
    _d.CPGF_MD_TEMPLATE _method("isR", &D::ClassType::isR);
    _d.CPGF_MD_TEMPLATE _method("isA", &D::ClassType::isA);
    _d.CPGF_MD_TEMPLATE _method("isB", &D::ClassType::isB);
    _d.CPGF_MD_TEMPLATE _method("isC", &D::ClassType::isC);
    _d.CPGF_MD_TEMPLATE _method("isD", &D::ClassType::isD);
}


template <typename D>
void buildMetaClass_MultipleInheritanceCastB(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("c", &D::ClassType::c);
    _d.CPGF_MD_TEMPLATE _method("c1", &D::ClassType::c1);
    _d.CPGF_MD_TEMPLATE _method("isR", &D::ClassType::isR);
    _d.CPGF_MD_TEMPLATE _method("isA", &D::ClassType::isA);
    _d.CPGF_MD_TEMPLATE _method("isB", &D::ClassType::isB);
    _d.CPGF_MD_TEMPLATE _method("isC", &D::ClassType::isC);
    _d.CPGF_MD_TEMPLATE _method("isD", &D::ClassType::isD);
}


template <typename D>
void buildMetaClass_MultipleInheritanceCastC(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("d", &D::ClassType::d);
    _d.CPGF_MD_TEMPLATE _method("isR", &D::ClassType::isR);
    _d.CPGF_MD_TEMPLATE _method("isA", &D::ClassType::isA);
    _d.CPGF_MD_TEMPLATE _method("isB", &D::ClassType::isB);
    _d.CPGF_MD_TEMPLATE _method("isC", &D::ClassType::isC);
    _d.CPGF_MD_TEMPLATE _method("isD", &D::ClassType::isD);
}


template <typename D>
void buildMetaClass_MultipleInheritanceCastD(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("e", &D::ClassType::e);
    _d.CPGF_MD_TEMPLATE _method("isR", &D::ClassType::isR);
    _d.CPGF_MD_TEMPLATE _method("isA", &D::ClassType::isA);
    _d.CPGF_MD_TEMPLATE _method("isB", &D::ClassType::isB);
    _d.CPGF_MD_TEMPLATE _method("isC", &D::ClassType::isC);
    _d.CPGF_MD_TEMPLATE _method("isD", &D::ClassType::isD);
    _d.CPGF_MD_TEMPLATE _method("iAmD", &D::ClassType::iAmD);
}


template <typename D>
void buildMetaClass_MultipleInheritanceCastFieldData(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _field("rawD", &D::ClassType::rawD);
    _d.CPGF_MD_TEMPLATE _field("rawR", &D::ClassType::rawR);
    _d.CPGF_MD_TEMPLATE _method("checkD", &D::ClassType::checkD);
    _d.CPGF_MD_TEMPLATE _method("checkR", &D::ClassType::checkR);
}


template <typename D>
void buildMetaClass_MultipleInheritanceCastR(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("r", &D::ClassType::r);
    _d.CPGF_MD_TEMPLATE _method("isR", &D::ClassType::isR);
    _d.CPGF_MD_TEMPLATE _method("isA", &D::ClassType::isA);
    _d.CPGF_MD_TEMPLATE _method("isB", &D::ClassType::isB);
    _d.CPGF_MD_TEMPLATE _method("isC", &D::ClassType::isC);
    _d.CPGF_MD_TEMPLATE _method("isD", &D::ClassType::isD);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
