//
// Created by System Administrator on 9/29/16.
//

#ifndef CIT_MONOMAKE_DATABASEMANAGER_H
#define CIT_MONOMAKE_DATABASEMANAGER_H

#include <string>
#include <cppconn/driver.h>
#include <mysql_driver.h>
#include <cppconn/resultset.h>

using namespace std;
using namespace sql;

class DatabaseManager {
private:
    string hostname,database,user,password,port;
    mysql::MySQL_Driver* driver;
    Connection* conn;
    Statement *stmt;
    ResultSet *res;
public:
    void closeDatabase();
    void connectDatabase();
    void manageException(SQLException& e);
    DatabaseManager();
    //const std::string hostname, const std::string dbName,const std::string user, const std::string passwd
    virtual ~DatabaseManager();

    PreparedStatement* getPreparedStatement(const SQLString &sql);
};


#endif //CIT_MONOMAKE_DATABASEMANAGER_H
