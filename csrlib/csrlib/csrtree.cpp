#include "csrtree.h"

int CreateTree(ptrToNode* eletree)
{
	ElementType ele;
	ElementType tmp;
	scanf("%d", &ele);	//获取元素
	tmp = getchar();
	if (ele == -1)
		*eletree = NULL;
	else
	{
		*eletree = (ptrToNode)malloc(sizeof(TreeNode));
		if (!(*eletree)) exit(-1);
		(*eletree)->Element = ele;
		cout << "输入%d的左子叶节点:" << ele << endl;
		CreateTree(&(*eletree)->LeftChild);
		cout << "输入%d的右子叶节点:" << ele << endl;
		CreateTree(&(*eletree)->rightChild);
	}
	return 1;
}

void TranverseHeadTree(ptrToNode eletree)
{

}


