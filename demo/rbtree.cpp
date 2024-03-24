#include <stdio.h>
#include <stdlib.h>
#include <map>
enum Color {
    BLACK = 1,
    RED,
    BLACKBLACK,
};

class Node {
public:
    Node() {
        m_pParent = nullptr;
        m_pLeftChild = nullptr;
        m_pRightChild = nullptr;
        m_nColor = Color::RED;
    }
    Node(const std::pair<int,int>& stEle) {
        m_stEle = stEle;
        m_pParent = nullptr;
        m_pLeftChild = nullptr;
        m_pRightChild = nullptr;
        m_nColor = Color::RED;
    }
    Node* m_pParent;
    Node* m_pLeftChild;
    Node* m_pRightChild;
    Color m_nColor;
    std::pair<int,int> m_stEle;
};

struct RbTree{
    RbTree(){}
    Node* m_pRoot = nullptr;
    bool m_bValid = true;
};

/*
        　|                            |
        Node                         Right
         /\                           /\
        /  \                         /  \
      Left Right       --->        Node RR
            /\                      /\
           /  \                    /  \
          RL  RR                 Left RL 
*/
Node* LeftRotate(Node *pNode)
{
    Node *pRight = pNode->m_pRightChild;
    Node *pLeft = pNode->m_pLeftChild;
    Node *pRightLeft = pRight->m_pLeftChild;
    Node *pRightRight = pRight->m_pRightChild;
    Node *pParent = pNode->m_pParent;
    if(pParent){
        if (pParent->m_pLeftChild == pNode) {
            pParent->m_pLeftChild = pRight;
        }
        else {
            pParent->m_pRightChild = pRight;
        }
    }
    pRight->m_pParent = pParent;

    pRight->m_pLeftChild = pNode;
    pNode->m_pParent = pRight;

    pNode->m_pRightChild = pRightLeft;
    if (pRightLeft != nullptr) {
        pRightLeft->m_pParent = pNode;
    }
    return pRight;
}

/*
        　 |                       |
         Node                    Left
          /\                      /\
         /  \                    /  \                   
       Left Right       --->    LL  Node    
        /\                          /  \
       /  \                        LR   R
      LL  LR
*/
Node * RightRotate(Node *pNode)
{
    Node *pLeft = pNode->m_pLeftChild;
    Node *pRight = pNode->m_pRightChild;
    Node *pLeftLeft = pLeft->m_pLeftChild;
    Node *pLeftRight = pLeft->m_pRightChild;
    Node *pParent = pNode->m_pParent;

    // 对pLeft完成关系调整
    // 父节点需要调整孩子
    if(pParent){
        if (pParent->m_pLeftChild == pNode) {
            pParent->m_pLeftChild = pLeft;
        }
        else {
            pParent->m_pRightChild = pLeft;
        }
    }
    pLeft->m_pParent = pParent;
    
    pLeft->m_pRightChild = pNode;
    // 对pNode完成关系调整
    pNode->m_pParent = pLeft;

    pNode->m_pLeftChild = pLeftRight;
    if (pLeftRight != nullptr) {
        pLeftRight->m_pParent = pNode;
    }
    return pLeft;
}

