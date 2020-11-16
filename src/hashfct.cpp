#include<iostream>
#include<functional> // why not use std::hash offered by functional
#include<unordered_map>

struct my_hash
{
    int operator()(unsigned char n) const
    {
        n = n ^ (n << 13);
        n = n ^ (n >> 17);
        n = n ^ (n << 5);
        return n;
    }
};

struct my_hash0
{
    int operator()(const std::string& s) const
    {
        return s.size();
    }
};

struct my_hash1
{
    int operator()(const std::string& s) const
    {
        int key = 0;
        for (auto c : s)
        {
            int t;
            t = c ^ (c << 13);
            t = t ^ (t >> 17);
            t = t ^ (t << 5);
            key = 31 * key + t;
        }
        return key;
    }
};

void test_hashfct0()
{
    std::unordered_map<std::string, int, my_hash1> map; map.reserve(20);
    map["abc"] = 1;
    map["This is an apple"] = 2;
    map["-----"] = 3;
    map["Peter"] = 4;
    map["MSVS"] = 5;
    map["ABCDEFG"] = 5;
    map["123"] = 6;
    map["SFINAE"] = 7;
    map["hash table..."] = 8;
    map["template is complex"] = 9;

    std::cout << "\nbuckets = " << map.bucket_count();
    for (int n = 0; n != map.bucket_count(); ++n)
    {
        std::cout << "\nbucket[" << n << "] ";
        for (auto iter = map.begin(n); iter != map.end(n); ++iter) std::cout << "[" << iter->first << ":" << iter->second << "] ";
    }
    std::cout << "\n\n\n";
}

void test_hashfct1()
{
    std::hash<unsigned char> h0;
    my_hash h1;
    for(unsigned char n=0; n!=200; ++n) std::cout << "\nhash[" << (int)n << "] = " << h0(n) << ", " << h1(n);
}
