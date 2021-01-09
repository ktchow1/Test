#include<iostream>
#include<vector>
#include<queue>
#include<tuple>
#include<string>
#include<type_traits>

// *********************** //
// *** template member *** //
// *********************** //
template<typename T> struct wrapper
{
    template<typename U> void fct() { std::cout << "template member"; }
};

template<typename T> void invoke_wrapper(wrapper<T>& x)
{
    x.template fct<std::string>();
}

void test_template_member()
{
    wrapper<int> x;
    invoke_wrapper(x);
}

// ******************************************** //
// *** member pointer as template parameter *** //
// ******************************************** //
struct simple_pod
{
    std::uint32_t x{0};
    std::string s{""};
};

std::ostream& operator<<(std::ostream& os, const simple_pod& pod)
{
    os << pod.x << " " << pod.s;
    return os;
}

void init(std::uint32_t& x) { x = 123;   }
void init(std::string& s)   { s = "abc"; }

template<typename T> void pass_as_non_template_para(simple_pod& pod, T simple_pod::* mem_ptr)
{
    init(pod.*mem_ptr);
};

template<typename M> void pass_as_template_para(simple_pod& pod, M mem_ptr)
{
    init(pod.*mem_ptr);
};

void test_template_member_ptr()
{
    simple_pod pod0;
    simple_pod pod1;

    std::cout << "\npod0 = " << pod0;
    pass_as_non_template_para(pod0, &simple_pod::x);
    pass_as_non_template_para(pod0, &simple_pod::s);
    std::cout << "\npod0 = " << pod0;

    std::cout << "\npod1 = " << pod1;
    pass_as_template_para(pod1, &simple_pod::x);
    pass_as_template_para(pod1, &simple_pod::s);
    std::cout << "\npod1 = " << pod1;
}

// ************************* //
// *** template template *** //
// ************************* //
template<typename T> struct vec
{
    void push_back(const T& x) { impl[next] = x; ++next; }
    const T& operator[](int n) const { return impl[n]; }

    T impl[100];
    int next = 0;
};

template<typename T, template<typename> class C> struct algo
{
    void add(const T& x)
    {
        values.push_back(x);
    }
    void print() const
    {
        std::cout << "\nContainer : ";
        std::cout << values[0];
        std::cout << values[1];
        std::cout << values[2];
    }
    C<T> values;
};

void test_template_template()
{
    algo<int, vec> x;
    x.add(1);
    x.add(2);
    x.add(3);
    x.print();

    // please note that this works only in windows and in gcc for c++17 and beyond
    // otherwise, std::vector is considered as a template with 2 template arguments 
    // and cannot match the requirement of C above, i.e. default template argument is not counted.
    algo<int, std::vector> y;
    y.add(1);
    y.add(2);
    y.add(3);
    y.print();
}

// ********************** //
// *** common and zip *** //
// ********************** //
struct A { };
struct B { };
struct C 
{
    C(const A&){}; 
    C(const B&){};
};
struct D 
{
    D(const B&) {};
    D(const C&) {};
};

template<typename T> void print_type(bool newline = true)
{
    if (newline) std::cout << "\n";
    if      constexpr (std::is_same<T,A>::value) std::cout << "type A";
    else if constexpr (std::is_same<T,B>::value) std::cout << "type B";
    else if constexpr (std::is_same<T,C>::value) std::cout << "type C";
    else if constexpr (std::is_same<T,D>::value) std::cout << "type D";
    else std::cout << "type unknown";
};

template<typename T> void print_pair_type()
{    
    typedef typename T::first_type T0;
    typedef typename T::second_type T1;
    std::cout << "\npair_type";
    std::cout << "  ";  print_type<T0>(false);
    std::cout << "  ";  print_type<T1>(false);
}

void test_template_common_type()
{
    print_type<std::common_type<A,A>::type>();
//  print_type<std::common_type<A,B>::type>(); // compile error
    print_type<std::common_type<A,C>::type>();
    print_type<std::common_type<C,A>::type>();
//  print_type<std::common_type<A,B,C>::type>(); // compile error
    print_type<std::common_type<B,C,D>::type>();
    print_type<std::common_type<D,B,C>::type>();
    
    // requirement 
    // convert ARG0 to ARG1, ARG1 to ARG2 and so on ...
}

// zip-with copied from STL reference : parameter pack
template<typename... Ts0> struct zip 
{
    template<typename... Ts1> struct with 
    {
        typedef std::tuple<std::pair<Ts0, Ts1>...> type;
    };
};

void test_template_zip_with()
{
    typedef zip<A,B,C,D>::with<C,D,A,B>::type result_type;
    print_pair_type<typename std::tuple_element<0, result_type>::type>();
    print_pair_type<typename std::tuple_element<1, result_type>::type>();
    print_pair_type<typename std::tuple_element<2, result_type>::type>(); 
    print_pair_type<typename std::tuple_element<3, result_type>::type>();
}


