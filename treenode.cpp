#include "treenode.h"

TreeNode::TreeNode()
{
    this->key = 0;
    this->symbol = nullptr;
    this->left = nullptr;
    this->right = nullptr;
    this->thisNode = this;
    this->parent = nullptr;
}

TreeNode::TreeNode(unsigned short key, char* symbol, TreeNode* left, TreeNode* right, TreeNode *parent)
{
    this->key = key;
    this->symbol = symbol;
    this->left = left;
    this->right = right;
    this->thisNode = this;
    this->parent = parent;
}

void TreeNode::DivByTwo()
{
    this->key /= 2;
}

unsigned short TreeNode::GetKey()
{
    return this->key;
}

char* TreeNode::GetSymbol()
{
    return this->symbol;
}
