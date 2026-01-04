#include "ImageHuffmanWrapper.h"
#include "ImageHelper.h"
#include "Encoder.h"
#include "Decoder.h"
#include "HuffmanTree.h"

#include <fstream>

using namespace std;

bool ImageHuffmanWrapper::compressImageBMP(
    const wstring& inputImage,
    const wstring& outputFile
) {
    // 1. Load ảnh
    ImageData img = ImageHelper::loadBitmap(inputImage);
    if (img.pixels.empty()) return false;

    // 2. Huffman encode
    Encoder enc;
    enc.countFrequency(img.pixels);

    HuffmanTree tree;
    Node* root = tree.buildTree(enc.getFrequency());
    enc.buildCode(root);

    vector<BYTE> encoded = enc.encodeToBytes(img.pixels);

    // 3. Ghi file nén
    ofstream out(outputFile, ios::binary);
    if (!out) return false;

    // Lưu metadata ảnh
    out.write((char*)&img.width, sizeof(int));
    out.write((char*)&img.height, sizeof(int));

    // Lưu bảng tần suất
    out.write((char*)enc.getFrequency(), 256 * sizeof(int));

    // Lưu size gốc
    int originalSize = (int)img.pixels.size();
    out.write((char*)&originalSize, sizeof(int));

    // Lưu dữ liệu nén
    out.write((char*)encoded.data(), encoded.size());

    out.close();
    return true;
}

bool ImageHuffmanWrapper::decompressImageBMP(
    const wstring& inputFile,
    const wstring& outputImage
) {
    ifstream in(inputFile, ios::binary);
    if (!in) return false;

    // 1. Đọc metadata ảnh
    ImageData img;
    in.read((char*)&img.width, sizeof(int));
    in.read((char*)&img.height, sizeof(int));

    // 2. Đọc bảng tần suất
    int freq[256];
    in.read((char*)freq, 256 * sizeof(int));

    // 3. Đọc size gốc
    int originalSize;
    in.read((char*)&originalSize, sizeof(int));

    // 4. Build lại cây Huffman
    HuffmanTree tree;
    Node* root = tree.buildTree(freq);

    // 5. Đọc dữ liệu nén
    vector<BYTE> encoded;
    BYTE b;
    while (in.read((char*)&b, 1)) {
        encoded.push_back(b);
    }
    in.close();

    // 6. Decode
    Decoder dec;
    img.pixels = dec.decodeFromBytes(encoded, root, originalSize);

    // 7. Ghi ảnh BMP
    ImageHelper::saveBitmap(outputImage, img);

    return true;
}