bool RbTree_AddEle(RbTree& stTree, std::pair<int,int> &stEle) {
	Node *pNode = new Node(stEle);
	if (stTree.m_pRoot == nullptr) {
		stTree.m_pRoot = pNode;
		stTree.m_pRoot->m_nColor = Color::BLACK;
		return true;
	}
	Node* pCur = stTree.m_pRoot;
    Node* pParent = nullptr;
	while (pCur) {
		if (pCur->m_stEle.first > stEle.first) {
            pParent = pCur;
            pCur = pCur->m_pLeftChild;
        }
        else if(pCur->m_stEle.first < stEle.first){
            pParent = pCur;
            pCur = pCur->m_pRightChild;
        }
        else{
            free(pNode);
            return false;
        }
    }
    if(pParent->m_stEle.first > stEle.first){
        pParent->m_pLeftChild = pNode;
        pNode->m_pParent = pParent;
    }
    else{
        pParent->m_pRightChild = pNode;
        pNode->m_pParent = pParent;
    }
    if (pParent->m_nColor == Color::RED) {
        Color _nColor = Color::RED;
        Node* pParentParent = pParent->m_pParent;
        while (true) {
            // 情况１
            if (pNode == pParent->m_pLeftChild && pParent == pParentParent->m_pLeftChild) {
                pNode = RightRotate(pParentParent);
                pNode->m_pLeftChild->m_nColor = (Color::BLACK);
                // 迭代终止条件
                if(pNode->m_pParent == nullptr){
                    stTree.m_pRoot = pNode;
                    stTree.m_pRoot->m_nColor = Color::BLACK;
                    break;
                }
                else if(pNode->m_pParent->m_nColor == Color::BLACK){
                    break;
                }
            }
            // 情况２
            else if (pNode == pParent->m_pLeftChild && pParent == pParentParent->m_pRightChild) {
                if(pParentParent->m_pLeftChild == nullptr){
                    pParentParent = LeftRotate(pParentParent);
                    LeftRotate(pParentParent->m_pLeftChild);
                    pParentParent = RightRotate(pParentParent);
                    pParentParent->m_nColor = Color::BLACK;
                    pParentParent->m_pLeftChild->m_nColor = Color::RED;
                    break;
                }
                else{
                    RightRotate(pParent);
                    pNode = LeftRotate(pParentParent);
                    pNode->m_pRightChild->m_nColor = (Color::BLACK);
                    if(pNode->m_pParent == nullptr){
                        stTree.m_pRoot = pNode;
                        stTree.m_pRoot->m_nColor = Color::BLACK;
                        break;
                    }
                    else if(pNode->m_pParent->m_nColor == Color::BLACK){
                        break;
                    }
                }
            }
            // 情况３
            else if (pNode == pParent->m_pRightChild && pParent == pParentParent->m_pLeftChild) {
                if(pParentParent->m_pRightChild == nullptr){
                    LeftRotate(pParent);
                    pNode = RightRotate(pParentParent);
                    pNode->m_nColor = Color::BLACK;
                    pNode->m_pRightChild->m_nColor = Color::RED;
                    break;
                } else{
                    LeftRotate(pParent);
                    pNode = RightRotate(pParentParent);
                    pNode->m_pLeftChild->m_nColor = (Color::BLACK);
                    if(pNode->m_pParent == nullptr){
                        stTree.m_pRoot = pNode;
                        stTree.m_pRoot->m_nColor = Color::BLACK;
                        break;
                    }
                    else if(pNode->m_pParent->m_nColor == Color::BLACK){
                        break;
                    }
                }
            }
            // 情况４
            else if (pNode == pParent->m_pRightChild && pParentParent->m_pRightChild == pParent) {
                pNode = LeftRotate(pParentParent);
                pNode->m_pRightChild->m_nColor = (Color::BLACK);
                if(pNode->m_pParent == nullptr){
                    stTree.m_pRoot = pNode;
                    stTree.m_pRoot->m_nColor = Color::BLACK;
                    break;
                }
                else if(pNode->m_pParent->m_nColor == Color::BLACK){
                    break;
                }
            }
        }
    }
    return true;
}

Node* RbTree_FindElePos(RbTree& stTree, int nKey){
    Node* pNode = stTree.m_pRoot;
    while(pNode){
        if(pNode->m_stEle.first == nKey){
            break;
        }
        else if(pNode->m_stEle.first < nKey){
            pNode = pNode->m_pRightChild;
        }
        else{
            pNode = pNode->m_pLeftChild;
        }
    }
    return pNode;
}

