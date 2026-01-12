#pragma once
#ifndef NODE_H
#define NODE_H

typedef unsigned char BYTE;

struct Node {
    BYTE ch;
    int freq;
    Node* left;
    Node* right;

    Node(BYTE c, int f)
        : ch(c), freq(f), left(nullptr), right(nullptr) {
    }

    bool isLeaf() const {
        return left == nullptr && right == nullptr;
    }
    Node* cloneTree(const Node* src)
    {
        if (!src) return nullptr; // Kiểm tra an toàn

        Node* n = new Node(src->ch, src->freq);
        n->left = cloneTree(src->left);  // Đệ quy bên trái
        n->right = cloneTree(src->right); // Đệ quy bên phải
        return n;
    }

};

#endif
