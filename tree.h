#ifndef TREE_H
#define TREE_H
#include "treenode.h"
#include <QMessageBox>
#include <QBitArray>
#include <QFile>
class Tree
{
public:
    Tree();
    ~Tree();
    Tree(TreeNode*);
    void PostOrder();
    QBitArray GetCode(TreeNode*);
    void Clear();
    void WriteOnFile();
    void LeafCount(TreeNode *node, int &count);
    void Decompress(QBitArray, QFile*);
    void DivNodes();
private:
    TreeNode *root;
    void Clear(TreeNode*);
    void PostOrder(TreeNode*);
    void DivNodes(TreeNode*);
};

#endif
