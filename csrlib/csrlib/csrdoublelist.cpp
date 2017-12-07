#include "csrdoublelist.h"

void csrdoulistCreate(list * t_list)
{
	//链表内数据初始化  
	t_list->m_data = -1;
	//双向循环链表初始化时next,prev均指向自己  
	t_list->next = t_list;
	t_list->prev = t_list;
}

void csrdoulistHeadInsert(list * t_list, int t_data)
{
	//声明一个结点，并申请内存  
	node t_node = (node)malloc(sizeof(list));
	//节点内容赋值  
	t_node->m_data = t_data;

	//接待内prev和next指向赋值  
	t_node->prev = t_list;
	t_node->next = t_list->next;

	//链表内链接节点  
	t_list->next->prev = t_node;
	t_list->next = t_node;
}

void csrdoulistTailInsert(list * t_list, int t_data)
{
	//声明一个节点  
	node t_node = (node)malloc(sizeof(list));

	//节点内容赋值  
	t_node->m_data = t_data;

	//节点prev和next指向赋值  
	t_node->next = t_list;
	t_node->prev = t_list->prev;

	t_list->prev->next = t_node;
	t_list->prev = t_node;

}


node csrdoulistFind(list * t_list, int t_data)
{
	node t_node = NULL;
	//遍历链表  
	node t_temp;

	for (t_temp = t_list->next;t_temp != t_list;t_temp = t_temp->next)
	{
		//如果节点存在  
		if (t_temp->m_data == t_data)
		{
			//记录并退出  
			t_node = t_temp;
			break;
		}
	}
	return t_node;
}

void csrdoulistModify(list * t_list, int t_old, int t_new)
{
	//查找节点  
	node t_node = csrdoulistFind(t_list, t_old);

	//如果不存在该内容节点  
	if (NULL == t_node)
	{
		//直接结束函数  
		return;
	}
	//节点内容赋值  
	t_node->m_data = t_new;
}

void csrdoulistDelete(list * t_list, int t_data)
{
	//查找节点  
	node t_node = csrdoulistFind(t_list, t_data);

	//如果不存在该内容节点  
	if (NULL == t_node)
	{
		//直接结束该函数  
		return;
	}
	//当前节点的前一个节点的next，指向当前节点的下一个  
	t_node->prev->next = t_node->next;
	//当前节点的下一节点的prev指向当前节点的上一个  
	t_node->next->prev = t_node->prev;
}

void csrdoulistDestroy(list * t_list)
{
	//声明两个节点  
	node t_temp, t_node;
	//获取当前节点的后一节点  
	t_temp = t_list->next;

	while (t_temp != t_list)
	{
		t_node = t_temp;

		t_temp = t_temp->next;

		free(t_node);
	}
	t_node = NULL;
}

void csrdoulistDisplayAsc(list * t_list)
{
	node t_temp;

	for (t_temp = t_list->next;t_temp != t_list;t_temp = t_temp->next)
	{
		printf("%d ", t_temp->m_data);
	}
	printf("\n");
}


void csrdoulistDisplayDesc(list * t_list)
{
	node t_temp;


	for (t_temp = t_list->prev;t_temp != t_list;t_temp = t_temp->prev)
	{
		printf("%d ", t_temp->m_data);
	}
	printf("\n");
}