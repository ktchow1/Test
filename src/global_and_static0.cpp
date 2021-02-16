#include <iostream>

       std::uint32_t global_var0 = 100;  // single file  global (can  be accessed through extern from another cpp)
static std::uint32_t global_var1 = 200;  // single file  global (cant be accessed through extern from another cpp)
extern std::uint32_t global_var2;        // across files global
extern std::uint32_t global_var3;        // we can extern, but cannot access

void access_global();
void access_static();
void test_global_and_static0()
{
    std::cout << "\nglobal_var0 = " << global_var0;
    std::cout << "\nglobal_var1 = " << global_var1;
    std::cout << "\nglobal_var2 = " << global_var2;
//  std::cout << "\nglobal_var3 = " << global_var3; // compile error
    access_static();
    std::cout << "\n";

    ++global_var0;
    ++global_var1;
    ++global_var2;
    std::cout << "\nglobal_var0 = " << global_var0;
    std::cout << "\nglobal_var1 = " << global_var1;
    std::cout << "\nglobal_var2 = " << global_var2;
    access_static();
    std::cout << "\n";

    access_global();
    std::cout << "\nglobal_var0 = " << global_var0;
    std::cout << "\nglobal_var1 = " << global_var1;
    std::cout << "\nglobal_var2 = " << global_var2;
    access_static();
    std::cout << "\n";
}


