#include "Decoder.h"

vector<unsigned char> Decoder::decodeFromBytes(
    const vector<unsigned char>& bytes,
    Node* root,
    int originalSize
) {
    vector<unsigned char> result;
    Node* cur = root;

    for (unsigned char byte : bytes) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;

            if (bit == 0) cur = cur->left;
            else          cur = cur->right;

            if (!cur->left && !cur->right) {
                result.push_back(cur->data);
                cur = root;

                if (result.size() == originalSize)
                    return result;
            }
        }
    }
    return result;
}
