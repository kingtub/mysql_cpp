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

        shared_ptr<sql::PreparedStatement> pst(conn->prepareStatement(
        "insert into customers(cust_name, cust_address,cust_city, cust_state) values(?, ?, ?, ?);"));
        pst->setString(1, "Pep3 Law");
        pst->setString(2, "100 Street");
        pst->setString(3, "New york");
        pst->setString(4, "NY");
        pst->execute();

        pst->setString(1, "Pep4 Law");
        pst->setString(2, "200 Street");
        pst->setString(3, "Washing ton");
        pst->setString(4, "LA");
        pst->execute();

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
