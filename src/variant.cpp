#include<iostream>
#include<vector>
#include<variant>
#include<any>

void print0(const std::variant<int, double, std::string>& v)
{
    if      (v.index() == 0) std::cout << "\nint    = " << std::get<0>(v);
    else if (v.index() == 1) std::cout << "\ndouble = " << std::get<1>(v);
    else if (v.index() == 2) std::cout << "\nstring = " << std::get<2>(v);
    else                     std::cout << "\nunknown";
}

void print1(const std::variant<int, double, std::string>& v)
{
    if      (std::holds_alternative<int>(v))         std::cout << "\nint    = " << std::get<int>(v);
    else if (std::holds_alternative<double>(v))      std::cout << "\ndouble = " << std::get<double>(v);
    else if (std::holds_alternative<std::string>(v)) std::cout << "\nstring = " << std::get<std::string>(v);
    else                                             std::cout << "\nunknown";
}

void print2(const std::variant<int, double, std::string>& v)
{
    if      (const auto& iter = std::get_if<int>(&v); iter)         std::cout << "\nint    = " << *iter;
    else if (const auto& iter = std::get_if<double>(&v); iter)      std::cout << "\ndouble = " << *iter;
    else if (const auto& iter = std::get_if<std::string>(&v); iter) std::cout << "\nstring = " << *iter;
    else                                                            std::cout << "\nunknown";
}

struct visitor
{
    void operator()(const int& i)         const { std::cout << "\nint    = " << i; }
    void operator()(const double& d)      const { std::cout << "\ndouble = " << d; }
    void operator()(const std::string& s) const { std::cout << "\nstring = " << s; }
};

// *********************** //
// *** My own visitor1 *** //
// *********************** //
template<typename VISITOR, typename VAR> void my_visit(const VISITOR& visitor, const VAR& v)
{
    // This method relies on runtime v.index() to different function std::get<N>().
    if      (v.index() == 0) visitor(std::get<0>(v));
    else if (v.index() == 1) visitor(std::get<1>(v));
    else if (v.index() == 2) visitor(std::get<2>(v));
//  else if (v.index() == 3) visitor(std::get<3>(v)); // compile error, std::get<3> not valid for VAR    
    
    // problem 1 : cannot compile if variant has smaller size
    // problem 2 : cannot extend to variant with larger size
}

// *********************** //
// *** My own visitor2 *** //
// *********************** //
template<typename VISITOR, typename VAR, int N = std::variant_size<VAR>::value-1> struct visit_traits
{
    void run(const VISITOR& visitor, const VAR& v)
    {
        if (v.index() == N) visitor(std::get<N>(v));
        else 
        {
            visit_traits<VISITOR, VAR, N-1> traits;
            traits.run(visitor, v);
        }
    }
};

template<typename VISITOR, typename VAR> struct visit_traits<VISITOR, VAR, 0>
{
    void run(const VISITOR& visitor, const VAR& v)
    {
        visitor(std::get<0>(v));
    }
};

template<typename VISITOR, typename VAR> void my_visit2(const VISITOR& visitor, const VAR& v)
{
    visit_traits<VISITOR, VAR> traits;
    traits.run(visitor, v);
}

// *************************** //
// *** Overloading lambda  *** //
// *************************** //
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; }; 
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;  

