#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <vector>
#include <cstdlib>

using namespace std;

/* 解析 YYYYMMDDhhmmss (UTC) */
time_t parse_utc(const string& s) {
    if (s.size() != 14) {
        throw runtime_error("Invalid time format");
    }

    tm t{};
    t.tm_year = stoi(s.substr(0, 4)) - 1900;
    t.tm_mon  = stoi(s.substr(4, 2)) - 1;
    t.tm_mday = stoi(s.substr(6, 2));
    t.tm_hour = stoi(s.substr(8, 2));
    t.tm_min  = stoi(s.substr(10, 2));
    t.tm_sec  = stoi(s.substr(12, 2));
    t.tm_isdst = 0;

    // 强制按 UTC 解释
    return timegm(&t);
}

int main(int argc, char* argv[]) {
    bool reverse = false;
    vector<string> args;

    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        if (a == "-r") {
            reverse = true;
        } else {
            args.push_back(a);
        }
    }

    if (args.empty() || args.size() > 2) {
        cerr << "Usage: timediff [-r] <time1> [time2]\n";
        return 1;
    }

    try {
        time_t t1 = parse_utc(args[0]);
        time_t t2;

        if (args.size() == 2) {
            t2 = parse_utc(args[1]);
        } else {
            t2 = time(nullptr); // 当前 UTC
        }

        long long diff = static_cast<long long>(t2 - t1);
        if (reverse) diff = -diff;

        char sign = (diff >= 0) ? '+' : '-';
        long long seconds = llabs(diff);

        constexpr long long SEC_MIN  = 60;
        constexpr long long SEC_HOUR = 60 * 60;
        constexpr long long SEC_DAY  = 24 * SEC_HOUR;
        constexpr long long SEC_WEEK = 7 * SEC_DAY;
        constexpr long long SEC_MONTH = 30 * SEC_DAY;
        constexpr long long SEC_YEAR  = 365 * SEC_DAY;

        long long years  = seconds / SEC_YEAR;  seconds %= SEC_YEAR;
        long long months = seconds / SEC_MONTH; seconds %= SEC_MONTH;
        long long weeks  = seconds / SEC_WEEK;  seconds %= SEC_WEEK;
        long long days   = seconds / SEC_DAY;   seconds %= SEC_DAY;
        long long hours  = seconds / SEC_HOUR;  seconds %= SEC_HOUR;
        long long mins   = seconds / SEC_MIN;   seconds %= SEC_MIN;
        long long secs   = seconds;

        cout << sign;

        auto print_unit = [](long long v, const char* name) {
            if (v > 0)
                cout << v << " " << name << (v > 1 ? "s " : " ");
        };

        print_unit(years,  "year");
        print_unit(months, "month");
        print_unit(weeks,  "week");
        print_unit(days,   "day");
        print_unit(hours,  "hour");
        print_unit(mins,   "minute");
        print_unit(secs,   "second");

        cout << "\n";

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
