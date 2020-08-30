#include<iostream>
#include<type_traits>
#include<vector>
#include<list>
#include<set>

/* 
// Construct 1 : integral constant
struct  true_type { typedef bool value_type; static const bool value =  true; };
struct false_type { typedef bool value_type; static const bool value = false; };

// Construct 2 : traits 
template<typename T, typename U> struct is_same          : std::false_type {};
template<typename T>             struct is_same<T, T>    : std:: true_type {};
template<typename T>             struct is_integral      : std::false_type {};
template<>                       struct is_integral<int> : std:: true_type {};
// The above traits are relatively easy to construct, see below for more complicated traits.

// Construct 3 : enable if on top of SFINAE
template<bool B, typename T = void> struct enable_if {};
template        <typename T>        struct enable_if<true, T> { typedef T type; };
// Idea is to check if there exists a type T, sometimes it doesn't really matter what T is.

// Add the following to constrain T
typename std::enable_of<is_xxx<T>::value, DUMMY>::value 
*/

// Solution1 (fails in MSVS, why?)
template<typename T> void fct0(typename std::enable_if<std::is_integral<T>::value, T>::type x)
{
	std::cout << "\nintegral implementation";
}

template<typename T> void fct0(typename std::enable_if<std::is_floating_point<T>::value, T>::type x)
{
	std::cout << "\nfloating point implementation";
}

// Solution2
template<typename T> typename std::enable_if<std::is_integral<T>::value, T>::type fct1(const T& x)
{
	std::cout << "\nintegral implementation";
	return x * 2;
}

template<typename T> typename std::enable_if<std::is_floating_point<T>::value, T>::type fct1(const T& x)
{
	std::cout << "\nfloating point implementation";
	return x * 2;
}

// Solution3
template<typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0> void fct2(const T& x)
{
	std::cout << "\nintegral implementation";
}

template<typename T, typename std::enable_if<std::is_floating_point<T>::value, int>::type = 0> void fct2(const T& x)
{
	std::cout << "\nfloating point implementation";
}

/*
// Solution3b : does not work, redefinition
template<typename T, typename DUMMY = std::enable_if<std::is_integral<T>::value, T>::type> void fct2(const T& x)
{
	std::cout << "\nintegral implementation";
}

template<typename T, typename DUMMY = std::enable_if<std::is_floating_point<T>::value, T>::type> void fct2(const T& x)
{
	std::cout << "\nfloating point implementation";
} */

// With more types
struct mytype0{};
struct mytype1{};
struct mytype2{};
template<typename T> struct is_target : public std::false_type {};
template<> struct is_target<mytype0>  : public std:: true_type {};
template<> struct is_target<mytype1>  : public std:: true_type {};
template<> struct is_target<mytype2>  : public std:: true_type {};

// For two implementations
template<typename T, typename std::enable_if<is_target<T>::value, int>::type = 0> void fctA(const T& x)
{
	std::cout << "\ntarget type implementation";
}

template<typename T, typename std::enable_if<!is_target<T>::value, int>::type = 0> void fctA(const T& x)
{
	std::cout << "\nother type implementation";
}

// For multiple implementations
template<typename T> void fctB(const T& x)
{
	if      constexpr (std::is_same<T, mytype0>::value) std::cout << "\nmytype0 implementation";
	else if constexpr (std::is_same<T, mytype1>::value) std::cout << "\nmytype1 implementation";
	else if constexpr (std::is_same<T, mytype2>::value) std::cout << "\nmytype2 implementation";
	else                                                std::cout << "\nmytype? implementation";
}

void test_sfinae0()
{
	// integral vs floating point
//	fct0(1);
//	fct0(1.0);
	fct1(1);
	fct1(1.0);
	fct2(1);
	fct2(1.0);
	std::cout << "\n";

	// various custom types
	fctA(mytype0{});
	fctA(mytype1{});
	fctA(mytype2{});
	fctA(std::string{});
	fctA(int{});
	fctA(std::vector<int>{});
	fctB(mytype0{});
	fctB(mytype1{});
	fctB(mytype2{});
	fctB(std::string{});
	fctB(int{});
	fctB(std::vector<int>{});
	std::cout << "\n";
}

// The following does not work, see test function ...
template<typename T, typename U>
struct is_container_failed : public std::false_type {};
template<typename T> struct is_container_failed<T, typename T::value_type>  : public std::true_type {}; 

// Yes there are two template parameters, but ...
// (1) only one template parameter will be provided by user 
// (2) assign 2nd parameter default type DEFTYPE
// (3) remove 2nd parameter name U
// (4) in specialization, the second template should map to DEFTYPE
// (5) in practice, we use "void and std::void_t<T>" instead of "DEFTYPE and MAP2DEF<T>"

