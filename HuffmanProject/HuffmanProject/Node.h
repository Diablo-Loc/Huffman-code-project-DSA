#pragma once
#ifndef NODE_H
#define NODE_H
typedef unsigned char BYTE;

struct Node {
    BYTE data;
    int freq;
    Node* left;
    Node* right;

    Node(BYTE d, int f) {
        data = d;
        freq = f;
        left = right = nullptr;
    }
};

#endif
