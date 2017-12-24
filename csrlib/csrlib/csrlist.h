#pragma once

#include "comheader.h"

typedef struct _Node {
	struct _Node* next;
	int data;
}Node, *ptrNode;

typedef struct _List {
	struct _Node* head;
}List, *ptrList;
/*
	增删改查
*/

//创建链表
ptrList createList();

//插入节点
void insertList(ptrList pList, ptrNode pNode);

//删除节点
void deleteList(ptrList pList, ptrNode pNode);

//修改节点信息
void modifyList(ptrList pList, ptrNode pNode);

ptrNode findList(ptrList pList, int data);

//list类型 链表
//List*类型 节点
typedef struct _linuxlist
{
	int m_data;//成员变量
	struct _linuxlist * next;//结构体调用自己的类型
}linuxlist, *linuxnode;

// 创建链表
void listCreate(linuxlist * t_list);

//头插法
void listHeadInsert(linuxlist * t_list, int t_data);
//尾插法
void listTailInsert(linuxlist * t_list, int t_data);

//修改数据
void listModify(linuxlist * t_list, int t_old, int t_new);

//删除数据
void listDelete(linuxlist * t_list, int t_data);

//查找数据
linuxnode listFind(linuxlist * t_list, int t_data);

//打印数据
void listDisplay(linuxlist * t_list);

//销毁链表
void listDestroy(linuxlist * t_list);

//翻转
void listReverse(linuxlist * t_list);

//求长度
int listLength(linuxlist * t_list);

//排序
void listBubbleSort(linuxlist * t_list);

void listQuickSort(linuxlist* t_list);