#pragma once
#include "HuffmanTree.h"

class MetricsCalculator
{
public:
    static double calculateEntropy(
        const int freq[256],
        int totalSize);

    static int calculateTreeDepth(Node* root);

    static double calculateSpaceSaving(
        size_t originalSize,
        size_t compressedSize);
};
