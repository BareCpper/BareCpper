#ifndef BARECPPER_TIME_HPP
#define BARECPPER_TIME_HPP

#include <ctime>
#include <cassert>
#include <cstdint>
#include <tuple>
#include <type_traits> //< std::underlying_type

namespace BareCpper {
    namespace Chrono {

        enum class Weekday
        {
              Sunday = 0
            , Monday = 1
            , Tuesday = 2
            , Wednesday = 3
            , Thursday = 4
            , Friday = 5
            , Saturday = 6
        };

        struct Time
        {
            /** Unsigned seconds since Epoch at 1 January 2000 (NTP_Epoch + 100years)
            * @remark Rolls over every 2^32 seconds (~136 years)
            * @note (2^32)/31536000 = 136.1925 years
            */
            uint32_t seconds;
        };

        /** Epoch of January 1, 2000
        */
        struct J2000Epoch
        {
            /** Seconds from NTP-Epoch  of January 1, 1900
            @note 24 leap-years between 1900-2000
                (100*365 + 24)*(60*60*24) = 3155673600 seconds
            @warning Unix 64Bit time-stamp must be used as dates beyond 7th February 2036 cannot
                     be uniquely-represented with Unsigned-32-bit NTP timestamps
            */
            static const uint32_t cNtpEpochOffset = 3155673600U;

            /** Offset from Posix (Unix) Epoch
            @note 7 leap-years between 1900-2000
                (30*365 + 7)*(60*60*24) = 946684800 seconds
            @warning Unix 64Bit time-stamp must be used as dates beyond 19th January 2038 cannot
                    be uniquely-represented under Signed-32-bit Unix timestamps
            */
            static const uint32_t cUnixEpochOffset = 946684800U;

            /** Weekday from days
             * @note J2000 Epoch is on Saturday
             * @return days since Sunday – [0, 6]
            */
            static constexpr Weekday daysSinceSunday(uint32_t daysSinceEpoch ) noexcept
            {
                return static_cast<Weekday>((daysSinceEpoch + std::underlying_type_t<Weekday>(Weekday::Saturday) ) % 7);
            }

            static constexpr uint32_t cYearsInEra = 400;
            static constexpr uint32_t cDaysInEra = 146097;

            /** Returns number of days since civil 2000-01-01. 
            */
            static constexpr uint32_t daysFromCivil(uint16_t year, uint8_t month, uint8_t day) noexcept
            {
                /// @note Move March 1 is the first day of the year i.e so Feb. 29 as the last day of the year, or actually the preceding year. 
                /// @note Era is 400 years (146097  days) after which the civil calendar exactly repeats each period!
                ///       + Calculate from start of Era at 1600-03-01
                year -= 1600 + (month < 3);
                month += (month < 3 ? 9 : -3);
                const int_least8_t era = year / cYearsInEra;
                const uint_least16_t yearOfEra = static_cast<unsigned>(year - (era * cYearsInEra));      // [0, 399]
                const uint_least16_t dayOfYear = ((153 * month) + 2) / 5 + (day - 1);  // [0, 365] @see http://howardhinnant.github.io/date_algorithms.html#days_from_civil
                const uint32_t dayOfEra = (static_cast<uint32_t>(yearOfEra) * 365U) + (yearOfEra / 4U) - (yearOfEra / 100U) + dayOfYear;         // [0, 146096]
                return ((era-1) * cDaysInEra) + static_cast<uint32_t>(dayOfEra) + 60; //< 60 = days since 0 era 2000-03-01
            }

