#include<iostream>
#include<memory>

struct ABC
{
    void f() const
    {
        std::cout << "\nABC object = " << x;
    }

    std::uint32_t x;
};

struct base
{
    base() = default;
    base(std::uint32_t x_) : x(x_)
    {
    }

    base(const base& rhs)
    {
        std::cout << "\nbase construction from base or derived : "; rhs.f();
    }

    base& operator=(const base& rhs) = delete;

    base(const ABC& abc)
    {
        std::cout << "\nbase construction from ABC";
        x = abc.x;
    }

    base(const ABC* ptr)
    {
        std::cout << "\nbase construction from ABC*";
        x = ptr->x;
    }

    operator ABC() const
    {
        std::cout << "\nbase conversion operator to ABC";
        return ABC{x};
    }

    operator ABC*() const
    {
        std::cout << "\nbase conversion operator to ABC*";
        ABC* ptr = new ABC{x};
        return ptr;
    }

    virtual void f() const
    {
        std::cout << "\nbase object = " << x;
    }

    std::uint32_t x;
};

struct derived : public base
{
    derived() = default;
    derived(std::uint32_t x_) : base(x_)
    {
    }

    derived(const derived& rhs)
    {
        std::cout << "\nderived construction from base or derived : "; rhs.f();
    }

    derived& operator=(const derived& rhs) = delete;

    void f() const
    {
        std::cout << "\nderived object = " << x;
    }
};

// *************** //
// *** Casting *** //
// *************** //
void test_casting()
{
    std::cout << "\n\nStatic cast in general";
    base b0(123);
    base b1(234);
    ABC   abc = static_cast<ABC> (b0);   abc. f();
    ABC* pabc = static_cast<ABC*>(b1);  pabc->f();
    base b2 = static_cast<base>(abc);     b2. f();
    base b3 = static_cast<base>(pabc);    b3. f();
    delete pabc;

    std::cout << "\n\nStatic cast and dynamic cast for inheritance";
    derived& rd0 =  static_cast<derived&>( b0); rd0.f();  // does not invoke any base or derived copy constructor
    derived* pd0 =  static_cast<derived*>(&b0); pd0->f(); // does not invoke any base or derived copy constructor
//  derived& rd1 = dynamic_cast<derived&>( b1); rd1.f();  // compile warning and runtime crash (dynamic cast does check)
//  derived* pd1 = dynamic_cast<derived*>(&b1); pd1->f(); // compile warning and runtime crash (dynamic cast does check)
    rd0.x = 1234; b0.f(); rd0.f(); pd0->f();

    derived d0(345);
    derived d1(345);
    base& rb0 =  static_cast<base&>( d0); rb0.f();  // does not invoke any base or derived copy constructor
    base* pb0 =  static_cast<base*>(&d0); pb0->f(); // does not invoke any base or derived copy constructor
    base& rb1 = dynamic_cast<base&>( d1); rb1.f();  // compile and run OK, no call to copy
    base* pb1 = dynamic_cast<base*>(&d1); pb1->f(); // compile and run OK, no call to copy
    rb0.x = 3456; d0.f(); rb0.f(); pb0->f();
    rb1.x = 3456; d1.f(); rb1.f(); pb1->f();
}

// ****************************** //
// *** Casting for shared_ptr *** //
// ****************************** //
void test_casting_shared_ptr()
{
    auto pb = std::make_shared<base>();
    pb->f(); // base pointer pointing to base obj

    auto pd = std::make_shared<derived>();
    pd->f(); // derived pointer pointing to derived obj

    pb = std::static_pointer_cast<base>(pd);
    pb->f(); // base pointer pointing to derived object

    auto pd0 = std::dynamic_pointer_cast<derived>(pb);
    pd0->f(); // derived pointer pointing to base object

    std::cout << "\nreference count = " << pd.use_count();
}
