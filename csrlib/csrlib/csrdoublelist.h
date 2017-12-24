#ifndef _LIST_H_  
#define _LIST_H_  

#include "comheader.h"

typedef struct _list
{
	int m_data;
	struct _list * next;
	struct _list * prev;
}list, *node;


void csrdoulistCreate(list * t_list);

void csrdoulistHeadInsert(list * t_list, int t_data);

void csrdoulistTailInsert(list * t_list, int t_data);

void csrdoulistDelete(list * t_list, int t_data);

node csrdoulistFind(list * t_list, int t_data);

void csrdoulistModify(list * t_list, int t_old, int t_new);

void csrdoulistDestroy(list * t_list);

void csrdoulistDisplayAsc(list * t_list);

void csrdoulistDisplayDesc(list * t_list);

#endif  