struct DEFTYPE {};
template <typename T> using  MAP2DEF = DEFTYPE; /* equivalent to
template <typename T> struct MAP2DEF { typedef DEFTYPE type; }; 
which requires syntax : MAP2DEF<T>::type */ 

// How to construct traits that T has a typedef?
template<typename, typename = DEFTYPE>
struct is_container : public std::false_type {};
template<typename T> struct is_container<T, MAP2DEF<typename T::value_type>> : public std::true_type {}; /*
template<typename T> struct is_container<T,         typename T::value_type>  : public std::true_type {}; */ 
// The latter does not work, why?

// How to construct traits that T has a member function?
template<typename, typename = void> 
struct is_iterator : public std::false_type {};
template<typename T> struct is_iterator<T, std::void_t<decltype(++std::declval<T>())>> : public std::true_type {}; 

void test_sfinae1()
{
	std::cout << "\nint                           " << is_container_failed<int, void>::value;
	std::cout << "\ndouble                        " << is_container_failed<double, void>::value;
	std::cout << "\nstd::string                   " << is_container_failed<std::string, char>::value;
	std::cout << "\nstd::vector<double>           " << is_container_failed<std::vector<double>, double>::value; 
	std::cout << "\nstd::list<std::string>        " << is_container_failed<std::list<std::string>, std::string>::value;
	std::cout << "\nstd::set<std::string>         " << is_container_failed<std::set<std::string>, std::string>::value;

	std::cout << "\nint                           " << is_container<int>::value;
	std::cout << "\ndouble                        " << is_container<double>::value;
	std::cout << "\nstd::string                   " << is_container<std::string>::value;
	std::cout << "\nstd::vector<double>           " << is_container<std::vector<double>>::value;
	std::cout << "\nstd::list<std::string>        " << is_container<std::list<std::string>>::value;
	std::cout << "\nstd::set<std::string>         " << is_container<std::set<std::string>>::value;

	std::cout << "\nstd::vector<double>           " << is_iterator<std::vector<double>>::value;
	std::cout << "\nstd::list<std::string>        " << is_iterator<std::list<std::string>>::value;
	std::cout << "\nstd::set<std::string>         " << is_iterator<std::set<std::string>>::value;
	std::cout << "\nstd::vector<double>::iter     " << is_iterator<std::vector<double>::iterator>::value;
	std::cout << "\nstd::list<std::string>::iter  " << is_iterator<std::list<std::string>::const_iterator>::value;
	std::cout << "\nstd::set<std::string>::iter   " << is_iterator<std::set<std::string>::iterator>::value;
}

/* In fact, std::true_type and std::false_type are specialization of :
template<typename T, T v> struct integral_constant
{
	typedef T value_type;
	static constexpr T value = v;

	constexpr   operator T() const noexcept { return v; }
	constexpr T operator()() const noexcept { return v; }
};
// which wraps constant as type, offering possibility of compile time calculation.
*/

template<int N> struct factorial    : std::integral_constant<int, N * factorial<N - 1>::value> {};
template<>      struct factorial<1> : std::integral_constant<int, 1> {};

template<int N> struct fibonacci    : std::integral_constant<int, fibonacci<N - 1>::value + fibonacci<N - 2>::value> {};
template<>      struct fibonacci<1> : std::integral_constant<int, 1> {};
template<>      struct fibonacci<2> : std::integral_constant<int, 1> {};

void test_sfinae2()
{	
	std::cout << "\nfactorial<2> = " << factorial<2>::value;
	std::cout << "\nfactorial<3> = " << factorial<3>::value;
	std::cout << "\nfactorial<4> = " << factorial<4>::value;
	std::cout << "\nfactorial<5> = " << factorial<5>::value;
	std::cout << "\nfactorial<6> = " << factorial<6>::value;
	std::cout << "\nfactorial<7> = " << factorial<7>::value;
	std::cout << "\nfactorial<8> = " << factorial<8>::value;	
	std::cout << "\nfibonacci<3> = " << fibonacci<3>::value;
	std::cout << "\nfibonacci<4> = " << fibonacci<4>::value;
	std::cout << "\nfibonacci<5> = " << fibonacci<5>::value;
	std::cout << "\nfibonacci<6> = " << fibonacci<6>::value;
	std::cout << "\nfibonacci<7> = " << fibonacci<7>::value;
	std::cout << "\nfibonacci<8> = " << fibonacci<8>::value;
	std::cout << "\n";

//	int n=6;            // compile error
//	const int n=6;      // not necessarily ok
	constexpr int n=6;  // compile succeed
	
	int test0[n]; 
	int test1[factorial<3>::value];
}


