#pragma once
#include <vector>
#include <string>

class FileHelper {
public:
    static std::vector<unsigned char> readBinary(const std::wstring& path);
    static void writeBinary(const std::wstring& path,
        const std::vector<unsigned char>& data);
};
