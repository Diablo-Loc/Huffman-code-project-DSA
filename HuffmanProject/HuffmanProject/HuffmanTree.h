#pragma once
#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include "Node.h"

class HuffmanTree {
private:
    Node* root;

public:
    HuffmanTree();
    Node* buildTree(int freq[]);
    Node* getRoot();
};

#endif
