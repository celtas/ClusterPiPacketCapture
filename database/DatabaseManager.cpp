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
    string fname = ".secret";
    ifstream *ifs = new ifstream(fname);
    string str;

    //読み込み失敗時
    if (ifs->fail()) {
        cerr << "設定ファイルの読み込み失敗" << endl;
        //設定ファイル書き込み
        std::ofstream ofs;
        ofs.open(fname, std::ios::app);
        ofs << "filesystem:mysql" << endl;
        ofs << "sqlite_filename:data.db" << endl;
        ofs << "hostname:localhost" << endl;
        ofs << "database:" << endl;
        ofs << "table:data" << endl;
        ofs << "port:3306" << endl;
        ofs << "user:raspberry" << endl;
        ofs << "password:raspberry" << endl;
        ifs = new ifstream(fname);
    }
    string ofname = "log";
    ofstream.open(ofname, std::ios::app);
    //ファイル読み込み
    while (getline(*ifs, str)) {
        string value = str.substr(str.find(':') + 1);
        string key = str.substr(0, str.find(':'));
        //switch文に文字列は使えない?
        if (key == "filesystem")
            filesystem = value;
        else if (key == "sqlite_filename")
            sqlite_filename = value.c_str();
        else if (key == "hostname")
            hostname = value;
        else if (key == "database")
            database = value;
        else if (key == "table")
            tname = value;
        else if (key == "port")
            port = value;
        else if (key == "user")
            user = value;
        else if (key == "password")
            password = value;
    }
    delete ifs;

    if (filesystem == "mysql")
        isMysql = true;
    else
        isMysql = false;

    if (!connectDatabase()) {
        cout << "データベースに接続できませんでした." << endl;
        exit(1);
    }

    if (exsistsTable(tname)) {
        cout << "データベースが存在します." << endl;
    } else {
        cout << "データベースが存在しません." << endl;
        createTable();
    }
//select文の例
//    PreparedStatement *statment = getPreparedStatement("SELECT * FROM data WHERE id = ?");
//    statment->setInt(1, 1);
//    res = statment->executeQuery();
//    while (res->next()) {
//        cout << res->getString("date_time") << endl;
//    }

    //closeDatabase();
}

DatabaseManager::~DatabaseManager() {

}

/**
 * コネクションを接続します.
 */
bool DatabaseManager::connectDatabase() {
    cout << "コネクションの確立中..." << endl;
    if (isMysql)
        cout << "MySQLを使用します." << endl;
    else
        cout << "SQLiteを使用します" << endl;
    cout << "接続先: " << hostname << endl;

    if (isMysql) {
        try {
            driver = get_mysql_driver_instance();
            cout << "tcp://" + hostname + ":" + port <<" " <<user<<" " << password << endl;
            conn = driver->connect("tcp://" + hostname + ":" + port, user, password);
            conn->setSchema(database);
            stmt = conn->createStatement();
            return true;
        } catch (sql::SQLException &e) {
            cout << e.what() << endl;
            this->manageException(e);
            throw;
            return false;
        }
    } else {
        //sqlite
        //sqlite3_open(sqlite_filename, &db);
        return true;
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
    if (isMysql) {
        if (!conn) {
            std::string msg = std::string(__func__) + " 失敗しました. ";
            msg += "システムはデータベースへ接続していません";
            throw std::runtime_error(msg);
        }
        conn->close();
    } else {
        //sqlite
        //sqlite3_close(db);
    }

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
        cout << e.what() << endl;
        //this->manageException(e);
        throw;
    }
}

void DatabaseManager::insertPacket(in_addr_t ip, char *date, int sport, int dport) {
    cout << date << "    " << ip << endl;
//    struct in_addr addr;
//    inet_aton("255.255.255.255", &addr);
//    cout << addr.s_addr << endl;
//    ofstream << date << "," << to_string(ip) << "," << to_string(sport)<< "," << to_string(dport) << "," << to_string(false) << endl;
    if (isMysql) {
        PreparedStatement *statment = getPreparedStatement(
                "INSERT INTO " + tname +
                " (date_time, ip_address, tcp_sport, tcp_dport, is_check) VALUES (?, ?, ?, ?, ?)");
        statment->setDateTime(1, date);//   日付
        statment->setInt64(2, ip);//        IPアドレス
        statment->setInt(3, sport);//       送信元ポート
        statment->setInt(4, dport);//       送信先ポート
        statment->setBoolean(5, false);//   データベースから既に取り出した行か？
        statment->executeQuery();
    } else {
        //sqlite
    }
}

bool DatabaseManager::exsistsTable(string tname) {
    if (isMysql) {
        string query = "SHOW TABLES LIKE '" + tname + "'";
        ResultSet *res = stmt->executeQuery(query);
        if (res->rowsCount() > 0)
            return true;
        else
            return false;
    } else {
        //sqlite
        return false;
    }
}

bool DatabaseManager::createTable() {

    if (isMysql) {
        string query = "CREATE TABLE IF NOT EXISTS " + tname + " (" + "  id int(11) unsigned NOT NULL AUTO_INCREMENT,"
                       + "date_time datetime NOT NULL ON UPDATE CURRENT_TIMESTAMP,"
                       + "ip_address int(10) unsigned NOT NULL,"
                       + "tcp_dport smallint(5) unsigned DEFAULT NULL,"
                       + "tcp_sport smallint(5) unsigned DEFAULT NULL,"
                       + "is_check tinyint(1) NOT NULL,"
                       + "PRIMARY KEY (id)"
                       + ") ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;";
        stmt->executeQuery(query);
    } else {
        //sqlite
    }
    return true;
}
