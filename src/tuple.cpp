#include<iostream>
#include<tuple>

struct A { void run() const { std::cout << "\nThis is A."; } };
struct B { void run() const { std::cout << "\nThis is B."; } };
struct C { void run() const { std::cout << "\nThis is C."; } };

// Please read : www.justsoftwaresolutions.co.uk/cplusplus/getting-tuple-elements-with-runtime-index.html
void test_tuple()
{
	typedef typename std::pair<A, B>::first_type A0;
	typedef typename std::pair<A, B>::second_type B0;
	std::cout << "\nis A0=A?   " << std::is_same<A0, A>::value;
	std::cout << "\nis B0=B?   " << std::is_same<B0, B>::value;

	typedef typename std::tuple_element<0, std::tuple<A,B,C>>::type A1;
	typedef typename std::tuple_element<1, std::tuple<A,B,C>>::type B1;
	typedef typename std::tuple_element<2, std::tuple<A,B,C>>::type C1;
	std::cout << "\nis A1=A?   " << std::is_same<A1,A>::value;
	std::cout << "\nis B1=B?   " << std::is_same<B1,B>::value;
	std::cout << "\nis C1=C?   " << std::is_same<C1,C>::value;
	std::cout << "\ntuple size " << std::tuple_size<std::tuple<A,B,C>>::value;	

	// method 1 : build a table naively
	// method 2 : build a table using variadic template


}
