#include "Encoder.h"
#include "Stack.h"

Encoder::Encoder() {
    reset();
}

void Encoder::reset() {
    for (int i = 0; i < 256; i++) {
        freq[i] = 0;
        code[i].clear();
    }
}

void Encoder::countFrequency(const vector<BYTE>& data) {
    for (BYTE b : data) {
        freq[b]++;
    }
}

const int* Encoder::getFrequency() const {
    return freq;
}

const string* Encoder::getCodeTable() const {
    return code;
}

//Duyệt cây Huffman → sinh bảng mã
void Encoder::buildCode(Node* root) {
    Stack st;
    st.push(root, "");

    while (!st.empty()) {
        Node* node;
        string curCode;
        st.pop(node, curCode);

        if (node->isLeaf()) {
            code[node->ch] = curCode;
        }

        if (node->right) st.push(node->right, curCode + "1");
        if (node->left)  st.push(node->left, curCode + "0");
    }
}

// Encode BYTE → BYTE (bit packing)
vector<BYTE> Encoder::encodeToBytes(const vector<BYTE>& data) {
    vector<BYTE> output;
    BYTE current = 0;
    int bitCount = 0;

    for (BYTE b : data) {
        const string& s = code[b];

        for (char bit : s) {
            current <<= 1;
            if (bit == '1') current |= 1;

            bitCount++;
            if (bitCount == 8) {
                output.push_back(current);
                current = 0;
                bitCount = 0;
            }
        }
    }

    if (bitCount > 0) {
        current <<= (8 - bitCount);
        output.push_back(current);
    }

    return output;
}
