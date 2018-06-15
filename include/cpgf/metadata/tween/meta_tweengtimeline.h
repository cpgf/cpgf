// Auto generated file, don't modify.

#ifndef CPGF_META_TWEENGTIMELINE_H
#define CPGF_META_TWEENGTIMELINE_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace cpgf;


namespace meta_tween { 


template <typename D>
void buildMetaClass_GTimeline(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _constructor<void * ()>();
    _d.CPGF_MD_TEMPLATE _method("removeForInstance", &D::ClassType::removeForInstance);
    _d.CPGF_MD_TEMPLATE _method("getDuration", &D::ClassType::getDuration);
    _d.CPGF_MD_TEMPLATE _method("append", &D::ClassType::append);
    _d.CPGF_MD_TEMPLATE _method("prepend", &D::ClassType::prepend);
    _d.CPGF_MD_TEMPLATE _method("insert", &D::ClassType::insert);
    _d.CPGF_MD_TEMPLATE _method("setAt", &D::ClassType::setAt);
    _d.CPGF_MD_TEMPLATE _method("getStartTime", &D::ClassType::getStartTime);
    _d.CPGF_MD_TEMPLATE _method("backward", &D::ClassType::backward);
    _d.CPGF_MD_TEMPLATE _method("useFrames", &D::ClassType::useFrames);
    _d.CPGF_MD_TEMPLATE _method("delay", &D::ClassType::delay);
    _d.CPGF_MD_TEMPLATE _method("timeScale", &D::ClassType::timeScale);
    _d.CPGF_MD_TEMPLATE _method("repeat", &D::ClassType::repeat);
    _d.CPGF_MD_TEMPLATE _method("_repeat", &D::ClassType::repeat);
    _d.CPGF_MD_TEMPLATE _method("repeatDelay", &D::ClassType::repeatDelay);
    _d.CPGF_MD_TEMPLATE _method("yoyo", &D::ClassType::yoyo);
    _d.CPGF_MD_TEMPLATE _method("onComplete", &D::ClassType::onComplete);
    _d.CPGF_MD_TEMPLATE _method("onDestroy", &D::ClassType::onDestroy);
    _d.CPGF_MD_TEMPLATE _method("onUpdate", &D::ClassType::onUpdate);
    _d.CPGF_MD_TEMPLATE _method("onRepeat", &D::ClassType::onRepeat);
}


} // namespace meta_tween




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
