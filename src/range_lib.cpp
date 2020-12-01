#include<iostream>
#include<vector>
#include<ranges>
#include<functional>

namespace alg
{
    // Just a wrapper supporting CTAD without explicity guide
    template<typename F> struct filter    { F impl; };
    template<typename F> struct transform { F impl; };
                         struct take      { std::uint32_t impl; };

    template<typename C> // C = container
    struct lazy_view
    {
    public:
        using T = typename C::value_type;

        template<typename F>
        lazy_view<C>& operator | (filter<F> x)
        {
            predicate = x.impl; 
            return *this;
        }

        template<typename F>
        lazy_view<C>& operator | (transform<F> x)
        {
            transformation = x.impl; 
            return *this;
        }

        lazy_view<C>& operator | (take x)
        {
            taken_count = 0;
            taken_total = x.impl; 
            return *this;
        }

        // casting operator
        operator C()
        {
            // This is lazy calculation.
            C output;
            for(auto i=i0; i!=i1; ++i)
            {
                if (predicate(*i))
                {
                    output.push_back(transformation(*i));

                    ++taken_count;
                    if (taken_count == taken_total) return output;
                }
            }
            return output;
        }

        typename C::iterator    i0;
        typename C::iterator    i1;    
        std::function<bool(T)>  predicate;
        std::function<T(T)>     transformation;
        std::uint32_t           taken_count;
        std::uint32_t           taken_total;
    };
    
    template<typename C, typename F>
    lazy_view<C> operator | (C& container, filter<F> x)
    {
        lazy_view<C> output;
        output.i0 = container.begin();
        output.i1 = container.end();
        output.predicate = x.impl;
        return output;
    }
}

void test_range_lib()
{
    std::vector<std::uint32_t> vec0;
    for(std::uint32_t n=0; n!=50; ++n)
    {
        vec0.push_back(rand()%100);
    }

    auto vec1 = vec0 | std::views::filter   ([](const auto& x) { return x%2==1; } )
                     | std::views::transform([](const auto& x) { return x*2;    } )
                     | std::views::take(10);
    auto vec2 = vec0 | std::views::filter   ([](const auto& x) { return x%2==1; } )
                     | std::views::transform([](const auto& x) { return x*2;    } );
    
    decltype(vec0) vec3;
    for(const auto& x : vec0 | std::views::filter   ([](const auto& x) { return x%2==1; } )
                             | std::views::transform([](const auto& x) { return x*2;    } ))
    {
        vec3.push_back(x);
    }

    // My algo implementation is not good enough :
    // 1. does not support "auto" vec4
    // 2. does not support "auto" argument in lambdas
    std::vector<std::uint32_t> vec4 = vec0 | alg::filter   ([](const std::uint32_t& x) { return x%2==1; } )
                                           | alg::transform([](const std::uint32_t& x) { return x*2;    } ) 
                                           | alg::take(10); 

    std::cout << "\nvec0 = "; for(const auto& x:vec0) std::cout << x << " ";
    std::cout << "\nvec1 = "; for(const auto& x:vec1) std::cout << x << " ";
    std::cout << "\nvec2 = "; for(const auto& x:vec2) std::cout << x << " ";
    std::cout << "\nvec3 = "; for(const auto& x:vec3) std::cout << x << " ";
    std::cout << "\nvec4 = "; for(const auto& x:vec4) std::cout << x << " ";
}


