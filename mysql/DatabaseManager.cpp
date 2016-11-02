//
// Created by System Administrator on 9/29/16.
//

#include "DatabaseManager.h"
#include <fstream>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql::mysql;

DatabaseManager::DatabaseManager() {
    //const std::string hostname, const std::string dbName, const std::string user,const std::string passwd
    ifstream *ifs = new ifstream(".secret");
    string str;
    if (ifs->fail()) {
        cerr << ".secretファイルの読み込み失敗" << endl;
        return;
    }
    while (getline(*ifs, str)) {
        string value = str.substr(str.find(':') + 1);
        string key = str.substr(0, str.find(':'));
        //switch文に文字列は使えない?
        if (key == "hostname") {
            hostname = value;
        } else if (key == "database") {
            database = value;
        } else if (key == "port") {
            port = value;
        } else if (key == "user") {
            user = value;
        } else if (key == "password") {
            password = value;
        }
    }
    delete ifs;

    connectDatabase();
    PreparedStatement *statment = getPreparedStatement("SELECT * FROM data WHERE id = ?");
    statment->setInt(1, 1);
    res = statment->executeQuery();
    while (res->next()) {
        cout << res->getString("date_time") << endl;
    }

    //closeDatabase();
}

DatabaseManager::~DatabaseManager() {

}

/**
 * コネクションを接続します.
 */
void DatabaseManager::connectDatabase() {
    cout << "コネクションの確立中..." << endl;
    cout << "接続先: " << hostname << endl;

    try {
        driver = get_mysql_driver_instance();
        conn = driver->connect("tcp://" + hostname + ":" + port, user, password);
        conn->setSchema(database);
        stmt = conn->createStatement();
    } catch (sql::SQLException &e) {
        this->manageException(e);
        throw;
    }


}

/**
 * エラー文を正規化します
 */
void DatabaseManager::manageException(sql::SQLException &e) {
    if (e.getErrorCode() != 0) {
        string msg = "SQL Exception : " + static_cast<string>(e.what());
        msg += " (MySQL error code : " + to_string(e.getErrorCode());
        msg += ", SQLState : " + e.getSQLState() + " )\n";
        std::cerr << msg << std::endl;
    }

}

/**
 * コネクションを切断します
 */
void DatabaseManager::closeDatabase() {
    if (!conn) {
        std::string msg = std::string(__func__) + " 失敗しました. ";
        msg += "システムはデータベースへ接続していません";
        throw std::runtime_error(msg);
    }

    conn->close();

    delete res;
    delete stmt;
    delete conn;
    delete driver;
    cout << "コネクションの切断" << endl;
}

/**
 * テーブルに情報を格納します
 */
PreparedStatement *DatabaseManager::getPreparedStatement(const sql::SQLString &sql) {
    try {
        return conn->prepareStatement(sql);
    } catch (sql::SQLException &e) {
        this->manageException(e);
        throw;
    }
}

void DatabaseManager::insertPacket(in_addr_t ip, char *date,int sport,int dport) {
    //ipが空白の場合は破棄する

    cout << date << "    " << ip << endl;

//    struct in_addr addr;
//    inet_aton("255.255.255.255", &addr);
//    cout << addr.s_addr << endl;

    PreparedStatement *statment = getPreparedStatement("INSERT INTO data (date_time, ip_address, tcp_sport, tcp_dport, is_check) VALUES (?, ?, ?, ?, ?)");
    statment->setDateTime(1, date);
    statment->setInt64(2, ip);
    statment->setInt(3, sport);
    statment->setInt(4, dport);
    statment->setBoolean(5, false);

    statment->executeQuery();
}
