/*
Copyright 2025-2026 ZiYuan Yang

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
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
