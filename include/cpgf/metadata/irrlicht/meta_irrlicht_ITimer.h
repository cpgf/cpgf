// Auto generated file, don't modify.

#ifndef __META_IRRLICHT_ITIMER_H
#define __META_IRRLICHT_ITIMER_H


#include "gmetaobjectlifemanager_irrlicht_ireferencecounted.h"
#include "cpgf/gmetadefine.h"
#include "cpgf/metadata/gmetadataconfig.h"
#include "cpgf/metadata/private/gmetadata_header.h"
#include "cpgf/gmetapolicy.h"


using namespace irr;


namespace meta_irrlicht { 


template <typename D>
void buildMetaClass_ITimer(const cpgf::GMetaDataConfigFlags & config, D _d)
{
    (void)config; (void)_d; (void)_d;
    using namespace cpgf;
    
    _d.CPGF_MD_TEMPLATE _method("getRealTime", &D::ClassType::getRealTime);
    _d.CPGF_MD_TEMPLATE _method("getRealTimeAndDate", &D::ClassType::getRealTimeAndDate);
    _d.CPGF_MD_TEMPLATE _method("getTime", &D::ClassType::getTime);
    _d.CPGF_MD_TEMPLATE _method("setTime", &D::ClassType::setTime);
    _d.CPGF_MD_TEMPLATE _method("stop", &D::ClassType::stop);
    _d.CPGF_MD_TEMPLATE _method("start", &D::ClassType::start);
    _d.CPGF_MD_TEMPLATE _method("setSpeed", &D::ClassType::setSpeed)
        ._default(copyVariantFromCopyable(1.0f))
    ;
    _d.CPGF_MD_TEMPLATE _method("getSpeed", &D::ClassType::getSpeed);
    _d.CPGF_MD_TEMPLATE _method("isStopped", &D::ClassType::isStopped);
    _d.CPGF_MD_TEMPLATE _method("tick", &D::ClassType::tick);
    _d.CPGF_MD_TEMPLATE _enum<typename D::ClassType::EWeekday>("EWeekday")
        ._element("EWD_SUNDAY", D::ClassType::EWD_SUNDAY)
        ._element("EWD_MONDAY", D::ClassType::EWD_MONDAY)
        ._element("EWD_TUESDAY", D::ClassType::EWD_TUESDAY)
        ._element("EWD_WEDNESDAY", D::ClassType::EWD_WEDNESDAY)
        ._element("EWD_THURSDAY", D::ClassType::EWD_THURSDAY)
        ._element("EWD_FRIDAY", D::ClassType::EWD_FRIDAY)
        ._element("EWD_SATURDAY", D::ClassType::EWD_SATURDAY)
    ;
    {
        GDefineMetaClass<ITimer::RealTimeDate> _nd = GDefineMetaClass<ITimer::RealTimeDate>::declare("RealTimeDate");
        _nd.CPGF_MD_TEMPLATE _field("Hour", &ITimer::RealTimeDate::Hour);
        _nd.CPGF_MD_TEMPLATE _field("Minute", &ITimer::RealTimeDate::Minute);
        _nd.CPGF_MD_TEMPLATE _field("Second", &ITimer::RealTimeDate::Second);
        _nd.CPGF_MD_TEMPLATE _field("Year", &ITimer::RealTimeDate::Year);
        _nd.CPGF_MD_TEMPLATE _field("Month", &ITimer::RealTimeDate::Month);
        _nd.CPGF_MD_TEMPLATE _field("Day", &ITimer::RealTimeDate::Day);
        _nd.CPGF_MD_TEMPLATE _field("Weekday", &ITimer::RealTimeDate::Weekday);
        _nd.CPGF_MD_TEMPLATE _field("Yearday", &ITimer::RealTimeDate::Yearday);
        _nd.CPGF_MD_TEMPLATE _field("IsDST", &ITimer::RealTimeDate::IsDST);
        _d.CPGF_MD_TEMPLATE _class(_nd);
    }
}


} // namespace meta_irrlicht




#include "cpgf/metadata/private/gmetadata_footer.h"


#endif
