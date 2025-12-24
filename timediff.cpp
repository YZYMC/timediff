#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <map>
#include <cstdlib>

using namespace std;

time_t parse_utc(const string& s) {
    if (s.size() != 14)
        throw runtime_error("Invalid time format");

    tm t{};
    t.tm_year = stoi(s.substr(0, 4)) - 1900;
    t.tm_mon  = stoi(s.substr(4, 2)) - 1;
    t.tm_mday = stoi(s.substr(6, 2));
    t.tm_hour = stoi(s.substr(8, 2));
    t.tm_min  = stoi(s.substr(10, 2));
    t.tm_sec  = stoi(s.substr(12, 2));
    t.tm_isdst = 0;

    return timegm(&t);
}

tm to_tm(time_t t) {
    tm out{};
    gmtime_r(&t, &out);
    return out;
}

bool add_year(tm& t) {
    tm tmp = t;
    tmp.tm_year += 1;
    time_t nt = timegm(&tmp);
    if (nt == -1) return false;
    t = tmp;
    return true;
}

bool add_month(tm& t) {
    tm tmp = t;
    tmp.tm_mon += 1;
    time_t nt = timegm(&tmp);
    if (nt == -1) return false;
    t = tmp;
    return true;
}

int main(int argc, char* argv[]) {
    bool reverse = false;
    bool show_all = false;
    string unit_only;
    vector<string> args;

    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        if (a == "-r") reverse = true;
        else if (a == "-a" || a == "--all") show_all = true;
        else if (a == "-u" || a == "--unit") {
            if (i + 1 >= argc) {
                cerr << "-u requires a unit\n";
                return 1;
            }
            unit_only = argv[++i];
        } else {
            args.push_back(a);
        }
    }

    if (args.empty() || args.size() > 2) {
        cerr << "Usage: timediff [-r] [-a|--all] [-u unit] <time1> [time2]\n";
        return 1;
    }

    time_t t1 = parse_utc(args[0]);
    time_t t2 = (args.size() == 2) ? parse_utc(args[1]) : time(nullptr);

    long long diff = t2 - t1;
    if (reverse) diff = -diff;

    char sign = diff >= 0 ? '+' : '-';
    long long abs_sec = llabs(diff);

    constexpr long long SEC_MIN  = 60;
    constexpr long long SEC_HOUR = 3600;
    constexpr long long SEC_DAY  = 86400;
    constexpr long long SEC_WEEK = 604800;

    /* ---------- unit-only ---------- */
    if (!unit_only.empty()) {
        map<string, long long> units = {
            {"seconds", abs_sec},
            {"minutes", abs_sec / SEC_MIN},
            {"hours",   abs_sec / SEC_HOUR},
            {"days",    abs_sec / SEC_DAY},
            {"weeks",   abs_sec / SEC_WEEK}
        };

        if (!units.count(unit_only)) {
            cerr << "Unsupported unit\n";
            return 1;
        }

        cout << sign << units[unit_only] << "\n";
        return 0;
    }

    /* ---------- all-units ---------- */
    if (show_all) {
        cout << sign << "\n";
        cout << "Seconds: " << abs_sec << "\n";
        cout << "Minutes: " << abs_sec / SEC_MIN << "\n";
        cout << "Hours:   " << abs_sec / SEC_HOUR << "\n";
        cout << "Days:    " << abs_sec / SEC_DAY << "\n";
        cout << "Weeks:   " << abs_sec / SEC_WEEK << "\n";
        return 0;
    }

    /* ---------- mixed calendar diff ---------- */
    time_t start = (diff >= 0) ? t1 : t2;
    time_t end   = (diff >= 0) ? t2 : t1;

    tm cur = to_tm(start);
    time_t cur_t = start;

    long long years = 0, months = 0;

    while (true) {
        tm tmp = cur;
        if (!add_year(tmp)) break;
        time_t nt = timegm(&tmp);
        if (nt > end) break;
        cur = tmp;
        cur_t = nt;
        years++;
    }

    while (true) {
        tm tmp = cur;
        if (!add_month(tmp)) break;
        time_t nt = timegm(&tmp);
        if (nt > end) break;
        cur = tmp;
        cur_t = nt;
        months++;
    }

    long long remain = llabs(end - cur_t);

    long long days  = remain / SEC_DAY;   remain %= SEC_DAY;
    long long hours = remain / SEC_HOUR;  remain %= SEC_HOUR;
    long long mins  = remain / SEC_MIN;   remain %= SEC_MIN;
    long long secs  = remain;

    cout << sign;
    if (years)  cout << years  << " year"  << (years  > 1 ? "s " : " ");
    if (months) cout << months << " month" << (months > 1 ? "s " : " ");
    if (days)   cout << days   << " day"   << (days   > 1 ? "s " : " ");
    if (hours)  cout << hours  << " hour"  << (hours  > 1 ? "s " : " ");
    if (mins)   cout << mins   << " minute"<< (mins   > 1 ? "s " : " ");
    if (secs)   cout << secs   << " second"<< (secs   > 1 ? "s " : " ");
    cout << "\n";

    return 0;
}
