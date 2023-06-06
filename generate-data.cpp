#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <filesystem>

using namespace std;

const string OUTPUT_DIRECTORY = "input";

int randomNumber(int min, int max) {
    random_device random;
    mt19937 generate(random());
    uniform_int_distribution<int> distributor(min, max);
    return distributor(generate);
}

string randomDate() {
    int day = randomNumber(1, 28);
    int month = randomNumber(1, 12);
    int year = randomNumber(1950, 2000);
    ostringstream date;
    date << setfill('0') << setw(2) << day << "-" << setw(2) << month << "-" << year;
    return date.str();
}

string randomName() {
    string names[] = { "Ivo", "Ana", "Pero", "Ivana", "Marko", "Marija", "Cvjetko", "Slavica" };
    int index = randomNumber(0, sizeof(names) / sizeof(names[0]) - 1);
    return names[index];
}

string randomSurname() {
    string surnames[] = { "Ivic", "Anic", "Peric", "Ivanic", "Markic", "Maric", "Cvjetic", "Slavic" };
    int index = randomNumber(0, sizeof(surnames) / sizeof(surnames[0]) - 1);
    return surnames[index];
}

int main() {
    int records = 10;

    if (!filesystem::exists(OUTPUT_DIRECTORY)) {
        filesystem::create_directory(OUTPUT_DIRECTORY);
    }
    string filePath = OUTPUT_DIRECTORY + "/data.csv";

    ofstream outputFile(filePath);
    if (!outputFile) {
        cerr << "Error opening output file." << endl;
        return 1;
    }

    for (int i = 0; i < records; ++i) {
        string name = randomName();
        string surname = randomSurname();
        string dob = randomDate();
        outputFile << name << ";" << surname << ";" << dob << endl;
    }

    outputFile.close();

    return 0;
}
