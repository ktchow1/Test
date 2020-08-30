#include<iostream>
#include<type_traits>
#include<string>

// Combining enable_if with perfect forwarding
template<typename T, typename std::enable_if<std::is_lvalue_reference<T>::value, int>::type dummy = 1>
void impl(T&& x)
{
	std::cout << "\n[lvalue impl] " << x;
}

template<typename T, typename std::enable_if<!std::is_lvalue_reference<T>::value, int>::type dummy = 1>
void impl(T&& x)
{
	std::cout << "\n[rvalue impl] " << x;
}

template<typename T>
void perfect_forwarding(T&& x)
{
	impl(std::forward<T>(x));
}

void test_perfect_forwarding()
{
	std::string s = "abcdefghij";
	int i = 12345;

	perfect_forwarding(s);
	perfect_forwarding(std::string{ "klmnopqrst" });
	perfect_forwarding(i);
	perfect_forwarding(67890);
	std::cout << "\n";
}
