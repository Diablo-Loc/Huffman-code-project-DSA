#include <iostream>
#include <string>
#include "Encoder.h"
#include "Decoder.h"
#include "Node.h"
using namespace std;

Node* createNode(char ch) {
	Node* node = new Node;
	node->ch = ch;
	node->freq = 0;
	node->left = NULL;
	node->right = NULL;
	return node;
}

int main() {
    // 1. Huffmain tree giả..

    Node* root = createNode('\0');
    root->left = createNode('a');

    root->right = createNode('\0');
    root->right->left = createNode('b');
    root->right->right = createNode('c');

    // 2. BẢNG MÃ HUFFMAN
    string huffmanCode[256];
    huffmanCode['a'] = "0";
    huffmanCode['b'] = "10";
    huffmanCode['c'] = "11";

    // 3. TEST ENCODER
    string text = "abc";
    string encoded = encoderText(text, huffmanCode);

    cout << "Text goc   : " << text << endl;
    cout << "Encoded   : " << encoded << endl;

    // 4. TEST DECODER
    string decoded = decoderText(encoded, root);
    cout << "Decoded   : " << decoded << endl;

    return 0;

}