Node* RbTree_MinPos(Node* pRoot){
    Node* pNode = pRoot;
    if(pNode == nullptr){
        return nullptr;
    }
    while(pNode->m_pLeftChild){
        pNode = pNode->m_pLeftChild;
    }
    return pNode;
}
Node* RbTree_MaxPos(Node* pRoot){
    Node* pNode = pRoot;
    if(pNode == nullptr){
        return nullptr;
    }
    while(pNode->m_pRightChild){
        pNode = pNode->m_pRightChild;
    }
    return pNode;
}

void RbTree_DelEle(RbTree& stTree, int nKey) {

    Node* pNode = RbTree_FindElePos(stTree, nKey);
    if (pNode == nullptr)
    {
        return;
    }
    RbTree_DelPos(stTree, pNode);
}
void RbTree_DelPos(RbTree& stTree, Node* pNode)
{
    // 最终转化为对叶子节点的移除
    if (pNode->m_pLeftChild == nullptr && pNode->m_pRightChild == nullptr) {
        if (pNode->m_pParent == nullptr) {
            delete pNode;
            stTree.m_pRoot = nullptr;
            return;
        }

        bool bNeedAdjust = pNode->m_nColor == Color::BLACK;
        Node* pParent = pNode->m_pParent;
        delete pNode;
        if (pNode->m_pParent->m_pLeftChild == pNode) {
            pParent->m_pLeftChild = nullptr;
        }
        else {
            pParent->m_pRightChild = nullptr;
        }

        if (bNeedAdjust)
        {
            // 通过调节结束移除
            // pParent的某个颜色为黑的孩子被移除了．此孩子是叶子节点．
            AdjustForBlackHeight(stTree, pParent);
        }
    }
    else if (pNode->m_pLeftChild != nullptr)
    {
        Node *pMovingNode = RbTree_MaxPos(pNode->m_pLeftChild);
        // 本来要移除pNode的，但通过将pMovingNode中元素转移到pNode位置．
        // 转化为移除pMovingNode了．
        pNode->m_stEle = pMovingNode->m_stEle;
        RbTree_DelPos(stTree, pMovingNode);
    }
    else if (pNode->m_pRightChild != nullptr)
    {
        Node *pMovingNode = RbTree_MinPos(pNode->m_pRightChild);
        pNode->m_stEle = pMovingNode->m_stEle;
        // 本来要移除pNode的，但通过将pMovingNode中元素转移到pNode位置．
        // 转化为移除pMovingNode了．
        RbTree_DelPos(stTree, pMovingNode);
    }
}
		
