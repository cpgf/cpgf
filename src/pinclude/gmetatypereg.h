#include "cpgf/gmetatype.h"



namespace cpgf {

class GMetaTypedItem;
class GMetaClass;
class GMetaEnum;
class GMetaFundamental;

namespace meta_internal {


const GMetaTypedItem * findRegisteredMetaType(const GTypeInfo & type);
const GMetaTypedItem * findRegisteredMetaType(const char * name);

const GMetaClass * findRegisteredMetaClass(const GMetaType & type);
const GMetaClass * findRegisteredMetaClass(const char * name);

const GMetaEnum * findRegisteredMetaEnum(const GMetaType & type);
const GMetaEnum * findRegisteredMetaEnum(const char * name);

const GMetaFundamental * findRegisteredMetaFundamental(const GMetaType & type);
const GMetaFundamental * findRegisteredMetaFundamental(const char * name);
const GMetaFundamental * findRegisteredMetaFundamental(GVariantType vt);

void registerMetaTypedItem(const GMetaTypedItem * typedItem);
void removeMetaTypedItem(const GMetaTypedItem * typedItem);


} // namespace meta_internal



} // namespace cpgf
