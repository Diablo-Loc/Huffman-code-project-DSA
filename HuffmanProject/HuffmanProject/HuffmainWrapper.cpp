#include "HuffmanWrapper.h"
#include "FileHelper.h"
#include "Encoder.h"
#include "Decoder.h"
#include "HuffmanTree.h"

#include <fstream>
#include <vector>

using namespace std;

/* =================== COMPRESS =================== */
bool HuffmanWrapper::compressFile(
    const string& inputPath,
    const string& outputPath)
{
    vector<BYTE> data = FileHelper::readBinary(inputPath);
    if (data.empty()) return false;

    // 1. Đếm tần suất
    Encoder enc;
    enc.countFrequency(data);

    // 2. Build cây Huffman
    HuffmanTree tree;
    Node* root = tree.buildTree(enc.getFrequency());

    // 3. Build mã Huffman
    enc.buildCode(root);

    // 4. Encode dữ liệu
    vector<BYTE> encoded = enc.encodeToBytes(data);

    ofstream out(outputPath, ios::binary);
    if (!out) return false;

    // 5. Ghi bảng tần suất
    out.write((char*)enc.getFrequency(), 256 * sizeof(int));

    // 6. Ghi kích thước dữ liệu gốc
    int originalSize = (int)data.size();
    out.write((char*)&originalSize, sizeof(int));

    // 7. Ghi dữ liệu đã nén
    out.write((char*)encoded.data(), encoded.size());

    out.close();
    return true;
}

/* =================== DECOMPRESS =================== */
bool HuffmanWrapper::decompressFile(
    const string& inputPath,
    const string& outputPath)
{
    ifstream in(inputPath, ios::binary);
    if (!in) return false;

    // 1. Đọc bảng tần suất
    int freq[256];
    in.read((char*)freq, 256 * sizeof(int));

    // 2. Đọc kích thước dữ liệu gốc
    int originalSize;
    in.read((char*)&originalSize, sizeof(int));

    // 3. Build lại cây Huffman
    HuffmanTree tree;
    Node* root = tree.buildTree(freq);

    // 4. Đọc dữ liệu nén
    vector<BYTE> encoded;
    BYTE b;
    while (in.read((char*)&b, 1)) {
        encoded.push_back(b);
    }
    in.close();

    // 5. Decode
    Decoder dec;
    vector<BYTE> decoded =
        dec.decodeFromBytes(encoded, root, originalSize);

    // 6. Ghi file giải nén
    FileHelper::writeBinary(outputPath, decoded);

    return true;
}