// pNode颜色是红色，其父节点颜色也是红色．所在的红黑树只有pNode和其父节点不满足约束．
void AdjustWhenNodeAndParentIsRed(RbTree& stTree, Node*pNode, Node* pParent){
    Color _nColor = Color::RED;
    Node* pParentParent = pParent->m_pParent;
    while (true) {
        // 情况１
        if (pNode == pParent->m_pLeftChild && pParent == pParentParent->m_pLeftChild) {
            pNode = RightRotate(pParentParent);
            pNode->m_pLeftChild->m_nColor = (Color::BLACK);
            // 迭代终止条件
            if(pNode->m_pParent == nullptr){
                stTree.m_pRoot = pNode;
                stTree.m_pRoot->m_nColor = Color::BLACK;
                break;
            }
            else if(pNode->m_pParent->m_nColor == Color::BLACK){
                break;
            }
        }
        // 情况２
        else if (pNode == pParent->m_pLeftChild && pParent == pParentParent->m_pRightChild) {
            if(pParentParent->m_pLeftChild == nullptr){
                pParentParent = LeftRotate(pParentParent);
                LeftRotate(pParentParent->m_pLeftChild);
                pParentParent = RightRotate(pParentParent);
                pParentParent->m_nColor = Color::BLACK;
                pParentParent->m_pLeftChild->m_nColor = Color::RED;
                break;
            }
            else{
                RightRotate(pParent);
                pNode = LeftRotate(pParentParent);
                pNode->m_pRightChild->m_nColor = (Color::BLACK);
                if(pNode->m_pParent == nullptr){
                    stTree.m_pRoot = pNode;
                    stTree.m_pRoot->m_nColor = Color::BLACK;
                    break;
                }
                else if(pNode->m_pParent->m_nColor == Color::BLACK){
                    break;
                }
            }
        }
        // 情况３
        else if (pNode == pParent->m_pRightChild && pParent == pParentParent->m_pLeftChild) {
            if(pParentParent->m_pRightChild == nullptr){
                LeftRotate(pParent);
                pNode = RightRotate(pParentParent);
                pNode->m_nColor = Color::BLACK;
                pNode->m_pRightChild->m_nColor = Color::RED;
                break;
            } else{
                LeftRotate(pParent);
                pNode = RightRotate(pParentParent);
                pNode->m_pLeftChild->m_nColor = (Color::BLACK);
                if(pNode->m_pParent == nullptr){
                    stTree.m_pRoot = pNode;
                    stTree.m_pRoot->m_nColor = Color::BLACK;
                    break;
                }
                else if(pNode->m_pParent->m_nColor == Color::BLACK){
                    break;
                }
            }
        }
        // 情况４
        else if (pNode == pParent->m_pRightChild && pParentParent->m_pRightChild == pParent) {
            pNode = LeftRotate(pParentParent);
            pNode->m_pRightChild->m_nColor = (Color::BLACK);
            if(pNode->m_pParent == nullptr){
                stTree.m_pRoot = pNode;
                stTree.m_pRoot->m_nColor = Color::BLACK;
                break;
            }
            else if(pNode->m_pParent->m_nColor == Color::BLACK){
                break;
            }
        }
    }
}

