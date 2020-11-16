#include<iostream>
#include<type_traits>

class base {};
class derived : public base {};
class non_derived {};

template<typename T, typename std::enable_if<!std::is_base_of<base, T>::value, int>::type = 0> void f(const T&)
{
    std::cout << "\nf : non-derived class implementation";
}
template<typename T, typename std::enable_if<std::is_base_of<base, T>::value, int>::type = 0> void f(const T&)
{
    std::cout << "\nf : derived class implementation";
}

template<typename T> void g(const T&)
{
    std::cout << "\ng : non-derived class implementation";
}
template<std::derived_from<base> T> void g(const T&)
{
    std::cout << "\ng : derived class implementation";
}
void test_sfinae_function()
{
    derived x0;
    non_derived x1;
    f(x0);
    f(x1);
    g(x0);
    g(x1);
}
