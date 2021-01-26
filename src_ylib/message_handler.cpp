#include<iostream>
#include<cassert> // for assert()
#include<string>
#include<vector>
#include<array>

// ********************************* //
// *** Message handler - C style *** //
// ********************************* //
namespace message
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
};

struct derived0
{
    base header;
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;
};

struct derived1
{
    base header;
    char s0[8];
    char s1[8];
};

struct derived2
{
    base header;
    char s[64];
    std::uint32_t len;
};

// ******************************************* //
// *** The concept in YLib::MessageHandler *** //
// ******************************************* //
class handler
{
public:
    using idx_type = std::underlying_type_t<TYPE>;
    using fct_type = void (handler::*)(const base*) const noexcept;

    handler()
    {
        vtable[static_cast<idx_type>(TYPE::DERIVED0)] = &handler::fct0;
        vtable[static_cast<idx_type>(TYPE::DERIVED1)] = &handler::fct1;
        vtable[static_cast<idx_type>(TYPE::DERIVED2)] = &handler::fct2;
    }

    void handle(const base* msg) const noexcept
    {
        auto ptr = vtable[static_cast<idx_type>(msg->type)];
        (this->*ptr)(msg); // Bracket around (this->*ptr) is a must.
    }

private:
    // Handler's real implementation. Each has 3 steps :
    // 1. assert in debug
    // 2. reinterpret_cast
    // 3. invoke member pointer
    void fct0(const base* msg) const noexcept
    {
        assert(msg->type == TYPE::DERIVED0);
        const derived0* derived = reinterpret_cast<const derived0*>(msg);
        std::cout << "\noriginal handler for msg-derived0 :"
                  << " " << derived->x
                  << " " << derived->y
                  << " " << derived->z;
    }

    void fct1(const base* msg) const noexcept
    {
        assert(msg->type == TYPE::DERIVED1);
        const derived1* derived = reinterpret_cast<const derived1*>(msg);
        std::cout << "\noriginal handler for msg-derived1 :"
                  << " " << std::string(derived->s0, 8)
                  << " " << std::string(derived->s1, 8);
    }

    void fct2(const base* msg) const noexcept
    {
        assert(msg->type == TYPE::DERIVED2);
        const derived2* derived = reinterpret_cast<const derived2*>(msg);
        std::cout << "\noriginal handler for msg-derived2 :"
                  << " " << std::string(derived->s, derived->len);
    }

protected:
    // static cast to underlying type is necessary, otherwise compile error
    std::array<fct_type, static_cast<idx_type>(TYPE::NUM_OF_DERIVED)> vtable;
};

class derived_handler : public handler
{
public:
    using idx_type = handler::idx_type;
    using fct_type = handler::fct_type;

    // Override
    derived_handler() : handler{}
    {
        // static_cast to base::mem_ptr is necessary, otherwise compile error
        handler::vtable[static_cast<idx_type>(TYPE::DERIVED0)] =
                        static_cast<fct_type>(&derived_handler::over_fct0);
        handler::vtable[static_cast<idx_type>(TYPE::DERIVED1)] =
                        static_cast<fct_type>(&derived_handler::over_fct1);
    }

private:
    void over_fct0(const base* msg) const noexcept
    {
        assert(msg->type == TYPE::DERIVED0);
        const derived0* derived = reinterpret_cast<const derived0*>(msg);
        std::cout << "\noverride handler for msg-derived0 :"
                  << " " << derived->x
                  << " " << derived->y
                  << " " << derived->z;
    }

    void over_fct1(const base* msg) const noexcept
    {
        assert(msg->type == TYPE::DERIVED1);
        const derived1* derived = reinterpret_cast<const derived1*>(msg);
        std::cout << "\noverride handler for msg-derived1 :"
                  << " " << std::string(derived->s0, 8)
                  << " " << std::string(derived->s1, 8);
    }
};
}

// ************ //
// *** Test *** //
// ************ //
void test_message_handler()
{
    message::derived0 d0{message::TYPE::DERIVED0, 11, 22, 33};
    message::derived1 d1{message::TYPE::DERIVED1, "abcdefg", "ABCDEFG"};
    message::derived2 d2{message::TYPE::DERIVED2, "This is a pen. This is a man.", 18};

    std::vector<message::base*> vec;
    vec.push_back(reinterpret_cast<message::base*>(&d0));
    vec.push_back(reinterpret_cast<message::base*>(&d1));
    vec.push_back(reinterpret_cast<message::base*>(&d2));

    std::cout << "\n\n*** Base handler ***";
    message::handler h;
    for(const auto& x:vec)
    {
        h.handle(x);
    }

    std::cout << "\n\n*** Derived handler ***";
    message::derived_handler h2;
    for(const auto& x:vec)
    {
        h2.handle(x);
    }
}