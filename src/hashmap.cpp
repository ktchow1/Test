#include<iostream>
#include<string>
#include<optional>
#include<functional>
#include<unordered_map>

template<typename K, typename V> struct hashmap
{
    struct cell
    {
        bool flag = false;
        size_t hashed_key = 0;
        K key;
        V value;
    };

    hashmap(int size=8) : size(size), mask(size-1), ptr(new cell[size])
    {
        for(int n=0; n!=size; ++n) if (ptr[n].flag) std::cout << "\nincorrect initialization" << std::flush;
    }

    ~hashmap()
    {
        if (ptr) delete [] ptr;
    }

    bool set(const K& key, const V& value)
    {
        std::hash<K> hash_fct;
        auto hashed_key = hash_fct(key);        
        
        for(int n=0; n!=size; ++n)
        {
            auto m = (hashed_key+n) & mask;
            if (ptr[m].flag == false || ptr[m].hashed_key == hashed_key || ptr[m].key == key)
            {
                ptr[m].flag       = true;
                ptr[m].hashed_key = hashed_key;
                ptr[m].key        = key;
                ptr[m].value      = value; 
                return true; 
            }
            else 
            {
                if (n==0) std::cout << " ... probing occurs" << std::flush; // for debugging
                continue;
            }            
        }
        return false;
    }

    std::optional<V> get(const K& key) const
    {        
        std::hash<K> hash_fct;
        auto hashed_key = hash_fct(key);

        std::optional<V> output;
        for(int n=0; n!=size; ++n)
        {
            auto m = (hashed_key + n) & mask;
            if      (ptr[m].flag == false)            { return std::nullopt; }
            else if (ptr[m].hashed_key == hashed_key) { return std::make_optional(ptr[m].value); }
            else if (ptr[m].key == key)               { return std::make_optional(ptr[m].value); }
            else continue;
        }
        return std::nullopt;
    }

    void debug() const
    {
        std::cout << "\ndebug hashmap";
        for(int n=0; n!=size; ++n)
        {
            std::cout << "\n" << n 
                      << " "  << ptr[n].flag 
                      << " "  << ptr[n].hashed_key
                      << " "  << ptr[n].key
                      << " "  << ptr[n].value;
        }
    }

    int size = 0;
    int mask = 0;
    cell* ptr = nullptr;
};

void test_hashmap()
{
    hashmap<std::string, int> h;
    std::cout << "\nset " << std::flush; h.set("axx", 101);
    std::cout << "\nset " << std::flush; h.set("bxx", 202);
    std::cout << "\nset " << std::flush; h.set("cxx", 303);
    std::cout << "\nset " << std::flush; h.set("dxx", 404);
    std::cout << "\nset " << std::flush; h.set("a..", 111);
    std::cout << "\nset " << std::flush; h.set("b..", 222);
    std::cout << "\nset " << std::flush; h.set("c..", 333);
    std::cout << "\nset " << std::flush; h.set("d..", 444);

    std::optional<int> x;
    std::cout << "\nget "; x = h.get("axx");  if (x) std::cout << *x; else std::cout << "not found";
    std::cout << "\nget "; x = h.get("ayy");  if (x) std::cout << *x; else std::cout << "not found";
    std::cout << "\nget "; x = h.get("bxx");  if (x) std::cout << *x; else std::cout << "not found";
    std::cout << "\nget "; x = h.get("byy");  if (x) std::cout << *x; else std::cout << "not found";
    std::cout << "\n";    
    h.debug();
    std::cout << "\n\n\n";
}

void test_hashmap2()
{
    std::unordered_multimap<std::string, int> map;

//  map["abc"] = 1;           // compile error
//  map.insert("abc", 1);     // compile error
    map.insert(std::make_pair("abc", 1));
    map.insert(std::make_pair("def", 2));
    map.insert(std::make_pair("def", 3));
    map.insert(std::make_pair("xyz", 4));
    map.insert(std::make_pair("abc", 5));
    map.insert(std::make_pair("abc", 6));

    for(const auto& x:map)
    {
        std::cout << "\n" << x.first << " : " << x.second;
    }

    std::cout << "\n\nsearching ...";
    auto range = map.equal_range("abc");
    for(auto iter = range.first; iter!=range.second; ++iter)
    {
        std::cout << "\n" << iter->first << " : " << iter->second;
    }
}
