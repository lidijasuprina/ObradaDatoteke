#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem> 
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

const int COLUMN_WIDTH = 20;
const string OUTPUT_DIRECTORY = "output";
const string ACCEPTED_EXTENSION = ".csv";

class Person {
    public:
        string firstName, lastName, dateOfBirth, firstNameHex, lastNameHex, dateOfBirthHex;

        Person(string first, string last, string dob) : firstName(first), lastName(last), dateOfBirth(dob) {
            firstNameHex = stringToHex(firstName);
            lastNameHex = stringToHex(lastName);
            dateOfBirth = formatDate(dateOfBirth);
            dateOfBirthHex = stringToHex(dateOfBirth);
            
        }

        void printDetails() const {
            cout << left << setw(COLUMN_WIDTH) << firstName;
            cout << left << setw(COLUMN_WIDTH) << lastName;
            cout << left << setw(COLUMN_WIDTH) << dateOfBirth;
            cout << left << setw(COLUMN_WIDTH) << firstNameHex;
            cout << left << setw(COLUMN_WIDTH) << lastNameHex;
            cout << left << setw(COLUMN_WIDTH) << dateOfBirthHex;
            cout << endl;
        }

    private:
        string stringToHex(string str) {
            stringstream result;
            result << hex << uppercase;
            
            for (char ch : str) {
                result << setw(2) << setfill('0') << static_cast<int>(ch);
            }

            return result.str();
        }

        string formatDate(string dob) {
            istringstream dateStream(dob);
            int day, month, year;
            char discard;
            dateStream >> day >> discard >> month >> discard >> year;

            ostringstream formattedDate;
            formattedDate << setfill('0') << setw(2) << day << '-'
                        << setfill('0') << setw(2) << month << '-'
                        << setfill('0') << setw(4) << year;
            return formattedDate.str();
        }
};

string trimWhitespace(string str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");

    if (start == string::npos || end == string::npos) {
        return "";
    }

    return str.substr(start, end - start + 1);
}

bool validateFile(string filePath) {
    if (!filesystem::exists(filePath)) {
        cerr << "Datoteka '" << filePath << "' ne postoji." << endl;
        return false;
    }

    if (filesystem::is_directory(filePath)) {
        cerr << "Putanja '" << filePath << "' predstavlja direktorij." << endl;
        return false;
    }

    string ext = filesystem::path(filePath).extension().string();
    if (ext != ACCEPTED_EXTENSION) {
        cerr << "Datoteka '" << filePath << "' ima pogresan nastavak (prihvaca se samo CSV datoteka)." << endl;
        return false;
    }

    return true;
}

vector<Person> readFromFile(string filePath) {
    vector<Person> people;
    ifstream file(filePath);
    if (!file) {
        cerr << "Nije uspjelo otvaranje datoteke!" << endl;
        return people;
    }

    string line;
    while (getline(file, line)) {
        line = trimWhitespace(line);
        if (line.empty()) {
            continue;
        }

        istringstream ss(line);
        string firstName, lastName, dateOfBirth;

        if (!getline(ss, firstName, ';') || !getline(ss, lastName, ';') || !getline(ss, dateOfBirth, ';')) {
            cerr << "Error: Missing column(s) in line: " << line << endl;
            continue;
        }

        if (firstName.empty() || lastName.empty() || dateOfBirth.empty()) {
            cerr << "Error: Empty column(s) in line: " << line << endl;
            continue;
        }

        transform(firstName.begin(), firstName.end(), firstName.begin(), [](unsigned char c) { return toupper(c); });
        transform(lastName.begin(), lastName.end(), lastName.begin(), [](unsigned char c) { return toupper(c); });

        Person person(firstName, lastName, dateOfBirth);
        people.push_back(person);
    }

    file.close();
    return people;
}

void createNewFile(string filePath, vector<Person> people) {
    ofstream outputFile(filePath);
    if (!outputFile) {
        cerr << "Error creating new CSV file." << endl;
        return;
    }

    for (const auto& person : people) {
        outputFile << person.firstName << ";"
                    << person.lastName << ";"
                    << person.dateOfBirth << ";"
                    << person.firstNameHex << ";"
                    << person.lastNameHex << ";"
                    << person.dateOfBirthHex << endl;
    }

    outputFile.close();
}

void printHeader() {
    cout << left << setw(COLUMN_WIDTH) << "Ime";
    cout << left << setw(COLUMN_WIDTH) << "Prezime";
    cout << left << setw(COLUMN_WIDTH) << "Datum rodenja";
    cout << left << setw(COLUMN_WIDTH) << "Ime (hex)";
    cout << left << setw(COLUMN_WIDTH) << "Prezime (hex)";
    cout << left << setw(COLUMN_WIDTH) << "Datum rodenja (hex)";
    cout << endl;
}

int main() {
    string filePath;
    bool fileOpened = false;
    int maxAttempts = 3, attempts = 0;

    while (!fileOpened && attempts < maxAttempts) {
        cout << "Unesite putanju do ulazne datoteke: ";
        cin >> filePath;
        attempts++;

        if (!validateFile(filePath)) {
            continue;
        }
        
        cout << endl;
        fileOpened = true;
        attempts = 0;
    };

    if (!fileOpened) {
        cerr << "Nije uspjelo otvaranje datoteke nakon pokusaja " << maxAttempts << ". Izlaz..." << endl;
        return 1;
    }

    vector<Person> people = readFromFile(filePath);

    printHeader();
    for (const Person& person : people) {
        person.printDetails();
    }

    if (!filesystem::exists(OUTPUT_DIRECTORY)) {
        filesystem::create_directory(OUTPUT_DIRECTORY);
    }
    time_t currentTime = time(nullptr);
    string newFilePath = OUTPUT_DIRECTORY + "/data_" + to_string(currentTime) + ".csv";
    createNewFile(newFilePath, people);
    
    return 0;
}