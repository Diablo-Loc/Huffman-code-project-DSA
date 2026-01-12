#include "FileHelper.h"
#include <fstream>

std::vector<BYTE> FileHelper::readBinary(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    std::vector<BYTE> data;

    if (!file) return data;

    BYTE b;
    while (file.read((char*)&b, 1))
        data.push_back(b);

    return data;
}

void FileHelper::writeBinary(const std::string& path,
    const std::vector<BYTE>& data) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return;

    file.write((char*)data.data(), data.size());
}
