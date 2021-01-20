#include "psql_API.h"

// *************** //
// *** Example *** //
// *************** //
enum class book_genre : std::uint32_t
{
    mathematics,
    programming,
    quant_finan,
    unclassified
};

inline std::ostream& operator<<(std::ostream& os, const book_genre& genre)
{
    switch(genre)
    {
        case book_genre::mathematics  : os << "MATH"; break;
        case book_genre::programming  : os << "PROG"; break;
        case book_genre::quant_finan  : os << "QFIN"; break;
        case book_genre::unclassified : os << "UNCL"; break;
    }
    return os;
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
    os << b.genre     << " | "
       << b.name      << " | " 
       << b.author    << " | " 
       << b.date_time << " | " 
       << b.book_id   << " | " 
       << b.version   << " | "; 
    return os;
}

// *********************************************** //
// *** Add these functions for customised enum *** //
// *********************************************** //
template<> std::string psql::psql_type<book_genre>() 
{
    return "VARCHAR(4) CHECK (genre IN ('MATH','PROG','QFIN'))";
}

template<> std::string psql::psql_value(const book_genre& x)  
{
    std::stringstream ss;
    ss << x;
    return ss.str();
}

template<> void psql::psql_fill_value(const std::string& s, book_genre& x) 
{
    if      (s == "MATH") x = book_genre::mathematics;
    else if (s == "PROG") x = book_genre::programming;
    else if (s == "QFIN") x = book_genre::quant_finan;
    else                  x = book_genre::unclassified;
}

// *************** //
// *** Testing *** //
// *************** //
void test_psql_API()
{
    system("sudo service postgresql start");
    PGconn* connection = PQconnectdb("user=dick password=12qwasZX dbname=book_db");
    if (!psql::check_status(connection)) return;

    psql::psql db
    {
        connection, "my_quant_library",
        psql::psql_item("genre",     &book::genre), // can further optimize by using macro 
        psql::psql_item("name",      &book::name),
        psql::psql_item("author",    &book::author),
        psql::psql_item("date_time", &book::date_time),
        psql::psql_item("book_id",   &book::book_id),
        psql::psql_item("version",   &book::version)
    };

    std::vector<book> data;
    data.emplace_back(book_genre::mathematics, "advanced calculus", "A.B.", "2012-06-01 12:30:00", 1001, 1);
    data.emplace_back(book_genre::mathematics, "complex analysis", "A.K.", "2013-07-01 13:40:10", 1002, 2);
    data.emplace_back(book_genre::programming, "design pattern", "C.J.", "2014-08-01 14:50:20", 1003, 3);
    data.emplace_back(book_genre::programming, "c++ in a month", "J.J.", "2015-09-01 15:00:30", 1004, 4);
    data.emplace_back(book_genre::programming, "c++ template", "P.K.", "2016-10-01 16:10:40", 1005, 5);
    data.emplace_back(book_genre::quant_finan, "derivatives", "T.O.", "2017-11-01 17:20:50", 1006, 6);

    if (!db.create<book>())      { PQfinish(connection);  std::cout << "\n\n";  return; }
    if (!db.insert<book>(data))  { PQfinish(connection);  std::cout << "\n\n";  return; }

    std::vector<book> books;
    if (!db.select(books))  { PQfinish(connection);  std::cout << "\n\n";  return; }
    for(const auto& x:books) std::cout << "\n" << x;

    PQfinish(connection);
    std::cout << "\n\n";
}
