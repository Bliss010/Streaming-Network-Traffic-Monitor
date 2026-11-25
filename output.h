#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <string>
#include "monitor_types.h"

// Prints the top devices as a ranked table. Expects the list sorted from highest count.
void printTopK(std::ostream& out, const DeviceCount* list, int n);

// Saves the top devices to a CSV file (rank,ip,count).
// Returns false and prints a message if the file cannot be written.
bool saveResults(const std::string& filename, const DeviceCount* list, int n,
                 std::ostream& err);

#endif
