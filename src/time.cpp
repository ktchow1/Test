#include<iostream>
#include<sstream>
#include<iomanip> // for std::put_time
#include<thread>
#include<chrono>
#include<ctime>   // for timespec

/*
timespec is a struct with two integer members : tv_sec and tv_nsec
timeval  is similar  with two integer members : tv_sec and tv_usec

std::chrono::duration is a count plus a unit
std::chrono::duration supports seconds / milliseconds / microseconds / nanoseconds (can be casted)

std::chrono::time_point implementation is encapsulated.
std::chrono::time_point can be converted into duration in 2 ways :
(1) difference between EPOCH (1970 Jan 01) and time point is given by member time_since_epoch()
(2) difference between two time_point returns duration

Printing duration is easy, by getting count().
Printing time_point is complicated, see function below.
*/

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
//  auto dur = std::chrono::seconds(ts.tv_sec) + 
//             std::chrono::nanoseconds(ts.tv_nsec);
    auto dur = std::chrono::nanoseconds{static_cast<std::uint64_t>(ts.tv_sec * 1'000'000'000U) + ts.tv_nsec};

    return std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> { dur };
}

void test_time()
{
    // [Task 1] Print time_point
    std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::now(); // method 1 : std::chrono
    timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts);                                         // method 2 : clock_gettime

    std::cout << "\ncurrent time = " << to_string(tp, true);
    std::cout << "\ncurrent time = " << to_string(tp, false);
    std::cout << "\ncurrent time = " << to_string(timespec_2_timepoint(ts), true)  << " [incorrect]";
    std::cout << "\ncurrent time = " << to_string(timespec_2_timepoint(ts), false) << " [incorrect]";
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

