#ifndef _LIST_H_  
#define _LIST_H_  

#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include<time.h>  


typedef struct _list
{
	int m_data;
	struct _list * next;
	struct _list * prev;

}list, *node;

void listCreate(list * t_list);

void listHeadInsert(list * t_list, int t_data);

void listDisplay(list * t_list);

#endif  
