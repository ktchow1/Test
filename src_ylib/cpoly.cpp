#include<iostream>
#include<cstring> // for memcpy
#include<string>
#include<vector>
#include<array>

// **************************************************** //
// Take away from this experiment
// 1. C   style polymorphism
// 2. C   style message-handler using custom-vtable
// 3. C++ style   event-handler using visitor-pattern
//
// Techniques for C-style polymorphism :
// 1. enum class & std::underlying_type_t
// 2. reinterpret_cast for POD
// 3. invocation of member pointers
//
// Techniques for error handling :
// 4. assert in debug mode
// 5. passing array by reference
// 6. throw std::runtime_error and string literals
// **************************************************** //
namespace test
{
enum class TYPE : std::uint8_t
{
    DERIVED0,
    DERIVED1,
    DERIVED2,
    NUM_OF_DERIVED
};

struct base
{
    TYPE type;

    // ****************** //
    // *** Approach 2 *** //
    // ****************** //
    inline void vir_fct() const noexcept;
};

struct derived0
{
    base header;
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;

    inline void vir_fct() const noexcept
    {
        std::cout << "\nderived0 = " << x << " " << y << " " << z
                       << " size = " << sizeof(derived0)
                       << " size = " << sizeof(header) + sizeof(x) +sizeof(y) + sizeof(z);
    }
};

struct derived1
{
    base header;
    char s0[8];
    char s1[8];

    inline void vir_fct() const noexcept
    {
        std::cout << "\nderived1 = " << std::string(s0,8)
                              << " " << std::string(s1,8)
                       << " size = " << sizeof(derived1)
                       << " size = " << sizeof(header) + sizeof(s0) +sizeof(s1);
    }
};

struct derived2
{
    base header;
    char s[64];
    std::uint32_t len;

    inline void vir_fct() const noexcept
    {
        std::cout << "\nderived2 = " << std::string(s,len)
                       << " size = " << sizeof(derived2)
                       << " size = " << sizeof(header) + sizeof(s) +sizeof(len);
    }
};

// ****************** //
// *** Approach 1 *** //
// ****************** //
// For C, ptr to POD can be reinterpreted as ptr to its 1st member (interchangably).
// We can implement C-style inheritance making use of this property.
void invoke(const base* msg)
{
    if (msg->type == TYPE::DERIVED0)
    {
        const derived0* derived = reinterpret_cast<const derived0*>(msg);
        derived->vir_fct();
    }
    else if (msg->type == TYPE::DERIVED1)
    {
        const derived1* derived = reinterpret_cast<const derived1*>(msg);
        derived->vir_fct();
    }
    else
    {
        const derived2* derived = reinterpret_cast<const derived2*>(msg);
        derived->vir_fct();
    }
}

// ****************** //
// *** Approach 2 *** //
// ****************** //
void base::vir_fct() const noexcept
{
    if (type == TYPE::DERIVED0)
    {
        const derived0* derived = reinterpret_cast<const derived0*>(this);
        derived->vir_fct();
    }
    else if (type == TYPE::DERIVED1)
    {
        const derived1* derived = reinterpret_cast<const derived1*>(this);
        derived->vir_fct();
    }
    else
    {
        const derived2* derived = reinterpret_cast<const derived2*>(this);
        derived->vir_fct();
    }
}
}

// ************ //
// *** Test *** //
// ************ //
void test_cpoly()
{
    test::derived0 d0{test::TYPE::DERIVED0, 11, 22, 33};
    test::derived1 d1{test::TYPE::DERIVED1, "abcdefg", "ABCDEFG"};
    test::derived2 d2{test::TYPE::DERIVED2, "This is a pen. This is a man.", 18};

    std::cout << "\n\n*** Invocation from object ***";
    d0.vir_fct();
    d1.vir_fct();
    d2.vir_fct();

    std::cout << "\n\n*** C style polymorphism (invoked by outside function) ***";
    auto* b0 = reinterpret_cast<test::base*>(&d0);
    auto* b1 = reinterpret_cast<test::base*>(&d1);
    auto* b2 = reinterpret_cast<test::base*>(&d2);
    test::invoke(b0);
    test::invoke(b1);
    test::invoke(b2);

    std::cout << "\n\n*** C style polymorphism (invoked by outside function) ***";
    std::vector<test::base*> vec;
    vec.push_back(reinterpret_cast<test::base*>(&d0));
    vec.push_back(reinterpret_cast<test::base*>(&d1));
    vec.push_back(reinterpret_cast<test::base*>(&d2));
    for(const auto& x:vec)
    {
        test::invoke(x);
    }

    std::cout << "\n\n*** C style polymorphism (invoked by itself) ***";
    for(const auto& x:vec)
    {
        x->vir_fct();
    }
}

// As long as both composing and parsing are in-house, there is no need to pragma-1.
void test_compose_and_parse()
{
    std::cout << "\n\n*** COmpose and parse without copying ***";
    test::derived0 d0{test::TYPE::DERIVED0, 11, 22, 33};
    test::derived0 D0{test::TYPE::DERIVED0, 77, 88, 99};
    std::array<char, sizeof(test::derived0)> buffer;

    std::cout << "\nbefore compose and parse : "; d0.vir_fct();
    std::cout << "\nbefore compose and parse : "; D0.vir_fct();
    std::memcpy(&buffer.front(), &d0, sizeof(test::derived0));
    std::memcpy(&D0, &buffer.front(), sizeof(test::derived0));
    std::cout << "\nafter  compose and parse : "; d0.vir_fct();
    std::cout << "\nafter  compose and parse : "; D0.vir_fct();
}