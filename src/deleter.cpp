#include<iostream>
#include<memory>

struct resource
{
    resource(std::uint32_t a_=1, std::uint32_t b_=2, std::uint32_t c_=3) : a(a_),b(b_),c(c_)
    {
    }

    std::uint32_t a;
    std::uint32_t b;
    std::uint32_t c;
};

std::ostream& operator<<(std::ostream& os, const resource& x)
{
    os << "resource: a=" << x.a << " b=" << x.b << " c=" << x.c << std::flush;
    return os;
}

template<std::uint32_t N> class resource_pool
{
public:
    resource_pool()
    {
        for(std::uint32_t n=0; n!=N; ++n)
        {
            flags[n] = true;
        }
    }

    class resource_deleter
    {
    public:
        resource_deleter(resource_pool<N>& pool_, std::uint32_t index_) : pool(pool_), index(index_)
        {
        }

        void operator()(resource* ptr)
        {
            pool.flags[index] = true;
        }

        resource_pool<N>& pool;
        std::uint32_t index;
    };

public:
    static const std::uint32_t size = N;
    friend class resource_deleter;

    // Normally we cannot assign smart pointer to stack object,
    // unless we provide appropriate deleter in declaring output.
    template<typename... ARGS> auto make_unique(ARGS&&... args)
    {
        for(std::uint32_t n=0; n!=N; ++n)
        {
            if (flags[n])    
            {
                flags[n] = false;

                resource_deleter deleter(*this, n);
                std::unique_ptr<resource, resource_deleter> output(nullptr, deleter);
                output.reset
                (
                    new (&impl[n]) resource{std::forward<ARGS>(args)...}
                );  
                return output;
            }
        }
        resource_deleter deleter(*this, 0);
        std::unique_ptr<resource, resource_deleter> output(nullptr, deleter);
        return output; 
    }

    void debug() const 
    {
        for(std::uint32_t n=0; n!=N; ++n)
        {
            std::cout << "\nimpl[" << n << "] flag=" << flags[n];
            if (!flags[n]) std::cout << " " << impl[n];
        }
    }


private:
    resource impl[N];
    bool flags[N]; // true for available
}; 

void test_deleter()
{
     auto x = std::make_unique<resource>(1,2,3);
     std::cout << "\n" << *x;
     std::cout << "\n";

     resource_pool<5> pool;
     auto x0 = pool.make_unique(11,12,13);
     auto x1 = pool.make_unique(21,22,23);
     {
         auto x2 = pool.make_unique(31,32,33);
         auto x3 = pool.make_unique(41,42,43);
         pool.debug();
         std::cout << "\n";
     }
     pool.debug();
     std::cout << "\n";

     auto x4(std::move(x1)); 
     auto x5 = pool.make_unique(51,52,53);
     auto x6 = pool.make_unique(61,62,63);
     auto x7 = pool.make_unique(71,72,73);
     auto x8 = pool.make_unique(81,82,83);
     pool.debug();
     std::cout << "\n";
     
     if (x6) std::cout << "\nx6 = " << *x6; else std::cout << "\nx6 is null";
     if (x7) std::cout << "\nx7 = " << *x7; else std::cout << "\nx7 is null";
     if (x8) std::cout << "\nx8 = " << *x8; else std::cout << "\nx8 is null";
     std::cout << "\n";
}



