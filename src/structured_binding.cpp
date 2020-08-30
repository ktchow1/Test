#include<iostream>
#include<tuple>

struct product
{
    int x;
    double y;
    std::string z;
};

struct producer
{
    producer() : p{123, 3.1415, "abcde"} {}

    const product& produce() const
    {
        return p;
    }

    product& modify()
    {
        return p;
    }

    product p;
};

auto produce()
{
    return std::make_tuple(9999, 2.71, "klmnopq");
}

void test_structured_binding()
{
    producer prod;
    auto [x0, y0, z0] = prod.produce();
    std::cout << "\nx0 = " << x0;
    std::cout << "\ny0 = " << y0;
    std::cout << "\nz0 = " << z0;

    const auto& [x1, y1, z1] = prod.produce();
    std::cout << "\nx1 = " << x1;
    std::cout << "\ny1 = " << y1;
    std::cout << "\nz1 = " << z1;

    auto& [wx, wy, wz] = prod.modify();
    wx = 456;
    wy = 0.4141;
    wz = "This is the point.";
    
    std::cout << "\nx0 = " << x0;
    std::cout << "\ny0 = " << y0;
    std::cout << "\nz0 = " << z0;
    std::cout << "\nx1 = " << x1;
    std::cout << "\ny1 = " << y1;
    std::cout << "\nz1 = " << z1;

//  const auto&  [x2, y2, z2] = produce(); // compile ok
//        auto&  [x2, y2, z2] = produce(); // compile error
          auto&& [x2, y2, z2] = produce(); // compile ok
    
    std::cout << "\nx2 = " << x2;
    std::cout << "\ny2 = " << y2;
    std::cout << "\nz2 = " << z2;
}
