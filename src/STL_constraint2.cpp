/* // requires YLib

#include<iostream>
#include<functional.h>
#include<sorted_list.h>
#include<Hitter/sorted_list.h>
#include<string>
#include<unordered_map>

// ********************************************************************************** //
// *** Study whether map <-> list is good enough for hitter (the following works) *** //
// ********************************************************************************** //
struct index_and_iter;
   using LIST_TYPE = YLib::SortedList <index_and_iter, std::less>; 
// using LIST_TYPE = YLib::SortedList <index_and_iter, std::greater>; 
// using LIST_TYPE = YLib::sorted_list<index_and_iter, std::less   <index_and_iter>>; 
// using LIST_TYPE = YLib::sorted_list<index_and_iter, std::greater<index_and_iter>>; 

struct T
{
    std::uint32_t info0;
    std::uint32_t info1;
    std::uint32_t info2;
    std::uint32_t info3;

    typename LIST_TYPE::iterator iter;   
};

struct index_and_iter
{
    std::uint32_t index;
    std::unordered_map<std::string, T>::iterator iter;

    bool operator==(const index_and_iter& rhs) const
    {
        return index == rhs.index;
    }
    auto operator<=>(const index_and_iter& rhs) const
    {
        return index <=> rhs.index;
    }    
};

void test_STL_constraint3()
{
    std::unordered_map<std::string, T> map;
    LIST_TYPE list;       // for SortedList
//  LIST_TYPE list(1024); // for sorted_list

    map["CASH"] = T{10,11,12,13,list.end()}; 
    map["EQTY"] = T{20,21,22,23,list.end()}; 
    map["CBBC"] = T{30,31,32,33,list.end()}; 
    map.emplace("BOND", T{40,41,42,43,list.end()});
//  map.emplace("BOND",  {40,41,42,43,list.end()}); // compile error 
//  map.emplace("BOND",   40,41,42,43,list.end() ); // compile error 
    
    auto i0 = map.find("CASH");
    auto ix = list.emplace(index_and_iter{123, i0});
    i0->second.iter = ix;
    
    auto i1 = map.find("EQTY");
    auto iy = list.emplace(index_and_iter{234, i1});
    i1->second.iter = iy;

    auto i2 = map.find("CBBC");
    auto iz = list.emplace(index_and_iter{345, i2});
    i2->second.iter = iz;

    auto i3 = map.find("BOND");
    auto iw = list.emplace(index_and_iter{456, i3});
    i3->second.iter = iw;

    for(const auto&x : list)
    {
        std::cout << "\n" << x.index 
                  << " "  << x.iter->first
                  << " "  << x.iter->second.info0 
                  << " "  << x.iter->second.info1
                  << " "  << x.iter->second.info2
                  << " "  << x.iter->second.info3
                  << " "  << x.iter->second.iter->index;
    }
}

*/
