#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cctype>

struct Student {
    int id;
    std::string name;
    double gwa;
};
int nextId = 1;
std::vector<Student> roster;
const std::string FILENAME = "roster.txt";

// --- Utility Functions ---
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string sanitizeName(std::string name) {
    size_t start = name.find_first_not_of(" \t\n\r");
size_t end = name.find_last_not_of(" \t\n\r");

if (start == std::string::npos) {
    name = "Unknown";
} else {
    name = name.substr(start, end - start + 1);
}
    if (name.length() > 25) {
        name = name.substr(0, 25);
    }
    
    bool newWord = true;
for (char &c : name) {
    if (std::isspace(c)) {
        newWord = true;
    } else {
        if (newWord) {
            c = std::toupper(c);
            newWord = false;
        } else {
            c = std::tolower(c);
        }
    }
}
    return name;
}

// --- Persistence (File I/O) ---
void loadState() {
    std::ifstream file(FILENAME);
    if (!file) return;
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string s_id, s_name, s_gwa;
        
        std::getline(ss, s_id, '|');
        std::getline(ss, s_name, '|');
        std::getline(ss, s_gwa, '|');

        try {
            Student s;
            s.id = std::stoi(s_id);
            s.name = s_name;
            s.gwa = std::stod(s_gwa);
            
            if (s.gwa < 1.0 || s.gwa > 5.0) continue;
            
            if (s.id >= nextId) {
            	nextId = s.id + 1;
            }
            roster.push_back(s);
        } catch (...) { continue; }
    }
    file.close();
}

void saveState() {
    std::ofstream file(FILENAME, std::ios::trunc); 
    for (const auto& s : roster) {
        file << s.id << "|" << s.name << "|" << s.gwa << "\n";
    }
    file.close();
}

bool isDeanList(double gwa) {
    return gwa <= 1.75;
}

// --- CRUD Operations ---
void createRecord() {
    Student s;
    
    s.id = nextId++;

    std::cout << "Enter Full Name: ";
    std::getline(std::cin >> std::ws, s.name);
    s.name = sanitizeName(s.name); 
    
    std::cout << "Enter GWA: ";
    while (!(std::cin >> s.gwa) || s.gwa < 1.0 || s.gwa > 5.0) {
        std::cout << "Invalid GWA. Enter a number 1-5: ";
        clearInput();
    }
    roster.push_back(s);
    
    std::cout << "Record added successfully. ID: " << s.id << "\n";
}

void readRecords() {
    std::cout << "\n" << std::setw(5) << "ID" << std::setw(25) << "NAME" << std::setw(10) << "GWA" << std::setw(18) << "DEAN'S LIST" << "\n";
    std::cout << std::string(58, '-') << "\n";

    for (const auto& s : roster) {
        std::cout << std::setw(5) << s.id << std::setw(25) << s.name << std::setw(10) << std::fixed << std::setprecision(2) << s.gwa << std::setw(18) << (isDeanList(s.gwa) ? "Eligible" : "Ineligible") << "\n";
    }
}

void updateRecord() {
    int id;

    std::cout << "Enter ID of student to update: ";
    while (!(std::cin >> id)) {
    	std::cout << "Invalid ID: ";
    	clearInput();
}
    for (auto& s : roster) {
        if (s.id == id) {
            clearInput();
            std::cout << "Enter New Name: ";
            std::getline(std::cin, s.name);
            s.name = sanitizeName(s.name);
            std::cout << "Enter New GWA: ";
            while (!(std::cin >> s.gwa) || s.gwa < 1.0 || s.gwa > 5.0) {
    std::cout << "Invalid GWA. Enter a number 1-5: ";
    clearInput();
}
            std::cout << "Record updated.\n";
            return;
        }
    }
    std::cout << "ID not found.\n";
}

void deleteRecord() {
    int id;
    std::cout << "Enter ID to delete: ";
    while (!(std::cin >> id)) {
    	std::cout << "Invalid ID: ";
    	clearInput();
}
    for (auto it = roster.begin(); it != roster.end(); ++it) {
        if (it->id == id) {
            roster.erase(it);
            std::cout << "Record removed.\n";
            return;
        }
    }
    std::cout << "ID not found.\n";
}

// --- Main Loop ---
int main() {
    loadState();
    char choice;

    do {
        std::cout << "\n-x- STUDENT ROSTER MAIN MENU -x-\n";
        std::cout << "[C]reate Record\n[R]ead All\n[U]pdate Record\n[D]elete Record\n[E]xit\nChoice: ";

        std::cin >> std::ws >> choice;
        choice = toupper(choice);
        clearInput();
        switch (choice) {
            case 'C': createRecord(); break;
            case 'R': readRecords(); break;
            case 'U': updateRecord(); break;
            case 'D': deleteRecord(); break;
            case 'E': saveState(); std::cout << "Record(s) saved. Thanks!\n"; break;
            default: std::cout << "Invalid option.\n";
        }
    } while (choice != 'E');
    return 0;
}