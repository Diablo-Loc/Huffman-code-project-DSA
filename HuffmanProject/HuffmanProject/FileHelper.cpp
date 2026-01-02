#include "FileHelper.h"
#include <fstream>

vector<unsigned char> FileHelper::readBinary(const string& path) {
    ifstream file(path, ios::binary);
    vector<unsigned char> data;

    if (!file) return data;

    unsigned char byte;
    while (file.read((char*)&byte, 1)) {
        data.push_back(byte);
    }

    file.close();
    return data;
}

void FileHelper::writeBinary(const string& path, const vector<unsigned char>& data) {
    ofstream file(path, ios::binary);

    for (unsigned char byte : data) {
        file.write((char*)&byte, 1);
    }

    file.close();
}
