#include<iostream>
#include<vector>
#include<string>

// For example :
// CT = std::vector<int>,         return std::true_type
// CT = std::vector<std::string>, return std::true_type

// ************************ //
// *** Implementation 0 *** //
// ************************ //
template<typename CT>
struct is_vector0
{
private:
    using CT_REMOVED = std::remove_cvref_t<CT>;

    template<typename T>
    static auto impl(std::vector<T>*) -> std::bool_constant<std::is_same_v<CT_REMOVED, std::vector<T>>>;
    static std::false_type impl(void*); // no definition needed

public:
    using type = decltype(impl(std::declval<CT_REMOVED*>()));
};

// ************************ //
// *** Implementation 1 *** //
// ************************ //
template<typename CT>
struct is_vector1
{
private:
    using CT_REMOVED = std::remove_cvref_t<CT>;

    template<typename T>
    static auto impl(std::vector<T>*) -> std::true_type;
    static std::false_type impl(void*); // no definition needed

public:
    using type = decltype(impl(std::declval<CT_REMOVED*>()));
};

// ************************ //
// *** Implementation 2 *** //
// ************************ //
template<typename CT> struct is_vector2                        { using type = std::false_type; };
template<typename T>  struct is_vector2<std::vector<T>>        { using type = std::true_type;  };
template<typename T>  struct is_vector2<const std::vector<T>>  { using type = std::true_type;  };
template<typename T>  struct is_vector2<const std::vector<T>&> { using type = std::true_type;  };

void test_is_vector()
{
    std::cout << "\n" << is_vector0<int>::type::value;
    std::cout << " "  << is_vector0<std::string>::type::value;
    std::cout << " "  << is_vector0<std::vector<int>>::type::value;
    std::cout << " "  << is_vector0<std::vector<std::string>>::type::value;
    std::cout << " "  << is_vector0<const std::vector<int>>::type::value;
    std::cout << " "  << is_vector0<const std::vector<std::string>>::type::value;
    std::cout << " "  << is_vector0<const std::vector<int>&>::type::value;
    std::cout << " "  << is_vector0<const std::vector<std::string>&>::type::value;
    std::cout << " "  << is_vector0<std::vector<int>&>::type::value;
    std::cout << " "  << is_vector0<std::vector<std::string>&>::type::value;

    std::cout << "\n" << is_vector1<int>::type::value;
    std::cout << " "  << is_vector1<std::string>::type::value;
    std::cout << " "  << is_vector1<std::vector<int>>::type::value;
    std::cout << " "  << is_vector1<std::vector<std::string>>::type::value;
    std::cout << " "  << is_vector1<const std::vector<int>>::type::value;
    std::cout << " "  << is_vector1<const std::vector<std::string>>::type::value;
    std::cout << " "  << is_vector1<const std::vector<int>&>::type::value;
    std::cout << " "  << is_vector1<const std::vector<std::string>&>::type::value;
    std::cout << " "  << is_vector1<std::vector<int>&>::type::value;
    std::cout << " "  << is_vector1<std::vector<std::string>&>::type::value;

    std::cout << "\n" << is_vector2<int>::type::value;
    std::cout << " "  << is_vector2<std::string>::type::value;
    std::cout << " "  << is_vector2<std::vector<int>>::type::value;
    std::cout << " "  << is_vector2<std::vector<std::string>>::type::value;
    std::cout << " "  << is_vector2<const std::vector<int>>::type::value;
    std::cout << " "  << is_vector2<const std::vector<std::string>>::type::value;
    std::cout << " "  << is_vector2<const std::vector<int>&>::type::value;
    std::cout << " "  << is_vector2<const std::vector<std::string>&>::type::value;
    std::cout << " "  << is_vector2<std::vector<int>&>::type::value;
    std::cout << " "  << is_vector2<std::vector<std::string>&>::type::value;
}



