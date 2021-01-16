#include<iostream>
#include<string>
#include<vector>
#include<tuple>
#include<utility>

// std::integer_sequence is a template class :
// 1. having no member
// 2. usually used as an argument of a template function
// 3. so as to deduce a variadic non-type pack of integers
// 4. used to access particular array-cell or tuple-element inside the template function 

template<typename C, std::size_t... NS> auto vec2tuple(const C& container) 
{
    return std::make_tuple(container[NS]...);
}

template<typename T, std::size_t... NS> auto tuple2tuple(const T& tuple)
{
    return std::make_tuple(std::get<NS>(tuple)...);
}

template<typename C, std::size_t... NS> 
auto vec2tuple_auto(const C& container, const std::index_sequence<NS...>& dummy)
{
    return std::make_tuple(container[NS]...);
}

template<typename T, std::size_t... NS> 
auto tuple2tuple_auto(const T& tuple, const std::index_sequence<NS...>& dummy)
{
    return std::make_tuple(std::get<NS>(tuple)...);
}

void test_index_seq0()
{
    std::vector<std::string> v = {"111", "222", "333", "444", "555"};
    auto t = std::tuple<std::uint32_t, std::uint32_t, std::string, std::string, std::string>{111, 222 , "333", "444", "555"};

    // 1. Primitive objective (using variadic non-type pack) 
    { 
        auto t0 = vec2tuple<decltype(v),0,2,4>(v); // use decltype to avoid typing
        std::cout << "\nt0 = " << std::get<0>(t0) << " " << std::get<1>(t0) << " " << std::get<2>(t0);

        auto t1 = vec2tuple<decltype(v),4,3,2,1>(v);
        std::cout << "\nt1 = " << std::get<0>(t1) << " " << std::get<1>(t1) << " " << std::get<2>(t1) << " " << std::get<3>(t1);

        auto t2 = tuple2tuple<decltype(t),0,2,4>(t);
        std::cout << "\nt2 = " << std::get<0>(t2) << " " << std::get<1>(t2) << " " << std::get<2>(t2);

        auto t3 = tuple2tuple<decltype(t),4,3,2,1>(t);
        std::cout << "\nt3 = " << std::get<0>(t3) << " " << std::get<1>(t3) << " " << std::get<2>(t3) << " " << std::get<3>(t3);
    }

    // 2. Auto deduction with argument : integer_sequence
    { 
        auto t0 = vec2tuple_auto(v, std::index_sequence<0,2,4>{}); 
        std::cout << "\nt0 = " << std::get<0>(t0) << " " << std::get<1>(t0) << " " << std::get<2>(t0);

        auto t1 = vec2tuple_auto(v, std::index_sequence<4,3,2,1>{});
        std::cout << "\nt1 = " << std::get<0>(t1) << " " << std::get<1>(t1) << " " << std::get<2>(t1) << " " << std::get<3>(t1);

        auto t2 = tuple2tuple_auto(t, std::index_sequence<0,2,4>{});
        std::cout << "\nt2 = " << std::get<0>(t2) << " " << std::get<1>(t2) << " " << std::get<2>(t2);

        auto t3 = tuple2tuple_auto(t, std::index_sequence<4,3,2,1>{});
        std::cout << "\nt3 = " << std::get<0>(t3) << " " << std::get<1>(t3) << " " << std::get<2>(t3) << " " << std::get<3>(t3);
    }

    // 3. Factory for contiguous sequence
    { 
        auto t0 = vec2tuple_auto(v, std::make_index_sequence<3>{}); 
        std::cout << "\nt0 = " << std::get<0>(t0) << " " << std::get<1>(t0) << " " << std::get<2>(t0);

        auto t1 = vec2tuple_auto(v, std::make_index_sequence<4>{});
        std::cout << "\nt1 = " << std::get<0>(t1) << " " << std::get<1>(t1) << " " << std::get<2>(t1) << " " << std::get<3>(t1);

        auto t2 = tuple2tuple_auto(t, std::make_index_sequence<3>{});
        std::cout << "\nt2 = " << std::get<0>(t2) << " " << std::get<1>(t2) << " " << std::get<2>(t2);

        auto t3 = tuple2tuple_auto(t, std::make_index_sequence<4>{});
        std::cout << "\nt3 = " << std::get<0>(t3) << " " << std::get<1>(t3) << " " << std::get<2>(t3) << " " << std::get<3>(t3);
    }
}

// Now lets try my own implementation ... 
template<std::size_t... NS> 
struct seq
{
};

/* Consider : make_seq<N> = seq_gen<N>::type
                          = seq_gen<N-1,N-1>::type 
                          = seq_gen<N-2,N-2,N-1>::type 
                          = seq_gen<N-3,N-3,N-2,N-1>::type 
                          = ...
                          = seq_gen<1,1,2,3,...,N-3,N-2,N-1>::type 
                          = seq_gen<0,0,1,2,3,...,N-3,N-2,N-1>::type 
                          = seq<0,1,2,3,...,N-3,N-2,N-1>                */

// ************************************** //
// *** Factory 1 : Contigous sequence *** //
// ************************************** //
template<std::size_t N, std::size_t... NS> struct seq_gen
{
    using type = typename seq_gen<N-1,N-1,NS...>::type;
//                                ^   ^
//                                |   |
//               next iteration --+   +-- create one element
};
template<std::size_t... NS> struct seq_gen<0, NS...>
{
    using type = seq<NS...>;
};
template<std::size_t N> 
using make_seq = typename seq_gen<N>::type; // alias