// 循环不变式：pNode节点颜色是黑黑．只有pNode自身不满足红黑树约束．
void AdjustForBlackBlack(RbTree& stTree, Node* pNode){
    // 循环不变式：pNode节点颜色是黑黑．只有pNode自身不满足红黑树约束．
    Node* pParent = pNode->m_pParent;
    while (true) {
        if (pNode == pParent->m_pLeftChild 
            && pParent->m_nColor == Color::BLACK 
            && pParent->m_pRightChild->m_nColor == Color::BLACK) {
            if (pParent->m_pRightChild->m_pLeftChild->m_nColor == Color::BLACK) {
                pParent = LeftRotate(pParent);
                pParent->m_nColor = Color::BLACKBLACK;
                pParent->m_pLeftChild->m_pLeftChild->m_nColor = Color::BLACK;
                pParent->m_pLeftChild->m_nColor = Color::RED;
                pNode = pParent;
                if(pNode->m_pParent == nullptr){
                    stTree.m_pRoot->m_nColor = Color::BLACK;
                    return;
                }
            }
            else if (pParent->m_pRightChild->m_pRightChild->m_nColor == Color::RED) {
                pParent = LeftRotate(pParent);
                LeftRotate(pParent->m_pLeftChild);
                pParent->m_pLeftChild->m_pLeftChild->m_pLeftChild->m_nColor = Color::BLACK;
                pParent->m_pLeftChild->m_pLeftChild->m_nColor = Color::RED;
                pParent->m_pLeftChild->m_nColor = Color::BLACK;
                pParent->m_pRightChild->m_nColor = Color::BLACK;
                return;
            }
            else {
                RightRotate(pParent->m_pRightChild);
                pParent = LeftRotate(pParent);
                pParent->m_pLeftChild->m_pLeftChild->m_nColor = Color::BLACK;
                pParent->m_nColor = Color::BLACK;
                return;
            }
        }
        else if (pNode == pParent->m_pLeftChild 
            && pParent->m_nColor == Color::BLACK 
            && pParent->m_pRightChild->m_nColor == Color::RED) {
            pParent = LeftRotate(pParent);
            if (pParent->m_pLeftChild->m_pRightChild->m_pLeftChild->m_nColor == Color::BLACK) {
                pParent->m_nColor = Color::BLACK;
                LeftRotate(pParent->m_pLeftChild);
                pParent->m_pLeftChild->m_pLeftChild->m_pLeftChild->m_nColor = Color::BLACK;
                pParent->m_pLeftChild->m_pLeftChild->m_nColor = Color::RED;
                return;
            }
            else {
                RightRotate(pParent->m_pLeftChild->m_pRightChild);
                LeftRotate(pParent->m_pLeftChild);
                pParent->m_nColor = Color::BLACK;
                pParent->m_pLeftChild->m_pLeftChild->m_pLeftChild->m_nColor = Color::BLACK;
                return;
            }
        }
        else if (pNode == pParent->m_pLeftChild && pParent->m_nColor == Color::RED) {
            pParent = LeftRotate(pParent);
            pParent->m_nColor = Color::BLACK;
            pParent->m_pLeftChild->m_nColor = Color::RED;
            pParent->m_pLeftChild->m_pLeftChild->m_nColor = Color::BLACK;
            if (pParent->m_pLeftChild->m_pRightChild->m_nColor == Color::RED) {
                AdjustWhenNodeAndParentIsRed(stTree, pParent->m_pLeftChild->m_pRightChild, pParent->m_pLeftChild);
            }
            return;
        }

        else if (pNode == pParent->m_pRightChild 
            && pParent->m_nColor == Color::BLACK 
            && pParent->m_pLeftChild->m_nColor == Color::BLACK) {
            if (pParent->m_pLeftChild->m_pRightChild->m_nColor == Color::BLACK) {
                pParent = RightRotate(pParent);
                pParent->m_pRightChild->m_pRightChild->m_nColor = Color::BLACK;
                pParent->m_pRightChild->m_nColor = Color::RED;
                pParent->m_nColor = Color::BLACKBLACK;
                pNode = pParent;
                if(pNode->m_pParent == nullptr){
                    stTree.m_pRoot->m_nColor = Color::BLACK;
                    return;
                }
            }
            else {
                LeftRotate(pParent->m_pLeftChild);
                pParent = RightRotate(pParent);
                pParent->m_nColor = Color::BLACK;
                pParent->m_pRightChild->m_pRightChild->m_nColor = Color::BLACK;
                return;
            }
        }
        else if (pNode == pParent->m_pRightChild && pParent->m_nColor == Color::BLACK 
            && pParent->m_pLeftChild->m_nColor == Color::RED) {
            if (pParent->m_pLeftChild->m_pRightChild->m_pRightChild->m_nColor == Color::BLACK) {
                pParent = RightRotate(pParent);
                RightRotate(pParent->m_pRightChild);
                pParent->m_pRightChild->m_pRightChild->m_pRightChild->m_nColor = Color::BLACK;
                pParent->m_pRightChild->m_pRightChild->m_nColor = Color::RED;
                pParent->m_nColor = Color::BLACK;
                return;
            }
            else {
                pParent = RightRotate(pParent);
                LeftRotate(pParent->m_pRightChild->m_pLeftChild);
                RightRotate(pParent->m_pRightChild);
                pParent->m_nColor = Color::BLACK;
                pParent->m_pRightChild->m_pRightChild->m_pRightChild->m_nColor = Color::BLACK;
                return;
            }
        }
        else if (pNode == pParent->m_pRightChild && pParent->m_nColor == Color::RED) {
            pParent = RightRotate(pParent);
            pParent->m_pRightChild->m_pRightChild->m_nColor = Color::BLACK;
            if (pParent->m_pRightChild->m_pLeftChild->m_nColor == Color::RED) {
                AdjustWhenNodeAndParentIsRed(stTree, pParent->m_pRightChild->m_pLeftChild, pParent->m_pRightChild);
            }
            return;
        }
    }
}

