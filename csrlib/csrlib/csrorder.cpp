#include "csrorder.h"

//ц╟ещеепР
void BubbleSort(int arrNum[], int nLen,  bool bLToS)
{
	for (int i = 0; i < nLen; ++i)
	{
		for (int j = 0; j < nLen - i - 1; ++j)
		{
			if (bLToS && arrNum[j] < arrNum[j + 1])
			{
				int tmp = 0;
				tmp = arrNum[j];
				arrNum[j] = arrNum[j + 1];
				arrNum[j + 1] = tmp;
			}
			else if (!bLToS && arrNum[j] > arrNum[j + 1])
			{
				int tmp = 0;
				tmp = arrNum[j];
				arrNum[j] = arrNum[j + 1];
				arrNum[j + 1] = tmp;
			}
		}
	}
}

void QuickSort(int arrNum[], bool bToS)
{

}

void InsertSort(int arrNum[], bool bToS)
{

}
