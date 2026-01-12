#pragma once
#ifndef DECODER_H
#define DECODER_H

#include <vector>
#include "Node.h"
using std::vector;

class Decoder {
public:
    vector<BYTE> decodeFromBytes(
        const vector<BYTE>& encoded,
        Node* root,
        int originalSize
    );
};

#endif