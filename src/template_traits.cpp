#include<iostream>
#include<vector>
#include<list>
#include<string>

// *********************** //
// *** Template traits *** //
// *********************** //
struct typeA {};
struct typeB {};
struct typeC {};
std::ostream& operator<<(std::ostream& os, const typeA&) { os << "A"; return os; }
std::ostream& operator<<(std::ostream& os, const typeB&) { os << "B"; return os; }
std::ostream& operator<<(std::ostream& os, const typeC&) { os << "C"; return os; }

// 1. class template (for define-value and define-type)
template<typename T> struct ctmp                             { static const bool value = false;  typedef typeA type; };
template<>           struct ctmp<std::uint16_t>              { static const bool value = true;   typedef typeB type; };
template<>           struct ctmp<std::uint32_t>              { static const bool value = true;   typedef typeC type; };
template<>           struct ctmp<std::vector<std::uint16_t>> { static const bool value = true;   typedef typeB type; };
template<>           struct ctmp<std::vector<std::uint32_t>> { static const bool value = true;   typedef typeC type; };

// 2. variable template (for define-value and mapped-value)
template<typename T> bool vtmp                             = false;
template<>           bool vtmp<std::uint16_t>              = true;
template<>           bool vtmp<std::uint32_t>              = true;
template<>           bool vtmp<std::vector<std::uint16_t>> = true;
template<>           bool vtmp<std::vector<std::uint32_t>> = true;
template<typename T> bool vtmp0 = ctmp<T>::value;

// 3. alias template (for defined-type and mapped-type)
template<typename T> using atmp  = typeA; /*
template<>           using atmp<std::uint16_t>              = typeB; // specialization does not work
template<>           using atmp<std::uint32_t>              = typeC;
template<>           using atmp<std::vector<std::uint16_t>> = typeB;
template<>           using atmp<std::vector<std::uint32_t>> = typeC;  */
template<typename T> using atmp0 = ctmp<T>::type;

void test_template_traits()
{
    // *** Application 1 *** //
    std::cout << "\nclass template (define-value) : ";
    std::cout << ctmp<bool>::value;
    std::cout << ctmp<std::uint8_t>::value;
    std::cout << ctmp<std::uint16_t>::value;
    std::cout << ctmp<std::uint32_t>::value;
    std::cout << ctmp<std::uint64_t>::value;
    std::cout << ctmp<std::vector<std::uint8_t>>::value;
    std::cout << ctmp<std::vector<std::uint16_t>>::value;
    std::cout << ctmp<std::vector<std::uint32_t>>::value;
    std::cout << ctmp<std::vector<std::uint64_t>>::value;
    std::cout << ctmp<std::string>::value;

    std::cout << "\nclass template (define-type) : ";
    std::cout << typename ctmp<bool>::type{};
    std::cout << typename ctmp<std::uint8_t>::type{};
    std::cout << typename ctmp<std::uint16_t>::type{};
    std::cout << typename ctmp<std::uint32_t>::type{};
    std::cout << typename ctmp<std::uint64_t>::type{};
    std::cout << typename ctmp<std::vector<std::uint8_t>>::type{};
    std::cout << typename ctmp<std::vector<std::uint16_t>>::type{};
    std::cout << typename ctmp<std::vector<std::uint32_t>>::type{};
    std::cout << typename ctmp<std::vector<std::uint64_t>>::type{};
    std::cout << typename ctmp<std::string>::type{};

    std::cout << "\nvariable template (define-value) : ";
    std::cout << vtmp<bool>;
    std::cout << vtmp<std::uint8_t>;
    std::cout << vtmp<std::uint16_t>;
    std::cout << vtmp<std::uint32_t>;
    std::cout << vtmp<std::uint64_t>;
    std::cout << vtmp<std::vector<std::uint8_t>>;
    std::cout << vtmp<std::vector<std::uint16_t>>;
    std::cout << vtmp<std::vector<std::uint32_t>>;
    std::cout << vtmp<std::vector<std::uint64_t>>;
    std::cout << vtmp<std::string>;

    std::cout << "\nvariable template (mapped-value) : ";
    std::cout << vtmp0<bool>;
    std::cout << vtmp0<std::uint8_t>;
    std::cout << vtmp0<std::uint16_t>;
    std::cout << vtmp0<std::uint32_t>;
    std::cout << vtmp0<std::uint64_t>;
    std::cout << vtmp0<std::vector<std::uint8_t>>;
    std::cout << vtmp0<std::vector<std::uint16_t>>;
    std::cout << vtmp0<std::vector<std::uint32_t>>;
    std::cout << vtmp0<std::vector<std::uint64_t>>;
    std::cout << vtmp0<std::string>;

    std::cout << "\nalias template (mapped-value) : ";
    std::cout << atmp<bool>{};
    std::cout << atmp<std::uint8_t>{};
    std::cout << atmp<std::uint16_t>{};
    std::cout << atmp<std::uint32_t>{};
    std::cout << atmp<std::uint64_t>{};
    std::cout << atmp<std::vector<std::uint8_t>>{};
    std::cout << atmp<std::vector<std::uint16_t>>{};
    std::cout << atmp<std::vector<std::uint32_t>>{};
    std::cout << atmp<std::vector<std::uint64_t>>{};
    std::cout << atmp<std::string>{};

    std::cout << "\nalias template (mapped-type) : ";
    std::cout << atmp0<bool>{};
    std::cout << atmp0<std::uint8_t>{};
    std::cout << atmp0<std::uint16_t>{};
    std::cout << atmp0<std::uint32_t>{};
    std::cout << atmp0<std::uint64_t>{};
    std::cout << atmp0<std::vector<std::uint8_t>>{};
    std::cout << atmp0<std::vector<std::uint16_t>>{};
    std::cout << atmp0<std::vector<std::uint32_t>>{};
    std::cout << atmp0<std::vector<std::uint64_t>>{};
    std::cout << atmp0<std::string>{};
    std::cout << "\n";
}

