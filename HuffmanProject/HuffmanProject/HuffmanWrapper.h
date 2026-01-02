#pragma once
#include <string>
using namespace std;

class HuffmanWrapper {
public:
    static bool compressFile(const string& inputPath,
        const string& outputPath);

    static bool decompressFile(const string& inputPath,
        const string& outputPath);
};
