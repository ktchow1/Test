#include<iostream>
#include<memory>
#include<vector>

// *************** //
// *** Deleter *** //
// *************** //
#include<memory>
struct X
{
    X()  { std::cout << "\nX::X() invoked";  }
    ~X() { std::cout << "\nX::~X() invoked"; }

    void print() const
    {
        std::cout << "\nX = " << a << " " << b << " " << c;
    }

    std::uint32_t a = 1;
    std::uint32_t b = 2;
    std::uint32_t c = 3;
};

struct X_deleter
{
    // It is responsible for calling X destructor.
    void operator()(X* ptr)
    {
        ptr->~X();
        std::cout << "\nspecific deleter is invoked";
    }
};

template<typename T> struct general_deleter
{
    void operator()(T* ptr)
    {
        ptr->~T();
        std::cout << "\ngeneral deleter is invoked";
    }
};

template<typename C> struct container_deleter
{
    using T = typename C::value_type;
    void operator()(T* ptr)
    {
        ptr->~T();
        std::cout << "\ncontainer deleter is invoked";
    }
};

void test_deleter()
{
    std::cout << "\n\nmanual invoke destructor (double destruction)";
    {
        X x;
        x.~X();
    }
    std::cout << "\n\nspecific deleter";
    {
    //  auto up0 = std::make_unique<X, X_deleter>(X_deleter{}); // make_unique does not work here
        auto up0 = std::unique_ptr<X, X_deleter>(new X, X_deleter{});
        up0->print();

        auto up1 = std::unique_ptr<X, X_deleter>{};
    }
    std::cout << "\n\ngeneral deleter";
    {
        auto up = std::unique_ptr<X, general_deleter<X>>(new X, general_deleter<X>{});
        up->print();
    }
    std::cout << "\n\ncontainer deleter";
    {
        auto up = std::unique_ptr<X, container_deleter<std::vector<X>>>(new X, container_deleter<std::vector<X>>{});
        up->print();
    }
}

// ********************************* //
// *** Deleter for resource pool *** //
// ********************************* //
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

void test_deleter_for_pool()
{
     auto x = std::make_unique<resource>((std::uint32_t)1,(std::uint32_t)2,(std::uint32_t)3);
     std::cout << "\n" << *x;
     std::cout << "\n";

     resource_pool<5> pool;
     auto x0 = pool.make_unique((std::uint32_t)11,(std::uint32_t)12,(std::uint32_t)13);
     auto x1 = pool.make_unique((std::uint32_t)21,(std::uint32_t)22,(std::uint32_t)23);
     {
         auto x2 = pool.make_unique((std::uint32_t)31,(std::uint32_t)32,(std::uint32_t)33);
         auto x3 = pool.make_unique((std::uint32_t)41,(std::uint32_t)42,(std::uint32_t)43);
         pool.debug();
         std::cout << "\n";
     }
     pool.debug();
     std::cout << "\n";

     auto x4(std::move(x1)); 
     auto x5 = pool.make_unique((std::uint32_t)51,(std::uint32_t)52,(std::uint32_t)53);
     auto x6 = pool.make_unique((std::uint32_t)61,(std::uint32_t)62,(std::uint32_t)63);
     auto x7 = pool.make_unique((std::uint32_t)71,(std::uint32_t)72,(std::uint32_t)73);
     auto x8 = pool.make_unique((std::uint32_t)81,(std::uint32_t)82,(std::uint32_t)83);
     pool.debug();
     std::cout << "\n";
     
     if (x6) std::cout << "\nx6 = " << *x6; else std::cout << "\nx6 is null";
     if (x7) std::cout << "\nx7 = " << *x7; else std::cout << "\nx7 is null";
     if (x8) std::cout << "\nx8 = " << *x8; else std::cout << "\nx8 is null";
     std::cout << "\n";
}


