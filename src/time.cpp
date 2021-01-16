#include<iostream>
#include<sstream>
#include<iomanip> // for std::put_time
#include<thread>
#include<chrono>
#include<ctime>   // for timespec

template<typename CLOCK>
std::string to_string(const std::chrono::time_point<CLOCK>& tp, bool favourite_format)
{
    std::stringstream ss;
    std::time_t tt =  std::chrono::system_clock::to_time_t(tp);
    std::tm     tm = *std::localtime(&tt); // conversion to local time

    if (favourite_format)
    {
        ss << std::put_time(&tm, "%F %T"); // 2021-01-12 13:34:45

        auto  s_since_epoch = std::chrono::duration_cast<std::chrono::    seconds>(tp.time_since_epoch()).count();
        auto ns_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(tp.time_since_epoch()).count();
        std::uint64_t ns = ns_since_epoch - s_since_epoch * 1e9;
        ss << "." << std::setfill('0') << std::setw(9) << ns; // Fortunately std::setw is not sticky.
    }
    else
    {
        ss << std::put_time(&tm, "%c %Z"); // Tue Jan 12 13:34:45 2021 HKT
    }
    return ss.str();
}

auto timespec_2_timepoint(const timespec& ts)
{
    auto dur = std::chrono::seconds(ts.tv_sec) + 
               std::chrono::nanoseconds(ts.tv_nsec);

    return std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> { dur };
}

void test_time()
{
    // [Task 1] Print time_point
    std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now(); // method 1 : std::chrono
    timespec ts0; clock_gettime(CLOCK_REALTIME,  &ts0);                                       // method 2 : clock_gettime
    timespec ts1; clock_gettime(CLOCK_MONOTONIC, &ts1);                                       // method 2 : clock_gettime (doesnt work)

    std::cout << "\ncurrent time = " << to_string(tp, true);
    std::cout << "\ncurrent time = " << to_string(tp, false);
    std::cout << "\ncurrent time = " << to_string(timespec_2_timepoint(ts0), true)  << " [correct]";
    std::cout << "\ncurrent time = " << to_string(timespec_2_timepoint(ts0), false) << " [correct]";
    std::cout << "\ncurrent time = " << to_string(timespec_2_timepoint(ts1), true)  << " [incorrect]";
    std::cout << "\ncurrent time = " << to_string(timespec_2_timepoint(ts1), false) << " [incorrect]";
    std::cout << "\n";

    // [Task 2] Second and sub-second starting from Epoch
    auto  s_since_epoch = std::chrono::duration_cast<std::chrono::     seconds>(tp.time_since_epoch());
    auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    auto us_since_epoch = std::chrono::duration_cast<std::chrono::microseconds>(tp.time_since_epoch());
    auto ns_since_epoch = std::chrono::duration_cast<std::chrono:: nanoseconds>(tp.time_since_epoch());
    std::uint64_t ms = ms_since_epoch.count() - s_since_epoch.count() * 1e3;
    std::uint64_t us = us_since_epoch.count() - s_since_epoch.count() * 1e6;
    std::uint64_t ns = ns_since_epoch.count() - s_since_epoch.count() * 1e9;
    std::cout << "\ntime point in ms = " << ms; // 3 digits integer
    std::cout << "\ntime point in us = " << us; // 6 digits integer
    std::cout << "\ntime point in ns = " << ns; // 9 digits integer
    std::cout << "\n";

    // [Task 3] Second and sub-second between two time_points
    std::chrono::time_point<std::chrono::system_clock> t0 = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    std::chrono::time_point<std::chrono::system_clock> t1 = std::chrono::system_clock::now();

    std::cout << "\ntime elapsed in ms = " << std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();
    std::cout << "\ntime elapsed in us = " << std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
    std::cout << "\ntime elapsed in ns = " << std::chrono::duration_cast<std::chrono:: nanoseconds>(t1-t0).count();
    std::cout << "\n";
}

