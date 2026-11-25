// Tests for the menu and output functions (Bliss).
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../src/menu.h"
#include "../src/output.h"

static std::string readFile(const std::string& name) {
    std::ifstream f(name);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

int main() {
    std::ostringstream out;

    // Normal: valid menu choices
    { std::istringstream in("6\n");    assert(readMenuChoice(in, out) == 6); }
    { std::istringstream in(" 10 \n"); assert(readMenuChoice(in, out) == 10); }

    // Invalid: letters, out of range, negative, empty line
    { std::istringstream in("abc\n"); assert(readMenuChoice(in, out) == -1); }
    { std::istringstream in("42\n");  assert(readMenuChoice(in, out) == -1); }
    { std::istringstream in("-1\n");  assert(readMenuChoice(in, out) == -1); }
    { std::istringstream in("\n");    assert(readMenuChoice(in, out) == -1); }

    // Boundary: lowest and highest allowed k
    { long k = 10; std::istringstream in("1\n");    assert(readSetting(in, out, "k", 1, 1000, k) && k == 1); }
    { long k = 10; std::istringstream in("1000\n"); assert(readSetting(in, out, "k", 1, 1000, k) && k == 1000); }

    // Invalid settings keep the old value
    { long k = 10; std::istringstream in("-5\n");  assert(!readSetting(in, out, "k", 1, 1000, k) && k == 10); }
    { long m = 64; std::istringstream in("0\n");   assert(!readSetting(in, out, "memory (KB)", 1, 4096, m) && m == 64); }
    { long m = 64; std::istringstream in("12x\n"); assert(!readSetting(in, out, "memory (KB)", 1, 4096, m) && m == 64); }

    // Mode
    { std::string mode = "both"; std::istringstream in("exact\n"); assert(readMode(in, out, mode) && mode == "exact"); }
    { std::string mode = "both"; std::istringstream in("fast\n");  assert(!readMode(in, out, mode) && mode == "both"); }

    // saveResults writes the expected CSV
    DeviceCount top[3] = {{"10.0.0.1", 8}, {"10.0.0.2", 6}, {"10.0.0.3", 4}};
    assert(saveResults("test_results.csv", top, 3, out));
    assert(readFile("test_results.csv") ==
           "rank,ip,count\n1,10.0.0.1,8\n2,10.0.0.2,6\n3,10.0.0.3,4\n");

    // Empty list: header only, no crash
    assert(saveResults("test_empty.csv", nullptr, 0, out));
    assert(readFile("test_empty.csv") == "rank,ip,count\n");

    // Invalid path: returns false, no crash
    assert(!saveResults("no_such_folder/results.csv", top, 3, out));

    // printTopK on an empty list prints a message
    { std::ostringstream o; printTopK(o, nullptr, 0); assert(o.str().find("No devices") != std::string::npos); }

    std::remove("test_results.csv");
    std::remove("test_empty.csv");
    std::cout << "All menu and output tests passed (19 checks).\n";
    return 0;
}
