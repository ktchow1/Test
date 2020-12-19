#include<iostream>
#include<type_traits>

struct B
{
};

struct B3
{
    std::uint8_t x;
    std::uint8_t y;
    std::uint8_t z;
};

struct BV
{
    virtual void f() = 0;
};

// *** Non contiguous *** //
struct D0 : public BV
{
};

// *** Contiguous, non-std-layout, non-trivial *** //
struct D1 : public B3
{
    std::uint8_t u = 1;
};

struct D2 : public B
{
    D2(){}
    public:  std::uint8_t u;
    private: std::uint8_t v;
};

// *** Contiguous, non-std-layout, trivial *** //
struct D3 : public B3
{
    std::uint8_t u;
};

struct D4 : public B
{
    D4() = default;
    public:  std::uint8_t u;
    private: std::uint8_t v;
};

// *** Contiguous, std-layout, trivial *** //
struct D5 : public B3
{
    void fct();
};

struct D6 : public B
{
    D6() = default;
    public:  std::uint8_t u;
             std::uint8_t v;
};

void test_pod()
{
    std::cout << "\nD0 " << std::boolalpha << std::is_standard_layout<D0>::value << " " << std::is_trivial<D0>::value;
    std::cout << "\nD1 " << std::boolalpha << std::is_standard_layout<D1>::value << " " << std::is_trivial<D1>::value;
    std::cout << "\nD2 " << std::boolalpha << std::is_standard_layout<D2>::value << " " << std::is_trivial<D2>::value;
    std::cout << "\nD3 " << std::boolalpha << std::is_standard_layout<D3>::value << " " << std::is_trivial<D3>::value;
    std::cout << "\nD4 " << std::boolalpha << std::is_standard_layout<D4>::value << " " << std::is_trivial<D4>::value;
    std::cout << "\nD5 " << std::boolalpha << std::is_standard_layout<D5>::value << " " << std::is_trivial<D5>::value;
    std::cout << "\nD6 " << std::boolalpha << std::is_standard_layout<D6>::value << " " << std::is_trivial<D6>::value;

    std::cout << "\n\n"; 
}

// ***************************** //
// *** Simulate YLib::String *** //
// ***************************** //
template<std::uint32_t N>
struct my_string
{
    char impl[N];
};

template<std::uint32_t N>
inline std::ostream& operator<<(std::ostream& os, const my_string<N>& s)
{
    std::string str(s.impl, N);
    os << "my_string = [" << str << "]";
    return os;
}

void test_pod_string()
{
    my_string<4> s0("abc");  // length of "abc"  = 4 (terminating char included)
//  my_string<4> s0("abcd"); // length of "abcd" = 5 (terminating char included) compile error
    std::cout << "\n" << s0;

    // We cannot construct array from std::string directly, unless we copy ...
    std::string str("abc");
//  my_string<4> s1(str);            // compile error
//  my_string<4> s1(str.c_str());    // compile error 
//  my_string<4> s1{str.c_str()};    // compile error
    my_string<4> s1{str.c_str()[0]}; // compile ok, but copy the first byte only 
    std::cout << "\n" << s1;

    // Conclusion : array is NOT pointer ... we need either a for-loop of memcpy( ... sizeof(T)*N) to init array
    char ac0[4] = "abc";
//  char ac1[4] =  ac0;              // compile error
//  char ac1[4] = {ac0};             // compile error
//  char ac1[4] = &ac0[4];           // compile error
    std::cout << "\n\n";
}

struct T
{
    int x;
};

inline std::ostream& operator<<(std::ostream& os, const T& t)
{
    os << t.x << " ";
    return os;
}

void test_pod_T()
{
    T array[4] = { T{11}, T{22}, T{33}, T{44} };
//  T array0[4] = array; // compile error
    for(int n=0; n!=4; ++n)
    {
        std::cout << array[n];
    }
    std::cout << "\n\n";
}



