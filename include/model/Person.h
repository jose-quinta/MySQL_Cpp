#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
private:
    int id;
    std::string name;
    std::string fLastname;
    std::string mLastname;
    int age;

public:
    Person();
    Person(int id, const std::string &name, const std::string &fLastname, const std::string &mLastname, int age);

    int getId() const;
    void setId(int id);

    std::string getName() const;
    void setName(const std::string &name);

    std::string getFLastname() const;
    void setFLastname(const std::string &fLastname);

    std::string getMLastname() const;
    void setMLastname(const std::string &mLastname);

    int getAge() const;
    void setAge(int age);

    std::string toString() const;
    std::string getAgeCategory() const;
};

#endif
