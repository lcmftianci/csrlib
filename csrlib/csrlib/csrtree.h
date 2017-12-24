#pragma once

#include "comheader.h"

using namespace std;

typedef int ElementType;

struct TreeNode;

typedef struct TreeNode* ptrPos;
typedef struct TreeNode* ptrToNode;
typedef struct TreeNode  treeNode;
struct TreeNode
{
	ElementType Element;
	ptrToNode LeftChild;
	ptrToNode rightChild;
};

//create
int CreateTree(ptrToNode* eletree);

//head tranverse
void TranverseHeadTree(ptrToNode eletree);

//middle tranverse
void TranverseMiddleTree(ptrToNode eletree);

void TranverseBackTree(ptrToNode eletree);

int TreeDeep(ptrToNode eletree);

int LeafCount(ptrToNode eletree);

ptrToNode MakeEmpty(ptrToNode eletree);

ptrPos Find(ElementType ele, ptrToNode eletree);

ptrPos FindMin(ptrToNode eletree);

ptrPos FindMax(ptrToNode eletree);

ptrToNode InsertNode(ElementType ele, ptrToNode eletree);

ptrToNode DeleteNode(ElementType ele, ptrToNode eletree);