            static constexpr std::tuple<uint16_t,uint8_t,uint8_t> civilFromDays(uint32_t daysSinceEpoch) noexcept
            {
                daysSinceEpoch += cDaysInEra - 60; ///< 60 = days since 0 era 2000-03-01
                const int_least8_t era = daysSinceEpoch / cDaysInEra;
                const uint32_t dayOfEra = daysSinceEpoch - era * cDaysInEra; // [0, 146096]
                const uint_least16_t yearOfEra = (dayOfEra - (dayOfEra / 1460) + (dayOfEra / 36524) - (dayOfEra / 146096)) / 365;  // [0, 399]
                const uint_least16_t year = static_cast<uint16_t>(yearOfEra) + 1600 + era * cYearsInEra;
                const uint_least16_t dayOfYear = dayOfEra - (365 * yearOfEra + yearOfEra / 4 - yearOfEra / 100); // [0, 365]
                const uint_least8_t monthInt = (5 * dayOfYear + 2) / 153;  // [0, 11]/[Mar, Feb]
                const uint_least8_t day = dayOfYear - (153 * monthInt + 2) / 5 + 1; // [1, 31]
                const uint_least8_t month = monthInt + (monthInt < 10 ? 3 : -9); // [1, 12]
                return std::tuple<uint16_t,uint8_t,uint8_t>(year + (month < 3), month, day);
            }

        };
        static_assert(J2000Epoch::daysFromCivil(2000, 1, 1) == 0, "2000-01-01 is day 0");
        static_assert(J2000Epoch::daysFromCivil(2020, 9, 8) == 7556, "2020-09-08 is day 7556");
        static_assert(J2000Epoch::daysFromCivil(2320, 12, 30) == 117241, "2320-12-30 is day 117241");
        static_assert(J2000Epoch::civilFromDays(0) == std::make_tuple(2000, 1, 1), "2000-01-01 is day 0");
        static_assert(J2000Epoch::civilFromDays(7556) == std::make_tuple(2020, 9, 8), "2020-09-08 is day 7556");
        static_assert(J2000Epoch::civilFromDays(117241) == std::make_tuple(2320, 12, 30), "2020-09-08 is day 7556");
        static_assert(J2000Epoch::daysSinceSunday(0) == Weekday::Saturday, "2000-01-01 is a Saturday");
        static_assert(J2000Epoch::daysSinceSunday(7556) == Weekday::Tuesday, "2020-09-08 is a Tuesday");
        static_assert(J2000Epoch::daysSinceSunday(117241) == Weekday::Thursday, "2320-12-30 is a Thursday");

        template<typename Epoch>
        std::tm makeUtc(RealtimeClock::time_point timePoint)
        { return makeUtc<Epoch>(RealtimeClock::epochOffset(), timePoint); }
        
        template<typename Epoch>
        constexpr std::tm makeUtc(RealtimeClock::epoch_time_point epochOffset, RealtimeClock::time_point timePoint)
        { return makeUtc<Epoch>(epochOffset + timePoint.time_since_epoch()); }

        /** Converts time since epoch to calendar time expressed as Universal Coordinated Time
        * @note Equivalent to std::gmtime @see https://en.cppreference.com/w/cpp/chrono/c/gmtime
        */
        template<typename Epoch>
        constexpr std::tm makeUtc( RealtimeClock::epoch_time_point timePoint )
        {
            using namespace std;
            using namespace std::chrono;
            using Days = duration<int, ratio_multiply<hours::period, ratio<24>>>;
        
            RealtimeClock::epoch_duration timeSinceEpoch = timePoint.time_since_epoch(); ///< Internal time-point from RealtimeClock::epoch()
            
            // t is time duration since Epoch (e.g.1970-01-01 or 2000-01-01)  
            Days daysSinceEpoch = std::chrono::duration_cast<Days>(timeSinceEpoch); // d is days since Epoch (e.g.1970-01-01 or 2000-01-01)      
            timeSinceEpoch -= daysSinceEpoch; // t is now time duration since midnight of day

            const auto [year,month,day] = Epoch::civilFromDays(daysSinceEpoch.count()); // break d down into year/month/day
        
            std::tm tm = {0}; // start filling in the tm with calendar info
            tm.tm_year = year - 1900;
            tm.tm_mon = month - 1;
            tm.tm_mday = day;
            tm.tm_wday = Epoch::daysSinceSunday(daysSinceEpoch.count());
            tm.tm_yday = 0; //< @todo Calculated within Epoch::civilFromDays   

            // Fill in the time
            tm.tm_hour = duration_cast<hours>(timeSinceEpoch).count();
            timeSinceEpoch -= hours(tm.tm_hour);

            tm.tm_min = duration_cast<minutes>(timeSinceEpoch).count();
            timeSinceEpoch -= minutes(tm.tm_min);

            tm.tm_sec = duration_cast<seconds>(timeSinceEpoch).count();
            return tm;
        }

    } //END: Chrono
} //END: BareCpper

#endif