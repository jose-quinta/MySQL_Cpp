#ifndef PERSON_REPOSITORY_H
#define PERSON_REPOSITORY_H

#include <vector>
#include "db/Database.h"
#include "model/Person.h"

class PersonRepository {
private:
    Database &db;

    bool pingOrError() const;

public:
    explicit PersonRepository(Database &db);

    bool createTableIfNotExists();
    bool insert(const Person &person);
    std::vector<Person> getAll() const;
    std::vector<Person> findByName(const std::string &name) const;
    Person *findById(int id) const;
    bool update(const Person &person);
    bool removeById(int id);
};

#endif
