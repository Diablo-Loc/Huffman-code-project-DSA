#pragma once
#ifndef ENCODER_H
#define ENCODER_H

#include "Node.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

class Encoder {
private:
    int freq[256];
    string huffmanCode[256];
public:
    Encoder();

    void countFrequency(const vector<BYTE>& data);
    int* getFrequency();

    void buildCode(Node* root);
    vector<BYTE> encodeToBytes(const vector<BYTE>& data);

    string getCode(BYTE b);
};

#endif
