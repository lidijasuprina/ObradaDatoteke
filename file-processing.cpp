#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <filesystem> 

using namespace std;

const int COLUMN_WIDTH = 20;
fstream file;

class cperson {
    public:
        string firstName, lastName, dateOfBirth, firstNameHex, lastNameHex, dateOfBirthHex;

        cperson(string first, string last, string date) : firstName(first), lastName(last), dateOfBirth(date) {
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

string trimWhitespace(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");

    if (start == string::npos || end == string::npos) {
        return "";
    }

    return str.substr(start, end - start + 1);
}

int main() {
    string filePath, directoryPath = "output", acceptExt = ".csv", ext;
    bool fileOpened = false;
    int maxAttempts = 3, attempts = 0;

    while (!fileOpened && attempts < maxAttempts) {
        cout << "Unesite putanju do ulazne datoteke: ";
        cin >> filePath;
        ext = filesystem::path(filePath).extension().string();
        attempts++;

        if (!filesystem::exists(filePath)) {
            cerr << "Datoteka '" << filePath << "' ne postoji." << endl;
        } else if (!ifstream(filePath)) {
            cerr << "Datoteku '" << filePath << "' se ne moze otvoriti." << endl;
        } else if (acceptExt != ext) {
            cerr << "Datoteka '" << filePath << "' ima pogresan nastavak (prihvaca se samo CSV datoteka)." << endl;
        } else {
            file.open(filePath, ios::in|ios::binary);
            if (!file) {
                cerr << "Nije uspjelo otvaranje datoteke!" << endl;
            } else {
                cout << endl;
                fileOpened = true;
                attempts = 0;
            }
        }
    };

    if (!fileOpened) {
        cerr << "Nije uspjelo otvaranje datoteke nakon pokusaja " << maxAttempts << ". Izlaz..." << endl;
        return 1;
    }


    vector<cperson> people;
    string line;

    while (getline(file, line)) {
        line = trimWhitespace(line);
        if (line.empty()) {
            continue;
        }

        istringstream ss(line);
        string firstName, lastName, dateOfBirth;

        if (!getline(ss, firstName, ',') || !getline(ss, lastName, ',') || !getline(ss, dateOfBirth, ',')) {
            cerr << "Error: Missing column(s) in line: " << line << endl;
            continue;
        }

        if (firstName.empty() || lastName.empty() || dateOfBirth.empty()) {
            cerr << "Error: Empty column(s) in line: " << line << endl;
            continue;
        }

        transform(firstName.begin(), firstName.end(), firstName.begin(),
            [](unsigned char c) { return toupper(c); });
        transform(lastName.begin(), lastName.end(), lastName.begin(),
            [](unsigned char c) { return toupper(c); });

        cperson person(firstName, lastName, dateOfBirth);
        people.push_back(person);
    }

    file.close();

    cout << left << setw(COLUMN_WIDTH) << "First Name";
    cout << left << setw(COLUMN_WIDTH) << "Last Name";
    cout << left << setw(COLUMN_WIDTH) << "Date of Birth";
    cout << left << setw(COLUMN_WIDTH) << "First Name (Hex)";
    cout << left << setw(COLUMN_WIDTH) << "Last Name (Hex)";
    cout << left << setw(COLUMN_WIDTH) << "Date of Birth (Hex)";
    cout << endl;

    for (const cperson& person : people) {
        person.printDetails();
    }

    if (!filesystem::exists(directoryPath)) {
        filesystem::create_directory("output");
    }

    ofstream outputFile("output/data.csv");
    if (!outputFile) {
        cerr << "Error creating new CSV file." << endl;
        return 1;
    }

    for (const auto& person : people) {
        outputFile << person.firstName << ","
                << person.lastName << ","
                << person.dateOfBirth << ","
                << person.firstNameHex << ","
                << person.lastNameHex << ","
                << person.dateOfBirthHex << endl;
    }
    outputFile.close();
    
    return 1;
}