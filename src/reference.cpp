#include<iostream>
#include<functional>
#include<string>

struct T
{
    int mem0;
    int mem1;
    int mem2;
};

std::ostream& operator<<(std::ostream& os, const T& x)
{
    os << std::dec << x.mem0 << " " << x.mem1 << " " << x.mem2;
    return os;
}
/*
void fct(std::reference_wrapper<const T>& ref) */ // compile error, as we cannot bind lvalue& to rvalue (std::ref creates rvalue)
void fct(std::reference_wrapper<const T> ref)
{
    std::cout << "\nin_fct  = " << ref.get();
}

struct X
{
    T& m;
};

struct Y
{
    std::reference_wrapper<T> m;
};

void test_reference()
{
    T obj(1,2,3);
    std::cout << "\nobj = " << obj;


    // ***************** //
    // *** Subtest 1 *** //
    // ***************** //
    auto r0 = std::ref(obj);
    auto r1 = std::cref(obj);

    std::cout << "\nis_same = " << std::is_same_v<decltype(r0), std::reference_wrapper<T>>;       
    std::cout << "\nis_same = " << std::is_same_v<decltype(r1), std::reference_wrapper<const T>>;
    std::cout << "\naddress = " << std::hex << &(obj.mem0) << " " << &(r0.get().mem0) << " " << &(r1.get().mem0);  
    std::cout << "\naddress = " << std::hex << &(obj.mem1) << " " << &(r0.get().mem1) << " " << &(r1.get().mem0); 


    // ***************** //
    // *** Subtest 2 *** //
    // ***************** //
    std::pair<T&,T&> p0{obj,obj}; // compile OK
    auto p1 = std::make_pair(std::ref(obj), std::ref(obj)); 
    p1.first.mem0 = 111;
    p1.first.mem1 = 222;
    p1.first.mem2 = 333;

    std::cout << "\nis_same = " << std::is_same_v<decltype(p1), std::pair<std::reference_wrapper<T>, std::reference_wrapper<T>>>;
    std::cout << "\nis_same = " << std::is_same_v<decltype(p1), std::pair<T&,T&>>;
    std::cout << "\nsecond  = " << p1.second;


    // ***************** //
    // *** Subtest 3 *** //
    // ***************** //
//  std::vector<T&> v; // compile error
    std::vector<std::reference_wrapper<T>> v{obj, obj, obj};
    v[0].get().mem0 = 444;
    v[1].get().mem1 = 555;
    v[2].get().mem2 = 666;

    std::cout << "\nobject  = " << obj;
    for(const auto& x:v) std::cout << "\nvector  = " << x.get();


    // ***************** /
    // *** Subtest 4 *** //
    // ***************** //
//  std::optional<T&> op; // cannot compile
    auto opt = std::make_optional(std::ref(obj));
    std::cout << "\nis_same = " << std::is_same_v<decltype(opt), std::optional<std::reference_wrapper<T>>>;
    std::cout << "\nis_same = " << std::is_same_v<decltype(opt), std::optional<T&>>;
    if (opt) std::cout << "\noption  = " << opt->get();
    else     std::cout << "\noption  = null";


    // ***************** //
    // *** Subtest 5 *** //
    // ***************** //
    fct(std::cref(obj));  // std::cref creates a temporary std::reference_wrapper<const T>
    
    
    // ***************** /
    // *** Subtest 6 *** //
    // ***************** //
    X x(obj); // both are ok ...
    Y y(obj); // both are ok ...
    obj.mem0 = 123; 
    obj.mem1 = 234; 
    obj.mem2 = 345; 
    std::cout << "\nX::m = " << x.m;
    std::cout << "\nY::m = " << y.m;

    X x2{obj};
    Y y2{obj}; 
//  x2 = x; // compile error
    y2 = y;
}
