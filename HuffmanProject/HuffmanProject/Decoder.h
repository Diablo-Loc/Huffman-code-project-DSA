#pragma once
#ifndef DECODER_H
#define DECODER_H

#include <vector>
#include "Node.h"
using namespace std;

class Decoder {
public:
    vector<unsigned char> decodeFromBytes(
        const vector<unsigned char>& bytes,
        Node* root,
        int originalSize
    );
};

#endif
