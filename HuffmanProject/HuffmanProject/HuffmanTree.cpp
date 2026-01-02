#include "HuffmanTree.h"
#include <queue>
#include <vector>

using namespace std;

// So sánh để tạo min-heap 
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

HuffmanTree::HuffmanTree() {
    root = nullptr;
}

Node* HuffmanTree::buildTree(int freq[]) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Tạo node lá
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            pq.push(new Node((BYTE)i, freq[i]));
        }
    }

    // Trường hợp chỉ có 1 ký tự
    if (pq.size() == 1) {
        Node* only = pq.top(); pq.pop();
        root = new Node(0, only->freq);
        root->left = only;
        return root;
    }

    // Gộp cây Huffman
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* parent = new Node(0, left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    root = pq.top();
    return root;
}

Node* HuffmanTree::getRoot() {
    return root;
}
