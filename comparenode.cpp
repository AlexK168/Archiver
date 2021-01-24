#include "comparenode.h"

bool CompareNode::operator()(TreeNode& a, TreeNode& b)
{
    return a.GetKey() > b.GetKey();
}
