#include<iostream>
#include<type_traits>


// ******************************************* //
// *** Implementation of is_base_of traits *** //
// ******************************************* //
template <typename B> std::false_type is_ptr_convertible_to(const void*);
template <typename B> std::true_type  is_ptr_convertible_to(const B*);
template <typename B, typename D>
struct is_base_of : public decltype(is_ptr_convertible_to<B>(static_cast<D*>(nullptr)))
{
};

class A {};
class B : public A {};
class C {};

void test_sfinae_is_base_implement()
{
    std::cout << "\n" << std::boolalpha << is_base_of<A,B>::value; // true
    std::cout << "\n" << std::boolalpha << is_base_of<B,A>::value; 
    std::cout << "\n" << std::boolalpha << is_base_of<B,C>::value;
    std::cout << "\n" << std::boolalpha << is_base_of<C,C>::value;
}

// ******************************* //
// *** Using is_base_of traits *** //
// ******************************* //
class base {};
class derived : public base {};
class non_derived {};

template<typename T, typename std::enable_if<!is_base_of<base, T>::value, int>::type = 0> void f(const T&)
{
    std::cout << "\nf : non-derived class implementation";
}
template<typename T, typename std::enable_if<is_base_of<base, T>::value, int>::type = 0> void f(const T&)
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
void test_sfinae_is_base_usage()
{
    derived x0;
    non_derived x1;
    f(x0);
    f(x1);
    g(x0);
    g(x1);
}
