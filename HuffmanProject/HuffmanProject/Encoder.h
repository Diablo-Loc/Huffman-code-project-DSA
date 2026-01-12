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
    string code[256];

public:
    Encoder();

    void reset();
    void countFrequency(const vector<BYTE>& data);
    void buildCode(Node* root);

    const int* getFrequency() const;
    const string* getCodeTable() const;

    vector<BYTE> encodeToBytes(const vector<BYTE>& data);
};

#endif
