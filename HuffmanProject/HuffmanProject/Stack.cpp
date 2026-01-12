#include "Stack.h"

Stack::Stack() {
    top = nullptr;
}

void Stack::push(Node* node, string code) {
    StackNode* temp = new StackNode{ node, code, top };
    top = temp;
}

void Stack::pop(Node*& node, string& code) {
    StackNode* temp = top;
    node = temp->treeNode;
    code = temp->code;
    top = temp->next;
    delete temp;
}

bool Stack::empty() {
    return top == nullptr;
}
Stack::~Stack() {}