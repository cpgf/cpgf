// Auto generated file, don't modify.

#ifndef __REGISTER_META_TEST_H
#define __REGISTER_META_TEST_H


#include "cpgf/gmetadefine.h"


using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_Global_global();
GDefineMetaInfo createMetaClass_Global_simpleobject();
GDefineMetaInfo createMetaClass_Global_testobjectarray();
GDefineMetaInfo createMetaClass_OAData();
GDefineMetaInfo createMetaClass_OAObject();
GDefineMetaInfo createMetaClass_SimpleAtom();
GDefineMetaInfo createMetaClass_SimpleData();
GDefineMetaInfo createMetaClass_SimpleObject();
GDefineMetaInfo createMetaClass_SimpleOverride();
GDefineMetaInfo createMetaClass_SimpleOverrideBase();


template <typename Meta>
void registerMain_metatest(Meta _d)
{
    _d._class(createMetaClass_Global_global());
    _d._class(createMetaClass_Global_simpleobject());
    _d._class(createMetaClass_Global_testobjectarray());
    _d._class(createMetaClass_OAData());
    _d._class(createMetaClass_OAObject());
    _d._class(createMetaClass_SimpleAtom());
    _d._class(createMetaClass_SimpleData());
    _d._class(createMetaClass_SimpleObject());
    _d._class(createMetaClass_SimpleOverride());
    _d._class(createMetaClass_SimpleOverrideBase());
}

} // namespace meta_test




#endif
