#include<iostream>
#include<string>
#include<variant>
#include<map>

struct pod
{
    std::uint32_t n;
    std::uint32_t m;
    std::string s0;
    std::string s1;
};

std::ostream& operator<<(std::ostream& os, const pod& x)
{
    os << x.n << " " << x.m << " " << x.s0 << " " << x.s1;
    return os;
}

template<typename T>
class pods
{
public:
    pods()
    {
        using namespace std::string_literals;
        map.emplace("0001"s, pod {11, 12, "abc"s, "def"s});
        map.emplace("0002"s, pod {21, 22, "bcd"s, "efg"s});
        map.emplace("0003"s, pod {31, 32, "cde"s, "fgh"s});
        map.emplace("0004"s, pod {41, 42, "def"s, "ghi"s});
        map.emplace("0005"s, pod {51, 52, "efg"s, "hij"s});
        map.emplace("0006"s, pod {61, 62, "fgh"s, "ijk"s});
    }

    template<typename MEMPTR, typename VARIANT>
    void change(const std::string& key, MEMPTR mptr, const VARIANT& variant) noexcept
    {
        auto iter = map.find(key);
        if (iter != map.end())
        {
            using TEMP = std::remove_cvref_t<decltype(std::declval<T>().*mptr)>;
            iter->second.*mptr = std::get<TEMP>(variant);
        }
    }

    void debug() const noexcept
    {
        for(const auto& [key, value] : map)
        {
            std::cout << "\n" << key << " : " << value;
        }
    }

private:
    std::map<std::string, T> map;
};

// ****************************************************************************** // 
// Imitate YLib::Hitter get relevant info from std::variant in HitterControlEvent //
// ****************************************************************************** // 
void test_variant_with_decltype()
{
    pods<pod> x;
    x.debug();
    std::cout << "\n";

    std::variant<std::uint32_t, std::string> v;
    using namespace std::string_literals;

    v = 99U;
    x.change("0001"s, &pod::n, v);
    x.change("0002"s, &pod::m, v);
    v = "XXX"s;
    x.change("0003"s, &pod::s0, v);
    x.change("0004"s, &pod::s1, v);
    v = "----"s;
    x.change("0005"s, &pod::s0, v);
    x.change("0006"s, &pod::s1, v);
    x.debug();

    std::cout << "\n\n";
}

