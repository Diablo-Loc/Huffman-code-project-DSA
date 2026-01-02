#pragma once
#include <vector>
#include <string>
using namespace std;

class FileHelper {
public:
    static vector<unsigned char> readBinary(const string& path);
    static void writeBinary(const string& path, const vector<unsigned char>& data);
};