// pNode的某个孩子被移除了．移除的孩子是叶子节点．移除节点的颜色是黑色．
void AdjustForBlackHeight(RbTree& stTree, Node *pNode)
{
    if (pNode == nullptr) {
        return;
    }

    if (pNode->m_pRightChild != nullptr) 
    {
        if (pNode->m_nColor == Color::RED)
        {
            Node *pTemp = LeftRotate(pNode);
            if (pTemp->m_pLeftChild->m_pRightChild != nullptr)
            {
                pNode = pTemp->m_pLeftChild->m_pRightChild;
                AdjustWhenNodeAndParentIsRed(stTree, pNode, pNode->m_pParent);
            }
            return;
        }

        // pNode->m_nColor == Color::BLACK
        if (pNode->m_pRightChild->m_nColor == Color::RED)
        {
            Node *pTemp = LeftRotate(pNode);
            LeftRotate(pTemp->m_pLeftChild);
            pTemp->m_nColor = Color::BLACK;
            pTemp->m_pLeftChild->m_pLeftChild->m_nColor = Color::RED;
            if (pTemp->m_pLeftChild->m_pLeftChild->m_pRightChild != nullptr)
            {
                pNode = pTemp->m_pLeftChild->m_pLeftChild->m_pRightChild;
                AdjustWhenNodeAndParentIsRed(stTree, pNode, pNode->m_pParent);
            }
            return;
        }

        // pNode->m_pRightChild->m_nColor == Color::BLACK
        Node *pTemp = LeftRotate(pNode);
        if (pTemp->m_pRightChild != nullptr)
        {
            pTemp->m_pRightChild->m_nColor = Color::BLACK;
            return;
        }
        
        // pTemp->m_pRightChild ＝= nullptr
        if (pTemp->m_pLeftChild->m_pRightChild != nullptr)
        {
            LeftRotate(pTemp->m_pLeftChild);
            pTemp = RightRotate(pTemp);
            pTemp->m_nColor = Color::BLACK;
            return;
        }

        // pTemp->m_pLeftChild->m_pRightChild == nullptr
        pTemp->m_pLeftChild->m_nColor = Color::RED;
        pTemp->m_nColor = Color::BLACKBLACK;
        pNode = pTemp;
        Node *pParent = pNode->m_pParent;
        if(pParent == nullptr){
            stTree.m_pRoot->m_nColor = Color::BLACK;
            return;
        }

        AdjustForBlackBlack(stTree, pNode);
    }
    else
    {
        if (pNode->m_nColor == Color::RED)
        {
            Node *pTemp = RightRotate(pNode);
            if (pTemp->m_pRightChild->m_pLeftChild != nullptr)
            {
                AdjustWhenNodeAndParentIsRed(stTree, pTemp->m_pRightChild->m_pLeftChild, pTemp->m_pRightChild);
            }
            return;
        }

        // pNode->m_nColor == Color::BLACK
        if (pNode->m_pLeftChild->m_nColor == Color::RED)
        {
            Node *pTemp = RightRotate(pNode);
            RightRotate(pTemp->m_pRightChild);
            pTemp->m_nColor = Color::BLACK;
            pTemp->m_pRightChild->m_pRightChild->m_nColor = Color::RED;
            if (pTemp->m_pRightChild->m_pRightChild->m_pLeftChild != nullptr)
            {
                AdjustWhenNodeAndParentIsRed(stTree, pTemp->m_pRightChild->m_pRightChild->m_pLeftChild, pTemp->m_pRightChild->m_pRightChild);
            }
            return;
        }

        // pNode->m_pLeftChild->m_nColor == Color::BLACK
        Node *pTemp = RightRotate(pNode);
        if (pTemp->m_pLeftChild)
        {
            pTemp->m_pLeftChild->m_nColor = Color::BLACK;
            return;
        }
        
        // pTemp->m_pLeftChild == null
        if (pTemp->m_pRightChild->m_pLeftChild)
        {
            RightRotate(pTemp->m_pRightChild);
            pTemp = LeftRotate(pTemp);
            pTemp->m_nColor = Color::BLACK;
            return;
        }

        // pTemp->m_pRightChild->m_pLeftChild == null
        pTemp->m_pRightChild->m_nColor = Color::RED;
        pTemp->m_nColor = Color::BLACKBLACK;
        AdjustForBlackBlack(stTree, pTemp);
    }
}

