#include "Encoder.h"
#include "Stack.h"

Encoder::Encoder() {
    for (int i = 0; i < 256; i++) {
        freq[i] = 0;
        huffmanCode[i] = "";
    }
}

// dếm tần suất pixel / byte
void Encoder::countFrequency(const vector<BYTE>& data) {
    for (BYTE b : data) {
        freq[b]++;
    }
}
int* Encoder::getFrequency() {
    return freq;
}

// duyệt cây
void Encoder::buildCode(Node* root) {
    Stack st;
    st.push(root, "");

    while (!st.empty()) {
        Node* node;
        string code;
        st.pop(node, code);
        if (!node->left && !node->right) {
            huffmanCode[node->data] = code;
        }
        if (node->right) st.push(node->right, code + "1");
        if (node->left)  st.push(node->left, code + "0");
    }
}
//ENCODE → BYTE
vector<BYTE> Encoder::encodeToBytes(const vector<BYTE>& data){
    vector<unsigned char> output;

    unsigned char currentByte = 0;
    int bitCount = 0;

    for (unsigned char c : data) {
        string code = huffmanCode[c];

        for (char bit : code) {
            currentByte <<= 1;
            if (bit == '1')
                currentByte |= 1;

            bitCount++;

            if (bitCount == 8) {
                output.push_back(currentByte);
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    // Nếu còn bit dư
    if (bitCount > 0) {
        currentByte <<= (8 - bitCount);
        output.push_back(currentByte);
    }

    return output;
}

string Encoder::getCode(BYTE b) {
    return huffmanCode[b];
}
