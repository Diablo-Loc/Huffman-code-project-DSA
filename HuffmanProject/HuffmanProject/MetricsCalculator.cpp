#include "MetricsCalculator.h"
#include <cmath>
#include <algorithm>

double MetricsCalculator::calculateEntropy(
    const int freq[256],
    int totalSize)
{
    double entropy = 0.0;
    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            double p = (double)freq[i] / totalSize;
            entropy -= p * log2(p);
        }
    }
    return entropy;
}

int MetricsCalculator::calculateTreeDepth(Node* root)
{
    if (!root) return 0;
    return 1 + std::max(
        calculateTreeDepth(root->left),
        calculateTreeDepth(root->right)
    );
}

double MetricsCalculator::calculateSpaceSaving(
    size_t originalSize,
    size_t compressedSize)
{
    if (originalSize == 0) return 0;
    return (1.0 - (double)compressedSize / originalSize) * 100.0;
}
