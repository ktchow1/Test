#include<iostream>
#include<vector>
#include<queue>
#include<tuple>
#include<string>
#include<type_traits>

// alias for value (keyword : constexpr)
template<typename T, typename U>
constexpr bool is_same = std::is_same<T, U>::value;

// alias for type (keyword : using)
template<typename T> using add_ref = T&;
template<typename T> using del_ref = typename std::remove_reference<T>::type;

void test_template_alias()
{
	std::cout << "\n" << std::is_same<int, std::string>::value;
	std::cout << "\n" << std::is_same<int, int>::value;
	std::cout << "\n" << is_same<int, std::string>;
	std::cout << "\n" << is_same<int, int>;
	std::cout << "\n" << is_same<int, int&>;
	std::cout << "\n" << is_same<int, del_ref<int&>>;

	std::vector<int> v{ 1,2,3,4,5,6,7 };
	std::cout << "\n"; for (const auto& x : v) std::cout << x << " ";
	int x0 = v[3]; x0 = 9;
	std::cout << "\n"; for (const auto& x : v) std::cout << x << " ";
	int& x1 = v[3]; x1 = 9;
	std::cout << "\n"; for (const auto& x : v) std::cout << x << " ";
	del_ref<int&> x2 = v[4]; x2 = 9;
	std::cout << "\n"; for (const auto& x : v) std::cout << x << " ";
	add_ref<int> x3 = v[4]; x3 = 9;
	std::cout << "\n"; for (const auto& x : v) std::cout << x << " ";
	std::cout << "\n\n\n";
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
//	print_type<std::common_type<A,B>::type>(); // compile error
	print_type<std::common_type<A,C>::type>();
	print_type<std::common_type<C,A>::type>();
//	print_type<std::common_type<A,B,C>::type>(); // compile error
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


