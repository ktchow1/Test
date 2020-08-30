#include<iostream>
#include<algorithm>
#include<execution> // need g++-10 
#include<thread>
#include<vector>

void test_parallel_algo()
{
    std::vector<int> data;
    for(int n=0; n!=2000; ++n) data.push_back(rand());
    
//  std::sort(std::execution::par, std::begin(data), std::end(data));
//  for(const auto& x : data) std::cout << x << "  ";

    std::cout << "\n\n\n";
    std::for_each(std::execution::par, std::begin(data), std::end(data), [](auto& x) { std::cout << std::this_thread::get_id(); });    
    std::cout << "\n\n\n";
    std::for_each(std::execution::seq, std::begin(data), std::end(data), [](auto& x) { std::cout << std::this_thread::get_id(); });
}
