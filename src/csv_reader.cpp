#include <iostream>
#include <fstream>
#include <string>
#include "csv_reader.h"

using namespace std;

bool openStream(ifstream& in, const string& path) {
    in.open(path);

    if (!in.is_open()) {
        cout << "File not found: " << path << endl;
        return false;
    }

    return true;
}

// returns true if s is a whole number
bool isWholeNumber(const string& s) {
    if (s.size() == 0 || s.size() > 15) {
        return false;
    }

    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') {
            return false;
        }
    }
    return true;
}

// returns true if s is a non-negative decimal number
bool isDecimalNumber(const string& s) {
    if (s.size() == 0 || s.size() > 20) {
        return false;
    }

    int dots = 0;
    int digits = 0;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            digits++;
        } else if (s[i] == '.') {
            dots++;
        } else {
            return false;
        }
    }

    if (dots > 1 || digits == 0) {
        return false;
    }
    return true;
}

// splits one line into a record. returns true if it parsed, false if not
bool parseLine(const string& line, Record& rec) {

    string text = line;

    // remove the '\r' that windows files leave at the end of a line
    if (text.size() > 0 && text[text.size() - 1] == '\r') {
        text = text.substr(0, text.size() - 1);
    }

    // empty line: broken
    if (text.size() == 0) {
        return false;
    }

    // split the line at each comma
    string fields[7];
    int count = 0;
    string current = "";

    for (size_t i = 0; i < text.size(); i++) {
        char c = text[i];
        if (c == ',') {
            if (count < 7) {
                fields[count] = current;
            }
            count++;
            current = "";
        } else {
            current = current + c;
        }
    }

    if (count < 7) {
        fields[count] = current;
    }
    count++;

    if (count != 7) {
        return false;
    }

    if (fields[1] == "" || fields[2] == "") {
        return false;
    }

    // every number field must contain a number
    if (!isDecimalNumber(fields[0])) {
        return false;
    }
    if (!isWholeNumber(fields[3]) || !isWholeNumber(fields[4]) ||
        !isWholeNumber(fields[5]) || !isWholeNumber(fields[6])) {
        return false;
    }

    // convert the text to numbers
    long long port = stoll(fields[3]);
    long long protocol = stoll(fields[4]);
    long long bytes = stoll(fields[5]);
    long long label = stoll(fields[6]);

    // check the allowed ranges
    if (port > 65535) {
        return false;
    }

    rec.timestamp = stod(fields[0]);
    rec.src_ip = fields[1];
    rec.dst_ip = fields[2];
    rec.dst_port = (int)port;
    rec.protocol = (int)protocol;
    rec.bytes = bytes;
    rec.label = (int)label;

    return true;
}