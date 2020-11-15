#include<iostream>
#include<vector>
#include<list>

// *********************** //
// *** About allocator *** //
// *********************** //
template<typename T> class my_allocator
{
public: // STL requires the following typedef
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T         value_type;

public:
    T* allocate(size_t n, const void* hints=0) // n = num of T-instances (not bytes)
    {
        T* p = new T[n];
        std::cout << "\nallocate " << std::dec << n << " objects at " << std::hex << (std::uint64_t)p;
        return p;
    }

    void deallocate(void* p, size_t n)
    {
        delete[] reinterpret_cast<T*>(p);
        std::cout << "\ndeallocate " << std::dec << n << " objects at " << std::hex << (std::uint64_t)p;
    }

private:
    // construct your own stack-mem allocator
};

void test_allocator()
{
    std::vector<int, my_allocator<int>> vec;
    std::list<int, my_allocator<int>> list;

    for(int n=0; n!=100; ++n) vec.push_back(n);
    std::cout << "\nvector : ";
    for(const auto& x:vec) std::cout << std::dec << x << " ";

    for(int n=0; n!=10; ++n) vec.pop_back();
    std::cout << "\nvector : ";
    for(const auto& x:vec) std::cout << std::dec << x << " ";

    for(int n=0; n!=10; ++n) list.push_back(n);
    std::cout << "\nlist : ";
    for(const auto& x:list) std::cout << std::dec << x << " ";

    std::cout << "\n\n";
}

