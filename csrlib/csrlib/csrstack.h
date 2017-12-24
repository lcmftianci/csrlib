#ifndef _CSR_STACK_H_
#define _CSR_STACK_H_

#include "comheader.h"

#define LEN 20

typedef struct _stack
{
	char m_element[LEN];
	int m_top;
}stack;

void push(stack *pStack, char element);

void pop(stack* pStack);

bool isFull(stack* pStack);

bool isEmpty(stack* pStack);

void clearStack(stack* pStack);

void display(stack* pStack);

bool initStack(stack* pStack);

#endif // _CSR_STACK_H_
