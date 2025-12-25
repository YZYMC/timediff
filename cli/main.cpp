#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>

#include <timediff/timediff.hpp>

using namespace std;
using namespace timediff;

int main(int argc, char** argv) {
    bool reverse = false;
    bool show_all = false;
    string unit_only;
    vector<string> args;

    /* ---------- 参数解析 ---------- */
    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        if (a == "-r") {
            reverse = true;
        } else if (a == "-a" || a == "--all") {
            show_all = true;
        } else if (a == "-u" || a == "--unit") {
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

    /* ---------- 时间解析 ---------- */
    DateTime t1 = parse_utc_string(args[0]);
    DateTime t2 = (args.size() == 2) ? parse_utc_string(args[1]) : now_utc();

    /* ---------- -u：独占 ---------- */
    if (!unit_only.empty()) {
        static const map<string, int64_t> div = {
            {"seconds", 1},
            {"minutes", 60},
            {"hours",   3600},
            {"days",    86400},
            {"weeks",   604800}
        };

        if (!div.count(unit_only)) {
            cerr << "Unsupported unit\n";
            return 1;
        }

        int64_t sec = diff_seconds(t1, t2, reverse);
        char sign = sec >= 0 ? '+' : '-';
        cout << sign << llabs(sec) / div.at(unit_only) << "\n";
        return 0;
    }

    int64_t sec = diff_seconds(t1, t2, reverse);
    char sign = sec >= 0 ? '+' : '-';
    int64_t s = llabs(sec);

    /* ---------- -a：等价换算 ---------- */
    if (show_all) {
        cout << sign << "\n";
        cout << "Seconds: " << s << "\n";
        cout << "Minutes: " << s / 60 << "\n";
        cout << "Hours:   " << s / 3600 << "\n";
        cout << "Days:    " << s / 86400 << "\n";
        cout << "Weeks:   " << s / 604800 << "\n";
        cout << "\nMixed:\n";
    }

    /* ---------- 默认：混合历法 ---------- */
    MixedDuration d = diff_mixed(t1, t2, reverse);

    cout << sign;
    if (d.years)   cout << llabs(d.years)   << " year"   << (llabs(d.years)   > 1 ? "s " : " ");
    if (d.months)  cout << llabs(d.months)  << " month"  << (llabs(d.months)  > 1 ? "s " : " ");
    if (d.days)    cout << llabs(d.days)    << " day"    << (llabs(d.days)    > 1 ? "s " : " ");
    if (d.hours)   cout << llabs(d.hours)   << " hour"   << (llabs(d.hours)   > 1 ? "s " : " ");
    if (d.minutes) cout << llabs(d.minutes) << " minute" << (llabs(d.minutes) > 1 ? "s " : " ");
    if (d.seconds) cout << llabs(d.seconds) << " second" << (llabs(d.seconds) > 1 ? "s " : " ");
    cout << "\n";

    return 0;
}