typedef void (*VisitFun)(RbTree& stTree, Node* pNode);
void Visit_Pre(RbTree& stTree, Node* pNode, VisitFun pFun){
    if(pNode == nullptr || stTree.m_bValid == false){
        return;
    }
    pFun(stTree, pNode);
    Visit_Pre(stTree, pNode->m_pLeftChild, pFun);
    Visit_Pre(stTree, pNode->m_pRightChild, pFun);
}
void Visit_Mid(RbTree& stTree, Node* pNode, VisitFun pFun){
    if(pNode == nullptr || stTree.m_bValid == false){
        return;
    }
    Visit_Mid(stTree, pNode->m_pLeftChild, pFun);
    pFun(stTree, pNode);
    Visit_Mid(stTree, pNode->m_pRightChild, pFun);
}
void Visit_Post(RbTree& stTree, Node* pNode, VisitFun pFun){
    if(pNode == nullptr || stTree.m_bValid == false){
        return;
    }
    Visit_Post(stTree, pNode->m_pLeftChild, pFun);
    Visit_Post(stTree, pNode->m_pRightChild, pFun);
    pFun(stTree, pNode);
}

void Visit(RbTree& stTree, Node* pNode){
    if(stTree.m_bValid == false){
        return;
    }

    if (pNode->m_nColor != Color::RED && pNode->m_nColor != Color::BLACK)
    {
        stTree.m_bValid = false;
        return;
    }

    if (pNode->m_nColor == Color::RED)
    {
        if (pNode->m_pLeftChild != nullptr && pNode->m_pLeftChild->m_nColor != Color::BLACK)
        {
            stTree.m_bValid = false;
            return;
        }

        if (pNode->m_pRightChild != nullptr && pNode->m_pRightChild->m_nColor != Color::BLACK)
        {
            stTree.m_bValid = false;
            return;
        }
    }

    int _nLeftBH = GetBlackHeight(pNode->m_pLeftChild);
    int _nRightBH = GetBlackHeight(pNode->m_pRightChild);
    if (_nLeftBH != _nRightBH)
    {
        stTree.m_bValid = false;
        return;
    }
}

bool CheckValid(RbTree& stTree) 
{
    if (stTree.m_pRoot == nullptr)
    {
        return true;
    }

    if (stTree.m_pRoot->m_nColor != Color::BLACK)
    {
        stTree.m_bValid = false;
        return false;
    }

    Visit_Pre(stTree, stTree.m_pRoot, Visit);
    return stTree.m_bValid;
}

int GetBlackHeight(Node *pNode) 
{
    if (pNode == nullptr) {
        return 0;
    }
    int _nBlackHeight = 0;
    if (pNode->m_nColor == Color::BLACK) {
        _nBlackHeight = 1;
    }
    int _nLeft = GetBlackHeight(pNode->m_pLeftChild);
    return _nLeft + _nBlackHeight;   
}

#include <stdio.h>
#include <stdlib.h>

class XNode{
public:
    int i = 0;
};

class XCon{
public:
    XNode* p = nullptr;
};

class Xiter{
public:
    Xiter(const XCon* pCon_, XNode* pNode_){
        pCon = pCon_;
        pNode = pNode_;
    }
    const XCon* pCon = nullptr;
    XNode* pNode = nullptr;
};

int main(){
    const XCon stCon;
    Xiter iter(&stCon, stCon.p);
}