// *************************************** //
// *** Factory 2: Alternative sequence *** //
// *************************************** //
template<std::size_t N, std::size_t... NS> struct alt_seq_gen
{
    using type = typename alt_seq_gen<N-2,N-1,NS...>::type;
};
template<std::size_t... NS> struct alt_seq_gen<0, NS...> // NS = 1,3,5,...
{
    using type = seq<NS...>;
};
template<std::size_t... NS> struct alt_seq_gen<1, NS...> // NS = 2,4,6,...
{
    using type = seq<0,NS...>;
};
template<std::size_t N> 
using make_alt_seq = typename alt_seq_gen<N>::type; // alias

// ************************************************************* //
// *** Overload auto-deduce-functions with my implementation *** //
// ************************************************************* //
template<typename C, std::size_t... NS> 
auto vec2tuple_auto(const C& container, const seq<NS...>& dummy)
{
    return std::make_tuple(container[NS]...);
}

template<typename T, std::size_t... NS> 
auto tuple2tuple_auto(const T& tuple, const seq<NS...>& dummy)
{
    return std::make_tuple(std::get<NS>(tuple)...);
}

void test_index_seq1()
{
    std::cout << "\n" << std::is_same<make_seq<4>, seq<0,1,2>>::value
                      << std::is_same<make_seq<4>, seq<0,1,2,3>>::value
                      << std::is_same<make_seq<4>, seq<0,1,2,3,4>>::value;
    std::cout << "\n" << std::is_same<make_alt_seq<7>, seq<0,2,4>>::value
                      << std::is_same<make_alt_seq<7>, seq<1,3,5>>::value
                      << std::is_same<make_alt_seq<7>, seq<0,2,4,6>>::value
                      << std::is_same<make_alt_seq<7>, seq<1,3,5,7>>::value
                      << std::is_same<make_alt_seq<7>, seq<0,2,4,6,8>>::value
                      << std::is_same<make_alt_seq<7>, seq<1,3,5,7,9>>::value;
    std::cout << "\n" << std::is_same<make_alt_seq<8>, seq<0,2,4>>::value
                      << std::is_same<make_alt_seq<8>, seq<1,3,5>>::value
                      << std::is_same<make_alt_seq<8>, seq<0,2,4,6>>::value
                      << std::is_same<make_alt_seq<8>, seq<1,3,5,7>>::value
                      << std::is_same<make_alt_seq<8>, seq<0,2,4,6,8>>::value
                      << std::is_same<make_alt_seq<8>, seq<1,3,5,7,9>>::value;

    std::vector<std::string> v = {"111", "222", "333", "444", "555", "666", "777"};
    auto t = std::tuple<std::uint32_t, std::uint32_t, std::string, std::string, std::string, std::string, std::string>{111, 222, "333", "444", "555", "666", "777"};

    // 4. Factory for alternative sequence
    auto t0 = vec2tuple_auto(v, make_alt_seq<5>{}); 
    std::cout << "\nt0 = " << std::get<0>(t0) << " " << std::get<1>(t0) << " " << std::get<2>(t0);

    auto t1 = vec2tuple_auto(v, make_alt_seq<6>{});
    std::cout << "\nt1 = " << std::get<0>(t1) << " " << std::get<1>(t1) << " " << std::get<2>(t1);

    auto t2 = tuple2tuple_auto(t, make_alt_seq<5>{});
    std::cout << "\nt2 = " << std::get<0>(t2) << " " << std::get<1>(t2) << " " << std::get<2>(t2);

    auto t3 = tuple2tuple_auto(t, make_alt_seq<6>{});
    std::cout << "\nt3 = " << std::get<0>(t3) << " " << std::get<1>(t3) << " " << std::get<2>(t3);
}

// ********************************************************* //
// *** Useful application : Invoke function on any tuple *** //
// ********************************************************* //
template<typename F, typename TUPLE, std::size_t...NS> void invoke_impl(F fct, const TUPLE& tuple, const std::index_sequence<NS...>& seq)
{
    fct(std::get<NS>(tuple)...);
}

template<typename F, typename... ARGS> void invoke(F fct, const std::tuple<ARGS...>& tuple) 
{
    invoke_impl(fct, tuple, std::make_index_sequence<sizeof...(ARGS)>{});
}

void test_index_seq2()
{
    auto t0 = std::tuple<std::uint32_t, std::uint32_t, std::string, std::string, std::string>{111, 222, "333", "444", "555"};
    auto t1 = std::tuple<std::uint32_t, std::uint32_t, std::string, std::string, std::string, std::string, std::string>{111, 222, "333", "444", "555", "666", "777"};

    // 5. Useful application - make the following code simpler ....
    auto f0 = [](std::uint32_t x0,
                 std::uint32_t x1,
                 const std::string& s0,
                 const std::string& s1,
                 const std::string& s2)
    {
        std::cout << "\n" << x0 << "_" << x1 << "_" 
                          << s0 << "_" << s1 << "_" << s2; 
    };

    auto f1 = [](std::uint32_t x0,
                 std::uint32_t x1,
                 const std::string& s0,
                 const std::string& s1,
                 const std::string& s2,
                 const std::string& s3,
                 const std::string& s4)
    {
        std::cout << "\n" << x0 << "_" << x1 << "_" 
                          << s0 << "_" << s1 << "_" 
                          << s2 << "_" << s3 << "_" << s4;
    };

    // Tradition way ... (1) cumbersome (2) need to change on adding element to t
    f0(std::get<0>(t0), std::get<1>(t0), std::get<2>(t0), std::get<3>(t0), std::get<4>(t0));
    f1(std::get<0>(t1), std::get<1>(t1), std::get<2>(t1), std::get<3>(t1), std::get<4>(t1), std::get<5>(t1), std::get<6>(t1));

    // New way ... (1) neat (2) no need to change on adding element to t
    invoke(f0,t0);
    invoke(f1,t1);
}













