#ifndef MONITOR_TYPES_H
#define MONITOR_TYPES_H

#include <string>

// One device and its count, used for top-k output.
struct DeviceCount {
    std::string ip;
    long count;
};

// Settings chosen from the menu.
struct Settings {
    std::string mode = "both";   // exact, approximate, or both
    long memoryKB = 64;          // memory budget for approximate structures
    long k = 10;                 // number of top devices to show
    long threshold = 50;         // new connections per window before an alert
    long window = 10000;         // records per window
};

#endif
