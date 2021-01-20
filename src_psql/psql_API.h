#ifndef __PSQL_API_H__
#define __PSQL_API_H__
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<tuple>

// For libpq.a
#include <stdlib.h> // for system() call
#include <libpq-fe.h>

namespace psql {
// ********************************* //
// *** libpq.a related functions *** //
// ********************************* //
inline bool check_status(PGconn* connection)
{
    if (PQstatus(connection) == CONNECTION_BAD) 
    {
        std::cout << "\n[PSQL] Connection to database failed : " << PQerrorMessage(connection);
        std::cout << "\n==============================================";
        std::cout << "\nDont forget to : ";
        std::cout << "\n1. sudo service postgresql start";
        std::cout << "\n2. sudo -u dick ./Test     (pwd=12qwasZX)"; 
        std::cout << "\n==============================================";
        PQfinish(connection);
        return false;
    }
    else
    {
        std::cout << "\n[PSQL] Connection to database succeed : version " << PQserverVersion(connection);
        std::cout << "\n==============================================";
        std::cout << "\nusername : " << PQuser(connection);
        std::cout << "\ndatabase : " << PQdb(connection);
        std::cout << "\npassword : " << PQpass(connection);
        std::cout << "\n==============================================";
        return true;
    }
}

inline bool check_status(const std::string& label, PGconn* connection, PGresult* result, auto expected_value)
{
    if (PQresultStatus(result) != expected_value) 
    {
        std::cout << "\n[PSQL] " << label << " failed : " << PQresultErrorMessage(result);
        PQclear(result);
        PQfinish(connection);
        return false;
    }
    else
    {
        std::cout << "\n[PSQL] " << label << " succeed";
        return true;
    }
}


// ********************** //
// *** Main interface *** //
// ********************** //
// Terminologies :
// key   = sql::col_name = pod::mem_name
// type  = sql::col_type = pod::mem_type
// value = sql::row_data = pod::mem_value
//
// Two similar techniques :
// 1. iterate parameter pack  in psql::psql() (just a normal variadic template)
// 2. iterate std::tuple<...> in psql::fct()  (must need std::apply)
// Thus std::apply converts std::tuple into variadic parameter pack.
// **************************************************************************** //
template<typename MPTR> struct psql_item // user can construct item as POD using CTAD
{
    using ptr_type = MPTR;

    std::string key;
    MPTR mptr;
};

// Keyword "inline" is needed for template specialization in header file. Otherwise multiple-definition error.
template<typename T> std::string psql_type()                { return "INVALID"; }
template<> inline    std::string psql_type<std::uint16_t>() { return "INTEGER"; }
template<> inline    std::string psql_type<std::uint32_t>() { return "INTEGER"; }
template<> inline    std::string psql_type<std::uint64_t>() { return "INTEGER"; }
template<> inline    std::string psql_type<std::string>()   { return "TEXT";    }

template<typename T> std::string psql_value(const T& x)              { return "INVALID";         }
template<> inline    std::string psql_value(const std::uint16_t& x)  { return std::to_string(x); }
template<> inline    std::string psql_value(const std::uint32_t& x)  { return std::to_string(x); }
template<> inline    std::string psql_value(const std::uint64_t& x)  { return std::to_string(x); }
template<> inline    std::string psql_value(const std::string&   x)  { return x;                 }

template<typename T> void psql_fill_value(const std::string& s, T& x) {}
template<> inline    void psql_fill_value(const std::string& s, std::uint16_t& x) { x = static_cast<std::uint16_t>(std::stoul (s)); }
template<> inline    void psql_fill_value(const std::string& s, std::uint32_t& x) { x = static_cast<std::uint32_t>(std::stoul (s)); }
template<> inline    void psql_fill_value(const std::string& s, std::uint64_t& x) { x = static_cast<std::uint64_t>(std::stoull(s)); }
template<> inline    void psql_fill_value(const std::string& s, std::string&   x) { x = s; }

template<typename... ITEMS> class psql
{
public:
    using tuple_type = std::tuple<typename ITEMS::ptr_type ...>;

    psql() = delete;
   ~psql() = default;
    psql(const psql&) = delete;
    psql(psql&&) = delete;
    psql& operator=(const psql&) = delete;
    psql& operator=(psql&&) = delete;

public:
    psql(PGconn* connection_, const std::string& table_name_, const ITEMS&... items); 

public:
    template<typename POD>
    bool create() const; 

    template<typename POD>
    bool insert(const std::vector<POD>& entries) const; 

    template<typename POD>
    bool select(std::vector<POD>& output) const;

private:
    mutable PGconn* connection;
    mutable PGresult* result;
    std::string table_name;

private:
    std::vector<std::string> keys; // This is a vector, as all elements are std::string.
    tuple_type mptrs;              // This is a tuple, as all elements are different types.
};
}

