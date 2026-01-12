#include "Decoder.h"

vector<BYTE> Decoder::decodeFromBytes(
    const vector<BYTE>& encoded,
    Node* root,
    int originalSize)
{
    vector<BYTE> output;
    output.reserve(originalSize);

    Node* cur = root;

    for (BYTE byte : encoded) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;

            cur = (bit == 0) ? cur->left : cur->right;

            if (cur->isLeaf()) {
                output.push_back(cur->ch);
                cur = root;

                if ((int)output.size() == originalSize)
                    return output;
            }
        }
    }
    return output;
}
