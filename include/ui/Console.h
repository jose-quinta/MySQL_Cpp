#ifndef CONSOLE_H
#define CONSOLE_H

#include "db/Database.h"
#include "repository/PersonRepository.h"

class Console {
private:
    PersonRepository repository;
    bool running;

    void displayMenu() const;
    void handleInsert();
    void handleListAll();
    void handleFindByName();
    void handleDelete();

public:
    explicit Console(Database &db);

    void run();
};

#endif
