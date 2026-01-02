#include <iostream>
#include <vector>
#include <string>

#include "Encoder.h"
#include "Decoder.h"
#include "HuffmanTree.h"
#include "Node.h"
#include "Stack.h"

using namespace std;

int main() {
    cout << "=== TEST HUFFMAN ENCODER / DECODER ===" << endl;

    // 1. Dữ liệu test (giả lập pixel / byte)
    vector<BYTE> data = { 255, 0, 128, 255, 0 };

    cout << "Original data: ";
    for (BYTE b : data)
        cout << (int)b << " ";
    cout << endl;

    // 2. Encoder: đếm tần suất
    Encoder enc;
    enc.countFrequency(data);

    // 3. Build Huffman Tree
    HuffmanTree tree;
    Node* root = tree.buildTree(enc.getFrequency());

    if (root == nullptr) {
        cout << "Build Huffman Tree failed!" << endl;
        return 1;
    }

    // 4. Build bảng mã Huffman
    enc.buildCode(root);

    cout << "\nHuffman Codes:" << endl;
    for (int i = 0; i < 256; i++) {
        string code = enc.getCode((BYTE)i);
        if (!code.empty()) {
            cout << "Byte " << i << " : " << code << endl;
        }
    }

    // 5. Encode dữ liệu
    vector<BYTE> encoded = enc.encodeToBytes(data);

    cout << "\nEncoded bytes: ";
    for (BYTE b : encoded)
        cout << (int)b << " ";
    cout << endl;

    // 6. Decode dữ liệu
    Decoder dec;
    vector<BYTE> decoded =
        dec.decodeFromBytes(encoded, root, data.size());

    cout << "\nDecoded data: ";
    for (BYTE b : decoded)
        cout << (int)b << " ";
    cout << endl;

    // 7. Kiểm tra kết quả
    if (data == decoded)
        cout << "\nRESULT: Decode SUCCESS (data identical)" << endl;
    else
        cout << "\nRESULT: Decode FAILED" << endl;

    cout << "====================================" << endl;
    return 0;
}
