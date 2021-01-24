#ifndef TREENODE_H
#define TREENODE_H

class TreeNode
{
public:
    TreeNode();
    TreeNode(unsigned short, char*, TreeNode*, TreeNode*, TreeNode*);
    void DivByTwo();
    unsigned short GetKey();
    char* GetSymbol();
    TreeNode *left, *right, *parent;
    TreeNode *thisNode;
private:
    unsigned short key;
    char *symbol;

};

#endif // TREENODE_H
