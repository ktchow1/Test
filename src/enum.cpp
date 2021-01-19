#include<iostream>
#include<type_traits>
 
enum enum0       { value0, value1, value2 };
enum class enum1 { value0, value1, value2 }; // enum class are scoped enum, thus no ambiguity
enum class enum2 : std::uint16_t { value0, value1, value2 };
enum class enum3 : std::uint32_t { value0, value1, value2 };
enum class enum4 : std::uint64_t { value0, value1, value2 };

/*
enum class enum4 : public std::uint64_t {value0, value1, value2 }; 
...  compile error, cannot declare public, why? 
*/

template<typename ENUM>
void print_type()
{
    std::cout << "\nenum is_same : " << std::is_same_v<std::underlying_type_t<ENUM>, std::uint8_t>
                                     << std::is_same_v<std::underlying_type_t<ENUM>, std::uint16_t>
                                     << std::is_same_v<std::underlying_type_t<ENUM>, std::uint32_t>
                                     << std::is_same_v<std::underlying_type_t<ENUM>, std::uint64_t>;
}

void fct(const enum0& x) { std::cout << "\nenum0 value = " << x + 10; } /*   // compile ok,    can    convert enum       to int
void fct(const enum1& x) { std::cout << "\nenum1 value = " << x + 10; }  */  // compile error, cannot convert enum-class to int 
void fct(const enum1& x) { std::cout << "\nenum1 value = " << static_cast<int>(x); }
void fct(const enum2& x) { std::cout << "\nenum2 value = " << static_cast<int>(x); }
void fct(const enum3& x) { std::cout << "\nenum3 value = " << static_cast<int>(x); }
void fct(const enum4& x) { std::cout << "\nenum4 value = " << static_cast<int>(x); }

void test_enum()
{
    print_type<enum0>();         
    print_type<enum1>();         
    print_type<enum2>();         
    print_type<enum3>();         
    print_type<enum4>();         
    
//  fct(0); // compile error, cannot convert int to enum
    fct(value0);
    fct(value1);
    fct(value2);
//  fct(0); // compile error, cannot convert int to enum-class
    fct(enum1::value0);
    fct(enum1::value1);
    fct(enum1::value2);
}
