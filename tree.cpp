#include "tree.h"

Tree::Tree()
{

}

Tree::~Tree()
{
    Clear();
}

Tree::Tree(TreeNode *root)
{
    this->root = root;
}

void Tree::PostOrder(TreeNode *node)
{
    if(node == nullptr)
        return;
    if(node->left != nullptr)  
        PostOrder(node->left);
    if(node->right != nullptr)
        PostOrder(node->right);
    QString fuild = "";
    if(node->GetSymbol() != nullptr)
        fuild += *node->GetSymbol();
    QMessageBox::information(nullptr, fuild, QString::number(node->GetKey()));
}

void Tree::PostOrder()
{
    PostOrder(root);
}

QBitArray Tree::GetCode(TreeNode* leaf)
{
    QBitArray code(0);
    TreeNode *current = leaf;
    while(current->parent != nullptr)
    {
        code.resize(code.size() + 1);
        if(current == current->parent->left)
            code.setBit(code.size() - 1, false);
        else
            code.setBit(code.size() - 1, true);
        current = current->parent;
    }
    QBitArray reverse;
    reverse.resize(code.size());
    for(int i = 0, j = code.size() - 1; i < code.size(); i++, j--)
        reverse.setBit(i, code.at(j));
    return reverse;
}

void Tree::Clear()
{
    Clear(this->root);
}

void Tree::Clear(TreeNode *node)
{
    if(node == nullptr)
        return;
    if(node->left != nullptr)
        Clear(node->left);
    if(node->right != nullptr)
        Clear(node->right);
    delete node;
}

void Tree::LeafCount(TreeNode *node, int &count)
{
    if(node == nullptr)
        return;
    if(node->left != nullptr)
        LeafCount(node->left, count);
    if(node->right != nullptr)
        LeafCount(node->right, count);
    if(node->right == nullptr && node->left == nullptr)
        count++;
}

void Tree::Decompress(QBitArray outputBits, QFile* outputFile)
{
    QByteArray stream;
    int counter = 0;
    TreeNode *current = root;
    while(counter < outputBits.size())
    {
        while(!(current->left == nullptr && current->right == nullptr) && counter < outputBits.size())
        {
            if(outputBits.at(counter))
                current = current->right;
            else
                current = current->left;
            counter++;
        }
        if(current->GetSymbol() != nullptr)
            stream.append(*current->GetSymbol());
        current = root;
    }
    outputFile->write(stream);
}

void Tree::DivNodes(TreeNode *node)
{
    if(node == nullptr)
        return;
    if(node->left != nullptr)
        DivNodes(node->left);
    if(node->right != nullptr)
        DivNodes(node->right);
    node->DivByTwo();
}

void Tree::DivNodes()
{
    DivNodes(root);
}
