#include<iostream>
#include<string>
#include<string.h> // for strcpy and strncpy

void test_cstr()
{
    // char array
    char ac0[8] = {'a','b','c','d','e','f','g','h'}; // method 1 : like array of T
    char ac1[8] = "abcdefg";                         // method 2 : shortcut for array of char
//  char ac2[8] = "abcdefgh";                        // array cannot accommodate '\0'

    // null-terminated char array
    char* pc0 = new char[64];
    char* pc1 = new char[64];
    for(std::uint32_t n=0; n!=46; ++n) pc0[n] = 128;
    for(std::uint32_t n=0; n!=46; ++n) pc1[n] = 128;


    strcpy (pc0, "abcdefgh");     // copy until null-terminated
    strncpy(pc1, "abcdefgh", 8);  // copy until number is reached
    std::cout << "\npc0[8] = " << (unsigned int)(pc0[8]);
    std::cout << "\npc1[8] = " << (unsigned int)(pc1[8]);

    // char array string
    // --- refer to YLib::static_string

    // null-terminated string
    std::string s0(pc0);          // '\0' is existing in source
    std::string s1(pc1);          // '\0' not exist in source
    std::string s2(pc1,8);        // '\0' not exist in source, but is appended

    std::cout << "\ns0 = " << s0;
    std::cout << "\ns1 = " << s1;
    std::cout << "\ns2 = " << s2;
    std::cout << "\n\n";
}