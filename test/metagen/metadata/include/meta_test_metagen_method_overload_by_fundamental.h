// Auto generated file, don't modify.

#ifndef CPGF_META_TEST_METAGEN_METHOD_OVERLOAD_BY_FUNDAMENTAL_H
#define CPGF_META_TEST_METAGEN_METHOD_OVERLOAD_BY_FUNDAMENTAL_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"




namespace meta_test { 


template <typename D>
void buildMetaClass_Global_metagen_method_overload_by_fundamental(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("global_overload_Boolean_Int", (std::string (*) (bool))&global_overload_Boolean_Int);
    _d.CPGF_MD_TEMPLATE _method("global_overload_Boolean_Int", (std::string (*) (int))&global_overload_Boolean_Int);
}


template <typename D>
void buildMetaClass_MetagenMethodOverloadByFundamental(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("overload_Boolean_Int", (std::string (D::ClassType::*) (bool))&D::ClassType::overload_Boolean_Int);
    _d.CPGF_MD_TEMPLATE _method("overload_Boolean_Int", (std::string (D::ClassType::*) (int))&D::ClassType::overload_Boolean_Int);
    _d.CPGF_MD_TEMPLATE _method("overload_Boolean_Real", (std::string (D::ClassType::*) (bool))&D::ClassType::overload_Boolean_Real);
    _d.CPGF_MD_TEMPLATE _method("overload_Boolean_Real", (std::string (D::ClassType::*) (double))&D::ClassType::overload_Boolean_Real);
    _d.CPGF_MD_TEMPLATE _method("overload_Boolean_Int_Real", (std::string (D::ClassType::*) (bool))&D::ClassType::overload_Boolean_Int_Real);
    _d.CPGF_MD_TEMPLATE _method("overload_Boolean_Int_Real", (std::string (D::ClassType::*) (int))&D::ClassType::overload_Boolean_Int_Real);
    _d.CPGF_MD_TEMPLATE _method("overload_Boolean_Int_Real", (std::string (D::ClassType::*) (double))&D::ClassType::overload_Boolean_Int_Real);
}


} // namespace meta_test




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
