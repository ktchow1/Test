#include<iostream>
#include<concepts>
#include<string>




template<typename T> concept countable  = std::is_integral<T>::value; // This is a concept.
template<typename T> using   countable2 = std::is_integral<T>::value; // This is an alias.

// We can use concept as a type.
template<countable T> struct temp {};

// We cannor use alias as a type.  
// template<countable2 T> struct temp2 {}; // compile error, countable2 is not a type.

void test_concepts()
{
    temp<std::uint8_t>  x0;
    temp<std::uint16_t> x1;
    temp<std::uint32_t> x2;
//  temp<std::string>   x3;  // compile error, non countable
//  temp<double>        x4;  // compile error, non countable  

}
