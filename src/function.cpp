#include<iostream>


struct A
{
    std::uint32_t x; 
    std::uint32_t y; 
    std::uint32_t z; 
};

// pass fixed-size const array by reference
void fct(const A (&array)[5]) 
{
    for(std::uint32_t n=0; n!=5; ++n)
    {
        std::cout << "\n" << array[n].x << " " << array[n].y << " " << array[n].z;
    }
}

void test_function_array_arg()
{
    A temp[5];
    temp[0] = {11,12,13};
    temp[1] = {21,22,23};
    temp[2] = {31,32,33};
    temp[3] = {41,42,43};
    temp[4] = {51,52,53};

    fct(temp);
}

// ************* //
// *** macro *** //
// ************* //
#define DECLARE_MEMBER(type, name0, name1) \
    type name0##_##name1  \

struct B
{
    DECLARE_MEMBER(std::uint32_t, mem, x);
    DECLARE_MEMBER(std::uint32_t, mem, y);
    DECLARE_MEMBER(std::uint32_t, ptr, z);
    void fct() const
    {
        std::cout << "\nB object = " << mem_x << " " << mem_y << " " << ptr_z;
    }
};

void test_function_macro()
{
    B b;
    b.mem_x = 123;
    b.mem_y = 234;
    b.ptr_z = 345;
    b.fct();
}
