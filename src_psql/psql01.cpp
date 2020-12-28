#include <iostream>
#include <vector>
#include <libpq-fe.h>
using namespace std::string_literals;

void test_psql01() 
{
    // Step 1 : Make Connection
    PGconn *connection = PQconnectdb("user=dick password=12qwasZX dbname=book_db");
    if (PQstatus(connection) == CONNECTION_BAD) 
    {
        std::cout << "\nConnection to database failed : " << PQerrorMessage(connection);
        std::cout << "\n==============================================";
        std::cout << "\nDont forget to : ";
        std::cout << "\n1. start postgresql daemon";
        std::cout << "\n2. sudo -u username";
        std::cout << "\n==============================================";
        PQfinish(connection);
        return;
    }
    else
    {
        std::cout << "\nConnection to database succeed : version " << PQserverVersion(connection);
        std::cout << "\nusername : " << PQuser(connection);
        std::cout << "\ndatabase : " << PQdb(connection);
        std::cout << "\npassword : " << PQpass(connection);
    }

    // Step 2 : Drop table
    PGresult *result = PQexec(connection, "DROP TABLE IF EXISTS book_table");
    if (PQresultStatus(result) != PGRES_COMMAND_OK) 
    {
        std::cout << "\nDrop table failed";
        PQclear(result);
        PQfinish(connection);
        return;
    }
    else
    {
        std::cout << "\nDrop table succeed";
        PQclear(result);
    }

    // Step 3 : Create table
    std::string str;
    str  = "CREATE TABLE book_table("s;
    str += "book_id serial PRIMARY KEY, "s; 
    str += "book_name VARCHAR(50) NOT NULL, "s;
    str += "catergory VARCHAR(50) check (catergory in ('maths','programming','finance','machine learning')), "s;
    str += "buy_date  date)"s; 

    result = PQexec(connection, str.c_str());
    if (PQresultStatus(result) != PGRES_COMMAND_OK) 
    {
        std::cout << "\nCreate table failed";
        PQclear(result);
        PQfinish(connection);
        return;
    }
    else
    {
        std::cout << "\nCreate table succeed";
        PQclear(result);
    }

    // Step 4 : Insertion
    std::vector<std::string> title = 
    {
        "differential geometry", "complex analysis", "stochastic calculus", 
        "C++20", "Modern C++, object oriented prog", "python in 10 days",
        "quantitative finance", "risk neutral pricing", "derivative pricing",
        "reinforcement learning", "neural network", "support vector machine"
    }; 
    std::vector<std::string> catergory = 
    {
        "maths", "maths", "maths",
        "programming", "programming", "programming", 
        "finance", "finance", "finance",
        "machine learning", "machine learning", "machine learning"
    };

    for(std::uint32_t n=0; n!=12; ++n)
    {
        str  = "INSERT INTO book_table (book_name, catergory, buy_date) VALUES (";
        str += "'"s += title[n] += "', "s;
        str += "'"s += catergory[n] += "', "s;
        str += "'"s += std::to_string(2010 + n) += "-01-01"s += "') "s;

        result = PQexec(connection, str.c_str());
        if (PQresultStatus(result) != PGRES_COMMAND_OK)
        {
            std::cout << "\nInsertion to table failed : " << PQresultErrorMessage(result);
            PQclear(result);
        //  PQfinish(connection);
        //  return;
        }
        else
        {
            std::cout << "\nInsertion to table succeed";
            PQclear(result);
        }
    }

    // Step 5 : Selection 
    result = PQexec(connection, "SELECT * FROM book_table WHERE buy_date > '2011-06-30' ");    
    if (PQresultStatus(result) != PGRES_TUPLES_OK) 
    {
        std::cout << "\nSelection from table failed : " << PQresultErrorMessage(result);
        PQclear(result);
        PQfinish(connection);
        return;
    }
    else
    { 
        std::uint32_t NumRows = PQntuples(result);
        for(std::uint32_t n=0; n!=NumRows; ++n)
        {
            std::string str0 = PQgetvalue(result, n, 0);
            std::string str1 = PQgetvalue(result, n, 1);
            std::string str2 = PQgetvalue(result, n, 2);
            std::string str3 = PQgetvalue(result, n, 3);
            std::cout << "\n" << str0 << " " << str1 << " " << str2 << " " << str3;
        }    
        PQclear(result);
    }

    // Step 6 : Selection 1 row for META-DATA
    result = PQexec(connection, "SELECT * FROM book_table LIMIT 1"); // LIMIT means take first n rows only    
    if (PQresultStatus(result) != PGRES_TUPLES_OK) 
    {
        std::cout << "\nSelection from table failed : " << PQresultErrorMessage(result);
        PQclear(result);
        PQfinish(connection);
        return;
    }
    else
    { 
        std::uint32_t NumCols = PQnfields(result); 
        for(std::uint32_t n=0; n!=NumCols; ++n)
        {
            std::string str = PQfname(result, n);
            std::cout << "\nCol_" << n << " is " << str;
        }    
        PQclear(result);
    }

    // Step 7 : List tables in database
    result = PQexec(connection, "CREATE TABLE dummy_table0(id serial PRIMARY KEY, name VARCHAR(50) NOT NULL)");      PQclear(result);
    result = PQexec(connection, "CREATE TABLE dummy_table1(id serial PRIMARY KEY, name VARCHAR(50) NOT NULL)");      PQclear(result);
    result = PQexec(connection, "CREATE TABLE dummy_table2(id serial PRIMARY KEY, name VARCHAR(50) NOT NULL)");      PQclear(result);

    result = PQexec(connection, "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'");    
    if (PQresultStatus(result) != PGRES_TUPLES_OK)
    {
        std::cout << "\nSelect from schema failed : " << PQresultErrorMessage(result);
        PQclear(result);
        PQfinish(connection);
        return;
    }
    else
    { 
        std::uint32_t NumRows = PQntuples(result);
        for(std::uint32_t n=0; n!=NumRows; ++n)
        {     
            std::cout << "\nTable_ " << n << " " << PQgetvalue(result, n, 0);
        }
        PQclear(result);
    }

    // Transaction is a sequence of SQL-statements grouped to form an ATOMIC operation.
    // Transaction is declared by :
    
/*  result = PQexec(conn, "BEGIN");  
    transaction statement 0
    transaction statement 1
    ...
    transaction statement N
    result = PQexec(conn, "COMMIT"); */

    // Terminate
    PQfinish(connection);
}

