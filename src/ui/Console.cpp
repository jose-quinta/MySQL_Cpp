#include "ui/Console.h"
#include <iostream>
#include <limits>
#include <cstdlib>

Console::Console(Database &db) : repository(db), running(true) {}

void Console::displayMenu() const {
    std::cout << "\n\t\t\tMENU\n"
              << "1. Insert person\n"
              << "2. Show all people\n"
              << "3. Find person by name\n"
              << "4. Delete person\n"
              << "0. Exit\n"
              << "Option: ";
}

void Console::handleInsert() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string name;
    std::string fLastname;
    std::string mLastname;
    int age;

    std::cout << "Name: ";
    std::getline(std::cin, name);

    std::cout << "Father's Last Name: ";
    std::getline(std::cin, fLastname);

    std::cout << "Mother's Last Name: ";
    std::getline(std::cin, mLastname);

    std::cout << "Age: ";
    std::cin >> age;

    Person person(0, name, fLastname, mLastname, age);
    repository.insert(person);
}

void Console::handleListAll() {
    auto people = repository.getAll();

    if (people.empty()) {
        std::cout << "No people found." << std::endl;
        return;
    }

    for (const auto &p : people) {
        std::cout << "********************* Person #" << p.getId()
                  << " ****************************\n"
                  << p.toString() << std::endl;
    }
}

void Console::handleFindByName() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string name;
    std::cout << "Enter the name to search: ";
    std::getline(std::cin, name);

    auto people = repository.findByName(name);

    if (people.empty()) {
        std::cout << "No person found with that name." << std::endl;
        return;
    }

    for (const auto &p : people) {
        std::cout << "********************* Person #" << p.getId()
                  << " ****************************\n"
                  << p.toString() << std::endl;
    }
}

void Console::handleDelete() {
    int id;
    std::cout << "Insert the ID of the person to delete: ";
    std::cin >> id;

    Person *person = repository.findById(id);
    if (person) {
        std::cout << "Deleting:\n" << person->toString() << std::endl;
        delete person;
        repository.removeById(id);
    } else {
        std::cout << "No person found with ID " << id << std::endl;
    }
}

void Console::run() {
    int option;

    while (running) {
        system("cls");
        std::cout << "Logged in." << std::endl;
        displayMenu();

        std::cin >> option;

        switch (option) {
            case 1: handleInsert(); break;
            case 2: handleListAll(); break;
            case 3: handleFindByName(); break;
            case 4: handleDelete(); break;
            case 0:
                std::cout << "Are you sure? (1: Yes / 0: No): ";
                int confirm;
                std::cin >> confirm;
                if (confirm == 1) running = false;
                break;
            default:
                std::cout << "Invalid option." << std::endl;
                break;
        }

        if (running && option != 0) {
            system("pause");
        }
    }

    std::cout << "BYE!!!" << std::endl;
}
