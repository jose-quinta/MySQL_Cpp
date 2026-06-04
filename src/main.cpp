#include <iostream>
#include "db/Database.h"
#include "repository/PersonRepository.h"
#include "ui/Console.h"

int main() {
    Database db("localhost", "TestCpp", "root_cpp", "person_test", 3306);

    if (!db.connect()) {
        std::cerr << "Connection failed. Exiting." << std::endl;
        system("pause");
        return 1;
    }

    PersonRepository(db).createTableIfNotExists();

    Console console(db);
    console.run();

    return 0;
}
