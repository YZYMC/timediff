#pragma once
#include <string>
#include <cstdint>

namespace timediff {

struct DateTime {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
};

struct MixedDuration {
    int64_t years;
    int64_t months;
    int64_t days;
    int64_t hours;
    int64_t minutes;
    int64_t seconds;
};

/* 解析 YYYYMMDDhhmmss (UTC) */
DateTime parse_utc_string(const std::string&);

/* 当前 UTC */
DateTime now_utc();

/* 纯秒差 */
int64_t diff_seconds(const DateTime& a,
                     const DateTime& b,
                     bool reverse = false);

/* 混合历法差值 */
MixedDuration diff_mixed(const DateTime& a,
                         const DateTime& b,
                         bool reverse = false);

} // namespace timediff
