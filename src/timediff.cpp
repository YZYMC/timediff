/*
Copyright 2025-2026 ZiYuan Yang

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#include <timediff/timediff.hpp>

#include <ctime>
#include <stdexcept>
#include <cstdlib>

namespace timediff {

/* ---------- 内部工具函数（不进头文件） ---------- */

static time_t to_time_t(const DateTime& d) {
    std::tm t{};
    t.tm_year = d.year - 1900;
    t.tm_mon  = d.month - 1;
    t.tm_mday = d.day;
    t.tm_hour = d.hour;
    t.tm_min  = d.minute;
    t.tm_sec  = d.second;
    t.tm_isdst = 0;
    return timegm(&t);
}

static DateTime from_time_t(time_t t) {
    std::tm out{};
    gmtime_r(&t, &out);
    return {
        out.tm_year + 1900,
        out.tm_mon + 1,
        out.tm_mday,
        out.tm_hour,
        out.tm_min,
        out.tm_sec
    };
}

/* ---------- API 实现 ---------- */

DateTime parse_utc_string(const std::string& s) {
    if (s.size() != 14)
        throw std::runtime_error("Invalid time format");

    return {
        std::stoi(s.substr(0, 4)),
        std::stoi(s.substr(4, 2)),
        std::stoi(s.substr(6, 2)),
        std::stoi(s.substr(8, 2)),
        std::stoi(s.substr(10, 2)),
        std::stoi(s.substr(12, 2))
    };
}

DateTime now_utc() {
    return from_time_t(std::time(nullptr));
}

int64_t diff_seconds(const DateTime& a,
                     const DateTime& b,
                     bool reverse) {
    int64_t diff = to_time_t(b) - to_time_t(a);
    return reverse ? -diff : diff;
}

/* ---------- 混合历法差值 ---------- */

MixedDuration diff_mixed(const DateTime& a,
                         const DateTime& b,
                         bool reverse) {
    time_t ta = to_time_t(a);
    time_t tb = to_time_t(b);

    bool neg = tb < ta;
    if (reverse) neg = !neg;

    time_t start = neg ? tb : ta;
    time_t end   = neg ? ta : tb;

    std::tm cur{};
    gmtime_r(&start, &cur);
    time_t cur_t = start;

    int64_t years = 0;
    int64_t months = 0;

    /* 年 */
    while (true) {
        std::tm tmp = cur;
        tmp.tm_year += 1;
        time_t nt = timegm(&tmp);
        if (nt > end) break;
        cur = tmp;
        cur_t = nt;
        years++;
    }

    /* 月 */
    while (true) {
        std::tm tmp = cur;
        tmp.tm_mon += 1;
        time_t nt = timegm(&tmp);
        if (nt > end) break;
        cur = tmp;
        cur_t = nt;
        months++;
    }

    int64_t remain = end - cur_t;

    constexpr int64_t DAY  = 86400;
    constexpr int64_t HOUR = 3600;
    constexpr int64_t MIN  = 60;

    MixedDuration d{};
    d.years   = years;
    d.months  = months;
    d.days    = remain / DAY;   remain %= DAY;
    d.hours   = remain / HOUR;  remain %= HOUR;
    d.minutes = remain / MIN;   remain %= MIN;
    d.seconds = remain;

    if (neg) {
        d.years   = -d.years;
        d.months  = -d.months;
        d.days    = -d.days;
        d.hours   = -d.hours;
        d.minutes = -d.minutes;
        d.seconds = -d.seconds;
    }

    return d;
}

} // namespace timediff
