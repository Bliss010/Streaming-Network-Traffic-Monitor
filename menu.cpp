#include "menu.h"

// Turns text into a whole number. Returns false if the text is not a whole number.
static bool toLong(const std::string& text, long& result) {
    if (text.empty()) return false;
    size_t i = 0;
    bool negative = false;
    if (text[0] == '-') { negative = true; i = 1; }
    if (i == text.size()) return false;
    long value = 0;
    for (; i < text.size(); i++) {
        if (text[i] < '0' || text[i] > '9') return false;
        value = value * 10 + (text[i] - '0');
        if (value > 1000000000L) return false;   // too large
    }
    result = negative ? -value : value;
    return true;
}

// Removes spaces at the start and end of a line.
static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

void showMenu(std::ostream& out) {
    out << "\n===== Streaming Network Traffic Monitor =====\n"
        << " 1. Open data file\n"
        << " 2. Choose mode (exact / approximate / both)\n"
        << " 3. Set memory budget (KB)\n"
        << " 4. Set k (number of top devices)\n"
        << " 5. Set alert threshold\n"
        << " 6. Run stream\n"
        << " 7. Show top-k devices\n"
        << " 8. Show alerts\n"
        << " 9. Run benchmark\n"
        << "10. Save results to CSV\n"
        << " 0. Exit\n"
        << "Enter choice: ";
}

int readMenuChoice(std::istream& in, std::ostream& out) {
    std::string line;
    if (!std::getline(in, line)) return 0;       // end of input: treat as exit
    long choice;
    if (!toLong(trim(line), choice) || choice < 0 || choice > 10) {
        out << "Invalid choice. Please enter a number from 0 to 10.\n";
        return -1;
    }
    return static_cast<int>(choice);
}

bool readSetting(std::istream& in, std::ostream& out, const std::string& name,
                 long minValue, long maxValue, long& value) {
    out << "Enter " << name << " (" << minValue << " to " << maxValue << "): ";
    std::string line;
    if (!std::getline(in, line)) return false;
    long newValue;
    if (!toLong(trim(line), newValue) || newValue < minValue || newValue > maxValue) {
        out << "Invalid " << name << ". Keeping the old value (" << value << ").\n";
        return false;
    }
    value = newValue;
    out << name << " set to " << value << ".\n";
    return true;
}

bool readMode(std::istream& in, std::ostream& out, std::string& mode) {
    out << "Enter mode (exact / approximate / both): ";
    std::string line;
    if (!std::getline(in, line)) return false;
    line = trim(line);
    if (line == "exact" || line == "approximate" || line == "both") {
        mode = line;
        out << "Mode set to " << mode << ".\n";
        return true;
    }
    out << "Invalid mode. Keeping the old mode (" << mode << ").\n";
    return false;
}
