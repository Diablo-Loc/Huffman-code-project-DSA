#include "HuffmanTree.h"
#include <queue>

using namespace std;

// So sánh để tạo min-heap
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::~HuffmanTree() {
    freeTree(root);
}

// Build cây Huffman từ bảng tần suất
Node* HuffmanTree::buildTree(const int freq[]) {
    priority_queue<Node*, vector<Node*>, Compare> pq;

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

    // Gộp cây
    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();

        Node* parent = new Node(0, left->freq + right->freq);
        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    root = pq.top();
    pq.pop();
    return root;
}

/*
 Quy ước:
  - Node nội bộ  : ghi 0
  - Node lá     : ghi 1 + BYTE ký tự
 (Duyệt tiền thứ tự - Preorder)
*/
void HuffmanTree::serialize(Node* node, vector<BYTE>& out) const {
    if (!node) return;

    if (node->isLeaf()) {
        out.push_back(1);
        out.push_back(node->ch);
    }
    else {
        out.push_back(0);
        serialize(node->left, out);
        serialize(node->right, out);
    }
}

Node* HuffmanTree::deserialize(const vector<BYTE>& data, int& index) {
    if (index >= (int)data.size()) return nullptr;

    BYTE flag = data[index++];

    if (flag == 1) {
        BYTE ch = data[index++];
        return new Node(ch, 0);
    }

    Node* node = new Node(0, 0);
    node->left = deserialize(data, index);
    node->right = deserialize(data, index);
    return node;
}

void HuffmanTree::freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}
