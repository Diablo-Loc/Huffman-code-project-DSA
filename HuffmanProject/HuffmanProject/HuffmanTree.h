#pragma once
#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include "Node.h"
#include <vector>

class HuffmanTree {
private:
    Node* root;

    void freeTree(Node* node);

public:
    HuffmanTree();
    ~HuffmanTree();

    // Build từ bảng tần suất
    Node* buildTree(const int freq[]);

    Node* getRoot() const { return root; }

    // Lưu cây → vector<BYTE>
    void serialize(Node* node, std::vector<BYTE>& out) const;

    // Khôi phục cây từ vector<BYTE>
    Node* deserialize(const std::vector<BYTE>& data, int& index);
};

#endif
