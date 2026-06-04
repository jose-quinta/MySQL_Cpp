#include "db/Database.h"
#include <iostream>

Database::Database(const std::string &host, const std::string &user,
                   const std::string &password, const std::string &database,
                   unsigned int port)
    : conn(nullptr), host(host), user(user), password(password),
      database(database), port(port) {}

Database::~Database() {
    disconnect();
}

bool Database::connect() {
    conn = mysql_init(nullptr);
    if (!conn) {
        std::cerr << "ERROR: MySQL object could not be created." << std::endl;
        return false;
    }

    if (!mysql_real_connect(conn, host.c_str(), user.c_str(),
                            password.c_str(), database.c_str(),
                            port, nullptr, 0)) {
        std::cerr << "ERROR: " << mysql_error(conn) << std::endl;
        mysql_close(conn);
        conn = nullptr;
        return false;
    }

    return true;
}

void Database::disconnect() {
    if (conn) {
        mysql_close(conn);
        conn = nullptr;
    }
}

bool Database::isConnected() const {
    return conn && mysql_ping(conn) == 0;
}

MYSQL *Database::getConnection() const {
    return conn;
}

std::string Database::getError() const {
    return conn ? mysql_error(conn) : "No connection";
}
