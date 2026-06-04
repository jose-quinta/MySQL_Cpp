#include "repository/PersonRepository.h"
#include <iostream>
#include <memory>
#include <cstring>

PersonRepository::PersonRepository(Database &db) : db(db) {}

bool PersonRepository::pingOrError() const {
    if (!db.isConnected()) {
        std::cerr << "ERROR: Database connection lost." << std::endl;
        return false;
    }
    return true;
}

bool PersonRepository::createTableIfNotExists()
{
    if (!pingOrError())
        return false;

    MYSQL *conn = db.getConnection();
    const char *query =
        "CREATE TABLE IF NOT EXISTS Customer ("
        "  id INT AUTO_INCREMENT PRIMARY KEY,"
        "  fullname VARCHAR(100),"
        "  age INT"
        ")";

    if (mysql_query(conn, query) != 0)
    {
        std::cerr << "ERROR creating table: " << mysql_error(conn) << std::endl;
        return false;
    }

    std::cout << "Table 'Customer' ready." << std::endl;
    return true;
}

bool PersonRepository::insert(const Person &person) {
    if (!pingOrError()) return false;

    MYSQL *conn = db.getConnection();
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt) {
        std::cerr << "ERROR: Could not initialize statement." << std::endl;
        return false;
    }

    const char *query = "INSERT INTO Customer(fullname, age) VALUES(?, ?)";
    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        std::cerr << "ERROR: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    std::string name = person.getFullName();
    int age = person.getAge();

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void *)name.c_str();
    bind[0].buffer_length = name.length();

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &age;

    if (mysql_stmt_bind_param(stmt, bind) != 0) {
        std::cerr << "ERROR: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        std::cerr << "ERROR: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    mysql_stmt_close(stmt);
    std::cout << "Person added successfully." << std::endl;
    return true;
}

std::vector<Person> PersonRepository::getAll() const {
    std::vector<Person> people;

    if (!pingOrError()) return people;

    MYSQL *conn = db.getConnection();
    if (mysql_query(conn, "SELECT id, fullname, age FROM Customer") != 0) {
        std::cerr << "ERROR: " << mysql_error(conn) << std::endl;
        return people;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res) return people;

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        int id = row[0] ? atoi(row[0]) : 0;
        std::string name = row[1] ? row[1] : "";
        int age = row[2] ? atoi(row[2]) : 0;
        people.emplace_back(id, name, age);
    }

    mysql_free_result(res);
    return people;
}

std::vector<Person> PersonRepository::findByName(const std::string &name) const {
    std::vector<Person> people;

    if (!pingOrError()) return people;

    MYSQL *conn = db.getConnection();

    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt) return people;

    const char *query = "SELECT id, fullname, age FROM Customer WHERE fullname = ?";
    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        mysql_stmt_close(stmt);
        return people;
    }

    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = (void *)name.c_str();
    bind.buffer_length = name.length();

    if (mysql_stmt_bind_param(stmt, &bind) != 0) {
        mysql_stmt_close(stmt);
        return people;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        mysql_stmt_close(stmt);
        return people;
    }

    MYSQL_RES *res = mysql_stmt_result_metadata(stmt);
    if (!res) {
        mysql_stmt_close(stmt);
        return people;
    }

    MYSQL_BIND result[3];
    memset(result, 0, sizeof(result));

    int idVal;
    char nameBuf[256];
    unsigned long nameLen;
    int ageVal;
    my_bool isNull[3];

    result[0].buffer_type = MYSQL_TYPE_LONG;
    result[0].buffer = &idVal;
    result[0].is_null = &isNull[0];

    result[1].buffer_type = MYSQL_TYPE_STRING;
    result[1].buffer = nameBuf;
    result[1].buffer_length = sizeof(nameBuf);
    result[1].length = &nameLen;
    result[1].is_null = &isNull[1];

    result[2].buffer_type = MYSQL_TYPE_LONG;
    result[2].buffer = &ageVal;
    result[2].is_null = &isNull[2];

    if (mysql_stmt_bind_result(stmt, result) != 0) {
        mysql_free_result(res);
        mysql_stmt_close(stmt);
        return people;
    }

    mysql_stmt_store_result(stmt);

    while (mysql_stmt_fetch(stmt) == 0) {
        std::string personName(nameBuf, nameLen);
        people.emplace_back(idVal, personName, ageVal);
    }

    mysql_free_result(res);
    mysql_stmt_close(stmt);
    return people;
}

Person *PersonRepository::findById(int id) const {
    if (!pingOrError()) return nullptr;

    MYSQL *conn = db.getConnection();

    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt) return nullptr;

    const char *query = "SELECT id, fullname, age FROM Customer WHERE id = ?";
    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        mysql_stmt_close(stmt);
        return nullptr;
    }

    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_LONG;
    bind.buffer = &id;

    if (mysql_stmt_bind_param(stmt, &bind) != 0) {
        mysql_stmt_close(stmt);
        return nullptr;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        mysql_stmt_close(stmt);
        return nullptr;
    }

    MYSQL_RES *res = mysql_stmt_result_metadata(stmt);
    if (!res) {
        mysql_stmt_close(stmt);
        return nullptr;
    }

    MYSQL_BIND result[3];
    memset(result, 0, sizeof(result));

    int idVal;
    char nameBuf[256];
    unsigned long nameLen;
    int ageVal;
    my_bool isNull[3];

    result[0].buffer_type = MYSQL_TYPE_LONG;
    result[0].buffer = &idVal;
    result[0].is_null = &isNull[0];

    result[1].buffer_type = MYSQL_TYPE_STRING;
    result[1].buffer = nameBuf;
    result[1].buffer_length = sizeof(nameBuf);
    result[1].length = &nameLen;
    result[1].is_null = &isNull[1];

    result[2].buffer_type = MYSQL_TYPE_LONG;
    result[2].buffer = &ageVal;
    result[2].is_null = &isNull[2];

    if (mysql_stmt_bind_result(stmt, result) != 0) {
        mysql_free_result(res);
        mysql_stmt_close(stmt);
        return nullptr;
    }

    mysql_stmt_store_result(stmt);

    Person *person = nullptr;
    if (mysql_stmt_fetch(stmt) == 0) {
        std::string personName(nameBuf, nameLen);
        person = new Person(idVal, personName, ageVal);
    }

    mysql_free_result(res);
    mysql_stmt_close(stmt);
    return person;
}

bool PersonRepository::removeById(int id) {
    if (!pingOrError()) return false;

    MYSQL *conn = db.getConnection();

    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt) return false;

    const char *query = "DELETE FROM Customer WHERE id = ?";
    if (mysql_stmt_prepare(stmt, query, strlen(query)) != 0) {
        std::cerr << "ERROR: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    MYSQL_BIND bind;
    memset(&bind, 0, sizeof(bind));
    bind.buffer_type = MYSQL_TYPE_LONG;
    bind.buffer = &id;

    if (mysql_stmt_bind_param(stmt, &bind) != 0) {
        std::cerr << "ERROR: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    if (mysql_stmt_execute(stmt) != 0) {
        std::cerr << "ERROR: " << mysql_stmt_error(stmt) << std::endl;
        mysql_stmt_close(stmt);
        return false;
    }

    mysql_stmt_close(stmt);
    std::cout << "Person deleted successfully." << std::endl;
    return true;
}
