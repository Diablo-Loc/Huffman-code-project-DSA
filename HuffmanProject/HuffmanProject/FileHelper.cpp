#include "FileHelper.h"
#include <fstream>

using namespace std;

vector<unsigned char> FileHelper::readBinary(const wstring& path)
{
    ifstream file(path, ios::binary);
    vector<unsigned char> data;

    if (!file) return data;

    unsigned char byte;
    while (file.read((char*)&byte, 1)) {
        data.push_back(byte);
    }
    return data;
}

void FileHelper::writeBinary(const wstring& path,
    const vector<unsigned char>& data)
{
    ofstream file(path, ios::binary);
    for (unsigned char b : data)
        file.write((char*)&b, 1);
}
