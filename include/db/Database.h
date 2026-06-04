#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <mysql.h>
#include <mysqld_error.h>

class Database {
private:
    MYSQL *conn;
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    unsigned int port;

public:
    Database(const std::string &host, const std::string &user,
             const std::string &password, const std::string &database,
             unsigned int port = 3306);

    ~Database();

    Database(const Database &) = delete;
    Database &operator=(const Database &) = delete;

    bool connect();
    void disconnect();
    bool isConnected() const;

    MYSQL *getConnection() const;
    std::string getError() const;
};

#endif
