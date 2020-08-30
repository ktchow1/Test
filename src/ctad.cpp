#include<iostream>
#include<vector>
#include<string>

template<typename U, typename V, typename T> struct my_class
{
    // CTAD fails, need deduction guide
    my_class(const U& u)
    {
        p = std::make_pair(u, 100);
        for(int n=0; n!=5; ++n) vec.push_back('x');
    }
   
    // CTAD fails, need deduction guide
    my_class(const U& u, const V& v)
    {
        p = std::make_pair(u,v);
        for(int n=0; n!=10; ++n) vec.push_back(v+n);
    }
   
    // CTAD works, as long as all U/V/T can be deduced
    my_class(const U& u, const V& v, const T& t0, const T& t1)
    {
        p = std::make_pair(u,v);
        for(auto t=t0; t!=t1; ++t) vec.push_back(t);
    }

    void debug() const
    {
        std::cout << "\nmy_class = ";
        std::cout << "(" << p.first << " --- " << p.second << ") ";
        std::cout << "[";
        for(const auto& x:vec) std::cout << x << " ";
        std::cout << "]";
    }

    std::pair<U,V> p;
    std::vector<T> vec;
};

// Deduction guide
template<typename U>
my_class(const U& u) -> my_class<U,int,char>; 
template<typename U, typename V>
my_class(const U& u, const V& v) -> my_class<U,V,V>; 


void test_ctad()
{
    my_class x0{std::string("Test obj0")};
    my_class x1{std::string("Test obj1"), 0.123};
    my_class x2{std::string("Test obj2"), 0.123, 50, 60};
    x0.debug();
    x1.debug();
    x2.debug();

    std::cout << "\nx0 correct type = " << std::is_same_v<decltype(x0), my_class<std::string, int, char>>;
    std::cout << "\nx1 correct type = " << std::is_same_v<decltype(x1), my_class<std::string, double, double>>;
    std::cout << "\nx2 correct type = " << std::is_same_v<decltype(x2), my_class<std::string, double, int>>;
}
