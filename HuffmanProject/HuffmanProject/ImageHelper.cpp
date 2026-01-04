#include "ImageHelper.h"
#include <fstream>

ImageData ImageHelper::loadBitmap(const std::wstring& path) {
    ImageData img;

    std::ifstream file(path, std::ios::binary);

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    file.read((char*)&fileHeader, sizeof(fileHeader));
    file.read((char*)&infoHeader, sizeof(infoHeader));

    img.width = infoHeader.biWidth;
    img.height = infoHeader.biHeight;

    int padding = (4 - (img.width * 3) % 4) % 4;

    img.pixels.resize(img.width * img.height * 3);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            BYTE bgr[3];
            file.read((char*)bgr, 3);

            int idx = (y * img.width + x) * 3;
            img.pixels[idx] = bgr[2]; // R
            img.pixels[idx + 1] = bgr[1]; // G
            img.pixels[idx + 2] = bgr[0]; // B
        }
        file.ignore(padding);
    }

    file.close();
    return img;
}

void ImageHelper::saveBitmap(const std::wstring& path, const ImageData& img) {
    std::ofstream file(path, std::ios::binary);

    BITMAPFILEHEADER fileHeader = {};
    BITMAPINFOHEADER infoHeader = {};

    int padding = (4 - (img.width * 3) % 4) % 4;
    int dataSize = (img.width * 3 + padding) * img.height;

    fileHeader.bfType = 0x4D42;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    fileHeader.bfSize = fileHeader.bfOffBits + dataSize;

    infoHeader.biSize = sizeof(BITMAPINFOHEADER);
    infoHeader.biWidth = img.width;
    infoHeader.biHeight = img.height;
    infoHeader.biPlanes = 1;
    infoHeader.biBitCount = 24;
    infoHeader.biCompression = BI_RGB;

    file.write((char*)&fileHeader, sizeof(fileHeader));
    file.write((char*)&infoHeader, sizeof(infoHeader));

    BYTE pad[3] = { 0,0,0 };

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            int idx = (y * img.width + x) * 3;
            BYTE bgr[3] = {
                img.pixels[idx + 2],
                img.pixels[idx + 1],
                img.pixels[idx]
            };
            file.write((char*)bgr, 3);
        }
        file.write((char*)pad, padding);
    }

    file.close();
}
