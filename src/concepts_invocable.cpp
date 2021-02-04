#include <iostream>
#include <functional>
#include <concepts>
#include <string>
using namespace std::string_literals;


struct RESULT
{
    std::string str;
};

inline std::ostream& operator<<(std::ostream& os, const RESULT& result)
{
    os << result.str << "_RESULT";
    return os;
}

template<std::invocable<std::string> F>
void custom_invoke(const F& fct, const std::string& str)
{
    std::cout << "\ninvoke : " << fct(str);
}

// Global function returning string
std::string f0(const std::string& str)
{
    return "f0_"s + str;
}

// Global function returning POD
RESULT f1(const std::string& str)
{
    return RESULT("f1_"s + str);
}

// Functor
struct f2
{
    std::string operator()(const std::string& str) const
    {
        return "f2_"s + str;
    }
};

// Function member
struct f3
{
    std::string mem(const std::string& str) const
    {
        return "f3_"s + str;
    }
};

void test_concepts_invocable()
{
    std::string str = "sample";

    custom_invoke(f0, str);
    custom_invoke(f1, str);
    custom_invoke(f2(), str);
    custom_invoke(std::bind(&f3::mem, f3(), std::placeholders::_1), str);
    custom_invoke([](const auto& s) { return RESULT("f4_"s + s); }, str); // Lambda
}
