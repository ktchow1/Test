#include<iostream>
#include<vector>
#include<ranges>

void test_range()
{
    std::vector<std::uint32_t> vec0;
    for(std::uint32_t n=0; n!=50; ++n)
    {
        vec0.push_back(rand()%100);
    }

    auto vec1 = vec0 | std::views::filter   ([](const auto& x) { return x%2==1; } )
                     | std::views::transform([](const auto& x) { return x*2;    } );

    std::cout << "\nvec0 = ";
    for(const auto& x:vec0) std::cout << x << " ";
    std::cout << "\nvec1 = ";
    for(const auto& x:vec1) std::cout << x << " ";
}
