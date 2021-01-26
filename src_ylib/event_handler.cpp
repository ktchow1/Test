#include<iostream>
#include<cstring> // for memcpy
#include<string>
#include<vector>
#include<array>

// ***************************************************************** //
// *** The concept in YLib::EventHandler (apply visitor pattern) *** //
// ***************************************************************** //
// In YLib, macro is used to ensure consistent name :
// 1. event class name
// 2. visitor handle function name
//
// Besides, here is the correspondence
// element           = event
// element::accept() = event::handle()               [use accept below]
// visitor           = event_handler
// visitor::visit()  = event_handler::on_xxx_event() [use on_xxx below]
//
// Beware the sequence of declaration in visitor pattern, since :
// * element needs a reference to visitor, while ...
// * visitor needs a reference to element
// ***************************************************************** //

namespace event
{
class handler; // forward declaration
struct base
{
    virtual void accept(handler& h) const = 0;
};

// ************************************************ //
// *** The following are not POD,               *** //
// *** customized constructor must be provided. *** //
// ************************************************ //
struct derived0 final : public base
{
    derived0(std::uint32_t x_,
             std::uint32_t y_,
             std::uint32_t z_) : x(x_), y(y_), z(z_) {}

    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;

    void accept(handler& h) const override;
};

struct derived1 final : public base
{
    // Bind "const char (&)[8]"
    // to   "ABCDEFG" with '\0'
    derived1(const char (& s0_)[8],
             const char (& s1_)[8])
    {
        std::memcpy(s0, s0_, 8);;
        std::memcpy(s1, s1_, 8);;
    }

    char s0[8];
    char s1[8];

    void accept(handler& h) const override;
};

struct derived2 final : public base
{
    // Bind "const char*"
    // to   "ABCDEFG" with '\0'
    derived2(const char* ptr, std::uint32_t str_len)
    {
        using namespace std::string_literals;
        if (str_len > 64)
        {
            throw std::runtime_error("derived2 too long : "s + std::to_string(str_len));
        }
        std::memcpy(s, ptr, str_len);
        len = str_len;
    }

    char s[64];
    std::uint32_t len;

    void accept(handler& h) const override;
};

class handler
{
public:
    // Default implementation : do nothing ...
    virtual void on_derived0_event(const derived0& event) noexcept { std::cout << "\ndefault skipped"; }
    virtual void on_derived1_event(const derived1& event) noexcept { std::cout << "\ndefault skipped"; }
    virtual void on_derived2_event(const derived2& event) noexcept { std::cout << "\ndefault skipped"; }
};

// ************************************** //
// *** Delayed definition of accept() *** //
// ************************************** //
// No keyword "override" needed for delayed definition, otherwise compile error.
//
void derived0::accept(handler& h) const
{
    h.on_derived0_event(*this);
}

void derived1::accept(handler& h) const
{
    h.on_derived1_event(*this);
}

void derived2::accept(handler& h) const
{
    h.on_derived2_event(*this);
}

// ********************************** //
// *** Concrete class for handler *** //
// ********************************** //
class pricer : public handler
{
public:
    void on_derived0_event(const derived0& event) noexcept override
    {
        std::cout << "\npricer process derived0-event ="
                  << " " << event.x
                  << " " << event.y
                  << " " << event.z;
    }

    void on_derived1_event(const derived1& event) noexcept override
    {
        std::cout << "\npricer process derived1-event ="
                  << " " << std::string(event.s0,8)
                  << " " << std::string(event.s1,8);
    }
};

class hitter : public handler
{
public:
    void on_derived1_event(const derived1& event) noexcept override
    {
        std::cout << "\nhitter process derived1-event ="
                  << " " << std::string(event.s0,8)
                  << " " << std::string(event.s1,8);
    }

    void on_derived2_event(const derived2& event) noexcept override
    {
        std::cout << "\nhitter process derived2-event ="
                  << " " << std::string(event.s,event.len);
    }
};

class quoter : public handler
{
public:
    void on_derived0_event(const derived0& event) noexcept override
    {
        std::cout << "\nquoter process derived0-event ="
                  << " " << event.x
                  << " " << event.y
                  << " " << event.z;
    }

    void on_derived2_event(const derived2& event) noexcept override
    {
        std::cout << "\nquoter process derived2-event ="
                  << " " << std::string(event.s,event.len);
    }
};
}

// ************ //
// *** Test *** //
// ************ //
void test_event_handler()
{
    event::derived0 d0{11, 22, 33};
    event::derived1 d1{"abcdefg", "ABCDEFG"};
    event::derived2 d2{"This is a pen. This is a man.", 18};

    std::vector<event::base*> vec;
    vec.push_back(reinterpret_cast<event::base*>(&d0));
    vec.push_back(reinterpret_cast<event::base*>(&d1));
    vec.push_back(reinterpret_cast<event::base*>(&d2));

    std::cout << "\n\n*** Pricer ***";
    event::pricer pricer;
    for(const auto& x:vec) x->accept(pricer);

    std::cout << "\n\n*** Hitter ***";
    event::hitter hitter;
    for(const auto& x:vec) x->accept(hitter);

    std::cout << "\n\n*** Quoter ***";
    event::quoter quoter;
    for(const auto& x:vec) x->accept(quoter);
}