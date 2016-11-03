//
// Created by System Administrator on 9/29/16.
//

#include "MysqlManager.h"
#include <fstream>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql::mysql;

MysqlManager::MysqlManager() {
    //const std::string hostname, const std::string dbName, const std::string user,const std::string passwd
    string fname = ".secret";
    ifstream *ifs = new ifstream(fname);
    string str;
    if (ifs->fail()) {
        cerr << "設定ファイルの読み込み失敗" << endl;
        //設定ファイル書き込み
        std::ofstream ofs;
        ofs.open(fname, std::ios::app);
        ofs << "hostname:localhost" << endl;
        ofs << "database:" << endl;
        ofs << "table:tada" << endl;
        ofs << "port:3306" << endl;
        ofs << "user:raspberry" << endl;
        ofs << "passwd:raspberry" << endl;
        ifs = new ifstream(fname);
    }
    while (getline(*ifs, str)) {
        string value = str.substr(str.find(':') + 1);
        string key = str.substr(0, str.find(':'));
        //switch文に文字列は使えない?
        if (key == "hostname") {
            hostname = value;
        } else if (key == "database") {
            database = value;
        } else if (key == "table") {
            tname = value;
        } else if (key == "port") {
            port = value;
        } else if (key == "user") {
            user = value;
        } else if (key == "password") {
            password = value;
        }
    }
    delete ifs;

    if(!connectDatabase()){
        cout << "データベースに接続できませんでした." << endl;
        exit(1);
    }

    if(exsistsTable(tname)){
        cout << "データベースが存在します." << endl;
    }else {
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

MysqlManager::~MysqlManager() {

}

/**
 * コネクションを接続します.
 */
boolean MysqlManager::connectDatabase() {
    cout << "コネクションの確立中..." << endl;
    cout << "接続先: " << hostname << endl;

    try {
        driver = get_mysql_driver_instance();
        conn = driver->connect("tcp://" + hostname + ":" + port, user, password);
        conn->setSchema(database);
        stmt = conn->createStatement();
        return true;
    } catch (sql::SQLException &e) {
        //this->manageException(e);
        //throw;
        return false;
    }


}

/**
 * エラー文を正規化します
 */
void MysqlManager::manageException(sql::SQLException &e) {
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
void MysqlManager::closeDatabase() {
    if (!conn) {
        std::string msg = std::string(__func__) + " 失敗しました. ";
        msg += "システムはデータベースへ接続していません";
        throw std::runtime_error(msg);
    }

    conn->close();

    delete stmt;
    delete conn;
    delete driver;
    cout << "コネクションの切断" << endl;
}

/**
 * テーブルに情報を格納します
 */
PreparedStatement *MysqlManager::getPreparedStatement(const sql::SQLString &sql) {
    try {
        return conn->prepareStatement(sql);
    } catch (sql::SQLException &e) {
        this->manageException(e);
        throw;
    }
}

void MysqlManager::insertPacket(in_addr_t ip, char *date,int sport,int dport) {
    cout << date << "    " << ip << endl;
//    struct in_addr addr;
//    inet_aton("255.255.255.255", &addr);
//    cout << addr.s_addr << endl;
    PreparedStatement *statment = getPreparedStatement("INSERT INTO "+tname+" (date_time, ip_address, tcp_sport, tcp_dport, is_check) VALUES (?, ?, ?, ?, ?)");
    statment->setDateTime(1, date);//   日付
    statment->setInt64(2, ip);//        IPアドレス
    statment->setInt(3, sport);//       送信元ポート
    statment->setInt(4, dport);//       送信先ポート
    statment->setBoolean(5, false);//   データベースから既に取り出した行か？
    statment->executeQuery();
}

boolean MysqlManager::exsistsTable(string tname) {
    string query = "SHOW TABLES LIKE '"+tname+"'";
    ResultSet *res = stmt->executeQuery(query);
    if(res->rowsCount() > 0)
        return true;
    else
        return false;
}

boolean MysqlManager::createTable() {
    string query = "CREATE TABLE IF NOT EXISTS "+tname+" ("+ "  id int(11) unsigned NOT NULL AUTO_INCREMENT,"
                   + "date_time datetime NOT NULL ON UPDATE CURRENT_TIMESTAMP,"
                   + "ip_address int(10) unsigned NOT NULL,"
                   + "tcp_dport smallint(5) unsigned DEFAULT NULL,"
                   + "tcp_sport smallint(5) unsigned DEFAULT NULL,"
                   + "is_check tinyint(1) NOT NULL,"
                   + "PRIMARY KEY (id)"
                   + ") ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;";
    stmt->executeQuery(query);
    return true;
}
