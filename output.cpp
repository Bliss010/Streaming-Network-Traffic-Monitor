#include "output.h"
#include <fstream>
#include <iomanip>

void printTopK(std::ostream& out, const DeviceCount* list, int n) {
    if (list == nullptr || n <= 0) {
        out << "No devices to show. Run the stream first.\n";
        return;
    }
    out << std::left << std::setw(6) << "Rank" << std::setw(18) << "Device (src IP)"
        << "Count\n";
    for (int i = 0; i < n; i++) {
        out << std::left << std::setw(6) << (i + 1) << std::setw(18) << list[i].ip
            << list[i].count << "\n";
    }
}

bool saveResults(const std::string& filename, const DeviceCount* list, int n,
                 std::ostream& err) {
    std::ofstream file(filename);
    if (!file) {
        err << "Error: could not open " << filename << " for writing.\n";
        return false;
    }
    file << "rank,ip,count\n";
    for (int i = 0; list != nullptr && i < n; i++) {
        file << (i + 1) << "," << list[i].ip << "," << list[i].count << "\n";
    }
    return true;
}
