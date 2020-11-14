#include<iostream>
#include<thread>
#include<time.h>

std::uint64_t to_nanosec(const timespec& ts)
{
    const std::uint64_t ONE_SEC = 1000000000ULL;
    return ts.tv_sec * ONE_SEC + ts.tv_nsec;
}

std::int64_t time_interval(const timespec& ts0, const timespec& ts1)
{
    const std::uint64_t ONE_SEC  = 1000000000ULL;
    std::int64_t diff = 0;

    diff = (ts1.tv_sec * ONE_SEC + ts1.tv_nsec) -
           (ts0.tv_sec * ONE_SEC + ts0.tv_nsec);
    return diff;
}

void timer_resolution(std::uint32_t N)
{
    for(std::uint32_t n=0; n!=N; ++n)
    {
        timespec ts0;
        timespec ts1;
        clock_gettime(CLOCK_MONOTONIC, &ts0);
        clock_gettime(CLOCK_MONOTONIC, &ts1);
        if (n%10==0) std::cout << "\n" << time_interval(ts0,ts1);
        else         std::cout << ", " << time_interval(ts0,ts1);
    }
}

void test_timer_resolution()
{
    while(true)
    {
        timer_resolution(100);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        std::cout << "\n";
    }
}


