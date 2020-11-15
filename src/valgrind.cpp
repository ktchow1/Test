#include<iostream>
#include<vector>
#include<optional>
#include<valgrind/callgrind.h>

void test_valgrind()
{
    std::vector<std::uint32_t> vec;
    vec.push_back(123UL);
    vec.push_back(123UL);
    vec.push_back(123UL);
    vec.push_back(123UL);
    vec.push_back(123UL);
    vec.push_back(123UL);
    for(const auto& x:vec) std::cout << "\n" << x;


    CALLGRIND_TOGGLE_COLLECT;
    std::optional<std::uint32_t> x = std::make_optional(999UL);
    if (x)
    {
        ++(*x);
    }
    CALLGRIND_TOGGLE_COLLECT;

    std::cout << "\noptional = " << *x;
}
