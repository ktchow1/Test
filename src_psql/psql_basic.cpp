#include <iostream>
#include <vector>
#include <stdlib.h> // for system() call
#include <libpq-fe.h>
using namespace std::string_literals;

bool check_status(PGconn* connection)
{
    if (PQstatus(connection) == CONNECTION_BAD) 
    {
        std::cout << "\nConnection to database failed : " << PQerrorMessage(connection);
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
        std::cout << "\nConnection to database succeed : version " << PQserverVersion(connection);
        std::cout << "\nusername : " << PQuser(connection);
        std::cout << "\ndatabase : " << PQdb(connection);
        std::cout << "\npassword : " << PQpass(connection);
        return true;
    }
}

bool check_status(const std::string& label, PGconn* connection, PGresult* result, auto expected_value)
{
    if (PQresultStatus(result) != expected_value) 
    {
        std::cout << "\n" << label << " failed : " << PQresultErrorMessage(result);
        PQclear(result);
        PQfinish(connection);
        return false;
    }
    else
    {
        std::cout << "\n" << label << " succeed";
        return true;
    }
}

void test_psql0() 
{
    system("sudo service postgresql start");


    // ******************************** //
    // *** Step 1 : Make Connection *** //
    // ******************************** //
    PGconn *connection = PQconnectdb("user=dick password=12qwasZX dbname=book_db");
    if (!check_status(connection)) return;

    // **************************************** //
    // *** No permission to create database *** //
    // **************************************** //
//  PGresult* result = PQexec(connection, "CREATE DATABASE new_db");
//  if (!check_status("Create database", connection, result, PGRES_COMMAND_OK)) return;
//  PQclear(result);

    // *************************** //
    // *** Step 2 : Drop table *** //
    // *************************** //
    PGresult *result = PQexec(connection, "DROP TABLE IF EXISTS book_table");
    if (!check_status("Drop table", connection, result, PGRES_COMMAND_OK)) return;
    PQclear(result);


    // ***************************** //
    // *** Step 3 : Create table *** //
    // ***************************** //
    std::string str;
    str  = "CREATE TABLE book_table("s;
    str += "book_id   SERIAL PRIMARY KEY, "s; 
    str += "book_name TEXT NOT NULL, "s;
    str += "catergory VARCHAR(50) check (catergory in ('maths','programming','finance','machine learning')), "s;
    str += "buy_date  TIMESTAMP)"s; 

    result = PQexec(connection, str.c_str());
    if (!check_status("Create table", connection, result, PGRES_COMMAND_OK)) return;
    PQclear(result);


    // ************************** //
    // *** Step 4 : Insertion *** //
    // ************************** //
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
        str += "'"s += std::to_string(2010 + n) += "-01-01"s            // yyyy-mm-dd space
                    += " 12:00:"s += std::to_string(30+n)               // hh:mm:ss
                    += ".0000"s   += std::to_string(20+n) += "') "s;    // .microsec

        result = PQexec(connection, str.c_str());
        if (!check_status("Insert table", connection, result, PGRES_COMMAND_OK)) return;
        PQclear(result);
    }
    std::cout << "\n";


    // ************************** //
    // *** Step 5 : Selection *** //
    // ************************** //
    result = PQexec(connection, "SELECT * FROM book_table WHERE buy_date > '2011-06-30' ");    
    if (!check_status("Select table", connection, result, PGRES_TUPLES_OK)) return;

    std::uint32_t NumRows = PQntuples(result);
    std::uint32_t NumCols = PQnfields(result);
    for(std::uint32_t n=0; n!=NumRows; ++n)
    {
        std::cout << "\n--- ";
        for(std::uint32_t m=0; m!=NumCols; ++m)
        {
            std::cout <<  PQgetvalue(result, n, m) << " | ";
        }
    }    
    PQclear(result);
    std::cout << "\n";


    // *********************************************** //
    // ***  Step 6 : Selection 1 row for META-DATA *** //
    // *********************************************** //
    result = PQexec(connection, "SELECT * FROM book_table LIMIT 1"); // LIMIT means take first n rows only    
    if (!check_status("Select table for META data", connection, result, PGRES_TUPLES_OK)) return;

    NumCols = PQnfields(result); 
    for(std::uint32_t n=0; n!=NumCols; ++n)
    {
        std::string str = PQfname(result, n);
        std::cout << "\n--- Col_" << n << " is " << str;
    }    
    PQclear(result);
    std::cout << "\n";


    // **************************************** //
    // *** Step 7 : List tables in database *** //
    // **************************************** //
    result = PQexec(connection, "CREATE TABLE dummy_table0(id serial PRIMARY KEY, name VARCHAR(50) NOT NULL)");      PQclear(result);
    result = PQexec(connection, "CREATE TABLE dummy_table1(id serial PRIMARY KEY, name VARCHAR(50) NOT NULL)");      PQclear(result);
    result = PQexec(connection, "CREATE TABLE dummy_table2(id serial PRIMARY KEY, name VARCHAR(50) NOT NULL)");      PQclear(result);
    result = PQexec(connection, "SELECT table_name FROM information_schema.tables WHERE table_schema = 'public'");    
    if (!check_status("Select from schema", connection, result, PGRES_TUPLES_OK)) return;

    NumRows = PQntuples(result);
    for(std::uint32_t n=0; n!=NumRows; ++n)
    {     
        std::cout << "\n--- Table_ " << n << " " << PQgetvalue(result, n, 0);
    }
    PQclear(result);


//  Transaction is a sequence of SQL-statements grouped to form an ATOMIC operation.
//  Transaction is declared by :
    
/*  result = PQexec(conn, "BEGIN");  
    transaction statement 0
    transaction statement 1
    ...
    transaction statement N
    result = PQexec(conn, "COMMIT"); */

    // Terminate
    PQfinish(connection);
}

