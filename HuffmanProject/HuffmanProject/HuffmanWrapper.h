#pragma once
#ifndef HUFFMAN_WRAPPER_H
#define HUFFMAN_WRAPPER_H

#include "Metrics.h"
#include <string>

class HuffmanWrapper {
public:
    static bool compressFile(const std::string& inputPath,
        const std::string& outputPath, CompressionMetrics& metrics);

    static bool decompressFile(const std::string& inputPath,
        const std::string& outputPath);
};

#endif
