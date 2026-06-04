#include "model/Person.h"
#include <sstream>

Person::Person() : id(0), fullName(""), age(0) {}

Person::Person(int id, const std::string &fullName, int age)
    : id(id), fullName(fullName), age(age) {}

int Person::getId() const { return id; }
void Person::setId(int id) { this->id = id; }

std::string Person::getFullName() const { return fullName; }
void Person::setFullName(const std::string &fullName) { this->fullName = fullName; }

int Person::getAge() const { return age; }
void Person::setAge(int age) { this->age = age; }

std::string Person::toString() const {
    std::stringstream out;
    out << "Id: " << id << "\n"
        << "Full Name: " << fullName << "\n"
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
