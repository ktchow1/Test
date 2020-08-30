#include<iostream>
#include<type_traits>
#include<string>

struct A {};
struct B {};

template<typename T> struct is_target : public std::false_type {};
template<> struct is_target<A> : public std::true_type {};
template<> struct is_target<B> : public std::true_type {};

template<typename T> struct my_class
{
	template<typename U> typename std::enable_if<!is_target<U>::value, void>::type f(const U&)
	{
		std::cout << "\nf non-target implementation";
	}
	template<typename U> typename std::enable_if<is_target<U>::value, void>::type f(const U&)
	{
		std::cout << "\nf is-target implementation";
	}

	// **************************************************************** //
	// The following does not work as T is not deduced, then :
	// (1) two overloads are unresolved
	// (2) return type is not defined when is_target<T>::value is false
	// **************************************************************** //
/*  typename std::enable_if<!is_target<T>::value, void>::type g()
	{
		std::cout << "\ng non-target implementation";
	}
	typename std::enable_if<is_target<T>::value, void>::type g()
	{
		std::cout << "\ng is-target implementation";
	} */

	// Solution :
	// Introduce dummy template parameter U that has to be deduced, given default type U=T
	template<typename U = T> typename std::enable_if<!is_target<U>::value, void>::type g()
	{
		std::cout << "\nf non-target implementation";
	}
	template<typename U = T> typename std::enable_if<is_target<U>::value, void>::type g()
	{
		std::cout << "\nf is-target implementation";
	}
};

void test_sfinae_class()
{
	my_class<int> x0;
	my_class<std::string> x1;
	my_class<A> x2;
	my_class<B> x3;

	// Specialization depends on U, not on T
	x0.f(123);
	x0.f(std::string("abc"));
	x0.f(A{});
	x0.f(B{});

	x0.g();
	x1.g();
	x2.g();
	x3.g();
}
