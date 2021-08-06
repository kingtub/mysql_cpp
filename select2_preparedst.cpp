#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#define EXAMPLE_HOST "localhost"
#define EXAMPLE_USER "king_li"
#define EXAMPLE_PASS "Aa123456"
#define EXAMPLE_DB "forcpp"

using namespace std;
int main(int argc, const char **argv)
{
	string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
	const string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
	const string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
	const string database(argc >= 5 ? argv[4] : EXAMPLE_DB);
	//cout << "Connector/C++ tutorial framework..." << endl;
	//cout << endl;
	try {
        /*sql::Connection cc;
        sql::Statement st;
        sql::ResultSet rs;*/


        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        shared_ptr<sql::Connection> conn(driver->connect(EXAMPLE_HOST, EXAMPLE_USER, EXAMPLE_PASS));
        conn->setSchema(EXAMPLE_DB);

        shared_ptr<sql::PreparedStatement> preparedSt(conn->prepareStatement(
        "select vend_id, prod_name, prod_price from products where vend_id = ? and prod_price < ?;"));
        preparedSt->setInt(1, 1003);
        preparedSt->setDouble(2, 10);
        shared_ptr<sql::ResultSet> res(preparedSt->executeQuery());
        cout<<"Row count: "<< res->rowsCount()<<endl;
        while(res->next()) {
            cout<<res->getInt(1)<<", "<<res->getString(2)<<", "<<res->getDouble(3)<<endl;
        }

        shared_ptr<sql::PreparedStatement> pst2(conn->prepareStatement(
        "select vend_id, prod_name, prod_price from products where vend_id = ? or vend_id = ?;"));
        pst2->setInt(1, 1003);
        pst2->setInt(2, 1002);
        shared_ptr<sql::ResultSet> res2(pst2->executeQuery());
        cout<<"Row count2: "<< res2->rowsCount()<<endl;
        while(res2->next()) {
            cout<<res2->getInt(1)<<", "<<res2->getString("prod_name")<<", "<<res2->getDouble(3)<<endl;
        }
	} catch (sql::SQLException &e) {
		/*
		MySQL Connector/C++ throws three different exceptions:
		- sql::MethodNotImplementedException (derived from sql::SQLException)
		- sql::InvalidArgumentException (derived from sql::SQLException)
		- sql::SQLException (derived from std::runtime_error)
		*/
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		/* what() (derived from std::runtime_error) fetches error message */
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		return EXIT_FAILURE;
	}
	//cout << "Done." << endl;
	return EXIT_SUCCESS;
}
