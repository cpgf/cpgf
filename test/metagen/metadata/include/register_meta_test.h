// Auto generated file, don't modify.

#ifndef __REGISTER_META_TEST_H
#define __REGISTER_META_TEST_H


#include "cpgf/gmetadefine.h"


using namespace cpgf;

namespace meta_test { 


GDefineMetaInfo createMetaClass_Global_global();
GDefineMetaInfo createMetaClass_SimpleAtom();
GDefineMetaInfo createMetaClass_SimpleData();
GDefineMetaInfo createMetaClass_SimpleObject();
GDefineMetaInfo createMetaClass_SimpleOverride();


template <typename Meta>
void registerMain_metatest(Meta _d)
{
    _d._class(createMetaClass_Global_global());
    _d._class(createMetaClass_SimpleAtom());
    _d._class(createMetaClass_SimpleData());
    _d._class(createMetaClass_SimpleObject());
    _d._class(createMetaClass_SimpleOverride());
}

} // namespace meta_test




#endif
