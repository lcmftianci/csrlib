#ifndef _CSR_STR_UTILS_H__
#define _CSR_STR_UTILS_H__

#include <stdio.h>
#include <iostream>
#include <vector>

char *mystrncpy(char *str, int n)
{
		char *p = str;
		if (p == NULL)
		{
			return NULL;
		}
		else
		{
			int i = 0;
			while (*p != '\0')
			{
				//循环直到达n个字符串终止
				if (i == n)
				{
					break;
				}
				i++;
				p++;
			}
			*(p++) = '\0';
			//赋值结束字符串
		}
		return str;
}

char* substr(char*str, unsigned int start, unsigned int end)
{
	unsigned n = end - start;
	static char stbuf[256];
	strncpy(stbuf, str + start, n);
	stbuf[n] = 0;
	return stbuf;
}

int splitwords(char* strSrc, std::vector<std::string> & arrStr, char chStr)
{
	unsigned int index = 0, len = 0, subIndex = 0, strlength = 0;
	while (*strSrc++ == '\n')
	{
		strlength++;
	}
	while (strlength--)
	{
		index++;
		subIndex++;
			if(*strSrc++ == chStr)
			{
				char* sr = (char*)malloc(index);
				if (index == subIndex)
					sr = substr(strSrc, 0, index);
				else
					sr = substr(strSrc, subIndex - index, subIndex);
				std::string str = sr;
				arrStr.push_back(str);
				index = 0;
			}
	}
	return arrStr.size();
}

#endif