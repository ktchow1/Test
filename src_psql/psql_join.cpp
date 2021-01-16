#include <iostream>
#include <vector>
#include <stdlib.h> // for system() call
#include <libpq-fe.h>
using namespace std::string_literals;

// Helper functions defined in psql_basic.cpp
bool check_status(PGconn* connection);
bool check_status(const std::string& label, PGconn* connection, PGresult* result, auto expected_value);

void test_psql1() 
{
    system("sudo service postgresql start");


    // ******************************** //
    // *** Step 1 : Make Connection *** //
    // ******************************** //
    PGconn *connection = PQconnectdb("user=dick password=12qwasZX dbname=book_db");
    if (!check_status(connection)) return;


    // *************************** //
    // *** Step 2 : Drop table *** //
    // *************************** //
    PGresult *result0 = PQexec(connection, "DROP TABLE IF EXISTS item");
    PGresult *result1 = PQexec(connection, "DROP TABLE IF EXISTS product");
    if (!check_status("Drop item",    connection, result0, PGRES_COMMAND_OK)) return;
    if (!check_status("Drop product", connection, result1, PGRES_COMMAND_OK)) return;
    PQclear(result0);
    PQclear(result1);


    // ***************************** //
    // *** Step 3 : Create table *** //
    // ***************************** //
    std::string str0, str1;
    str0 = "CREATE TABLE item   (item_id    serial PRIMARY KEY, order_id VARCHAR(50), product_type VARCHAR(50))"s;
    str1 = "CREATE TABLE product(product_id serial PRIMARY KEY, product_type VARCHAR(50), price DECIMAL)"s;

    result0 = PQexec(connection, str0.c_str());
    result1 = PQexec(connection, str1.c_str());
    if (!check_status("Create item",    connection, result0, PGRES_COMMAND_OK)) return;
    if (!check_status("Create product", connection, result1, PGRES_COMMAND_OK)) return;
    PQclear(result0);
    PQclear(result1);


    // ************************** //
    // *** Step 4 : Insertion *** //
    // ************************** //
    str0  = "INSERT INTO item (order_id, product_type) VALUES";
    str0 += "('ABC_0001', 'PROD_A' ),"s;
    str0 += "('ABC_0001', 'PROD_B' ),"s;
    str0 += "('ABC_0001', 'PROD_B' ),"s;
    str0 += "('ABC_0002', 'PROD_A' ),"s;
    str0 += "('ABC_0002', 'PROD_C' ),"s;
    str0 += "('ABC_0003', 'PROD_B' ),"s;
    str0 += "('ABC_0003', 'UNKNOWN'),"s;
    str0 += "('ABC_0004', 'PROD_C' );"s;

    str1  = "INSERT INTO product (product_type, price) VALUES";
    str1 += "('PROD_A',  12.50),"s;
    str1 += "('PROD_B',  19.99),"s;
    str1 += "('PROD_C',  25.00),"s;
    str1 += "('PROD_D',  85.00),"s;
    str1 += "('PROD_E', 199.99);"s;

    result0 = PQexec(connection, str0.c_str());
    result1 = PQexec(connection, str1.c_str());
    if (!check_status("Insert item",    connection, result0, PGRES_COMMAND_OK)) return;
    if (!check_status("Insert product", connection, result1, PGRES_COMMAND_OK)) return;
    PQclear(result0);
    PQclear(result1);
    std::cout << "\n";


    // ************************** //
    // *** Step 5 : Selection *** //
    // ************************** //
    result0 = PQexec(connection, "SELECT * FROM item");    
    result1 = PQexec(connection, "SELECT * FROM product");    
    if (!check_status("Select item",    connection, result0, PGRES_TUPLES_OK)) return;
    if (!check_status("Select product", connection, result1, PGRES_TUPLES_OK)) return;

    std::uint32_t NumRows0 = PQntuples(result0);
    std::uint32_t NumRows1 = PQntuples(result1);
    std::cout << "\nData in item";
    for(std::uint32_t n=0; n!=NumRows0; ++n)
    {
        std::cout << "\n--- " << PQgetvalue(result0, n, 0)
                       << " " << PQgetvalue(result0, n, 1)
                       << " " << PQgetvalue(result0, n, 2);
    }    
    std::cout << "\nData in product";
    for(std::uint32_t n=0; n!=NumRows1; ++n)
    {
        std::cout << "\n--- " << PQgetvalue(result1, n, 0)
                       << " " << PQgetvalue(result1, n, 1)
                       << " " << PQgetvalue(result1, n, 2);
    }    
    PQclear(result0);
    PQclear(result1);
    std::cout << "\n";


    // *************************** //
    // *** Step 6 : Join table *** //
    // *************************** //
    for(std::uint32_t n=0; n!=5; ++n)
    {
        std::string s;
        if (n==0) s = "SELECT * FROM item CROSS JOIN product";    
        if (n==1) s = "SELECT * FROM item INNER JOIN product ON item.product_type = product.product_type ORDER BY item.item_id"; 
        if (n==2) s = "SELECT * FROM item LEFT  OUTER JOIN product ON item.product_type = product.product_type ORDER BY item.item_id"; 
        if (n==3) s = "SELECT * FROM item RIGHT OUTER JOIN product ON item.product_type = product.product_type ORDER BY item.item_id"; 
        if (n==4) s = "SELECT * FROM item FULL  OUTER JOIN product ON item.product_type = product.product_type ORDER BY item.item_id"; 

        PGresult* result = PQexec(connection, s.c_str());
        if (!check_status("JOIN", connection, result, PGRES_TUPLES_OK)) return;

        std::uint32_t NumRows = PQntuples(result);
        std::cout << "\nData in JOIN";
        for(std::uint32_t n=0; n!=NumRows; ++n)
        {
            std::cout << "\n--- " << PQgetvalue(result, n, 0)
                           << " " << PQgetvalue(result, n, 1)
                           << " " << PQgetvalue(result, n, 2)
                           << " " << PQgetvalue(result, n, 3)
                           << " " << PQgetvalue(result, n, 4)
                           << " " << PQgetvalue(result, n, 5);
        }    
        PQclear(result);
        std::cout << "\n";
    }
    PQfinish(connection);
    std::cout << "\n"; 
}

