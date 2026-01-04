#pragma once
#pragma once
#include <vector>
#include <string>
#include <windows.h>

using BYTE = unsigned char;

struct ImageData {
    int width;
    int height;
    std::vector<BYTE> pixels; // RGB liên tiếp
};

class ImageHelper {
public:
    static ImageData loadBitmap(const std::wstring& path);
    static void saveBitmap(const std::wstring& path, const ImageData& img);
};
