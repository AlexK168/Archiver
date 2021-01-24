#ifndef COMPARENODE_H
#define COMPARENODE_H
#include "treenode.h"

class CompareNode
{
public:
    bool operator()(TreeNode& a, TreeNode& b);
};

#endif // COMPARENODE_H
