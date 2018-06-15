// Auto generated file, don't modify.

#ifndef CPGF_META_TWEENGTWEENCOMMON_H
#define CPGF_META_TWEENGTWEENCOMMON_H


#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace cpgf;


namespace meta_tween { 


template <typename D>
void buildMetaClass_Global_gtweencommon(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("tweenRepeatInfinitely", &tweenRepeatInfinitely);
    _d.CPGF_MD_TEMPLATE _field("tweenNoRepeat", &tweenNoRepeat);
}


template <typename D>
void buildMetaClass_GTweenEaseParam(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _field("current", &D::ClassType::current);
    _d.CPGF_MD_TEMPLATE _field("total", &D::ClassType::total);
}


template <typename D>
void buildMetaClass_GTweenable(D _d)
{
    (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("removeForInstance", &D::ClassType::removeForInstance);
    _d.CPGF_MD_TEMPLATE _method("getDuration", &D::ClassType::getDuration);
    _d.CPGF_MD_TEMPLATE _method("getTotalDuration", &D::ClassType::getTotalDuration);
    _d.CPGF_MD_TEMPLATE _method("getCurrentTime", &D::ClassType::getCurrentTime);
    _d.CPGF_MD_TEMPLATE _method("setCurrentTime", &D::ClassType::setCurrentTime);
    _d.CPGF_MD_TEMPLATE _method("getTotalTime", &D::ClassType::getTotalTime);
    _d.CPGF_MD_TEMPLATE _method("setTotalTime", &D::ClassType::setTotalTime);
    _d.CPGF_MD_TEMPLATE _method("getCurrentProgress", &D::ClassType::getCurrentProgress);
    _d.CPGF_MD_TEMPLATE _method("setCurrentProgress", &D::ClassType::setCurrentProgress);
    _d.CPGF_MD_TEMPLATE _method("getTotalProgress", &D::ClassType::getTotalProgress);
    _d.CPGF_MD_TEMPLATE _method("setTotalProgress", &D::ClassType::setTotalProgress);
    _d.CPGF_MD_TEMPLATE _method("restart", &D::ClassType::restart);
    _d.CPGF_MD_TEMPLATE _method("restartWithDelay", &D::ClassType::restartWithDelay);
    _d.CPGF_MD_TEMPLATE _method("pause", &D::ClassType::pause);
    _d.CPGF_MD_TEMPLATE _method("resume", &D::ClassType::resume);
    _d.CPGF_MD_TEMPLATE _method("immediateTick", &D::ClassType::immediateTick);
    _d.CPGF_MD_TEMPLATE _method("tick", &D::ClassType::tick);
    _d.CPGF_MD_TEMPLATE _method("backward", &D::ClassType::backward);
    _d.CPGF_MD_TEMPLATE _method("useFrames", &D::ClassType::useFrames);
    _d.CPGF_MD_TEMPLATE _method("delay", &D::ClassType::delay);
    _d.CPGF_MD_TEMPLATE _method("timeScale", &D::ClassType::timeScale);
    _d.CPGF_MD_TEMPLATE _method("repeat", &D::ClassType::repeat);
    _d.CPGF_MD_TEMPLATE _method("_repeat", &D::ClassType::repeat);
    _d.CPGF_MD_TEMPLATE _method("repeatDelay", &D::ClassType::repeatDelay);
    _d.CPGF_MD_TEMPLATE _method("yoyo", &D::ClassType::yoyo);
    _d.CPGF_MD_TEMPLATE _method("onInitialize", &D::ClassType::onInitialize);
    _d.CPGF_MD_TEMPLATE _method("onComplete", &D::ClassType::onComplete);
    _d.CPGF_MD_TEMPLATE _method("onDestroy", &D::ClassType::onDestroy);
    _d.CPGF_MD_TEMPLATE _method("onUpdate", &D::ClassType::onUpdate);
    _d.CPGF_MD_TEMPLATE _method("onRepeat", &D::ClassType::onRepeat);
    _d.CPGF_MD_TEMPLATE _method("isRunning", &D::ClassType::isRunning);
    _d.CPGF_MD_TEMPLATE _method("isPaused", &D::ClassType::isPaused);
    _d.CPGF_MD_TEMPLATE _method("isCompleted", &D::ClassType::isCompleted);
    _d.CPGF_MD_TEMPLATE _method("isUseFrames", &D::ClassType::isUseFrames);
    _d.CPGF_MD_TEMPLATE _method("isBackward", &D::ClassType::isBackward);
    _d.CPGF_MD_TEMPLATE _method("isYoyo", &D::ClassType::isYoyo);
    _d.CPGF_MD_TEMPLATE _method("isRepeat", &D::ClassType::isRepeat);
    _d.CPGF_MD_TEMPLATE _method("isRepeatInfinitely", &D::ClassType::isRepeatInfinitely);
    _d.CPGF_MD_TEMPLATE _method("getRepeatCount", &D::ClassType::getRepeatCount);
    _d.CPGF_MD_TEMPLATE _method("getRepeatDelay", &D::ClassType::getRepeatDelay);
    _d.CPGF_MD_TEMPLATE _method("getDelay", &D::ClassType::getDelay);
    _d.CPGF_MD_TEMPLATE _method("getTimeScale", &D::ClassType::getTimeScale);
}


} // namespace meta_tween




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
