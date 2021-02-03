#include<iostream>
#include<memory>

void f0(std::unique_ptr<std::string> up) // accept rvalue input only
{
    auto tmp = std::move(up); // compile error without std::move
    std::cout << "\nf0 = " << *tmp;
}

void f1(std::unique_ptr<std::string>& up) // accept lvalue input only
{
    auto tmp = std::move(up); // compile error without std::move
    std::cout << "\nf1 = " << *tmp << " [taking ownership from lvalue is dangerous]";
}

void g0(std::unique_ptr<std::string> up) // accept rvalue input only
{
    std::cout << "\ng0 = " << *up << " [no one takes ownership, resource is destructed]";
}

void g1(std::unique_ptr<std::string>& up) // accept lvalue input only
{
    std::cout << "\ng1 = " << *up;
}

void test_unique_ptr()
{
    auto up0 = std::make_unique<std::string>("This is test0.");
    auto up1 = std::make_unique<std::string>("This is test1.");
    auto up2 = std::make_unique<std::string>("This is test2.");
    auto up3 = std::make_unique<std::string>("This is test3.");
    auto up4 = std::make_unique<std::string>("This is test4.");
    auto up5 = std::make_unique<std::string>("This is test5.");

    f0(std::move(up0));
//  f0(up0);            // compile error
//  f1(std::move(up1)); // compile error
    f1(up1);

    g0(std::move(up2));
//  g0(up2);            // compile error
//  g1(std::move(up3)); // compile error
    g1(up3);
}

// ********************************************** //
// *** shared pointer to non-copyable objects *** //
// ********************************************** //
struct A
{
    A(int n) : x(n) {}
    A(const A&) = delete;
    A(A&&) = delete;
    const A& operator=(const A&) = delete;
    const A& operator=(A&&) = delete;

    void get() const { std::cout << "\nA=" << x; }
    void set(int n)  { x = n; }
    int x = 0;    
};

void test_unique_ptr_to_noncopyable()
{
    auto sp0 = std::make_shared<A>(123);
    auto sp1 = sp0;
    auto sp2 = sp0;

    sp0->get();
    sp1->set(234); sp0->get();
    sp2->set(345); sp0->get();
}
