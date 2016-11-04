//
// Created by System Administrator on 9/29/16.
//

#ifndef CIT_MONOMAKE_DATABASEMANAGER_H
#define CIT_MONOMAKE_DATABASEMANAGER_H

#include <string>
#include <cppconn/driver.h>
#include <mysql_driver.h>
#include <cppconn/resultset.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jmorecfg.h>

using namespace std;
using namespace sql;

class DatabaseManager {
private:
    string filesystem,hostname,database,tname,user,password,port;
    const char *sqlite_filename;
    boolean isMysql;
    mysql::MySQL_Driver* driver;
    Connection* conn;
    Statement* stmt;
public:
    void closeDatabase();
    boolean connectDatabase();
    void manageException(SQLException& e);
    DatabaseManager();
    //const std::string hostname, const std::string dbName,const std::string user, const std::string passwd
    virtual ~DatabaseManager();

    PreparedStatement* getPreparedStatement(const SQLString &sql);
    void insertPacket(in_addr_t ip, char string[20],int sport,int dport);

    boolean exsistsTable(string tname);

    boolean createTable();
};


#endif //CIT_MONOMAKE_DATABASEMANAGER_H