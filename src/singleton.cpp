#include<iostream>
#include<vector>
#include<thread>
#include<mutex>
#include<atomic>

template<typename T>
void print_singleton(int n, const T& s)
{
    std::cout << "\nsingleton" << n << " = " << s.a << ", " << s.b << ", " << s.c;
}

class singleton0 
{
private:
    singleton0() = default;
    ~singleton0() = default;
    singleton0(const singleton0&) = delete;
    singleton0& operator=(const singleton0&) = delete;

public:
    static singleton0& get_instance()
    {
        static singleton0 x;
        return x;
    }

    int a = 0;
    int b = 0;
    int c = 0;
};

class singleton1
{
private:
    singleton1() = default;
    ~singleton1() = default;
    singleton1(const singleton1&) = delete;
    singleton1& operator=(const singleton1&) = delete;

public:
    static singleton1& get_instance()
    {
        std::call_once(flag, []() { px = new singleton1; });        
        return *px;
    }

    static singleton1* px;
    static std::once_flag flag;

    int a = 0;
    int b = 0;
    int c = 0;
};

singleton1*    singleton1::px = nullptr;
std::once_flag singleton1::flag;

class singleton2
{
private:
    singleton2() = default;
    ~singleton2() = default;
    singleton2(const singleton2&) = delete;
    singleton2& operator=(const singleton2&) = delete;

public:
    static singleton2& get_instance()
    {
        std::lock_guard<std::mutex> lock(m);
        if (!px) { px = new singleton2; };
        return *px;
    }

    static singleton2* px;
    static std::mutex m;

    int a = 0;
    int b = 0;
    int c = 0;
};

singleton2* singleton2::px = nullptr;
std::mutex singleton2::m;

class singleton3
{
private:
    singleton3() = default;
    ~singleton3() = default;
    singleton3(const singleton3&) = delete;
    singleton3& operator=(const singleton3&) = delete;

public:
    static singleton3& get_instance()
    {
        singleton3* px = apx.load();
        if (!px) // double checked loop pattern
        {
            std::lock_guard<std::mutex> lock(m);
            if (!px) 
            {
                px = new singleton3; 
                apx.store(px);
            };
        }
        return *px;
    }

    static std::atomic<singleton3*> apx;
    static std::mutex m;

    int a = 0;
    int b = 0;
    int c = 0;
};

std::atomic<singleton3*> singleton3::apx = nullptr;
std::mutex singleton3::m;


void test_singleton()
{    
    singleton0::get_instance().a = 10;
    singleton0::get_instance().b = 11;
    singleton0::get_instance().c = 12;
    print_singleton(0, singleton0::get_instance());

    singleton1::get_instance().a = 20;
    singleton1::get_instance().b = 21;
    singleton1::get_instance().c = 22;
    print_singleton(1, singleton1::get_instance());

    singleton2::get_instance().a = 30;
    singleton2::get_instance().b = 31;
    singleton2::get_instance().c = 32;
    print_singleton(2, singleton2::get_instance());

    singleton3::get_instance().a = 40;
    singleton3::get_instance().b = 41;
    singleton3::get_instance().c = 42;
    print_singleton(3, singleton3::get_instance());
}

void test_size()
{
    std::cout << "\nsizeof(char)      " << sizeof(char);       // 1byte  
    std::cout << "\nsizeof(short)     " << sizeof(short);      // 2bytes
    std::cout << "\nsizeof(long)      " << sizeof(long);       // 4bytes
    std::cout << "\nsizeof(long long) " << sizeof(long long);  // 8bytes
    std::cout << "\nsizeof(int)       " << sizeof(int);        // 4bytes
    std::cout << "\nsizeof(float)     " << sizeof(float);      // 4bytes = 1 bit sign +  8 bits exponential + 23 bits significand
    std::cout << "\nsizeof(double)    " << sizeof(double);     // 8bytes = 1 bit sign + 11 bits exponential + 52 bits significand
}


