#include "HuffmanWrapper.h"
#include "FileHelper.h"
#include "Encoder.h"
#include "Decoder.h"
#include "HuffmanTree.h"
#include "MetricsCalculator.h"
#include "Metrics.h"

#include <chrono>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

static const char MAGIC[4] = { 'H','U','F','1' };

//compress
bool HuffmanWrapper::compressFile(
    const string& inputPath,
    const string& outputPath,
    CompressionMetrics& metrics)
{
    using namespace std::chrono;
    auto start = high_resolution_clock::now();
    vector<BYTE> data = FileHelper::readBinary(inputPath);
    if (data.empty()) return false;

    Encoder enc;
    enc.countFrequency(data);

    HuffmanTree* tree = new HuffmanTree();
    Node* root = tree->buildTree(enc.getFrequency());
    if (!root) {
        delete tree;
        return false;
    }

    enc.buildCode(root);
    vector<BYTE> encoded = enc.encodeToBytes(data);

    ofstream out(outputPath, ios::binary);
    if (!out) return false;

    out.write(MAGIC, 4);

    int originalSize = (int)data.size();
    out.write((char*)&originalSize, sizeof(int));

    out.write((char*)enc.getFrequency(), 256 * sizeof(int));
    out.write((char*)encoded.data(), encoded.size());

    out.close();
    auto end = high_resolution_clock::now();

    // METRICS
    metrics.originalSize = data.size();
    metrics.compressedSize =
        4 + sizeof(int) + 256 * sizeof(int) + encoded.size();

    metrics.entropy =
        MetricsCalculator::calculateEntropy(
            enc.getFrequency(), data.size());

    metrics.maxTreeDepth =
        MetricsCalculator::calculateTreeDepth(root);

    metrics.spaceSaving =
        MetricsCalculator::calculateSpaceSaving(
            metrics.originalSize,
            metrics.compressedSize);

    metrics.processingTimeMs =
        duration<double, std::milli>(end - start).count();
    metrics.root = root;

    return true;
}

//decompress
bool HuffmanWrapper::decompressFile(
    const string& inputPath,
    const string& outputPath,
    CompressionMetrics& metrics)
{
    auto start = chrono::high_resolution_clock::now();
    ifstream in(inputPath, ios::binary);
    if (!in) return false;

    // 1. Kiểm tra Magic Number
    char magic[4];
    in.read(magic, 4);
    if (memcmp(magic, MAGIC, 4) != 0) return false;

    // 2. Đọc kích thước gốc
    int originalSize;
    in.read((char*)&originalSize, sizeof(int));

    // 3. Đọc bảng tần số
    int freq[256];
    in.read((char*)freq, 256 * sizeof(int));

    // 4. Tái tạo cây Huffman
    HuffmanTree* tree = new HuffmanTree();
    Node* root = tree->buildTree(freq);
    if (!root) return false;

    // 5. Đọc toàn bộ dữ liệu mã hóa còn lại
    in.seekg(0, ios::end);
    long long fileSize = in.tellg();
    long long currentPos = 4 + sizeof(int) + (256 * sizeof(int));
    long long dataSize = fileSize - currentPos;
    in.seekg(currentPos, ios::beg);

    vector<BYTE> encoded(dataSize);
    in.read((char*)encoded.data(), dataSize);
    in.close();

    // 6. Giải mã
    Decoder dec;
    vector<BYTE> decoded = dec.decodeFromBytes(encoded, root, originalSize);

    // 7. Ghi file kết quả
    FileHelper::writeBinary(outputPath, decoded);

    // 8. Trả root về cho UI
    metrics.root = root;
    auto end = chrono::high_resolution_clock::now();

    // ĐIỀN THÔNG TIN VÀO METRICS ĐỂ UI HIỂN THỊ
    metrics.root = root;
    metrics.maxTreeDepth = MetricsCalculator::calculateTreeDepth(root);
    metrics.processingTimeMs = chrono::duration<double, std::milli>(end - start).count();
    return true;
}
