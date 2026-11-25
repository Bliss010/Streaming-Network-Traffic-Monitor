#include <iostream>
#include <fstream>
#include <string>
#include "src/csv_reader.h"

using namespace std;

int main() {
    string fileName;
    cout << "Enter file name: ";
    cin >> fileName;

    ifstream in;
    if (!openStream(in, fileName)) {
        return 1;
    }

    string line;

    //skip the header line
    getline(in, line);

    int validCount = 0;
    int brokenCount = 0;

    while (getline(in, line)) {
        Record rec;
        if (parseLine(line, rec)) {
            validCount++;
        } else {
            brokenCount++;
            cout << "Broken line: " << line << endl;
        }
    }

    cout << "Valid lines: " << validCount << endl;
    cout << "Broken lines: " << brokenCount << endl;
    return 0;
}