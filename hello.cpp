#include <iostream>
#include <memory>

#include"sql_manager.hpp"
#include<cppconn/statement.h>

using namespace std;

void runSimpleQuery(shared_ptr<sql::Connection> conn);

int main(int argc, char **argv)
{
    SQLManager manager;
    manager.init();

    shared_ptr<sql::Connection> conn = manager.getConn();
    /* Connect to the MySQL 'DB_NAME' database */
    conn->setSchema(DB_NAME);
    runSimpleQuery(conn);

    return 0;
}

void runSimpleQuery(shared_ptr<sql::Connection> conn)
{
    shared_ptr<sql::Statement> stmt(conn->createStatement());
    // Use the method sql::Statement::execute() if your query does not return a result set
    // or if your query returns more than one result set.
    //stmt->execute(string("use ") + DB_NAME);

    //By default, Connector/C++ buffers all result sets on the client to support cursors.
    shared_ptr<sql::ResultSet> res(stmt->executeQuery("show tables;"));
    while(res->next()) {
        /*// You can use either numeric offsets...
        cout << "id = " << res->getInt(1); // getInt(1) returns the first column
        // ... or column names for accessing results.
        // The latter is recommended.
        cout << ", label = '" << res->getString("label") << "'" << endl;*/
        cout<<"table = "<<res->getString(1)<<endl;
    }

    shared_ptr<sql::ResultSet> products(stmt->executeQuery("select prod_id, prod_name, prod_price, vend_id from products order by prod_name;"));
    cout<<"rowsCount: "<<products->rowsCount()<<endl;
    while(products->next()) {
        cout<<"prod_id: "<<products->getString(1)<<", prod_name: "<<products->getString("prod_name")<<
        ", prod_price: "<<products->getDouble(3)<<", vend_id: "<<products->getInt("vend_id")<<endl;
    }
}