// **************************** //
// 1st psql = namespace
// 2nd psql = class template
// 3rd psql = class constructor
// **************************** //
template<typename... ITEMS> 
psql::psql<ITEMS...>::psql(PGconn* connection_, const std::string& table_name_, const ITEMS&... items) 
    : connection(connection_), table_name(table_name_)
{
    // No std::apply is needed, just variadic unpack
    (keys.push_back(items.key),...);
    mptrs = std::make_tuple(items.mptr ...);
}

template<typename... ITEMS> 
template<typename POD>
bool psql::psql<ITEMS...>::create() const
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
    std::string drop_str("DROP TABLE IF EXISTS");
    drop_str +=  " "s += table_name;

    std::string create_str("CREATE TABLE");
    create_str += " "s += table_name;
    create_str += " "s += ss.str();
    
    // ************** //
    // *** Part 3 *** // 
    // ************** //
//  std::cout << "\n[PSQL] " << drop_str;   // remember to drop before create
//  std::cout << "\n[PSQL] " << create_str; // re-create an existing table fails

    result = PQexec(connection, drop_str.c_str());
    if (!check_status("Drop table", connection, result, PGRES_COMMAND_OK)) return false;
    PQclear(result);

    result = PQexec(connection, create_str.c_str());
    if (!check_status("Create table", connection, result, PGRES_COMMAND_OK)) return false;
    PQclear(result);
    return true;
}

template<typename... ITEMS> 
template<typename POD>
bool psql::psql<ITEMS...>::insert(const std::vector<POD>& entries) const
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
                    ss1 << "'" 
                        << psql_value(entries[n].*unpacked_mptrs) 
                        << "'"
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

    // ************** //
    // *** Part 3 *** // 
    // ************** //
//  std::cout << "\n[PSQL] " << insert_str;

    result = PQexec(connection, insert_str.c_str());
    if (!check_status("Insert table", connection, result, PGRES_COMMAND_OK)) return false;
    PQclear(result);
    return true;
}

template<typename... ITEMS> 
template<typename POD>
bool psql::psql<ITEMS...>::select(std::vector<POD>& entries) const
{
    // *************** //
    // *** Part 2 *** // 
    // *************** //
    using namespace std::string_literals; // for string literal
    std::string select_str("SELECT * FROM");
    select_str += " "s += table_name;

    // ************** //
    // *** Part 3 *** // 
    // ************** //
//  std::cout << "\n[PSQL] " << select_str;

    result = PQexec(connection, select_str.c_str());
    if (!check_status("Select table", connection, result, PGRES_TUPLES_OK)) return false;

    std::uint32_t NumRows = PQntuples(result);
//  std::uint32_t NumCols = PQnfields(result);
    for(std::uint32_t n=0; n!=NumRows; ++n)
    {
        std::uint32_t m=0;
        std::apply
        (
            [this, &entries, &n, &m](const typename ITEMS::ptr_type&... unpacked_mptrs)
            {
                POD pod;
                ((
                    psql_fill_value(PQgetvalue(result, n, m++), pod.*unpacked_mptrs)

                ),...); 
                entries.emplace_back(std::move(pod));
            }
            , mptrs 
        );
    }
    PQclear(result);
    return true;
}

#endif
