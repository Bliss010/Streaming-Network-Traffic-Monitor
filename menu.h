#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>

// Prints the main menu.
void showMenu(std::ostream& out);

// Reads one menu choice. Returns 0-10, or -1 if the input is invalid.
int readMenuChoice(std::istream& in, std::ostream& out);

// Reads a whole number between minValue and maxValue.
// On valid input, stores it in value and returns true.
// On invalid input, prints a message, keeps the old value, and returns false.
bool readSetting(std::istream& in, std::ostream& out, const std::string& name,
                 long minValue, long maxValue, long& value);

// Reads the mode (exact, approximate, or both). Keeps the old mode if invalid.
bool readMode(std::istream& in, std::ostream& out, std::string& mode);

#endif
