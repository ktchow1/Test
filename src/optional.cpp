#include<iostream>
#include<string>
#include<optional>
#include<functional> // for std::reference_wrapper
#include<type_traits>

// Example 1 : optional of object
struct T
{
    int i;
    double d;
    std::string s;
};

void print(std::optional<T>& opt)
{
    if (!opt) std::cout << "\nEmpty T";
    else      std::cout << "\nInit T = " << opt->i << " " << opt->d << " " << opt->s;
}
void print(std::optional<std::reference_wrapper<T>>& opt)
{
    if (!opt) std::cout << "\nEmpty T_ref";
    else      std::cout << "\nInit T_ref = " << opt->get().i << " " << opt->get().d << " " << opt->get().s;
}

// Example 2 : optional of movable object
struct X
{
    X()             { std::cout << "\nX::X()";         }
    X(const X& rhs) { std::cout << "\nX::X(const X&)"; }
    X(X&& rhs)      { std::cout << "\nX::X(X&&)";      }

    const X& operator=(const X& rhs) { std::cout << "\nX::operator=(const X&)"; return *this; } 
    const X& operator=(X&& rhs)      { std::cout << "\nX::operator=(X&&)";      return *this; }
};

// Example 3 : optional with SFINAE
template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
auto create_optional()
{
    return std::optional<T>(12345);
}

template<typename T, typename std::enable_if<!std::is_integral<T>::value, int>::type = 0>
auto create_optional()
{
    return std::make_optional<std::string>("This is a string.");
}

void test_optional()
{
    T obj(12345, 0.12345, "temp-obj");

    std::cout << "\ntesting reference wrapper ...";
    auto ref = std::ref(obj);  
    auto obj2 = static_cast<T>(ref);
    std::cout << "\nIs type correct = " << std::is_same_v<decltype(ref), std::reference_wrapper<T>>;
    std::cout << "\naddress of i = " << std::hex << &(obj.i) << " " << &(ref.get().i);
    std::cout << "\naddress of d = " << std::hex << &(obj.d) << " " << &(ref.get().d);
    std::cout << "\naddress of s = " << std::hex << &(obj.s) << " " << &(ref.get().s);
    std::cout << "\naddress of i = " << std::hex << &(obj.i) << " " << &(obj2.i);
    std::cout << "\naddress of d = " << std::hex << &(obj.d) << " " << &(obj2.d);
    std::cout << "\naddress of s = " << std::hex << &(obj.s) << " " << &(obj2.s);
    std::cout << std::dec;


    // Example 1
    std::cout << "\n\noptional of object";
    std::optional<T> opt0;
    std::optional<T> opt1(T{100, 3.14, "abc"});
    std::optional<T> opt2 = T{200, 1.23, "def"};
    auto opt3 = std::make_optional<T>({300, 9.99, "----"});
    print(opt0);
    print(opt1);
    print(opt2);
    print(opt3);

    std::cout << "\n\noptional of reference";
//  std::optional<T&> opt(obj);                        // compile error
//  std::optional<std::reference_wrapper<T>> opt(obj); // compile error
    std::optional<std::reference_wrapper<T>> opt4;
    std::optional<std::reference_wrapper<T>> opt5(std::ref(obj));
    std::optional<std::reference_wrapper<T>> opt6 = std::ref(obj); 
    auto opt7 = std::make_optional<std::reference_wrapper<T>>(std::ref(obj)); 
    print(opt4);
    print(opt5);
    print(opt6);
    print(opt7);  

    std::cout << "\nmodify the object ...";
    opt5->get().i = 54321;
    opt5->get().d = 0.54321;
    opt5->get().s = "temp-obj modified";
    print(opt5);
    print(opt6);
    print(opt7);  
    
    std::cout << "\n\noptional of movable object";
    X x;
    std::optional<X> optx0;
    std::optional<X> optx1(x);
    std::optional<X> optx2(std::move(x)); 
    optx1 = x;
    optx1 = std::move(x);
 
    // Example 2
    std::cout << "\n\noptioanl with SFINAE";
    std::cout << "\n" << *create_optional<int>();
    std::cout << "\n" << *create_optional<long>();
    std::cout << "\n" << *create_optional<double>();
    std::cout << "\n" << *create_optional<std::string>();
}