void test_variant_and_any()
{
    // std::variant
    std::variant<int, double, std::string> v0;
    std::variant<int, double, std::string> v1(3.14);
    std::variant<int, double, std::string> v2 = "abcdef";
    std::cout << "\nnum of type = " << std::variant_size<std::variant<int, double, std::string>>::value;
    
    std::cout << "\n\nmethod0 : check-index   "; print0(v0); print0(v1); print0(v2);    
    std::cout << "\n\nmethod1 : check-type    "; print1(v0); print1(v1); print1(v2);
    std::cout << "\n\nmethod2 : check-pointer "; print2(v0); print2(v1); print2(v2);

    // method3 : visitor
    std::cout << "\n\nmethod3 : visitor ";
    std::visit(visitor{}, v0);
    std::visit(visitor{}, v1);
    std::visit(visitor{}, v2);
    my_visit(visitor{}, v0);
    my_visit(visitor{}, v1);
    my_visit(visitor{}, v2);    
    for(const auto& v : std::vector<std::variant<int, double, std::string>>{ v0, v1, v2 }) my_visit(visitor{}, v);
    for(const auto& v : std::vector<std::variant<int, double, std::string>>{ v0, v1, v2 }) my_visit2(visitor{}, v);

    // method4 : overloading lambda
    std::cout << "\n\nmethod4 : visitor with overloading lambda";
    for (const auto& v : std::vector<std::variant<int, double, std::string>>{ v0, v1, v2 })
    {
        std::visit(overloaded
        {
            [](const int& i)          { std::cout << "\nint    = " << i; },
            [](const double& d)       { std::cout << "\ndouble = " << d; },
            [](const std::string & s) { std::cout << "\nstring = " << s; }
        }, v);
    }

    // bivariant
    std::cout << "\n\nbivariant : visitor with overloading lambda";
    for (const auto& v : std::vector<std::variant<int, double, std::string>>{ v0, v1, v2 })
    {
        for (const auto& u : std::vector<std::variant<int, double, std::string>>{ v0, v1, v2 })
        {
            std::visit(overloaded
            {
                [](const int& i0,    const int& i1)        { std::cout << "\nint plus int       = " << i0 << " " << i1; },
                [](const int& i,     const double& d)      { std::cout << "\nint plus double    = " << i  << " " << d;  },
                [](const int& i,     const std::string& s) { std::cout << "\nint plus string    = " << i  << " " << s;  },
                [](const double& d,  const int& i)         { std::cout << "\ndouble plus int    = " << d  << " " << i;  },
                [](const double& d0, const double& d1)     { std::cout << "\ndouble plus double = " << d0 << " " << d1; },
                [](const double& d,  const std::string& s) { std::cout << "\ndouble plus string = " << d  << " " << s;  },
                [](const auto& x,    const auto& y)        { std::cout << "\nmiscellaneous case = " << x  << " " << y;  }
            }, v, u);
        }
    }

    // std::any
    std::cout << "\n\ntest std::any";
    std::any a0;                                       std::cout << "\nany = " << a0.has_value(); 
    std::any a1(123);                                  std::cout << "\nany = " << std::any_cast<int>(a1);
    std::any a2 = std::string("abcdef");               std::cout << "\nany = " << std::any_cast<std::string>(a2);
    auto a3 = std::make_any<std::string>("abcdef");    std::cout << "\nany = " << std::any_cast<std::string>(a3);
}

// ***************************************** //
// *** Overloading lambda : step by step *** //
// ***************************************** //
// merging ideas from two articles :
// dev.to/tmr232/that-overloaded-trick-overloading-lambdas-in-c17
// dev.to/fenbf/2-lines-of-code-and-3-c-17-features-the-overload-pattern-pgg
//
// Using 3 features :
// (1) parameter pack in using declaration
// (2) aggregate initialization for derived class
// (3) class template argument deduction (CTAD), also known as 
//     user defined deduction guide to save us from writing factory
//     [factory allows template argument deduction]

struct A {};
struct B {};
struct C {};
struct D {};
struct A_visitor { void operator()(const A& x) const { std::cout << "\nvisit A"; } };
struct B_visitor { void operator()(const B& x) const { std::cout << "\nvisit B"; } };
struct C_visitor { void operator()(const C& x) const { std::cout << "\nvisit C"; } };
struct D_visitor { void operator()(const D& x) const { std::cout << "\nvisit D"; } };
struct ABCD_visitor : public A_visitor, B_visitor, C_visitor, D_visitor
{
    using A_visitor::operator();
    using B_visitor::operator();
    using C_visitor::operator();
    using D_visitor::operator();
    // compile may not resolve operators without bringing them into scope of derived class 
    // however msvs can resolve operators without using
};

