#ifndef SQL_MANAGER_HPP
#define SQL_MANAGER_HPP

#include<mutex>
#include<iostream>
#include<memory>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/metadata.h>

#define MYSQL_HOST "tcp://127.0.0.1:3306"
#define USER "king_li"
#define PASSWORD "Aa123456"
#define DB_NAME "forcpp"

using namespace std;

class SQLManager {
public:
   SQLManager();
   ~SQLManager();
   void init();
   bool sqlConnected();
   shared_ptr<sql::Connection> getConn();
private:
   mutex mtx;
   shared_ptr<sql::Connection> conn;
   bool inited;
};

SQLManager::SQLManager() {
    inited = false;
}

SQLManager::~SQLManager() {
    if(inited) {
        // if you do not need Connection object any more, free it;
        //delete conn;
        //conn = nullptr;
        // But do not explicitly free driver, the connector object. Connector/C++ takes care of freeing that.
    }
}

void SQLManager::init() {
    if(!inited) {
    /*  get_mysql_driver_instance() calls get_driver_instance(), which
        is not thread-safe. Either avoid invoking these methods from within multiple
        threads at once, or surround the calls with a mutex to prevent simultaneous
        execution in multiple threads. */
        lock_guard<mutex> lg(mtx);
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        conn = shared_ptr<sql::Connection>(driver->connect(MYSQL_HOST, USER, PASSWORD));
        inited = true;

        // print something
        bool printSomething = true;
        if(printSomething) {
            cout << "# " << driver->getName() << ", version ";
            cout << driver->getMajorVersion() << "." << driver->getMinorVersion();
            cout << "." << driver->getPatchVersion() << endl;


            sql::DatabaseMetaData * con_meta = conn->getMetaData();
            cout << "# CDBC (API) major version = " << con_meta->getCDBCMajorVersion() << endl;
            if (con_meta->getCDBCMajorVersion() <= 0) {
              throw runtime_error("API major version must not be 0");
            }
            cout << "# CDBC (API) minor version = " << con_meta->getCDBCMinorVersion() << endl;
        }

    }
}

bool SQLManager::sqlConnected() {
    bool b = false;
    if(inited && conn) {
        b = conn->isValid();
    }
    return b;
}

shared_ptr<sql::Connection> SQLManager::getConn() {
    return conn;
}

#endif // SQL_MANAGER_HPP
