// Auto generated file, don't modify.

#include "cpgf/metatraits/gmetaconverter_string.h"
#include "cpgf/metatraits/gmetaconverter_widestring.h"
#include "cpgf/metatraits/gmetaserializer_string.h"

#include "../../include/simpleobject.h"

#include "../include/meta_test_simpleobject.h"

using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_SimpleAtom()
{
    GDefineMetaClass<SimpleAtom> _d = GDefineMetaClass<SimpleAtom>::declare("SimpleAtom");
    buildMetaClass_SimpleAtom(0, _d, NULL);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SimpleData()
{
    GDefineMetaClass<SimpleData> _d = GDefineMetaClass<SimpleData>::Policy<MakePolicy<GMetaRuleDefaultConstructorAbsent> >::declare("SimpleData");
    buildMetaClass_SimpleData(0, _d, NULL);
    return _d.getMetaInfo();
}


GDefineMetaInfo createMetaClass_SimpleObject()
{
    GDefineMetaClass<SimpleObject> _d = GDefineMetaClass<SimpleObject>::declare("SimpleObject");
    buildMetaClass_SimpleObject(0, _d, NULL);
    return _d.getMetaInfo();
}


} // namespace meta_test


