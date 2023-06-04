#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <filesystem> 

using namespace std;

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
            cout << left << setw(20) << firstName;
            cout << left << setw(20) << lastName;
            cout << left << setw(20) << dateOfBirth;
            cout << left << setw(20) << firstNameHex;
            cout << left << setw(20) << lastNameHex;
            cout << left << setw(20) << dateOfBirthHex;
            cout << endl;
        }

    private:
        string stringToHex(string str) {
            stringstream result;
            for (char ch : str) {
                result << hex << uppercase << setw(2) << setfill('0') << static_cast<int>(ch);
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


int main() {
    string filePath, directoryPath = "output";
    bool fileOpened = false;

    do {
        cout << "Unesite putanju do ulazne datoteke: ";
        cin >> filePath;

        file.open(filePath, ios::in|ios::binary);
        if (!file) {
            cerr << "Neuspjelo otvaranje datoteke!" << endl;
        } else {
            fileOpened = true;
        }
    } while (!fileOpened);

    vector<cperson> people;
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string firstName, lastName, dateOfBirth;
        getline(ss, firstName, ',');
        getline(ss, lastName, ',');
        getline(ss, dateOfBirth, ',');
        transform(firstName.begin(), firstName.end(), firstName.begin(),
            [](unsigned char c) { return toupper(c); });
        transform(lastName.begin(), lastName.end(), lastName.begin(),
            [](unsigned char c) { return toupper(c); });
        cperson person(firstName, lastName, dateOfBirth);
        people.push_back(person);
    }

    file.close();

    cout << left << setw(20) << "First Name";
    cout << left << setw(20) << "Last Name";
    cout << left << setw(20) << "Date of Birth";
    cout << left << setw(20) << "First Name (Hex)";
    cout << left << setw(20) << "Last Name (Hex)";
    cout << left << setw(20) << "Date of Birth (Hex)";
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
        return 0;
    }

    for (const auto& person : people) {
        outputFile << person.firstName << ","
                << person.lastName << ","
                << person.dateOfBirth << ","
                << person.firstNameHex << ","
                << person.lastNameHex << ","
                << person.dateOfBirthHex << std::endl;
    }
    outputFile.close();
    
    return 0;
}