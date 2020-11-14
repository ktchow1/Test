#include<iostream>
#include<memory>

struct base
{
    virtual void f() const
    {
        std::cout << "\nbase object";
    }
};

struct derived : public base
{
    void f() const
    {
        std::cout << "\nderived object";
    }
};

void test_shared_ptr_cast()
{
    auto pb = std::make_shared<base>();
    pb->f();

    auto pd = std::make_shared<derived>();
    pd->f();

    pb = std::static_pointer_cast<base>(pd);
    pb->f();

    auto pd0 = std::dynamic_pointer_cast<derived>(pb);
    if (pd0) pd0->f();

    std::cout << "\nreference count = " << pd.use_count();
}
