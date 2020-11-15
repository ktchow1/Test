#include<iostream>
#include<array>
#include<vector>
#include<list>

// ****************************************************** //
// *** Default constructibility of T in STL container *** //
// ****************************************************** //
struct non_def_but_copy
{
    non_def_but_copy(int x_) : x(x_){}
    non_def_but_copy() = delete;
    non_def_but_copy(const non_def_but_copy&) = default;
    non_def_but_copy(non_def_but_copy&&) = delete;
    non_def_but_copy& operator=(const non_def_but_copy&) = default;
    non_def_but_copy& operator=(non_def_but_copy&&) = delete;
    int x;
};

struct non_def_but_move
{
    non_def_but_move(int x_) : x(x_){}
    non_def_but_move() = delete;
    non_def_but_move(const non_def_but_move&) = delete;
    non_def_but_move(non_def_but_move&&) = default;
    non_def_but_move& operator=(const non_def_but_move&) = delete;
    non_def_but_move& operator=(non_def_but_move&&) = default;
    int x;
};

void test_STL_constraint0()
{
    std::vector<non_def_but_copy> v0;
    std::vector<non_def_but_move> v1;
    non_def_but_copy a0(0);
    non_def_but_copy a1(1);
    non_def_but_copy a2(2);
    non_def_but_move b0(0);
    non_def_but_move b1(1);
    non_def_but_move b2(2);

    v0.push_back(a0);
    v0.push_back(a1);
    v0.push_back(a2);
//  v0.push_back(non_def_but_copy{0}); // compile error
//  v0.push_back(non_def_but_copy{1}); // compile error
//  v0.push_back(non_def_but_copy{2}); // compile error
//  v1.push_back(b0);                  // compile error
//  v1.push_back(b1);                  // compile error
//  v1.push_back(b2);                  // compile error
    v1.push_back(non_def_but_move{1});
    v1.push_back(non_def_but_move{2});
    v1.push_back(non_def_but_move{3});
}

// ************************************ //
// *** T reference in STL container *** //
// ************************************ //
struct T
{
    T(const int& referee) : ref(referee){} // please ensure x_ lives long enough
    const int& ref;
};

void test_STL_constraint1()
{
    std::cout << "\nis T default constructible? " << std::is_default_constructible<T>::value;

//  std::array<T,10> a0;     // compile error, need default constructibility

    std::vector<T>   v0;
//  std::vector<T>   v1(10); // compile error, need default constructibility
//  std::vector<T&>  v2;     // compile error, need default constructibility

    std::list<T>  L0;
//  std::list<T>  L1(10);    // compile error, need default constructibility
//  std::list<T&> L2;        // compile error, need default constructibility

    int a = 111;
    int b = 222;
    int c = 333;

    v0.push_back(T{a});
    v0.push_back(T{b});
    v0.push_back(T{c});
    for(const auto& x:v0) std::cout << "\n" << x.ref;

    L0.push_back(T{a});
    L0.push_back(T{b});
    L0.push_back(T{c});
    for(const auto& x:L0) std::cout << "\n" << x.ref;
}

// ******************************************* //
// *** Complete type of T in STL container *** //
// ******************************************* //
#include<list>
#include<set>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<memory>

// Forward declaration is good enough (if no complete type is needed)
struct S;
S  create_S();
S& static_S();

void test_STL_constraint2()
{
//  S x;                                                 // compile error : incomplete type
    S* p;                                                // compile OK
    std::shared_ptr<S> sp;                               // compile OK

    // ************** //
    // *** vector *** //
    // ************** //
    std::vector<S> v0;                                   // compile OK : however complete type is needed on insertion
    std::vector<S> v1(3);                                // compile OK : however complete type is needed on insertion
//  v0[0] = S{};                                         // compile error : incomplete type
//  v0[0] = create_S();                                  // compile error : incomplete type
//  v0[0] = static_S();                                  // compile error : incomplete type
    std::vector<S>::iterator i = v0.begin();             // compile OK

    // *********** //
    // *** map *** //
    // *********** //
    std::map<int, S> m;                                  // compile OK : however complete type is needed on insertion
//  m[0] = S{};                                          // compile error : incomplete type
//  m[0] = create_S();                                   // compile error : incomplete type
//  m[0] = static_S();                                   // compile error : incomplete type
    std::map<int, S>::iterator j = m.begin();            // compile OK

    // ********************* //
    // *** unordered map *** //
    // ********************* //
//  std::unordered_map<int, S> h;                        // compile error : incomplete type
//  std::unordered_map<int, S>::iterator k = h.begin();  // compile error : incomplete type
}

struct S
{
    std::uint32_t x;
    std::uint32_t y;
    std::uint32_t z;
};

S create_S()
{
    S output{1,2,3};
    return output;
}

S& static_S()
{
    static S output{1,2,3};
    return output;
}