template<typename...Ts>
struct overloader : public Ts... 
{
    using Ts::operator()...; 
};

// *** Factory (usually used to avoid specifying parameter type *** //
template<typename...Ts>
auto make_overloader(Ts&&... args)
{
    return overloader<Ts...>{std::forward<Ts>(args)...}; 
}

// *** Class template argument deduction (without factory) *** //
// This is not using. 
// This is not function. T
// This is a deduction guide.
template<typename...Ts> overloader(Ts...) -> overloader<Ts...>; 

void test_variant_overloading_lambda()
{
    std::vector<std::variant<A,B,C,D>> vec{ A{}, B{}, C{}, D{}, C{}, B{}, A{} };
    std::cout << "\n\nversion 1";
    for(const auto& v : vec) std::visit(ABCD_visitor{}, v);
    std::cout << "\n\nversion 2";
    for(const auto& v : vec) std::visit(overloader<A_visitor,B_visitor,C_visitor,D_visitor>{}, v);
    std::cout << "\n\nversion 3";
    for(const auto& v : vec) std::visit(make_overloader(A_visitor{}, B_visitor{}, C_visitor{}, D_visitor{}), v);
    std::cout << "\n\nversion 4";
    for(const auto& v : vec) std::visit(overloader{A_visitor{}, B_visitor{}, C_visitor{}, D_visitor{}}, v);
    std::cout << "\n\nversion 5 (replace by lambda) ";
    for (const auto& v : vec) 
    {
        std::visit(overloader
        {
            [](const A& x) { std::cout << "\nlambda visit A"; },
            [](const B& x) { std::cout << "\nlambda visit B"; },
            [](const C& x) { std::cout << "\nlambda visit C"; },
            [](const D& x) { std::cout << "\nlambda visit D"; }
        }, v);
    }

    // Overload lambda invoked inplace
    int n=0;

    std::cout << "\n\nOverload lambda invoked inplace";
    auto temp = overloader
    {
        [&n](const A& x) { std::cout << "\nlambda visit A"; n = 11; },
        [&n](const B& x) { std::cout << "\nlambda visit B"; n = 12; },
        [&n](const C& x) { std::cout << "\nlambda visit C"; n = 13; },
        [&n](const D& x) { std::cout << "\nlambda visit D"; n = 14; }
    };
    temp(A{}); std::cout << " " << n;
    temp(B{}); std::cout << " " << n;
    temp(C{}); std::cout << " " << n;
    temp(D{}); std::cout << " " << n;
}

struct base0 { base0(const std::string& x) { std::cout << "\nbase0::base0" << x;             } };
struct base1 { base1(int x, int y)         { std::cout << "\nbase1::base1" << x << " " << y; } };
struct base2 { base2(int x)                { std::cout << "\nbase2::base2" << x;             } };
struct derived : public base0, base1, base2
{
    // no constructor, lets test if aggregate init works
};

void test_variant_aggregate_init()
{
    std::cout << "\n\naggregate init for inheritance";
    std::string s("abcdef");
    derived d{{s}, {123,456}, {789}};
}

template<typename T, typename U> struct my_class
{ 
    void run() 
    {
        std::cout << "\nmem0 = " << mem0; 
        std::cout << "\nmem1 = " << mem1;
    }
    T mem0;
    U mem1;
};

template<typename T, typename U> my_class(T,U)->my_class<T,U>;

void test_variant_CTAD()
{
    std::cout << "\n\nCTAD";
    std::string s = "abcdef";
    int n = 12345;

    // cannot compile unless you have either [1] or [2]
    my_class obj0{s, n}; // note : instead of my_class<std::string, int>
    my_class obj1{n, s}; // note : instead of my_class<int, std::string>
//  my_class obj0(s, n); // compile error does not work, why?
//  my_class obj1(n, s); // compile error does not work, why?
    obj0.run();
    obj1.run();
}
