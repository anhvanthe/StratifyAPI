/*! \file */ //Copyright 2011-2018 Tyler Gilbert; All Rights Reserved

#ifndef SAPI_RTC_HPP_
#define SAPI_RTC_HPP_

#include <sos/dev/rtc.h>
#include "Periph.hpp"


namespace hal {


/*! \brief RTC Class
 * \details This class implements an RTC. (Not yet implemented)
 */
class Rtc : public Periph<rtc_info_t, rtc_attr_t, 'r'> {
public:
	Rtc(port_t port);

	enum {
        FLAG_ENABLE = RTC_FLAG_ENABLE,
        FLAG_DISABLE = RTC_FLAG_DISABLE,
        FLAG_IS_SOURCE_EXTERNAL_32768 = RTC_FLAG_IS_SOURCE_EXTERNAL_32768,
        FLAG_IS_SOURCE_INTERNAL_40000 = RTC_FLAG_IS_SOURCE_INTERNAL_40000,
        FLAG_ENABLE_ALARM = RTC_FLAG_ENABLE_ALARM,
        FLAG_DISABLE_ALARM = RTC_FLAG_DISABLE_ALARM,
        FLAG_IS_ALARM_ONCE = RTC_FLAG_IS_ALARM_ONCE,
        FLAG_IS_ALARM_MINUTE = RTC_FLAG_IS_ALARM_MINUTE,
        FLAG_IS_ALARM_HOURLY = RTC_FLAG_IS_ALARM_HOURLY,
        FLAG_IS_ALARM_DAILY = RTC_FLAG_IS_ALARM_DAILY,
        FLAG_IS_ALARM_WEEKLY = RTC_FLAG_IS_ALARM_WEEKLY,
        FLAG_IS_ALARM_MONTHLY = RTC_FLAG_IS_ALARM_MONTHLY,
        FLAG_IS_ALARM_YEARLY = RTC_FLAG_IS_ALARM_YEARLY,
        FLAG_ENABLE_COUNT_EVENT = RTC_FLAG_ENABLE_COUNT_EVENT,
        FLAG_IS_COUNT_SECOND = RTC_FLAG_IS_COUNT_SECOND,
        FLAG_IS_COUNT_MINUTE = RTC_FLAG_IS_COUNT_MINUTE,
        FLAG_IS_COUNT_HOUR = RTC_FLAG_IS_COUNT_HOUR,
        FLAG_IS_COUNT_DAY_OF_WEEK = RTC_FLAG_IS_COUNT_DAY_OF_WEEK,
        FLAG_IS_COUNT_DAY_OF_MONTH = RTC_FLAG_IS_COUNT_DAY_OF_MONTH,
        FLAG_IS_COUNT_DAY_OF_YEAR = RTC_FLAG_IS_COUNT_DAY_OF_YEAR,
        FLAG_IS_COUNT_WEEK = RTC_FLAG_IS_COUNT_WEEK,
        FLAG_IS_COUNT_MONTH = RTC_FLAG_IS_COUNT_MONTH,
        FLAG_IS_COUNT_YEAR = RTC_FLAG_IS_COUNT_YEAR,
        FLAG_DISABLE_COUNT_EVENT = RTC_FLAG_DISABLE_COUNT_EVENT,

        ENABLE /*! Set the alarm */ = RTC_FLAG_ENABLE,
        DISABLE /*! Set the alarm */ = RTC_FLAG_DISABLE,
        IS_SOURCE_EXTERNAL_32768 /*! External 32.768KHz Crystal */ = RTC_FLAG_IS_SOURCE_EXTERNAL_32768,
        IS_SOURCE_INTERNAL_40000 /*! Internal 40KHz Oscillator */ = RTC_FLAG_IS_SOURCE_INTERNAL_40000,
        ENABLE_ALARM /*! Enable the alarm */ = RTC_FLAG_ENABLE_ALARM,
        DISABLE_ALARM /*! Enable the alarm */ = RTC_FLAG_DISABLE_ALARM,
        IS_ALARM_ONCE /*! One time alarm */ = RTC_FLAG_IS_ALARM_ONCE,
        IS_ALARM_MINUTE /*! Alarm every minute */ = RTC_FLAG_IS_ALARM_MINUTE,
        IS_ALARM_HOURLY /*! Alarm every hour */ = RTC_FLAG_IS_ALARM_HOURLY,
        IS_ALARM_DAILY /*! Daily alarm */ = RTC_FLAG_IS_ALARM_DAILY,
        IS_ALARM_WEEKLY /*! Weekly alarm */ = RTC_FLAG_IS_ALARM_WEEKLY,
        IS_ALARM_MONTHLY /*! Monthly alarm */ = RTC_FLAG_IS_ALARM_MONTHLY,
        IS_ALARM_YEARLY /*! Yearly alarm */ = RTC_FLAG_IS_ALARM_YEARLY,
        ENABLE_COUNT_EVENT /*! Enable a count event */ = RTC_FLAG_ENABLE_COUNT_EVENT,
        IS_COUNT_SECOND /*! One time alarm */ = RTC_FLAG_IS_COUNT_SECOND,
        IS_COUNT_MINUTE /*! One time alarm */ = RTC_FLAG_IS_COUNT_MINUTE,
        IS_COUNT_HOUR /*! One time alarm */ = RTC_FLAG_IS_COUNT_HOUR,
        IS_COUNT_DAY_OF_WEEK /*! One time alarm */ = RTC_FLAG_IS_COUNT_DAY_OF_WEEK,
        IS_COUNT_DAY_OF_MONTH /*! One time alarm */ = RTC_FLAG_IS_COUNT_DAY_OF_MONTH,
        IS_COUNT_DAY_OF_YEAR /*! One time alarm */ = RTC_FLAG_IS_COUNT_DAY_OF_YEAR,
        IS_COUNT_WEEK /*! One time alarm */ = RTC_FLAG_IS_COUNT_WEEK,
        IS_COUNT_MONTH /*! One time alarm */ = RTC_FLAG_IS_COUNT_MONTH,
        IS_COUNT_YEAR /*! One time alarm */ = RTC_FLAG_IS_COUNT_YEAR,
        DISABLE_COUNT_EVENT /*! Enable a count event */ = RTC_FLAG_DISABLE_COUNT_EVENT,
	};

	/*! \details Set RTC time. */
    int set_time(const rtc_time_t & time) const;

	/*! \details Gets RTC time. */
    int get_time(rtc_time_t & time) const;

private:

};

}

#endif /* SAPI_RTC_HPP_ */