// ************************* //
// *** Variable template *** //
// ************************* //
template<typename T> std::vector<T> var_template = {1,2,3,4,5};
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
    os << "[";
    for(const auto& x:vec) os << x << ",";
    os << "]";
    return os;
}

struct A
{
    A(std::uint32_t x) : m(x) {}
    std::uint32_t m;
};
std::ostream& operator<<(std::ostream& os, const A& a)
{
    os << "(lvalue-A" << a.m << ")";
    return os;
}
std::ostream& operator<<(std::ostream& os, A&& a)
{
    os << "(rvalue-A" << a.m << ")";
    return os;
}

void test_template_VT()
{
    std::cout << "\ncount_vector = " << var_template<std::uint16_t>;
    std::cout << "\ncount_vector = " << var_template<std::uint32_t>;
    std::cout << "\ncount_vector = " << var_template<std::uint16_t>;
    std::cout << "\ncount_vector = " << var_template<double>;
    std::cout << "\ncount_vector = " << var_template<A>;
}

// ************************************* //
// *** Abbreviated function template *** //
// ************************************* //
void abbv_fct_template(const auto& x, auto&& y) // support universal reference
{
    // Good! How can we forward auto universal reference? What should we put inside std::forward<...>?
    std::cout << "\nx = " << x << ", y = " << std::forward<decltype(y)>(y);
}

void test_template_AFT()
{
    A a(345);

    abbv_fct_template(1,a);
    abbv_fct_template(1.23,a);
    abbv_fct_template(std::string("abcde"),a);
    abbv_fct_template(1, A{456});
    abbv_fct_template(1.23, A{567});
    abbv_fct_template(std::string("abcde"), A{678});
    std::cout << "\n";
}

// Four useful traits
// std::is_invocable            <F,ARGS>
// std::is_invocable_r        <R,F,ARGS> // R = return type
// std::is_nothrow_invocable    <F,ARGS>
// std::is_nothrow_invocable_r<R,F,ARGS> // R = return type
// std::invocable is concept

#include<concepts>
// ************************************************************************************************************** //
// This is a abbv function template using auto (with concept).
// With auto, it accepts all function pointer, lambda, std::function by keeping their own types.
// Without auto, we need to use std::function as arg type, everything is convert to std::function, which is slow.
// ************************************************************************************************************** //
void execute(std::invocable<std::uint32_t, const std::vector<std::uint32_t>&> auto callback, auto init, const auto& data)
{
    std::cout << "\nexecute " << callback(init, data);
}

auto fct(std::uint32_t s, const std::vector<std::uint32_t>& v)
{
    std::uint32_t sum = s;
    for(const auto& x:v) s+=x;
    return s;
}

void test_template_AFT_invocable()
{
    std::vector<std::uint32_t> vec{1,2,3,4,5,6,7,8,9,10};

    execute(fct, 100, vec);
    execute([](std::uint32_t s, const std::vector<std::uint32_t>& v)
    {
        std::uint32_t sum = s;
        for(const auto& x:v) s+=x;
        return s;
    },
    100, std::vector<std::uint32_t>{1,2,3,4,5,6,7,8,9,10});
}
