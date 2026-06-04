#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
private:
    int id;
    std::string fullName;
    int age;

public:
    Person();
    Person(int id, const std::string &fullName, int age);

    int getId() const;
    void setId(int id);

    std::string getFullName() const;
    void setFullName(const std::string &fullName);

    int getAge() const;
    void setAge(int age);

    std::string toString() const;
    std::string getAgeCategory() const;
};

#endif
