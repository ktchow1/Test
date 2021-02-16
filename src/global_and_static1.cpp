#include<iostream>

// ************** //
// *** Global *** //
// ************** //
extern std::uint32_t global_var0; /*
extern std::uint32_t global_var1;  */ // fail to extern a static global
       std::uint32_t global_var2 = 300;
static std::uint32_t global_var3 = 400;

void access_global()
{
    global_var0 += 10;
//  global_var1 += 10;
    global_var2 += 10;
}

void access_static()
{
    static std::uint32_t static_var0 = 500;

    std::cout << "\nstatic_var0 = " << static_var0;
    static_var0 += 2;
}

// ***************************** //
// *** Static initialization *** //
// ***************************** //
// 1. force const init with constexpr
// 2. force const init with constinit
// 3. runtime init with const
//
template<std::uint32_t N>
struct is_compile_time
{
    is_compile_time() : value(N){}
    std::uint32_t value;
};

template<typename T>
struct square
{
    constexpr explicit square(const T& x) : side(x){}
//  constexpr T area() const { std::cout << "*"; return side*side; }  // compile error : cannot ostream inside constexpr
//  constexpr T peri() const { std::cout << "."; return side*4; }     // compile error : cannot ostream inside constexpr
    constexpr T area() const { return side*side; }
    constexpr T peri() const { return side*4; }
    std::uint32_t side;
};

void increment_square()
{
    static constexpr square<std::uint32_t> sq0(12);
    static constinit square<std::uint32_t> sq1(12);
//  static constexpr std::string label0 = std::string("abcdef"); // compile error : cannot init string in compile time as it has pointer inside
    static const     std::string label1 = std::string("abcdef");

    std::cout << "\nsq0.area = " << sq0.area() << " is_compile_time = " << is_compile_time<sq0.area()>().value;
//  std::cout << "\nsq1.area = " << sq1.area() << " is_compile_time = " << is_compile_time<sq1.area()>().value; // compile error : cannot use constinit inside template
    std::cout << "\nsq1.area = " << sq1.area();

//  sq0.side *= 2; // compile error : cannot modify constexpr
    sq1.side *= 2;
}

void test_global_and_static1()
{
    constexpr square<std::uint32_t> sq(123);
//  constinit square<std::uint32_t> sq(123);  // constinit can only be applied to static, otherwise compile error
    std::cout << "\nsq.area = " << sq.area() << " is_compile_time = " << is_compile_time<sq.area()>().value;
    std::cout << "\nsq.peri = " << sq.peri() << " is_compile_time = " << is_compile_time<sq.peri()>().value;
    std::cout << "\n";
    increment_square();
    std::cout << "\n";
    increment_square();
    std::cout << "\n";
    increment_square();
    std::cout << "\n";
}