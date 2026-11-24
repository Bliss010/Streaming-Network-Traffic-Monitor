#ifndef CSV_READER_H
#define CSV_READER_H

#include <string>
#include <fstream>

// one network traffic record 
struct Record {
    double timestamp;
    std::string src_ip;
    std::string dst_ip;
    int dst_port;
    int protocol;
    long long bytes;
    int label;
};

// opens data file. returns true if it opened, false if not. 
bool openStream(std::ifstream& in, const std::string& path);

// splits one line into a record. returns true if it parsed, false if not.
bool parseLine(const std::string& line, Record& rec);

#endif