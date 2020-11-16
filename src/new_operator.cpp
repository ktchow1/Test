#include<iostream>


char buffer[1024];

struct my_class
{
    my_class() { std::cout << "\nconstructor invoked" << std::flush; }
   ~my_class() { std::cout << "\ndestructor invoked" << std::flush; }
    my_class(std::uint32_t A, std::uint32_t B, std::uint32_t C) : a(A), b(B), c(C)
    {
        std::cout << "\nconstructor ABC invoked" << std::flush;
    }

    void* operator new (size_t n) { std::cout << "\nnew operator invoked, size=" << n << std::flush; return buffer; }
    void* operator new[] (size_t n) { std::cout << "\nnew[] operator invoked, size=" << n << std::flush; return buffer; }
    void  operator delete (void* ptr) { std::cout << "\ndelete operator invoked" << std::flush; }
    void  operator delete[] (void* ptr) { std::cout << "\ndelete[] operator invoked" << std::flush; }

    void init(std::uint32_t A, std::uint32_t B, std::uint32_t C) { a=A; b=B; c=C; }
    void debug() const { std::cout << "\na=" << a << " b=" << b << " c=" << c; }

    std::uint32_t a=1;
    std::uint32_t b=2;
    std::uint32_t c=3;
};

void test_new_operator()
{
    auto* p0 = new my_class;
    p0->debug();
    delete p0;

    auto* p1 = new my_class[1]{{11,12,13}};
    p1->debug();
    delete[] p1;

    auto* p2 = new my_class[4]{{11,12,13},{21,22,23},{31,32,33},{41,42,43}};
    for(int n=0; n!=4; ++n) p2[n].debug();
    delete[] p2;

    auto* p3 = new my_class[4];
    for(int n=0; n!=4; ++n) p3[n].debug();
    std::cout << "\naddr of buffer = " << std::hex << (std::uint64_t)buffer;
    std::cout << "\naddr of p0[0]  = " << std::hex << (std::uint64_t)(p0);
    std::cout << "\naddr of p1[0]  = " << std::hex << (std::uint64_t)(p1);
    std::cout << "\naddr of p2[0]  = " << std::hex << (std::uint64_t)(p2);
    std::cout << "\naddr of p3[0]  = " << std::hex << (std::uint64_t)(p3);
    std::cout << "\naddr of p3[1]  = " << std::hex << (std::uint64_t)(p3+1);
    std::cout << "\naddr of p3[2]  = " << std::hex << (std::uint64_t)(p3+2);
    std::cout << "\naddr of p3[3]  = " << std::hex << (std::uint64_t)(p3+3);
    *(reinterpret_cast<std::uint64_t*>(buffer)) = 3;
    delete[] p3;
}    



