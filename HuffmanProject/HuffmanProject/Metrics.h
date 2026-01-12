#pragma once
#include "Node.h"
struct CompressionMetrics
{
    double spaceSaving;
    double entropy;
    int    maxTreeDepth;
    double processingTimeMs;

    size_t originalSize;
    size_t compressedSize;
    Node* root;
};