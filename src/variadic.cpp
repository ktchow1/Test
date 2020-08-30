#include<iostream>
#include<type_traits>
#include<vector>

// example 1
template<typename ARG> void fct(ARG&& arg)
{
    std::cout << "\nnewline = " << std::forward<ARG>(arg) << std::flush;
}

void apply_fct() 
{
}

template<typename ARG, typename... ARGS> void apply_fct(ARG&& arg, ARGS&&... args)  
{
    fct(std::forward<ARG>(arg));
    apply_fct(std::forward<ARGS>(args)...);
}

// example 2
template<typename OS_TYPE, typename... ARGS> void printer(OS_TYPE& os, ARGS&&... args)
{
    (os << ... << std::forward<ARGS>(args));
//  (os << ... << "\n" << std::forward<ARGS>(args)); // compile error
}

// example 3
struct my_container
{
    template<typename T>
    void run(T&& x)
    {
        std::cout << "[" << std::forward<T>(x) << "] ";
    }
};

template<typename C, typename... ARGS> void batch_push_back(C& vec, ARGS&&... args)
{
    (..., vec.push_back(std::forward<ARGS>(args)));
}

template<typename C, typename... ARGS> void batch_run(C& vec, ARGS&&... args)
{
    (..., vec.run(std::forward<ARGS>(args)));
}

// example 4 sizeof... operator
template<typename... ARGS> struct compile_time_count
{
    static const int value = sizeof...(ARGS);
};

struct A{};
struct B{};
struct C{};
struct D{};
struct E{};

// ************************ //
// when ... happens in
// forward
// sizeof
// using 
// ************************ //
void test_variadic()
{    
    std::cout << "\n\nfct : ";
    apply_fct(1,2,3,3.1415,"abcdef");
    
    std::cout << "\n\nprinter : ";
    printer(std::cout,1,2,3,3.1415,"abcdef");

    std::cout << "\n\nvec : ";
    std::vector<int> vec;
    batch_push_back(vec,1,2,3,4,5,6,7,8);
    for(const auto& x:vec) std::cout << x << " ";

    std::cout << "\n\nmy_container : ";
    my_container container;
    batch_run(container,1,2,3,3.1415,"abcdef"); 

    std::cout << "\n";
    std::cout << "\ncount = " << compile_time_count<A,B,C>::value;
    std::cout << "\ncount = " << compile_time_count<A,B,C,D>::value;
    std::cout << "\ncount = " << compile_time_count<A,B,C,D,E>::value;
    std::cout << "\ncount = " << compile_time_count<A,A,B,B,B>::value;
    std::cout << "\n\n\n";
}

