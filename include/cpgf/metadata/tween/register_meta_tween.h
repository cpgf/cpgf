// Auto generated file, don't modify.

#ifndef CPGF_REGISTER_META_TWEEN_H
#define CPGF_REGISTER_META_TWEEN_H


#include "cpgf/gmetadefine.h"


using namespace cpgf;

namespace meta_tween { 


GDefineMetaInfo createMetaClass_BackEase();
GDefineMetaInfo createMetaClass_BounceEase();
GDefineMetaInfo createMetaClass_CircleEase();
GDefineMetaInfo createMetaClass_CubicEase();
GDefineMetaInfo createMetaClass_ElasticEase();
GDefineMetaInfo createMetaClass_ExponentEase();
GDefineMetaInfo createMetaClass_GTimeline();
GDefineMetaInfo createMetaClass_GTween();
GDefineMetaInfo createMetaClass_GTweenEaseParam();
GDefineMetaInfo createMetaClass_GTweenList();
GDefineMetaInfo createMetaClass_GTweenable();
GDefineMetaInfo createMetaClass_Global_gtweencommon();
GDefineMetaInfo createMetaClass_LinearEase();
GDefineMetaInfo createMetaClass_QuadEase();
GDefineMetaInfo createMetaClass_QuartEase();
GDefineMetaInfo createMetaClass_QuintEase();
GDefineMetaInfo createMetaClass_SineEase();


template <typename Meta>
void registerMain_tween(Meta _d)
{
    _d._class(createMetaClass_BackEase());
    _d._class(createMetaClass_BounceEase());
    _d._class(createMetaClass_CircleEase());
    _d._class(createMetaClass_CubicEase());
    _d._class(createMetaClass_ElasticEase());
    _d._class(createMetaClass_ExponentEase());
    _d._class(createMetaClass_GTimeline());
    _d._class(createMetaClass_GTween());
    _d._class(createMetaClass_GTweenEaseParam());
    _d._class(createMetaClass_GTweenList());
    _d._class(createMetaClass_GTweenable());
    _d._class(createMetaClass_Global_gtweencommon());
    _d._class(createMetaClass_LinearEase());
    _d._class(createMetaClass_QuadEase());
    _d._class(createMetaClass_QuartEase());
    _d._class(createMetaClass_QuintEase());
    _d._class(createMetaClass_SineEase());
}

} // namespace meta_tween




#endif
