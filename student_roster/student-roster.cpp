#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>

struct Student {
    int id;
    std::string name;
    double gpa;
};
std::vector<Student> roster;
const std::string FILENAME = "roster.txt";

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string sanitizeName(std::string name) {
    if (name.length() > 20) {
        name = name.substr(0, 20);
    }
    return name;
}

void loadState() {
    std::ifstream file(FILENAME);
    if (!file) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string s_id, s_name, s_gpa;

        std::getline(ss, s_id, ',');
        std::getline(ss, s_name, ',');
        std::getline(ss, s_gpa, ',');

        try {
            Student s;
            
            s.id = std::stoi(s_id);
            s.name = s_name;
            s.gpa = std::stod(s_gpa);
            roster.push_back(s);
        } catch (...) { continue; }
    }
    file.close();
}

void saveState() {
    std::ofstream file(FILENAME, std::ios::trunc);
    for (const auto& s : roster) {
        file << s.id << "," << s.name << "," << s.gpa << "\n";
    }
    file.close();
}

void createRecord() {
    Student s;

    std::cout << "Enter Student ID (Numeric): ";
    while (!(std::cin >> s.id)) {
        std::cout << "Invalid ID. Enter a number: ";
        clearInput();
    }
    clearInput();

    std::cout << "Enter Full Name: ";
    std::getline(std::cin, s.name);
    s.name = sanitizeName(s.name);

    std::cout << "Enter GPA: ";
    while (!(std::cin >> s.gpa)) {
        std::cout << "Invalid GPA. Enter a number: ";
        clearInput();
    }
    roster.push_back(s);
    std::cout << "Record added successfully.\n";
}

void readRecords() {
    std::cout << "\n" << std::setw(5) << "ID" << std::setw(25) << "NAME" << std::setw(10) << "GPA" << "\n";
    std::cout << std::string(40, '-') << "\n";

    for (const auto& s : roster) {
        std::cout << std::setw(5) << s.id << std::setw(25) << s.name << std::setw(10) << std::fixed << std::setprecision(2) << s.gpa << "\n";
    }
}

void updateRecord() {
    int id;

    std::cout << "Enter ID of student to update: ";
    std::cin >> id;

    for (auto& s : roster) {
        if (s.id == id) {
            clearInput();
            std::cout << "Enter New Name: ";
            std::getline(std::cin, s.name);
            s.name = sanitizeName(s.name);

            std::cout << "Enter New GPA: ";
            std::cin >> s.gpa;
            std::cout << "Record updated.\n";

            return;
        }
    }
    std::cout << "ID not found.\n";
}

void deleteRecord() {
    int id;

    std::cout << "Enter ID to delete: ";
    std::cin >> id;

    for (auto it = roster.begin(); it != roster.end(); ++it) {
        if (it->id == id) {
            roster.erase(it);
            std::cout << "Record removed.\n";
            return;
        }
    }
    std::cout << "ID not found.\n";
}

int main() {
    loadState();
    char choice;

    do {
        std::cout << "\n--- STUDENT ROSTER MAIN MENU ---\n";
        std::cout << "[C]reate Record\n[R]ead All\n[U]pdate Record\n[D]elete Record\n[E]xit\nChoice: ";
        std::cin >> choice;
        choice = toupper(choice);

        switch (choice) {
            case 'C':
                createRecord(); 
                break;
            case 'R':
                readRecords();
                break;
            case 'U':
                updateRecord();
                break;
            case 'D':
                deleteRecord();
                break;
            case 'E':
                saveState();
                std::cout << "System saved. Goodbye!\n";
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    } while (choice != 'E');

    return 0;
}