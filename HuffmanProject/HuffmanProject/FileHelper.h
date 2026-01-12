#pragma once
#include <vector>
#include <string>

typedef unsigned char BYTE;

class FileHelper {
public:
    static std::vector<BYTE> readBinary(const std::string& path);
    static void writeBinary(const std::string& path,
        const std::vector<BYTE>& data);
};
