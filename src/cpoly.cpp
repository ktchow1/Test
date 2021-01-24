#include<iostream>
#include<cassert> // for assert()
#include<string>
#include<array>

// Take away from this experiment
// 1. C-style poly
// 2. enum class
// 3. reinterpret_cast
// 4. invocation of member pointers
// 5. assert in debug mode

enum class TYPE : std::uint8_t
{
    derived0,
    derived1,
    derived2,
    num_of_derived
};

struct BASE
{
    TYPE type;
};

struct DERIVED0
{
    BASE base;
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;

    inline void vir_fct() const noexcept
    {
        std::cout << "\nDERIVED0 = " << x << " " << y << " " << z;
    }
};

struct DERIVED1
{
    BASE base;
    char s0[8];
    char s1[8];

    inline void vir_fct() const noexcept
    {
        std::cout << "\nDERIVED1 = " << std::string(s0,8)
                              << " " << std::string(s1,8);
    }
};

struct DERIVED2
{
    BASE base;
    char s[64];
    std::uint32_t len;

    inline void vir_fct() const noexcept
    {
        std::cout << "\nDERIVED2 = " << std::string(s,len);
    }
};

void handle(const BASE* base)
{
    if (base->type == TYPE::derived0)
    {
        const DERIVED0* derived = reinterpret_cast<const DERIVED0*>(base);
        derived->vir_fct();
    }
    else if (base->type == TYPE::derived1)
    {
        const DERIVED1* derived = reinterpret_cast<const DERIVED1*>(base);
        derived->vir_fct();
    }
    else
    {
        const DERIVED2* derived = reinterpret_cast<const DERIVED2*>(base);
        derived->vir_fct();
    }
}

// ************************************ //
// *** How to avoid sequence of IF? *** //
// ************************************ //
class manager  // The concept in YLib::MessageHandler.
{
public:
    using handle_type = void (manager::*)(const BASE*) const noexcept;

    manager()
    {
        vtable[static_cast<std::uint32_t>(TYPE::derived0)] = &manager::handle0;
        vtable[static_cast<std::uint32_t>(TYPE::derived1)] = &manager::handle1;
        vtable[static_cast<std::uint32_t>(TYPE::derived2)] = &manager::handle2;
    }

    void handle(const BASE* base) const noexcept
    {
        auto ptr = vtable[static_cast<std::uint32_t>(base->type)];

        // Don't forget the bracket around (this->*ptr), otherwise compile error.
        (this->*ptr)(base);
    }

private:
    void handle0(const BASE* base) const noexcept
    {
        // Always follow these 3 steps in handle ...
        assert(base->type == TYPE::derived0);
        const DERIVED0* derived = reinterpret_cast<const DERIVED0*>(base);
        derived->vir_fct();
    }

    void handle1(const BASE* base) const noexcept
    {
        assert(base->type == TYPE::derived1);
        const DERIVED1* derived = reinterpret_cast<const DERIVED1*>(base);
        derived->vir_fct();
    }

    void handle2(const BASE* base) const noexcept
    {
        assert(base->type == TYPE::derived2);
        const DERIVED2* derived = reinterpret_cast<const DERIVED2*>(base);
        derived->vir_fct();
    }

protected:
    // static cast from enum class to std::uint32_t is needed, otherwise compile error
    std::array<handle_type, static_cast<std::uint32_t>(TYPE::num_of_derived)> vtable;
};

class derived_manager : public manager
{
public:
    using handle_type = void (manager::*)(const BASE*) const noexcept;

    // Override
    derived_manager() : manager{}
    {
        // static_cast is necessary, otherwise compile error, cannot assign to array of diff types
        manager::vtable[static_cast<std::uint32_t>(TYPE::derived0)] =
                        static_cast<handle_type>(&derived_manager::HANDLER0);
        manager::vtable[static_cast<std::uint32_t>(TYPE::derived1)] =
                        static_cast<handle_type>(&derived_manager::HANDLER1);
    }

private:
    void HANDLER0(const BASE* base) const noexcept
    {
        assert(base->type == TYPE::derived0);
        const DERIVED0* derived = reinterpret_cast<const DERIVED0*>(base);
        std::cout << "\noverwrite handler for derived0";
    }

    void HANDLER1(const BASE* base) const noexcept
    {
        assert(base->type == TYPE::derived1);
        const DERIVED1* derived = reinterpret_cast<const DERIVED1*>(base);
        std::cout << "\noverwrite handler for derived1";
    }
};

// ************ //
// *** Test *** //
// ************ //
void test_cpoly()
{
    DERIVED0 d0{TYPE::derived0, 11, 22, 33};
    DERIVED1 d1{TYPE::derived1, "abcdefg", "ABCDEFG"};
    DERIVED2 d2{TYPE::derived2, "This is a pen. This is a man.", 18};

    // Invocation from object
    std::cout << "\n*** Part1 ***";
    d0.vir_fct();
    d1.vir_fct();
    d2.vir_fct();

    // C style polymorphism
    std::cout << "\n*** Part2 ***";
    handle(reinterpret_cast<BASE*>(&d0));
    handle(reinterpret_cast<BASE*>(&d1));
    handle(reinterpret_cast<BASE*>(&d2));

    // C style polymorphism
    std::cout << "\n*** Part3 ***";
    auto* b0 = reinterpret_cast<BASE*>(&d0);
    auto* b1 = reinterpret_cast<BASE*>(&d1);
    auto* b2 = reinterpret_cast<BASE*>(&d2);
    handle(b0);
    handle(b1);
    handle(b2);

    // No IF checking
    std::cout << "\n*** Part4 ***";
    manager mgr;
    mgr.handle(b0);
    mgr.handle(b1);
    mgr.handle(b2);

    std::cout << "\n*** Part5 ***";
    derived_manager mgr2;
    mgr2.handle(b0);
    mgr2.handle(b1);
    mgr2.handle(b2);
    std::cout << "\n\n";
}


