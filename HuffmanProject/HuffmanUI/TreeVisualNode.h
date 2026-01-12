#pragma once
#include <string>

struct TreeVisualNode
{
    int id;             // ID node
    int parentId;       // -1 nếu là root
    char symbol;        // ký tự (0 nếu node nội)
    int frequency;
    bool isLeft;        // true = con trái
};
