#pragma once
#include "Node.h"
#include <string>
using std::string;

struct StackNode {
    Node* treeNode;
    string code;
    StackNode* next;
};

class Stack {
public:
    StackNode* top;

    Stack();
    ~Stack();
    void push(Node* node, string code);
    void pop(Node*& node, string& code);
    bool empty();
};
