#include "csrorder.h"

int csrswapdata(int *a, int *b)
{
	if (*a == *b)
		return 0;
	int tmp = 0;
	tmp = *a;
	*a = *b;
	*b = tmp;
	return 1;
}

void csrswapdata(int &a, int &b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

void csrdisplay(int arr[], int num)
{
	for (int index = 0; index < num; ++index)
		printf("%d ", arr[index]);
	printf("\n");
}

void csrdisplay(int arr[], long maxlen, int nbegin, int nend)
{
	int i = 0;
	for (i = 0; i < nbegin; i++)
		printf(" ");

	for (i = nbegin; i <= nend; i++)
		printf("%4d", arr[i]);

	printf("\n");
}

//冒泡排序
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

void QuickSort(int arrNum[], int maxLen, int nbegin, int nend, bool bToS)
{
	int i, j;					//临时变量
	if (nbegin < nend)
	{
		i = nbegin + 1;			//初始值是基准值不进行移动
		j = nend;

		/*
			以arrNum[nbegin]为基准，对arrNum[begin + 1] 和arrNum[nend]之间的数进行分组
		*/
	
			if (bToS)
			{
				while (i != j)
				{
					if (arrNum[i] < arrNum[nbegin])
					{
						if (csrswapdata(&arrNum[i], &arrNum[j]) == 1)
							;//csrdisplay(arrNum, maxLen, nbegin, nend);
						j--;
					}
					else
						++i;
				}

				if (arrNum[i] <= arrNum[nbegin])
					--i;

				if (csrswapdata(&arrNum[nbegin], &arrNum[i]) == 1)
					;//csrdisplay(arrNum, maxLen, nbegin, nend);

				QuickSort(arrNum, maxLen, nbegin, i - 1, bToS);
				QuickSort(arrNum, maxLen, j, nend, bToS);
			}
			else
			{
				while (i != j)
				{
					if (arrNum[i] > arrNum[nbegin])
					{
						if (csrswapdata(&arrNum[i], &arrNum[j]) == 1)
							;//csrdisplay(arrNum, maxLen, nbegin, nend);
						j--;
					}
					else
						++i;
				}

				if (arrNum[i] >= arrNum[nbegin])
					--i;

				if (csrswapdata(&arrNum[nbegin], &arrNum[i]) == 1)
					;//csrdisplay(arrNum, maxLen, nbegin, nend);

				QuickSort(arrNum, maxLen, nbegin, i - 1, bToS);
				QuickSort(arrNum, maxLen, j, nend, bToS);
			}
	}
}

void InsertSort(int arrNum[], bool bToS)
{

}
