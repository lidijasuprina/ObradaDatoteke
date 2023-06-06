#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem> 
#include <vector>
#include <algorithm>
#include <regex>

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
            regex dayMonthYear(R"(^(0[1-9]|1[0-9]|2[0-9]|3[01])[-./](0[1-9]|1[0-2])[-./](0\d{3}|[1-9]\d{3})$)");
            regex yearMonthDay(R"(^(0\d{3}|[1-9]\d{3})[-./](0[1-9]|1[0-2])[-./](0[1-9]|1\d|2[0-9]|3[01])$)");
            regex monthDayYear(R"(^(0[1-9]|1[0-2])[/.-](0[1-9]|1[0-9]|2[0-9]|3[01])[/.-](0\d{3}|[1-9]\d{3})$)");

            if (regex_match(dob, dayMonthYear)) {
                replace(dob.begin(), dob.end(), '/', '-');
                replace(dob.begin(), dob.end(), '.', '-');
            } else if (regex_match(dob, yearMonthDay)) {
                dob = dob.substr(8, 2) + "-" + dob.substr(5, 2) + "-" + dob.substr(0, 4);

            } else if (regex_match(dob, monthDayYear)) {
                dob = dob.substr(3, 2) + "-" + dob.substr(0, 2) + "-" + dob.substr(6, 4);
                
            } else {
                cerr << "Nevažeći format datuma: " << dob << endl;
            }

            return dob;
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
        cerr << "Datoteka '" << filePath << "' ima pogrešan nastavak (prihvaća se samo CSV datoteka)." << endl;
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
            cerr << "Pogreška: nedostaju stupci u redu: " << line << endl;
            continue;
        }

        if (firstName.empty() || lastName.empty() || dateOfBirth.empty()) {
            cerr << "Pogreška: prazni stupci u redu: " << line << endl;
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
        cerr << "Pogreška pri izradi nove CSV datoteke." << endl;
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
    cout << left << setw(COLUMN_WIDTH) << "Datum rođenja";
    cout << left << setw(COLUMN_WIDTH) << "Ime (hex)";
    cout << left << setw(COLUMN_WIDTH) << "Prezime (hex)";
    cout << left << setw(COLUMN_WIDTH) << "Datum rođenja (hex)";
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
        cerr << "Nije uspjelo otvaranje datoteke nakon pokušaja " << maxAttempts << ". Izlaz..." << endl;
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