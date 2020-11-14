#include<iostream>
#include<utility>
#include<type_traits>

template<typename T> void print_type()
{
    typedef typename std::remove_reference<T>::type TR;    
    std::cout << "\n" << typeid(TR).name() << " ";

    if constexpr (std::is_const<TR>::value)            std::cout << "const ";
    if constexpr (std::is_volatile<TR>::value)         std::cout << "volatile ";
    if constexpr (std::is_lvalue_reference<T>::value)  std::cout << "&";
    if constexpr (std::is_rvalue_reference<T>::value)  std::cout << "&&";
}

int&  foo_l();
int&& foo_x();
int   foo_pr();

void test_decltype()
{    
    int i = 0;
    const int ci = 0;
    int* p;

    print_type<decltype(i)>();
    print_type<decltype((i))>();
    print_type<decltype(ci)>();
    print_type<decltype((ci))>();
    print_type<decltype(p)>();
    print_type<decltype((p))>();
    print_type<decltype(static_cast<int&>(i))>();
    print_type<decltype(static_cast<int&&>(i))>();
    print_type<decltype(static_cast<int>(i))>();
    print_type<decltype(foo_l())>();
    print_type<decltype(foo_x())>();
    print_type<decltype(foo_pr())>();
    std::cout << "\n";
}

struct A{};
struct B{};
struct C{};
struct X{ void operator()() { std::cout << "\nThis is X."; }};
struct Y{ void operator()() { std::cout << "\nThis is Y."; }};
struct Z{ void operator()() { std::cout << "\nThis is Z."; }};

auto f(const A& a) { return X(); }
auto f(const B& b) { return Y(); }
auto f(const C& c) { return Z(); }

void test_declval()
{
    decltype(f(std::declval<A>())) obj0;
    decltype(f(std::declval<B>())) obj1;
    decltype(f(std::declval<C>())) obj2;
    obj0();
    obj1();
    obj2();
    std::cout << "\n";
}


void test_declval2()
{
    std::cout << "\n" << std::is_same<std::uint16_t, decltype(std::declval<std::uint32_t>() - std::declval<std::uint32_t>())>::value;
    std::cout << "\n" << std::is_same<std:: int16_t, decltype(std::declval<std::uint32_t>() - std::declval<std::uint32_t>())>::value;
    std::cout << "\n" << std::is_same<std::uint32_t, decltype(std::declval<std::uint32_t>() - 1)>::value; // true
    std::cout << "\n" << std::is_same<std:: int32_t, decltype(std::declval<std::uint32_t>() - std::declval<std::uint32_t>())>::value;
    std::cout << "\n" << std::is_same<std::uint64_t, decltype(std::declval<std::uint32_t>() - std::declval<std::uint32_t>())>::value;
    std::cout << "\n" << std::is_same<std:: int64_t, decltype(std::declval<std::uint32_t>() - std::declval<std::uint32_t>())>::value;
}
