#include<iostream>
#include<sstream>
#include<csignal>
#include<boost/stacktrace.hpp>

struct my_exception : public std::exception
{
    my_exception(const std::string& s) : message(s) {}

    const char* what() const throw() 
    {
        return message.c_str();
    }

    std::string message;
};

void handler(int signal_num) 
{
    std::cout << "\nsignal handler " << signal_num << std::flush;
    ::signal(signal_num    , SIG_DFL);
    boost::stacktrace::safe_dump_to("./backtrace.dump");
    ::raise(SIGABRT);
}

void set_handlers()
{
    signal(SIGABRT, handler); // when invoke std::abort() std::terminate()
    signal(SIGSEGV, handler); 
    signal(SIGFPE,  handler);    
    signal(SIGINT,  handler); // when user typing Ctrl-C
    signal(SIGTERM, handler); // when user typing kill-15
}

void f4() 
{
    std::stringstream ss;
    ss << "\n" << boost::stacktrace::stacktrace();
    throw my_exception(ss.str());
}
void f3() { f4(); }
void f2() { f3(); }
void f1() { f2(); }
void f0() { f1(); }

void test_error_handling()
{
    set_handlers();
    try
    {
        f0();
    }
    catch(std::exception& e)
    {
        std::cout << "\nexception : " << e.what();    
        std::set_terminate([]()
        {
            std::cout << "\nterminate handler"; 
        });
        std::terminate();
    }    
}

void test_error_handling2()
{
    
}

