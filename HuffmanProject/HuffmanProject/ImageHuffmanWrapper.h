#pragma once
#include <string>

class ImageHuffmanWrapper {
public:
    static bool compressImageBMP(
        const std::wstring& inputImage,
        const std::wstring& outputFile
    );

    static bool decompressImageBMP(
        const std::wstring& inputFile,
        const std::wstring& outputImage
    );
};
