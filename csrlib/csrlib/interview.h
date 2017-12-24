#pragma once

#include "comheader.h"

//#define _CRT_SECURE_NO_WARNINGS

//整数求和
int AddNum(int a, int b)
{
	if (b == 0)return a;
	int sum, carry;
	sum = a^b;			//不进位相加
	carry = a&b << 1;	//进位左移运算
	return AddNum(sum, carry);
}

//取两个数中较大的
template <class T>
T MaxNum(T t1, T t2)
{
	return ((t1 + t2) + abs(t1 - t2)) / 2;
}

//取两个数中较大的
template <class T>
T MaxNumByte(T t1, T t2)
{
	T c = t1 - t2;
	c = unsigned(c) >> (sizeof(T) * 8 - 1);
	if (c == 1)
		return t2;
	return t1;
}

//判断整数
template <class T>
bool judgeInt(T t)
{
	return int(t) == t;
}

//交换数据
#define swap(a, b) { char tempBuf[10]; memcpy(tempBuf, &a, sizeof(a)); memcpy(&a, &b, sizeof(b)); memcpy(&b, tempBuf, sizeof(b)); }

//宏定义
#define SUB(x, y) (x-y)
#define ACCESS_BUFORE(element, offset, value) *SUB(&element, offset) = value

//int to string
string IntToStr(int num)
{
	//char* tempStr = "";
	//tempStr = (char*)malloc(sizeof(num));
	//memset(tempStr, 0, sizeof(num));
	
	char tempStr[240];
	sprintf(tempStr, "%d", num);
	return (string)tempStr;
}


/*
	进制之间转换
	strSrc 原始数据，
	返回值  返回结果数据，
	numSrc 原始进制
	numDes 返回结果进制
*/
string thranslateScale(string strSrc, int numSrc, int numDes)
{
	int num = 0; //转换成十进制后的数
	int t = 0;
	//1 转十进制
	for (int index = 0; index < strSrc.size(); ++index)
	{
		if (strSrc[index] >= '0' && strSrc[index] <= '9')
			t = strSrc[index] - '0';
		else
			t = strSrc[index] - 'A' + 10;

		num = numSrc*num + t;
	}

	//转numDes进制
	string strDes = "";
	for (;;)
	{
		t = num%numDes;
		num /= numDes;
		if (t <= 9)
			strDes = IntToStr(t) + strDes;
		else
			strDes = (char)((t - 10 + 'A')) + strDes;
		if(num == 0)
			break;
	}

	return strDes;
}

/*
	//a = 10100101
	//b = ~10100101 >> 5 => 00000010 = 2
	16位寄存器
	a = 0000000010100101 
	b = ~0000000010100101 >> 5 => 1111111101011010 >> 5 => 0000011111111010
*/
void addressBin()
{
	unsigned char a = 0XA5;
	unsigned char b = ~a >> 4 + 1;
	printf("%d\n", b);
}

//求平均数,必须同奇同偶
int meanNum(int x, int y)
{
	return (x&y) + ((x^y) >> 1);
}