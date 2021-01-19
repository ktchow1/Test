#include<iostream>
#include<vector>
#include<string>
#include<tuple>

// Techniques
// 1. iterate variadic parameter pack normally
//    iterate variadic std::tuple using std::apply
// 2. comma operator (the multiple brackets)
// 3. CTAD
// 4. "decltype + declval" to extract type


struct sample
{
    std::uint32_t n0;
    std::uint32_t n1;
    std::string   s0;
    std::string   s1;
};

inline std::ostream& operator<<(std::ostream& os, const sample& x)
{
    os << x.n0 << " " << x.n1 << " " << x.s0 << " " << x.s1;
    return os;
}

template<typename MPTR>
struct label_ptr
{
//  using type = decltype(std::declval<object_type>().*mptr);
    using ptr_type = MPTR;

    std::string key;
    MPTR mptr;
};

template<typename... LABEL_PTRS>
auto unzip(const std::tuple<LABEL_PTRS...>& tuple)
{
    std::vector<std::string> keys;
    std::tuple<typename LABEL_PTRS::ptr_type ...> mptrs;

    // Misconception : std::apply is called once
    // it simply converts fct("tuple") into fct("elements...")
    std::apply
    (
        [&keys, &mptrs](const LABEL_PTRS&... pairs)
        {
            // Comma operator (xxx,yyy,zzz);
            // put everything inside (,...); as if single line
            (keys.push_back(pairs.key),...);
            mptrs = std::make_tuple(pairs.mptr ...); // no comma needed here
        }
        , tuple
    );
    return std::make_pair(keys, mptrs);
}

template<typename... LABEL_PTRS>
struct ctad_resource
{
    ctad_resource(const std::tuple<LABEL_PTRS...>& tuple) : _tuple(tuple)
    {
    }

    std::tuple<LABEL_PTRS...> _tuple;
};

// *****************************************************************
// Tuple of member pointers
// (as they are of diff types, cannot use vector, use tuple instead)
// *****************************************************************
void test_psql_tuple()
{
    auto t0 = std::make_tuple(&sample::n0,
                              &sample::n1,
                              &sample::s0,
                              &sample::s1);

    // With CTAD, there is no need to specify the template type of label_ptr ...
    auto t1 = std::make_tuple(label_ptr{"AA", &sample::n0},
                              label_ptr{"BB", &sample::n1},
                              label_ptr{"CC", &sample::s0},
                              label_ptr{"DD", &sample::s1});

    sample x{1, 2, "abc", "peter"};
    std::cout << "\nsample = " << x;

    x.*std::get<0>(t0) = 11;
    x.*std::get<1>(t0) = 12;
    x.*std::get<2>(t0) = "def";
    x.*std::get<3>(t0) = "david";
    std::cout << "\nsample = " << x;

    x.*(std::get<0>(t1).mptr) = 21;
    x.*(std::get<1>(t1).mptr) = 22;
    x.*(std::get<2>(t1).mptr) = "ijk";
    x.*(std::get<3>(t1).mptr) = "susan";
    std::cout << "\nsample = " << x;

    auto [keys, mptrs] = unzip(t1);

    x.*(std::get<0>(mptrs)) = 31;
    x.*(std::get<1>(mptrs)) = 32;
    x.*(std::get<2>(mptrs)) = "mno";
    x.*(std::get<3>(mptrs)) = "frank";
    std::cout << "\nsample = " << x;
    std::cout << ", keys = "; for(const auto& key:keys) std::cout << key << " ";


    ctad_resource ctad(t1); 
    x.*(std::get<0>(ctad._tuple).mptr) = 41;
    x.*(std::get<1>(ctad._tuple).mptr) = 42;
    x.*(std::get<2>(ctad._tuple).mptr) = "xyz";
    x.*(std::get<3>(ctad._tuple).mptr) = "nancy";
    std::cout << "\nsample = " << x << " [Yes, CTAD works !!!]";
    std::cout << "\n\n";
}
