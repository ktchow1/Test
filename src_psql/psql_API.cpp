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


// ********************** //
// *** Main interface *** //
// ********************** //
// Two techniques :
// 1. iterate parameter pack  in constructor
// 2. iterate std::tuple<...> in other functions

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
        // *** Part 1 *** // 
        std::stringstream ss;
        std::uint32_t n=0;
        std::apply
        (
            [this, &ss, &n](const typename ITEMS::ptr_type&... mptrs)
            {
                // comma operator takes this form (XXX,...)
                // if XXX is complex, please add one more bracket around (XXX)
                (( 
                    ss << keys[n++] << " " 
                    // << psql_type<decltype(std::declval<POD>().*mptrs)>() // Does not work, return INVALID
                       << psql_type<std::remove_cvref_t<decltype(std::declval<POD>().*mptrs)>>()
                       << (n!=keys.size()? ", " : "")
                ) ,...);
            }
            , mptrs
        );

        // *** Part 2 *** // 
        using namespace std::string_literals; // for string literal
        std::string create_str("CREATE TABLE");
        create_str += " "s += table_name;
        create_str += "("s += ss.str();
        create_str += ")"s;
        
        std::cout << "\n[PSQL] " << create_str;
        // psql.execute ...
        return true;
    }


/*
public: // Insertion
    template<typename POD>
    bool insert(const POD& entry);

    template<typename POD>
    bool insert(const std::vector<POD>& entries);

public: // Selection
    template<typename POD>
    bool select(std::vector<POD>& output); */

private:
    std::string table_name;
    std::vector<std::string> keys;
    tuple_type mptrs;
};


// *********************** //
// *** Testing example *** //
// *********************** //
enum class book_genre : std::uint32_t
{
    mathematics,
    programming,
    quant_finan
};

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
    switch(b.genre)
    {
        case book_genre::mathematics : os << "math "; break;
        case book_genre::programming : os << "prog "; break;
        case book_genre::quant_finan : os << "qfin "; break;
    }

    os << " " << b.name
       << " " << b.author
       << " " << b.date_time
       << " " << b.book_id
       << "_" << b.version;
    return os;
}

void test_psql_API()
{
    psql db
    {
        "my_quant_library",
        psql_item("genre",     &book::genre),
        psql_item("name",      &book::name),
        psql_item("author",    &book::author),
        psql_item("date_time", &book::date_time),
        psql_item("book_id",   &book::book_id),
        psql_item("version",   &book::version)
    };

    db.create<book>();

/*
    db.insert(genre::mathematics, "advanced calculus", "A.B.", "2012-06-01 12:30:00", 1001, 1);
    db.insert(genre::mathematics, "complex analysis", "A.K.", "2013-07-01 13:40:10", 1002, 2);
    db.insert(genre::programming, "design pattern", "C.J.", "2014-08-01 14:50:20", 1003, 3);
    db.insert(genre::programming, "c++ in a month", "J.J.", "2015-09-01 15:00:30", 1004, 4);
    db.insert(genre::programming, "c++ template", "P.K.", "2016-10-01 16:10:40", 1005, 5);
    db.insert(genre::quant_finan, "derivatives", "T.O.", "2017-11-01 17:20:50", 1006, 6);

    std::vector<book> books;
    db.select(books);
    for(const auto& x:books) std::cout << "\n" << x;
*/
    std::cout << "\n\n";
}
