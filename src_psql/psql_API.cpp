#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<tuple>


// Terminologies :
// key   = sql::col_name = pod::mem_name
// type  = sql::col_type = pod::mem_type
// value = sql::row_data = pod::mem_value

template<typename MPTR> struct psql_item
{
    using ptr_type = MPTR;

    std::string key;
    MPTR mptr;
};

// Add different psql types here ...
template<typename T> std::string psql_type()                { return "INVALID"; }
template<>           std::string psql_type<std::uint16_t>() { return "INTEGER"; }
template<>           std::string psql_type<std::uint32_t>() { return "INTEGER"; }
template<>           std::string psql_type<std::uint64_t>() { return "INTEGER"; }
template<>           std::string psql_type<std::string>()   { return "TEXT";    }

template<typename T> std::string psql_value(const T& x)              { return "INVALID";         }
template<>           std::string psql_value(const std::uint16_t& x)  { return std::to_string(x); }
template<>           std::string psql_value(const std::uint32_t& x)  { return std::to_string(x); }
template<>           std::string psql_value(const std::uint64_t& x)  { return std::to_string(x); }
template<>           std::string psql_value(const std::string&   x)  { return x;                 }


// ********************** //
// *** Main interface *** //
// ********************** //
// Two similar techniques :
// 1. iterate parameter pack  in constructor (just a normal variadic template)
// 2. iterate std::tuple<...> in other functions (must need std::apply)

template<typename... ITEMS> class psql
{
public:
    using tuple_type = std::tuple<typename ITEMS::ptr_type ...>;

    psql(const std::string& table_name_, const ITEMS&... items) : table_name(table_name_)
    {
        // No std::apply is needed, just variadic unpack
        (keys.push_back(items.key),...);
        mptrs = std::make_tuple(items.mptr ...);
    }

public:
    template<typename POD>
    bool create() const
    {
        // ************** //
        // *** Part 1 *** // 
        // ************** //
        std::stringstream ss; 
        std::uint32_t n=0;
        std::apply
        (
            [this, &ss, &n](const typename ITEMS::ptr_type&... unpacked_mptrs)
            {
                ss << "(";
                // comma operator takes the form (XXX,...)
                // if XXX is complex, add extra bracket around (XXX)
                (( 
                    ss << keys[n++] << " " 
                       << psql_type<std::remove_cvref_t
                                   <decltype(std::declval<POD>().*unpacked_mptrs)>>()
                    // << psql_type<decltype(std::declval<POD>().*unpacked_mptrs)>() // Does not work, return INVALID
                       << (n!=keys.size()? ", " : "")
                ),...);
                ss << ")";
            }
            , mptrs
        );

        // ************** //
        // *** Part 2 *** // 
        // ************** //
        using namespace std::string_literals; // for string literal
        std::string create_str("CREATE TABLE ");
        create_str += " "s += table_name;
        create_str += " "s += ss.str();
        
        std::cout << "\n[PSQL] " << create_str;
        // psql.execute ...
        return true;
    }


public: 
    template<typename POD>
    bool insert(const std::vector<POD>& entries)
    {
        // *************** //
        // *** Part 1A *** // 
        // *************** //
        std::stringstream ss0; 
        ss0 << "(";
        for(std::uint32_t n=0; n!=keys.size(); ++n)
        {
            ss0 << keys[n] << (n!=keys.size()-1? ", ":"");
        }
        ss0 << ")";

        // *************** //
        // *** Part 1B *** // 
        // *************** //
        std::stringstream ss1; 
        for(std::uint32_t n=0; n!=entries.size(); ++n)
        { 
            std::uint32_t m=0;  
            std::apply
            (
                [this, &ss1, &entries, &n, &m](const typename ITEMS::ptr_type&... unpacked_mptrs)
                {
                    ss1 << "(";
                    ((
                        ss1 << psql_value(entries[n].*unpacked_mptrs)
                            << (++m!=keys.size()? ", " : "")
                    ),...); 
                    ss1 << ")" << (n!=entries.size()-1? ", ":"");
                }
                , mptrs 
            );
        }

        // *************** //
        // *** Part 2 *** // 
        // *************** //
        using namespace std::string_literals; // for string literal
        std::string insert_str("INSERT INTO");
        insert_str += " "s += table_name;
        insert_str += " "s += ss0.str();
        insert_str += " VALUES"s;
        insert_str += " "s += ss1.str();

        std::cout << "\n[PSQL] " << insert_str;
        // psql.execute ...
        return true;
    }

public: 
    template<typename POD>
    bool select(std::vector<POD>& output)
    {
        return true;
    }

private:
    std::string table_name;
    std::vector<std::string> keys; // This is a vector, as all elements are std::string.
    tuple_type mptrs;              // This is a tuple, as all elements are different types.
};


// *************** //
// *** Example *** //
// *************** //
enum class book_genre : std::uint32_t
{
    mathematics,
    programming,
    quant_finan
};

inline std::ostream& operator<<(std::ostream& os, const book_genre& genre)
{
    switch(genre)
    {
        case book_genre::mathematics : os << "MATH"; break;
        case book_genre::programming : os << "PROG"; break;
        case book_genre::quant_finan : os << "QFIN"; break;
    }
    return os;
}

// Add these functions for custom-made enum ...
template<> std::string psql_type<book_genre>() 
{
    return "VARCHAR(4)";
}

template<> std::string psql_value(const book_genre& x)  
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

struct book
{
    book_genre     genre;
    std::string    name;
    std::string    author;
    std::string    date_time;
    std::uint32_t  book_id;
    std::uint32_t  version;
};

inline std::ostream& operator<<(std::ostream& os, const book& b)
{
    os << " " << b.genre
       << " " << b.name
       << " " << b.author
       << " " << b.date_time
       << " " << b.book_id
       << "_" << b.version;
    return os;
}

// *************** //
// *** Testing *** //
// *************** //
void test_psql_API()
{
    psql db
    {
        "my_quant_library",
        psql_item("genre",     &book::genre), // can further optimize by using macro 
        psql_item("name",      &book::name),
        psql_item("author",    &book::author),
        psql_item("date_time", &book::date_time),
        psql_item("book_id",   &book::book_id),
        psql_item("version",   &book::version)
    };

    std::vector<book> data;
    data.emplace_back(book_genre::mathematics, "advanced calculus", "A.B.", "2012-06-01 12:30:00", 1001, 1);
    data.emplace_back(book_genre::mathematics, "complex analysis", "A.K.", "2013-07-01 13:40:10", 1002, 2);
    data.emplace_back(book_genre::programming, "design pattern", "C.J.", "2014-08-01 14:50:20", 1003, 3);
    data.emplace_back(book_genre::programming, "c++ in a month", "J.J.", "2015-09-01 15:00:30", 1004, 4);
    data.emplace_back(book_genre::programming, "c++ template", "P.K.", "2016-10-01 16:10:40", 1005, 5);
    data.emplace_back(book_genre::quant_finan, "derivatives", "T.O.", "2017-11-01 17:20:50", 1006, 6);

    db.create<book>();
    db.insert<book>(data);

/*
    std::vector<book> books;
    db.select(books);
    for(const auto& x:books) std::cout << "\n" << x;
*/
    std::cout << "\n\n";
}
