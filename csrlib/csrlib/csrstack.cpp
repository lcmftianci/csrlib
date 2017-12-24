
#include "csrstack.h"


void push(stack *pStack, char element)
{
	if (NULL == pStack)
	{
		printf("Input param is invalid in %s at %d\n", __FILE__, __LINE__);
		return;
	}
	if (pStack->m_top < LEN)
	{
		pStack->m_element[pStack->m_top] = element;
		pStack->m_top++;
	}
	else
	{
		printf("The stack is already full in %s at %d\n", __FILE__, __LINE__);
	}
	return;
}

