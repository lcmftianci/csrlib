#ifndef _CSR_ORDER_H_
#define _CSR_ORDER_H_

#include "comheader.h"

using namespace std;

//template <class T>
#define SwapData(T1, T2) float tmp = T1; \
                           T1 = T2;  \
                           T2 = tmp; \

//交换数据
template <class T>
void SwapDataT(T& t1, T& t2)
{
	T tmp = t1;
	t1 = t2;
	t2 = t1;
}

//冒泡排序
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
		cout << tinfo[varData] << " ";
	}
	cout << endl;
}

/*
	在计算机科学所使用的排序算法通常被分类为：
	计算的时间复杂度（最差、平均、和最好性能），依据列表（list）的大小(n)。一般而言，好的性能是O(n log n)，且坏的性能是O(n2)。对于一个排序理想的性能是O(n)。
	仅使用一个抽象关键比较运算的排序算法总平均上总是至少需要O(n logn)。
	存储器使用量（以及其他电脑资源的使用）
	稳定性：稳定排序算法会让原本有相等键值的纪录维持相对次序。也就是如果一个排序算法是稳定的，当有两个相等键值的纪录R和S，且在原本的列表中R出现在S之前，在排序过的列表中R也将会是在S之前。
	依据排序的方法：插入、交换、选择、合并等等。

	1、稳定的排序
	冒泡排序（bubble sort）— O(n2)
	鸡尾酒排序（cocktail sort）—O(n2)
	插入排序（insertion sort）—O(n2)
	桶排序（bucket sort）—O(n);需要O(k)额外空间
	计数排序（counting sort）—O(n+k);需要O(n+k)额外空间
	归并排序（merge sort）—O(n log n);需要O(n)额外空间
	原地归并排序— O(n2)
	二叉排序树排序（binary tree sort）— O(n log n)期望时间; O(n2)最坏时间;需要O(n)额外空间
	鸽巢排序（pigeonhole sort）—O(n+k);需要O(k)额外空间
	基数排序（radix sort）—O(n·k);需要O(n)额外空间
	侏儒排序（gnome sort）— O(n2)
	图书馆排序（library sort）— 时间复杂度通常是O(n log n),需要(1+ε)n额外空间

	2、不稳定的排序
	选择排序（selection sort）—O(n2)
	希尔排序（shell sort）—O(n log2 n)如果使用最佳的现在版本
	Clover排序算法（Clover sort）—O(n)期望时间，O（n^2/2）最坏情况
	梳排序— O(n log n)
	堆排序（heap sort）—O(n log n)
	平滑排序（smooth sort）— O(n log n)
	快速排序（quick sort）—O(n log n)期望时间, O(n2)最坏情况;对于大的、乱数列表一般相信是最快的已知排序
	内省排序（introsort）—O(n log n)
	耐心排序（patience sort）—O(n log n + k)最坏情况时间，需要额外的O(n + k)空间，也需要找到最长的递增子序列（longest increasing subsequence）

	3、时间复杂度
	冒泡排序O(n2)
	选择排序O(n2)
	插入排序O(n2)
	希尔排序O(n1.25)
	堆排序O(n log n)
	归并排序O(n log n)
	快速排序O(n log n)
	基数排序O(n)

	虽然完全逆序的情况下，快速排序会降到选择排序的速度，不过从概率角度来说（参考信息学理论，和概率学），不对算法做编程上优化时，快速排序的平均速度比堆排序要快一些。

	4、不实用的排序
	Bogo排序— O(n × n!)，最坏的情况下期望时间为无穷。
	Stupid排序—O(n3);递归版本需要O(n2)额外存储器
	珠排序（bead sort）— O(n) or O(√n),但需要特别的硬件
	煎饼排序—O(n),但需要特别的硬件
	臭皮匠排序（stooge sort）算法简单，但需要约n^2.7的时间
*/

//交换数据
int csrswapdata(int *a, int *b);

void csrswapdata(int &a, int &b);

//打印数据
void csrdisplay(int arr[], int num);

void csrdisplay(int arr[], long maxlen, int nbegin, int nend);

//冒泡
void BubbleSort(int arrNum[], int nLen, bool bLToS);

//快排
void QuickSort(int arrNum[], int maxLen, int nbegin, int nend, bool bToS = false);

//插入法排序
void InsertSort(int arrNum[], bool bToS);

#endif
