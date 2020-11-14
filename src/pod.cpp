#include<iostream>
#include<type_traits>

struct B
{
};

struct B3
{
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t z;
};

struct BV
{
    virtual void f() = 0;
};

// *** Non contiguous *** //
struct D0 : public BV
{
};

// *** Contiguous, non-std-layout, non-trivial *** //
struct D1 : public B3
{
    std::uint8_t u = 1;
};

struct D2 : public B
{
    D2(){}
    public:  std::uint8_t u;
    private: std::uint8_t v;
};

// *** Contiguous, non-std-layout, trivial *** //
struct D3 : public B3
{
    std::uint8_t u;
};

struct D4 : public B
{
    D4() = default;
    public:  std::uint8_t u;
    private: std::uint8_t v;
};

// *** Contiguous, std-layout, trivial *** //
struct D5 : public B3
{
    void fct();
};

struct D6 : public B
{
    D6() = default;
    public:  std::uint8_t u;
             std::uint8_t v;
};


void test_pod()
{
    std::cout << "\nD0 " << std::boolalpha << std::is_standard_layout<D0>::value << " " << std::is_trivial<D0>::value;
    std::cout << "\nD1 " << std::boolalpha << std::is_standard_layout<D1>::value << " " << std::is_trivial<D1>::value;
    std::cout << "\nD2 " << std::boolalpha << std::is_standard_layout<D2>::value << " " << std::is_trivial<D2>::value;
    std::cout << "\nD3 " << std::boolalpha << std::is_standard_layout<D3>::value << " " << std::is_trivial<D3>::value;
    std::cout << "\nD4 " << std::boolalpha << std::is_standard_layout<D4>::value << " " << std::is_trivial<D4>::value;
    std::cout << "\nD5 " << std::boolalpha << std::is_standard_layout<D5>::value << " " << std::is_trivial<D5>::value;
    std::cout << "\nD6 " << std::boolalpha << std::is_standard_layout<D6>::value << " " << std::is_trivial<D6>::value;

    std::cout << "\n\n"; 
}

