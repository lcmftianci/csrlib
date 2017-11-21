#ifndef _CSR_ORDER_H_
#define _CSR_ORDER_H_

#include <iostream>

using namespace std;

//template <class T>
#define SwapData(T1, T2) float tmp = T1; \
                           T1 = T2;  \
                           T2 = tmp; \

//½»»»Êý¾Ý
template <class T>
void SwapDataT(T& t1, T& t2)
{
	T tmp = t1;
	t1 = t2;
	t2 = t1;
}

//Ã°ÅÝÅÅÐò
template <class T>
void SortBubble(T tinfo[], int arrNum, bool bLToS = true)
{
	for(auto varOutter = 0; varOutter < arrNum; ++varOutter)
	{
		for (auto varInner = 0; varInner < arrNum - varOutter - 1; ++varInner)
		{
			if (bLToS)
			{
				if (tinfo[varInner] < tinfo[varInner + 1])
				{
					SwapData(tinfo[varInner], tinfo[varInner + 1]);
				}
					
			}
			else
			{
				if (tinfo[varInner] > tinfo[varInner + 1])
				{
					SwapData(tinfo[varInner], tinfo[varInner + 1]);
				}
					
			}
		}
	}
}

template <class T>
void DisplayData(T tinfo[], int arrNum)
{
	for (auto varData = 0; varData < arrNum; ++varData)
	{
		cout << tinfo[varData] << endl;
	}
}

#endif
