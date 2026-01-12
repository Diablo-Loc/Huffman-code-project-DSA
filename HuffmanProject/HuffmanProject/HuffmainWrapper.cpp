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

    HuffmanTree tree;
    Node* root = tree.buildTree(enc.getFrequency());
    if (!root) return false;

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
    const string& outputPath)
{
    ifstream in(inputPath, ios::binary);
    if (!in) return false;

    char magic[4];
    in.read(magic, 4);
    if (memcmp(magic, MAGIC, 4) != 0) return false;

    int originalSize;
    in.read((char*)&originalSize, sizeof(int));

    int freq[256];
    in.read((char*)freq, 256 * sizeof(int));

    HuffmanTree tree;
    Node* root = tree.buildTree(freq);
    if (!root) return false;

    vector<BYTE> encoded;
    BYTE b;
    while (in.read((char*)&b, 1))
        encoded.push_back(b);

    Decoder dec;
    vector<BYTE> decoded =
        dec.decodeFromBytes(encoded, root, originalSize);

    FileHelper::writeBinary(outputPath, decoded);
    return true;
}
