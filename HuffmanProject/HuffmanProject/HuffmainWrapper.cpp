#include "HuffmanWrapper.h"
#include "FileHelper.h"
#include "Encoder.h"
#include "Decoder.h"
#include "HuffmanTree.h"

/*bool HuffmanWrapper::compressFile(
    const string& inputPath,
    const string& outputPath)
{
    vector<BYTE> data = FileHelper::readBinary(inputPath);
    if (data.empty()) return false;

    Encoder enc;
    enc.countFrequency(data);

    Node* root = TreeBuilder::build(enc.getFrequency());
    enc.buildCode(root);

    vector<BYTE> encoded = enc.encodeToBytes(data);

    ofstream out(outputPath, ios::binary);
    if (!out) return false;

    // 1️ ghi bảng tần suất
    out.write((char*)enc.getFrequency(), 256 * sizeof(int));

    // 2️ ghi kích thước gốc
    int originalSize = data.size();
    out.write((char*)&originalSize, sizeof(int));

    // 3️ ghi dữ liệu nén
    out.write((char*)encoded.data(), encoded.size());

    out.close();
    return true;
}
bool HuffmanWrapper::decompressFile(
    const string& inputPath,
    const string& outputPath)
{
    ifstream in(inputPath, ios::binary);
    if (!in) return false;

    // 1️ đọc bảng tần suất
    int freq[256];
    in.read((char*)freq, 256 * sizeof(int));

    // 2️ đọc size gốc
    int originalSize;
    in.read((char*)&originalSize, sizeof(int));

    // 3️ build lại cây Huffman
    Node* root = TreeBuilder::build(freq);

    // 4️ đọc phần dữ liệu nén
    vector<BYTE> encoded;
    BYTE b;
    while (in.read((char*)&b, 1)) {
        encoded.push_back(b);
    }
    in.close();

    // 5️ decode
    Decoder dec;
    vector<BYTE> decoded =
        dec.decodeFromBytes(encoded, root, originalSize);

    // 6️ ghi file gốc
    FileHelper::writeBinary(outputPath, decoded);
    return true;
}
*/
