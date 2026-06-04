#include "model/Person.h"
#include <sstream>

Person::Person() : id(0), name(""), fLastname(""), mLastname(""), age(0) {}

Person::Person(int id, const std::string &name, const std::string &fLastname, const std::string &mLastname, int age)
    : id(id), name(name), fLastname(fLastname), mLastname(mLastname), age(age) {}

int Person::getId() const { return id; }
void Person::setId(int id) { this->id = id; }

std::string Person::getName() const { return this->name; }
void Person::setName(const std::string &name) { this->name = name; }

std::string Person::getFLastname() const { return this->fLastname; }
void Person::setFLastname(const std::string &fLastname) { this->fLastname = fLastname; }

std::string Person::getMLastname() const { return this->mLastname; }
void Person::setMLastname(const std::string &mLastname) { this->mLastname = mLastname; }

int Person::getAge() const { return age; }
void Person::setAge(int age) { this->age = age; }

std::string Person::toString() const {
    std::stringstream out;
    out << "Id: " << id << "\n"
        << "Full Name: " << name << " " << fLastname << " " << mLastname << "\n"
        << "Age: " << age << "\n"
        << getAgeCategory() << "\n";
    return out.str();
}

std::string Person::getAgeCategory() const {
    if (age <= 0)  return "Invalid age";
    if (age < 4)   return "Baby";
    if (age < 11)  return "Child";
    if (age < 19)  return "Adolescent";
    if (age < 40)  return "Adult";
    if (age < 60)  return "Upper adult";
    if (age < 100) return "Elderly";
    return "Centenarian";
}
